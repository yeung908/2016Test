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
		if (SipUtilities::CompareStrCase(trn->topvia->protocol, "TCP")) // TCP传输
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
		osip_message_t* regSipMsg = nullptr;
		if (sipMsg->status_code == 401)
		{
			// 创建带鉴权的SipMsg
			sipMsgBuilder.CreateAuthRegisterMsg(trn->orig_request, trn->last_response, regSipMsg);

			// 发送消息
			osip_event_t* regEvent = osip_new_outgoing_sipmessage(regSipMsg);
			sipServer->mSipEvents.push_back(regEvent);
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
	else if (SipUtilities::CompareStrCase(sipMsgMethod, "MESSAGE"))
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
			sipMsgBuilder.CreateForbiddenSipMsg(sipMsg, respSipMsg);
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
			sipMsgBuilder.CreateForbiddenSipMsg(sipMsg, respSipMsg);
			osip_event_t* event = osip_new_outgoing_sipmessage(respSipMsg);	// 根据消息创建消息事件
			if (event)
			{
				event->transactionid = trn->transactionid; // 设置事件的transactionid为本次回调transaction的id
				sipServer->mSipEvents.push_back(event);
			}
			return;
		}
	}
	
	//int expires = 0;
	SipMsgParser sipMsgParser;
	osip_header_t *expires = nullptr;
	osip_message_get_expires(sipMsg, 0, &expires);
	if (expires == nullptr) // 不存在expires字段
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
	} 
	else
	{
		// 查找registerDialog是否存在
		std::string fromSipID = sipMsg->from->url->username;
		osip_dialog_t* registerDialog = sipDialogs->FindRegisterDialog(fromSipID, DialogHint::UAS);

		SipMsgBuilder sipMsgBuilder;
		if (!strcmp(expires->hvalue, "0")) // 注销
		{
			sipDB->SetDeviceStatus(fromSipID, "OFF");// 设置平台是否在线状态为'OFF'
		
			osip_message_t* dstMsg = nullptr;
			if (registerDialog != nullptr) // 下级或设备已注册
			{				
				if (sipMsg->authorizations.node == nullptr) // 不带鉴权，401
				{
					sipMsgBuilder.CreateUnauthrizedMsg(sipMsg, dstMsg);
				}
				else // 带鉴权
				{
					if (true) // 鉴权验证通过，200
					{
						// 回复带data的200OK
						sipMsgBuilder.CreateRegister200OKSipMsg(sipMsg, dstMsg);
					}
					else // 鉴权验证未通过，403
					{
						sipMsgBuilder.CreateForbiddenSipMsg(sipMsg, dstMsg);
					}
				}
			} 
			else// 下级或设备未注册，回复404
			{
				sipMsgBuilder.CreateNotFoundSipMsg(sipMsg, dstMsg);
			}

			osip_event_t* event = osip_new_outgoing_sipmessage(dstMsg); // 根据消息创建消息事件
			event->transactionid = trn->transactionid;	// 设置事件的transactionid为本次回调transaction的id
			sipServer->mSipEvents.push_back(event);	// 将消息事件加入到本transaction待处理事件队列中
		}
		else // 注册
		{
			osip_message_t* dstMsg = nullptr;
			if (registerDialog != nullptr) // 平台已经注册,刷新注册
			{
				// 获取dialog的callID和fromTag
				std::string dialogCallID = registerDialog->call_id;
				std::string dialogFromTag = registerDialog->remote_tag;
				std::string dialogToTag = registerDialog->local_tag;

				// 获取sipMsg的callID和fromTag
				std::string sipMsgCallID = "";
				std::string sipMsgFromTag = "";
				//sipMessageObj.GetSipMsgCallIdAndFromTag(sipMsg, sipMsgCallID, sipMsgFromTag);

				// 判断平台是否已经注册
				if ((dialogCallID == sipMsgCallID) && (dialogFromTag == sipMsgFromTag)) // 已经注册平台刷新注册
				{ 
					if (sipMsg->authorizations.node == nullptr) // 不带鉴权，401
					{
						sipMsgBuilder.CreateUnauthrizedMsg(sipMsg, dstMsg, dialogToTag);
					}
					else // 带鉴权
					{
						if (true) // 鉴权验证通过，200
						{
							// 回复带data的200OK
							sipMsgBuilder.CreateRegister200OKSipMsg(sipMsg, dstMsg);

							// 设置平台状态为“ON”
							sipDB->SetDeviceStatus(fromSipID, "ON");

							// 创建registerDialog
							osip_dialog_t* newRegDialog = sipDialogs->CreateSipDialog(trn, dstMsg, REGISTER_DIALOG);
							sipDialogs->InsertSipDialog(newRegDialog, REGISTER_DIALOG);
						}
						else // 鉴权验证未通过，403
						{
							sipMsgBuilder.CreateForbiddenSipMsg(sipMsg, dstMsg);
						}
					}
				}
				else // 已注册平台异常退出，重新注册
				{
					DialogInfo* regDialogInfo = (DialogInfo*)registerDialog->your_instance;

					// 删除已经存在dialog
					sipDialogs->RemoveSipDialogByDialogID(regDialogInfo->dialogID, REGISTER_DIALOG);
					sipDialogs->DestorySipDialog(registerDialog);

					// 同 新平台第一次注册
					if (sipMsg->authorizations.node == nullptr) // 不带鉴权，401
					{
						sipMsgBuilder.CreateUnauthrizedMsg(sipMsg, dstMsg);
					}
					else // 带鉴权
					{
						if (true) // 鉴权验证通过，200
						{
							// 回复带data的200OK
							sipMsgBuilder.CreateRegister200OKSipMsg(sipMsg, dstMsg);

							// 设置平台状态为“ON”
							sipDB->SetDeviceStatus(fromSipID, "ON");

							// 创建registerDialog
							osip_dialog_t* newRegDialog = sipDialogs->CreateSipDialog(trn, dstMsg, REGISTER_DIALOG);
							sipDialogs->InsertSipDialog(newRegDialog, REGISTER_DIALOG);
						}
						else // 鉴权验证未通过，403
						{
							sipMsgBuilder.CreateForbiddenSipMsg(sipMsg, dstMsg);
						}
					}
				}
			} 
			else // 平台第一次向本级注册
			{
				if (sipMsg->authorizations.node == nullptr) // 不带鉴权，401
				{
					sipMsgBuilder.CreateUnauthrizedMsg(sipMsg, dstMsg);
				} 
				else // 带鉴权
				{
					if (true) // 鉴权验证通过，200
					{
						// 回复带data的200OK
						sipMsgBuilder.CreateRegister200OKSipMsg(sipMsg, dstMsg);

						// 设置平台状态为“ON”
						sipDB->SetDeviceStatus(fromSipID, "ON");

						// 创建registerDialog
						osip_dialog_t* newRegDialog = sipDialogs->CreateSipDialog(trn, dstMsg, REGISTER_DIALOG);
						sipDialogs->InsertSipDialog(newRegDialog, REGISTER_DIALOG);
					} 
					else // 鉴权验证未通过，403
					{
						sipMsgBuilder.CreateForbiddenSipMsg(sipMsg, dstMsg);
					}
				}
			}
			osip_event_t* event = osip_new_outgoing_sipmessage(dstMsg); // 根据消息创建消息事件
			event->transactionid = trn->transactionid;	// 设置事件的transactionid为本次回调transaction的id
			sipServer->mSipEvents.push_back(event);	// 将消息事件加入到本transaction待处理事件队列中
		}
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
		osip_event_t* badRequestEvent = osip_new_outgoing_sipmessage(respSipMsg);	// 根据消息创建消息事件
		if (badRequestEvent)
		{
			badRequestEvent->transactionid = trn->transactionid; // 设置事件的transactionid为本次回调transaction的id
			sipServer->mSipEvents.push_back(badRequestEvent);
		}
		return;
	}

	if (SipUtilities::CompareStrCase("Control", sipMsgParser.mXmlParam.reqRespType.c_str()))
	{
		if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_DEVICE_PTZ || // PTZ控制信息，无应答控制
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_TELE_BOOT ||  // 远程重启，无应答控制
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_IFAME ||      // 强制关键帧，无应答控制
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_DRAG_ZOOM)    // 拉框放大、缩小，无应答控制
		{	// 无应答控制：云台、远程重启、强制关键帧、拉框放大、拉框缩小
			bool isUpper = true;
			if (isUpper) // 本级上级
			{
				// 向上级回复200OK

				bool is28181 = true;
				int platformID = 1;
				if (platformID == 1 && !is28181)// 本级非国标设备
				{
				}
				else // 本级国标设备或下级设备
				{
				}
			} 
			else // 非本级上级
			{
			}
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_RECORD || // 手动录像、手动停止录像，有应答控制
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_GUARD ||  // 报警布防、撤防，有应答控制
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_ALARM ||  // 报警复位，有应答控制
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_HOME_POSITION || // 看守位，有应答控制
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_DEVICE_CONFIG)   // 设备配置，有应答控制
		{// 有应答控制：录像控制、报警布防/撤防、报警复位、看守位控制、设备配置

			bool isUpper = true;
			if (isUpper) // 本级上级
			{
				// 向上级回复200OK

				// 判断设备所属的域和设备类型
				bool is28181 = true;
				int platformID = 1;
				if (platformID == 1 && !is28181)// 本级非国标设备
				{
					switch (sipMsgParser.mXmlParam.manscdpType)// 与中心信令通信，向上级响应
					{
					case CONTROL_RECORD:
						break;
					case CONTROL_GUARD:
						break;
					case CONTROL_ALARM:
						break;
					case CONTROL_HOME_POSITION:
						break;
					case CONTROL_DEVICE_CONFIG:
						break;
					default:
						break;
					}
				}
				else // 本级28181国标设备或下级设备
				{
					// 向下级转发

					// 记录信息
					switch (sipMsgParser.mXmlParam.manscdpType)
					{
					case CONTROL_DEVICE_CONFIG: // 设备配置设置
						//xmlDialog.cmdType = RESPONSE_DEVICE_CONFIG; // 设备配置响应
						break;
					default:// 设备控制
						//xmlDialog.cmdType = RESPONSE_DEVICE_CONTROL; // 设备控制响应
						break;
					}
				}
			}
			else // 非本级上级
			{
			}
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
			bool isUpper = true;
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* respSipMsg = nullptr;

			if (isUpper) // 上级
			{				
				sipMsgBuilder.Create200OKSipMsg(sipMsg, respSipMsg);// 向下级回复200OK
				sipDB->SetDeviceStatus(sipMsg->from->url->username, "ON");

				// 创建完整的目录组织架构并发送第一条目录信息
				std::string protol = "";
				protol = trn->topvia->protocol;
				//StartResponseCatalog(fromSipID, sipMessageObj.mXmlCmdInfo.deviceID, sipMessageObj.mXmlCmdInfo.sn, protol);
			}
			else // 非上级
			{
				sipMsgBuilder.CreateForbiddenSipMsg(sipMsg, respSipMsg);// Forbidden
				sipDB->SetDeviceStatus(sipMsg->from->url->username, "OFF");
			}
			osip_event_t* keepaliveEvent = osip_new_outgoing_sipmessage(respSipMsg); // 根据消息创建消息事件
			if (keepaliveEvent)
			{
				keepaliveEvent->transactionid = trn->transactionid;	// 设置事件的transactionid为本次回调transaction的id
				sipServer->mSipEvents.push_back(keepaliveEvent);    // 将消息事件加入到本transaction待处理事件队列中
			}
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_RECORD_INFO) // 历史视音频文件检索
		{
			bool isUpper = true;
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* respSipMsg = nullptr;
			if (isUpper) // 向上级回复200OK
			{
				sipMsgBuilder.Create200OKSipMsg(sipMsg, respSipMsg);// 向下级回复200OK
				osip_event_t* recordEvent = osip_new_outgoing_sipmessage(respSipMsg); // 根据消息创建消息事件
				if (recordEvent)
				{
					recordEvent->transactionid = trn->transactionid;	// 设置事件的transactionid为本次回调transaction的id
					sipServer->mSipEvents.push_back(recordEvent);		// 将消息事件加入到本transaction待处理事件队列中
				}
			}

			// 检查是集中存储还是前段设备存储
			bool isDeviceStore = true;
			if (isDeviceStore) // 前端设备存储
			{
				// 向设备、下级转发

				// 记录查询信息

			} 
			else // 集中存储、查询数据库
			{
			}
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
			osip_dialog_t* registerDialog = nullptr;
			registerDialog = sipDialogs->FindRegisterDialog(sipMsg->from->url->username, DialogHint::UAS);

			SipMsgBuilder sipMsgBuilder;
			osip_message_t* respSipMsg = nullptr;
			if (registerDialog != nullptr) // 下级已经注册
			{
				// 更新LastResponseTime
				DialogInfo* dialogInfo = (DialogInfo*)(registerDialog->your_instance);
				dialogInfo->lastResponseTime = time(NULL);				
				sipMsgBuilder.Create200OKSipMsg(sipMsg, respSipMsg);// 向下级回复200OK
				sipDB->SetDeviceStatus(sipMsg->from->url->username, "ON");
			} 
			else // 下级未注册
			{				
				sipMsgBuilder.CreateForbiddenSipMsg(sipMsg, respSipMsg);// Forbidden
				sipDB->SetDeviceStatus(sipMsg->from->url->username, "OFF");
			}

 			osip_event_t* keepaliveEvent = osip_new_outgoing_sipmessage(respSipMsg); // 根据消息创建消息事件
			if (keepaliveEvent)
			{
				keepaliveEvent->transactionid = trn->transactionid;	// 设置事件的transactionid为本次回调transaction的id
				sipServer->mSipEvents.push_back(keepaliveEvent);    // 将消息事件加入到本transaction待处理事件队列中
			}
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_ALARM)
		{// 收到报警通知，立即回复200OK，再回复报警响应，向目标设备转发报警通知，报警通知处进行处理
			// 将报警信息写入到数据库

			// 向发送方回复200OK
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* okSipMsg = nullptr;
			sipMsgBuilder.Create200OKSipMsg(sipMsg, okSipMsg);
			osip_event_t* okEvent = osip_new_outgoing_sipmessage(okSipMsg); // 根据消息创建消息事件
			if (okEvent)
			{
				okEvent->transactionid = trn->transactionid;	// 设置事件的transactionid为本次回调transaction的id
				sipServer->mSipEvents.push_back(okEvent);		// 将消息事件加入到本transaction待处理事件队列中
			}

			// 向下级回复报警响应
			osip_message_t* responseBroadcastNotify = nullptr;
			sipMsgBuilder.mXmlParam.manscdpType = RESPONSE_ALARM_NOTIFY;
			sipMsgBuilder.mXmlParam.deviceID = sipMsgParser.mXmlParam.deviceID;
			sipMsgBuilder.mXmlParam.sn = sipMsgParser.mXmlParam.sn;
			sipMsgBuilder.mXmlParam.responseParam.result = "OK";
			sipMsgBuilder.CreateSipMsgXml(responseBroadcastNotify, sipMsg->from->url->username);
			osip_event_t* respAlarmNotifyEvent = osip_new_outgoing_sipmessage(responseBroadcastNotify);
			if (respAlarmNotifyEvent)
			{
				sipServer->mSipEvents.push_back(respAlarmNotifyEvent); // 将消息事件加入到本transaction待处理事件队列中
			}

			// 向上级回复报警通知
			if (true)
			{
			} 
			else //向客户端回复报警通知
			{
			}

		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_MEDIA_STATUS)
		{
			// 向发送方回复200OK
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* okSipMsg = nullptr;
			sipMsgBuilder.Create200OKSipMsg(sipMsg, okSipMsg);
			osip_event_t* okEvent = osip_new_outgoing_sipmessage(okSipMsg); // 根据消息创建消息事件
			if (okEvent)
			{
				okEvent->transactionid = trn->transactionid;	// 设置事件的transactionid为本次回调transaction的id
				sipServer->mSipEvents.push_back(okEvent);		// 将消息事件加入到本transaction待处理事件队列中
			}

			// 向上级/客户端发送MediaStutas
			osip_dialog_t* playbackDialog = nullptr;
			DialogInfo* playbackDialogInfo = nullptr;
			if (playbackDialog != nullptr)// 此历史视频调看/下载存在
			{
				playbackDialogInfo = (DialogInfo*)playbackDialog->your_instance;

				// 查找与上级建立的dialog
				osip_dialog_t* callerDialog = nullptr;
				DialogInfo* callerDialogInfo = nullptr;
				int callerDialogID = playbackDialogInfo->callerDialogID;

				callerDialog = sipDialogs->FindSipDialogByDialogID(callerDialogID, DialogType::PLAYBACK_DIALOG);
				if (callerDialog)// 本平台非级联平台最上级，则向上级平台转发fileToEnd
				{
					callerDialogInfo = (DialogInfo*)callerDialog->your_instance;

				} 
				else // 向streaming发送fileToEnd,本平台为级联平台最上级，向下级发送bye
				{
					// 与streaming通信

					// 向下级发送bye？
				}
			}
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_BROADCAST)
		{
			// 向发送方回复200OK
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* okSipMsg = nullptr;
			sipMsgBuilder.Create200OKSipMsg(sipMsg, okSipMsg);
			osip_event_t* okEvent = osip_new_outgoing_sipmessage(okSipMsg); // 根据消息创建消息事件
			if (okEvent)
			{
				okEvent->transactionid = trn->transactionid;	// 设置事件的transactionid为本次回调transaction的id
				sipServer->mSipEvents.push_back(okEvent);		// 将消息事件加入到本transaction待处理事件队列中
			}

			// 向发送方回复语音广播应答
			osip_message_t* responseBroadcastNotify = nullptr;
			sipMsgBuilder.mXmlParam.manscdpType = RESPONSE_BROADCAST;
			sipMsgBuilder.mXmlParam.deviceID = sipMsgParser.mXmlParam.notifyParam.broadcast.targetID; // 语音输出设备编码
			sipMsgBuilder.mXmlParam.responseParam.result = "OK";
			sipMsgBuilder.CreateSipMsgXml(responseBroadcastNotify, sipMsg->from->url->username);
			osip_event_t* respBroadcastNotifyEvent = osip_new_outgoing_sipmessage(responseBroadcastNotify);
			if (respBroadcastNotifyEvent)
			{
				sipServer->mSipEvents.push_back(respBroadcastNotifyEvent); // 将消息事件加入到本transaction待处理事件队列中
			}
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_MOBILE_POSITION)
		{// 在订阅通知收到订阅信息处处理
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_CATALOG)
		{// 在订阅通知收到订阅信息处处理
		}
	} 
	else if (SipUtilities::CompareStrCase("Response", sipMsgParser.mXmlParam.reqRespType.c_str()))
	{
		if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONTROL || // 有应答设备控制，向上级转发，向下级回复
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONFIG)    // 有应答设备控制，设备配置
		{ 
			// 向下级回复200OK
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* okSipMsg = nullptr;
			sipMsgBuilder.Create200OKSipMsg(sipMsg, okSipMsg);
			osip_event_t* okEvent = osip_new_outgoing_sipmessage(okSipMsg); // 根据消息创建消息事件
			if (okEvent)
			{
				okEvent->transactionid = trn->transactionid;	// 设置事件的transactionid为本次回调transaction的id
				sipServer->mSipEvents.push_back(okEvent);		// 将消息事件加入到本transaction待处理事件队列中
			}

			// 查找xmlDialog
			//XmlDialog xmlDialog;
			//xmlDialog.cmdType = sipMessageObj.mXmlCmdInfo.manscdpType;
			//xmlDialog.dstDeviceID = sipMessageObj.mXmlCmdInfo.deviceID;

			//if (!sipServer->GetXmlCmdDialog(xmlDialog))// 上级级发起的控制信令，向上级转发控制应答
			//{
			//	osip_message_t* dstMsg = nullptr;
			//	sipMessageObj.CreateSipMsgCloneBody(sipMsg, dstMsg, xmlDialog.srcDeviceID, xmlDialog.toTag, xmlDialog.fromTag);
			//	osip_event_t* event = osip_new_outgoing_sipmessage(dstMsg);
			//	sipServer->mSipEvents.push_back(event);
			//}
			//return;
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CATALOG_QUERY ||  // 收到下级回复的目录信息
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_CATALOG_QUERY_RECEIVED|| // 收到下级目录查询响应
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_CATALOG_RECEIVED ||      // 收到下级目录信息响应
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_BROADCAST ||             // 语音广播应答
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_ALARM_NOTIFY)
		{
			// 向发送方回复200OK
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* okSipMsg = nullptr;
			sipMsgBuilder.Create200OKSipMsg(sipMsg, okSipMsg);
			//SipMsgHeader::SetSipMsgViaProtocal(responseMsg, trn->topvia->protocol);
			osip_event_t* okEvent = osip_new_outgoing_sipmessage(okSipMsg); // 根据消息创建消息事件
			if (okEvent)
			{
				okEvent->transactionid = trn->transactionid;	// 设置事件的transactionid为本次回调transaction的id
				sipServer->mSipEvents.push_back(okEvent);		// 将消息事件加入到本transaction待处理事件队列中
			}
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_INFO_QUERY ||     // 收到下级设备信息查询响应
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_STATUS_QUERY ||   // 收到下级设备状态查询响应
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONFIG_DOWNLOAD|| // 设备配置查询应答
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_PRESET_QUERY ||   // 预置位查询应答
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DOCUMENT_RECORD_INFO)    // 历史文件查询
		{
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* okSipMsg = nullptr;
			sipMsgBuilder.Create200OKSipMsg(sipMsg, okSipMsg);
			//SipMsgHeader::SetSipMsgViaProtocal(responseMsg, trn->topvia->protocol);
			osip_event_t* okEvent = osip_new_outgoing_sipmessage(okSipMsg); // 根据消息创建消息事件
			if (okEvent)
			{
				okEvent->transactionid = trn->transactionid;	// 设置事件的transactionid为本次回调transaction的id
				sipServer->mSipEvents.push_back(okEvent);		// 将消息事件加入到本transaction待处理事件队列中
			}

			// 查找xmlDialog
			//XmlDialog xmlDialog;
			switch (sipMsgParser.mXmlParam.manscdpType)
			{
			case RESPONSE_DEVICE_INFO_QUERY:
				//xmlDialog.cmdType = QUERY_DEVICE_INFO;
				break;
			case RESPONSE_DEVICE_STATUS_QUERY:
				//xmlDialog.cmdType = QUERY_DEVICE_STATUS;
				break;
			case RESPONSE_DEVICE_CONFIG_DOWNLOAD:
				//xmlDialog.cmdType = QUERY_CONFIG_DOWNLOAD;
				break;
			case RESPONSE_DEVICE_PRESET_QUERY:
				//xmlDialog.cmdType = QUERY_PRESET_QUERY;
				break;
			case RESPONSE_DOCUMENT_RECORD_INFO:
				//xmlDialog.cmdType = QUERY_RECORD_INFO;
				break;
			}

			if (true) // 上级发起查询，向上级转发
			{
			} 
			else // 客户端发起请求
			{
				switch (sipMsgParser.mXmlParam.manscdpType)
				{
				case RESPONSE_DEVICE_INFO_QUERY:
				{
					break;
				}					
				case RESPONSE_DEVICE_STATUS_QUERY:
				{
					break;
				}
				case RESPONSE_DEVICE_CONFIG_DOWNLOAD:
				{
					break;
				}
				case RESPONSE_DEVICE_PRESET_QUERY:
				{
					break;
				}
				case RESPONSE_DOCUMENT_RECORD_INFO:
				{
					break;
				}
				}
			}
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
