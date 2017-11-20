#include "SipServer.h"

// SipServer唯一实例初始化
SipServer* SipServer::mInstance = nullptr;

SipServer::SipServer()
{
}

SipServer::~SipServer()
{
}

SipServer * SipServer::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new SipServer();
	}
	return mInstance;
}

int SipServer::StartSipServer()
{
	int result = -1;

	// 初始化Sip服务
	result = InitSipServer();
	if (result)	// 服务初始化失败
	{
		std::cout << "Sip服务初始化失败..." << std::endl;
		system("pause");
	} 
	else
	{
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		}
	}

	return 0;
}

int SipServer::InitSipServer()
{
	// osip全局初始化，注册osip回调函数
	osip_init(&sip);
	RegisterSipMsgCallbacks();
	RegisterCommonCallbacks();

	// 服务启动时自动向上级注册
	AutoRegister();

	return 0;
}

int SipServer::AutoRegister()
{
	SipMsgBuilder* sipMsgBuilder = nullptr;

	// 将所有平台状态设置为'OFF'
	mSipDB->ResetPlatformStatus();

	// 获取上联平台和互联平台SipID
	std::vector<std::string> sipIDs;
	mSipDB->GetUpperPlatformIDs(sipIDs);

	// 遍历平台，进行注册
	for (std::vector<std::string>::iterator itr = sipIDs.begin(); itr != sipIDs.end(); itr++)
	{
		sipMsgBuilder = new SipMsgBuilder;

		osip_message_t* sipMsg;
		sipMsgBuilder->CreateRegisterMsg(sipMsg, (*itr).c_str());
		osip_event_t* event = osip_new_outgoing_sipmessage(sipMsg);
		ProcessOutGoingMessage(nullptr, event);
		delete sipMsgBuilder;
		sipMsgBuilder = nullptr;
	}

	return 0;
}

int SipServer::ProcessInComingMessage(std::string message)
{
	if (message.length() < 100)
	{
		return -1;
	}

	size_t msgLen = message.length() + 1;
	osip_event_t* event = osip_parse(message.c_str(), msgLen);
	if (event == nullptr)
	{
		return -1;
	}

	osip_transaction_t* transaction = __osip_find_transaction(sip, event, 0);

	// 收到reponse信息
	if (EVT_IS_INCOMINGRESP(event) && transaction == nullptr)
	{
		return -1;
	}

	// 收到request消息
	if (transaction == nullptr)
	{
		CreateTransaction(transaction, event);
	}

	if (transaction != nullptr)
	{
		osip_transaction_execute(transaction, event);

		std::vector<osip_event_t*>::iterator iter;
		for (iter = mSipEvents.begin(); iter != mSipEvents.end(); iter++)
		{
			if ((*iter) == nullptr || (*iter)->sip == nullptr)
			{
				continue;
			}
			if ((*iter)->sip->status_code > 0)// response message，无callTransaction,
			{
				ProcessOutGoingMessage(nullptr, *iter);
			}
			else
			{
				ProcessOutGoingMessage(transaction, *iter);
			}
		}
		mSipEvents.clear();
	}
	return 0;
}

int SipServer::ProcessOutGoingMessage(osip_transaction_t * callTransaction, osip_event_t *& event)
{
	osip_transaction_t* transaction = __osip_find_transaction(sip, event, 0);

	if (transaction == nullptr)
	{
		CreateTransaction(transaction, event);
	}

	if (transaction != nullptr)
	{
		osip_transaction_execute(transaction, event);

		if (callTransaction != nullptr)
		{
			AppContext* appContext = (AppContext*)transaction->your_instance;
			appContext->callTransaction = callTransaction;
		}
	}
	return 0;
}

int SipServer::CreateTransaction(osip_transaction_t *& transaction, osip_event_t * event)
{
	bool isInvite = false;

	if (event == nullptr)
	{
		return -1;
	}

	// 收到请求
	if (EVT_IS_INCOMINGREQ(event))
	{
		if (EVT_IS_RCV_INVITE(event))
		{
			osip_transaction_init(&transaction, IST, sip, event->sip);
			//transaction->state = IST_PRE_PROCEEDING;
			isInvite = true;
		}
		else if (EVT_IS_RCV_ACK(event))
		{
			osip_transaction_init(&transaction, IST, sip, event->sip);
			transaction->state = IST_COMPLETED;
		}
		else // EVT_IS_RCV_REQUEST
		{
			osip_transaction_init(&transaction, NIST, sip, event->sip);
			//transaction->state = NIST_PRE_TRYING;
		}
	}

	// 发出请求
	if (EVT_IS_OUTGOINGREQ(event))
	{
		if (EVT_IS_SND_INVITE(event))
		{
			osip_transaction_init(&transaction, ICT, sip, event->sip);
			//transaction->state = ICT_PRE_CALLING;
			isInvite = true;
		}
		else if (EVT_IS_SND_ACK(event))
		{
			osip_transaction_init(&transaction, ICT, sip, event->sip);
			//transaction->state = ICT_PRE_CALLING;
		}
		else // EVT_IS_SND_REQUEST
		{
			osip_transaction_init(&transaction, NICT, sip, event->sip);
			//transaction->state = NICT_PRE_TRYING;
		}
	}

	// 创建transaction是否成功
	if (transaction == nullptr)
	{
		return -1;
	}
	else // transaction != nullptr
	{
		AppContext* appContext = AppContext::CreateAppContext();
		appContext->sip = sip;
		//appContext->mSipDialogs = mSipDialogs;
		appContext->callTransaction = nullptr;

		if (isInvite)
		{
			SipMsgParser sipMsgParser;
			sipMsgParser.ParserSipMsgBody(event->sip);
			if (transaction->ctx_type == ICT || transaction->ctx_type == IST) // 确定码流接收方ip、port
			{
				appContext->mRecvIP = sipMsgParser.mSdpParam.cParam.addr;
				appContext->mRecvPort = sipMsgParser.mSdpParam.mParam.port;
			}
		}
		transaction->your_instance = (void*)appContext;
	}
	return 0;
}


void SipServer::RegisterSipMsgCallbacks()
{
	// send message callback，发送sip消息函数
	osip_set_cb_send_message(sip, SipCallback::osip_send_message_cb);

	// receive message callback，收到sip消息后回调函数
	osip_set_message_callback(sip, OSIP_ICT_STATUS_2XX_RECEIVED, SipCallback::osip_ict_rcv_2xx_cb);
	osip_set_message_callback(sip, OSIP_ICT_STATUS_3XX_RECEIVED, SipCallback::osip_ict_rcv_3xx_cb);
	osip_set_message_callback(sip, OSIP_ICT_STATUS_4XX_RECEIVED, SipCallback::osip_ict_rcv_45xx_cb);

	osip_set_message_callback(sip, OSIP_IST_INVITE_RECEIVED, SipCallback::osip_ist_rcv_invite_cb);
	osip_set_message_callback(sip, OSIP_IST_ACK_RECEIVED, SipCallback::osip_ist_rcv_ack_cb);

	osip_set_message_callback(sip, OSIP_NICT_STATUS_2XX_RECEIVED, SipCallback::osip_nict_rcv_2xx_cb);
	osip_set_message_callback(sip, OSIP_NICT_STATUS_3XX_RECEIVED, SipCallback::osip_nict_rcv_3xx_cb);
	osip_set_message_callback(sip, OSIP_NICT_STATUS_4XX_RECEIVED, SipCallback::osip_nict_rcv_4xx_cb);
	osip_set_message_callback(sip, OSIP_NICT_NOTIFY_SENT, SipCallback::osip_nict_send_notify_cb);
	osip_set_message_callback(sip, OSIP_NICT_SUBSCRIBE_SENT, SipCallback::osip_nict_send_subscribe_cb);
	osip_set_message_callback(sip, OSIP_NICT_REGISTER_SENT, SipCallback::osip_nict_send_register_cb);

	osip_set_message_callback(sip, OSIP_NIST_REGISTER_RECEIVED, SipCallback::osip_nist_rcv_register_cb);
	osip_set_message_callback(sip, OSIP_NIST_NOTIFY_RECEIVED, SipCallback::osip_nist_rcv_notify_cb);
	osip_set_message_callback(sip, OSIP_NIST_SUBSCRIBE_RECEIVED, SipCallback::osip_nist_rcv_subscribe_cb);
	osip_set_message_callback(sip, OSIP_NIST_BYE_RECEIVED, SipCallback::osip_nist_rcv_bye_cb);
	osip_set_message_callback(sip, OSIP_NIST_INFO_RECEIVED, SipCallback::osip_nist_rcv_info_cb);

	osip_set_message_callback(sip, OSIP_NIST_UNKNOWN_REQUEST_RECEIVED, SipCallback::osip_rcv_message_cb);

	// timeout callback
	osip_set_message_callback(sip, OSIP_NICT_STATUS_TIMEOUT, SipCallback::osip_nict_status_timeout_cb);

}

void SipServer::RegisterCommonCallbacks()
{
	// kill transaction callback
	osip_set_kill_transaction_callback(sip, OSIP_ICT_KILL_TRANSACTION, SipCallback::osip_ict_kill_transaction_cb);
	osip_set_kill_transaction_callback(sip, OSIP_IST_KILL_TRANSACTION, SipCallback::osip_ist_kill_transaction_cb);
	osip_set_kill_transaction_callback(sip, OSIP_NICT_KILL_TRANSACTION, SipCallback::osip_nict_kill_transaction_cb);
	osip_set_kill_transaction_callback(sip, OSIP_NIST_KILL_TRANSACTION, SipCallback::osip_nist_kill_transaction_cb);

	// transport error callback
	osip_set_transport_error_callback(sip, OSIP_ICT_TRANSPORT_ERROR, SipCallback::osip_ict_transport_error_cb);
	osip_set_transport_error_callback(sip, OSIP_IST_TRANSPORT_ERROR, SipCallback::osip_ist_transport_error_cb);
	osip_set_transport_error_callback(sip, OSIP_NICT_TRANSPORT_ERROR, SipCallback::osip_nict_transport_error_cb);
	osip_set_transport_error_callback(sip, OSIP_NIST_TRANSPORT_ERROR, SipCallback::osip_nist_transport_error_cb);
}

int SipServer::CheckPlatformStatus()
{
	SipServer* sipServer = SipServer::GetInstance();
	int timeout = 60 * 3; // 信令超时时间设为60*3秒
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(30));// 上下级在线状态检测间隔时间30s

		// 上下级平台状态检测
		sipServer->CheckLowerStatus(timeout);
		sipServer->CheckUpperStatus(timeout);

		// 更新Group表的ParentID字段
		sipServer->mSipDB->SetGroupParentID();
	}
	return 0;
}

void SipServer::CheckUpperStatus(int timeout)
{

}

void SipServer::CheckLowerStatus(int timeout)
{
}
