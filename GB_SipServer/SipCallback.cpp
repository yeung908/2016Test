#include "SipCallback.h"

int SipCallback::osip_send_message_cb(osip_transaction_t * trn, osip_message_t * sip_msg, char * dest_socket_str, int dest_prot, int send_sock)
{
	char* message = nullptr;
	osip_message_to_str(sip_msg, &message, 0);
	std::string ss = message;
	if (message)
	{
		if (strlen(message) < 100)
		{
			std::cout << "错误的sip消息，无法发送！！！" << std::endl;
			return -1;
		}

		// 判断是UDP发送还是TCP发送
		int type = 0;
		if (!strcmp(trn->topvia->protocol, "TCP")) // TCP传输
		{
			type = 1;
		}

		SipServer* sipServer = SipServer::GetInstance();
		//sipServer->mSocketServer->SendSipMsg(message, dest_socket_str, dest_port, type);

		// 释放message内存
		osip_free(message);
	}
}

void SipCallback::osip_nict_rcv_2xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
	if (trn == nullptr || trn->your_instance == nullptr || sipMsg == nullptr)
	{
		return;
	}
	
	SipServer* sipServer = SipServer::GetInstance();
	SipDialogs* sipDialogs = sipServer->mSipDialogs;
	SipDatabase* sipDB = sipServer->mSipDB;
	AppContext* appContext = (AppContext*)trn->your_instance;

	SipMsgParser sipMsgParser;
	const char* sipMsgMethod = trn->orig_request->sip_method;
	if (SipUtilities::CompareStrCase(sipMsgMethod, "REGISTER")) // 注册、注销
	{
		std::string expire = "";
		sipMsgParser.GetSipMsgExpires(trn->orig_request, expire);
		if (expire == "0") // 注销
		{
			osip_dialog_t* registerDialog = nullptr;
			registerDialog = sipDialogs->FindRegisterDialog(trn->orig_request->req_uri->username, DialogHint::UAC);

			if (registerDialog != nullptr)
			{
				DialogInfo* registerDialogInfo = (DialogInfo*)registerDialog->your_instance;

				// 停止上级调看的所有视频
				osip_dialog_t* upperInviteDialog = nullptr;
				DialogInfo* upperInviteDialogInfo = nullptr;
				for each (int upperInviteDialogID in registerDialogInfo->inviteDialogIDs)
				{
					upperInviteDialog = sipServer->mSipDialogs->FindSipDialogByDialogID(upperInviteDialogID, DialogType::PLAY_INVITE_DIALOG);
					if (upperInviteDialog)
					{
						upperInviteDialogInfo = (DialogInfo*)upperInviteDialog->your_instance;

						// 向上级发送bye


						// 向下级发送bye

					}
				}

				// 删除已存在注册dialog
				sipDialogs->RemoveSipDialogByDialogID(registerDialogInfo->dialogID, REGISTER_DIALOG);
				sipDialogs->DestorySipDialog(registerDialog);
			}

			// 将上级状态设置为‘OFF’
			sipDB->SetDeviceStatus(trn->orig_request->req_uri->username, "OFF");
			std::cout << "注销成功..." << std::endl;
		} 
		else // 注册
		{
			osip_dialog_t* registerDialog = nullptr;
			registerDialog = sipDialogs->FindRegisterDialog(trn->orig_request->req_uri->username, DialogHint::UAC);

			if (registerDialog == nullptr) // 首次注册、异常退出重启注册
			{				
				registerDialog = sipDialogs->CreateSipDialog(trn, sipMsg, REGISTER_DIALOG);// 创建dialog
				sipDialogs->InsertSipDialog(registerDialog, REGISTER_DIALOG);
			} 
			else // 有效期结束，重新注册需要更新birthTime和cesqNum
			{
				DialogInfo* dialogInfo = (DialogInfo*)registerDialog->your_instance;
				dialogInfo->lastResponseTime = time(NULL);
				dialogInfo->birthTime = time(NULL);

				//registerDialog->local_cseq = std::stoi(trn->orig_request->cseq->number);
				++registerDialog->local_cseq;
			}

			// 将上级状态设置为‘ON’
			sipDB->SetDeviceStatus(trn->orig_request->req_uri->username, "ON");
			std::cout << "注册成功..." << std::endl;
		}
	}
	else if (SipUtilities::CompareStrCase(sipMsgMethod, "BYE"))
	{
		
	}
	else if (SipUtilities::CompareStrCase(sipMsgMethod, "MESSAGE"))
	{
	}
	else if (SipUtilities::CompareStrCase(sipMsgMethod, "SUBSCRIBE")) // 订阅
	{
	}
	else if (SipUtilities::CompareStrCase(sipMsgMethod, "INFO")) // 历史控制
	{
	}
}

void SipCallback::osip_nict_rcv_3xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nict_rcv_4xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
	if (trn == nullptr || trn->your_instance == nullptr || sipMsg == nullptr)
	{
		return;
	}

	SipServer* sipServer = SipServer::GetInstance();
	SipDialogs* sipDialogs = sipServer->mSipDialogs;
	SipDatabase* sipDB = sipServer->mSipDB;
	AppContext* appContext = (AppContext*)trn->your_instance;

	SipMsgParser sipMsgParser;
	const char* sipMsgMethod = trn->orig_request->sip_method;
	if (SipUtilities::CompareStrCase(sipMsgMethod, "REGISTER")) // 注册、注销
	{
		SipMsgBuilder sipMsgBuilder;
		osip_message_t* resSipMsg = nullptr;
		if (sipMsg->status_code == 401)
		{
			// 创建带鉴权的SipMsg
			sipMsgBuilder.CreateAuthRegisterMsg(trn->orig_request, trn->last_response, resSipMsg);

			// 发送消息
			osip_event_t* event = osip_new_outgoing_sipmessage(resSipMsg);
			sipServer->mSipEvents.push_back(event);
		}
		else
		{
			std::string expires = "";
			sipMsgParser.GetSipMsgExpires(trn->orig_request, expires);
			if (expires == "0")
			{
				std::cout << "注销失败！！！" << std::endl;
			}
			else
			{
				std::cout << "注册失败！！！" << std::endl;
			}
		}
	}
	else
	{

	}
}

void SipCallback::osip_nict_send_register_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nict_send_notify_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nict_send_subscribe_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ict_rcv_2xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ict_rcv_3xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ict_rcv_45xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ict_send_invite_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ist_rcv_invite_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ist_rcv_ack_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nist_rcv_register_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
	if (trn == nullptr || trn->your_instance == nullptr || sipMsg == nullptr)
	{
		return;
	}

	SipServer* sipServer = SipServer::GetInstance();
	SipDialogs* sipDialogs = sipServer->mSipDialogs;
	SipDatabase* sipDB = sipServer->mSipDB;
	AppContext* appContext = (AppContext*)trn->your_instance;

	// 判断是平台还是设备
	bool isPlatform = true;
	if (isPlatform)
	{
		// 非本级的下级平台或互联平台直接回复403
		int connectState = 0;
		sipDB->GetIntData("", connectState);
		if (connectState != 2 && connectState != 3)
		{
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* respSipMsg = nullptr;
			sipMsgBuilder.CreateBadRequestSipMsg(sipMsg, respSipMsg);
			osip_event_t* event = osip_new_outgoing_sipmessage(respSipMsg);	// 根据消息创建消息事件
			if (event)
			{
				event->transactionid = trn->transactionid; // 设置事件的transactionid为本次回调transaction的id
				sipServer->mSipEvents.push_back(event);
			}
			return;
		}
	} 
	else // 后期需要补充28181设备检验
	{
		// ...
		if (true) // 非本级平台所连设备
		{
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* respSipMsg = nullptr;
			sipMsgBuilder.CreateBadRequestSipMsg(sipMsg, respSipMsg);
			osip_event_t* event = osip_new_outgoing_sipmessage(respSipMsg);	// 根据消息创建消息事件
			if (event)
			{
				event->transactionid = trn->transactionid; // 设置事件的transactionid为本次回调transaction的id
				sipServer->mSipEvents.push_back(event);
			}
			return;
		}
	}
	
	SipMsgParser sipMsgParser;
	if (0) // 注销
	{
	} 
	else // 注册
	{
	}
}

void SipCallback::osip_nist_rcv_notify_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nist_rcv_subscribe_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nist_rcv_bye_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nist_rcv_info_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_rcv_message_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
	// 判断sip消息等是否存在
	if (trn == nullptr || trn->your_instance == nullptr || sipMsg == nullptr)
	{
		return;
	}
	
	SipServer* sipServer = SipServer::GetInstance();
	SipDialogs* sipDialogs = sipServer->mSipDialogs;
	SipDatabase* sipDB = sipServer->mSipDB;
	AppContext* appContext = (AppContext*)trn->your_instance;

	SipMsgParser sipMsgParser;
	if (sipMsgParser.ParserSipMsgBody(sipMsg)) // 解析消息体失败
	{
		osip_message_t* respSipMsg = nullptr;
		sipMsgParser.CreateBadRequestSipMsg(sipMsg, respSipMsg);
		osip_event_t* event = osip_new_outgoing_sipmessage(respSipMsg);	// 根据消息创建消息事件
		if (event)
		{
			event->transactionid = trn->transactionid; // 设置事件的transactionid为本次回调transaction的id
			sipServer->mSipEvents.push_back(event);
		}
		return;
	}

	if (SipUtilities::CompareStrCase("Control", sipMsgParser.mXmlParam.reqRespType.c_str()))
	{
		if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_DEVICE_PTZ || // PTZ控制信息，无应答控制
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_TELE_BOOT ||  // 远程重启，无应答控制
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_IFAME ||      // 强制关键帧，无应答控制
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_DRAG_ZOOM)    // 拉框放大、缩小，无应答控制
		{
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_RECORD || // 手动录像、手动停止录像，有应答控制
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_GUARD ||  // 报警布防、撤防，有应答控制
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_ALARM ||  // 报警复位，有应答控制
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_HOME_POSITION || // 看守位，有应答控制
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_DEVICE_CONFIG)   // 设备配置，有应答控制
		{
		}
	} 
	else if (SipUtilities::CompareStrCase("Query", sipMsgParser.mXmlParam.reqRespType.c_str()))
	{
		if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_DEVICE_STATUS ||   // 设备状态查询命令
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_DEVICE_INFO ||     // 设备信息查询命令
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_CONFIG_DOWNLOAD || // 设备配置查询
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_PRESET_QUERY)      // 预置位查询
		{
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_DEVICE_CATALOG)// 收到上级查询本级目录
		{

		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_RECORD_INFO) // 历史视音频文件检索
		{

		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_ALARM) // 报警信息订阅处
		{

		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_MOBILE_POSITION) // 移动设备信息订阅处
		{

		}
	} 
	else if (SipUtilities::CompareStrCase("Notify", sipMsgParser.mXmlParam.reqRespType.c_str()))
	{
		if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_KEEPALIVE)
		{
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_ALARM)
		{
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_MEDIA_STATUS)
		{
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_BROADCAST)
		{
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_MOBILE_POSITION)
		{
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_CATALOG)
		{
		}
	} 
	else if (SipUtilities::CompareStrCase("Response", sipMsgParser.mXmlParam.reqRespType.c_str()))
	{
		if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONTROL || // 有应答设备控制，向上级转发，向下级回复
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONFIG)    // 有应答设备控制，设备配置
		{ 
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CATALOG_QUERY ||  // 收到下级回复的目录信息
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_CATALOG_QUERY_RECEIVED|| // 收到下级目录查询响应
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_CATALOG_RECEIVED ||      // 收到下级目录信息响应
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_BROADCAST ||             // 语音广播应答
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_ALARM_NOTIFY)
		{
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_INFO_QUERY ||     // 收到下级设备信息查询响应
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_STATUS_QUERY ||   // 收到下级设备状态查询响应
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONFIG_DOWNLOAD|| // 设备配置查询应答
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_PRESET_QUERY ||   // 预置位查询应答
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DOCUMENT_RECORD_INFO)    // 历史文件查询
		{
		}
	}
}

void SipCallback::osip_nict_status_timeout_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ict_kill_transaction_cb(int type, osip_transaction_t * trn)
{
}

void SipCallback::osip_ist_kill_transaction_cb(int type, osip_transaction_t * trn)
{
}

void SipCallback::osip_nict_kill_transaction_cb(int type, osip_transaction_t * trn)
{
}

void SipCallback::osip_nist_kill_transaction_cb(int type, osip_transaction_t * trn)
{
}

void SipCallback::osip_ict_transport_error_cb(int type, osip_transaction_t *, int error)
{
}

void SipCallback::osip_ist_transport_error_cb(int type, osip_transaction_t *, int error)
{
}

void SipCallback::osip_nict_transport_error_cb(int type, osip_transaction_t *, int error)
{
}

void SipCallback::osip_nist_transport_error_cb(int type, osip_transaction_t *, int error)
{
}
