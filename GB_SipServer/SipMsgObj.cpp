#include "SipMsgObj.h"

SipDatabase* SipMsgObj::mSipDB = new SipDatabase;

SipMsgObj::SipMsgObj()
{
	mSipMsgHeader = new SipMsgHeader;
	mSipMsgBody = nullptr;
}


SipMsgObj::~SipMsgObj()
{
	if (mSipMsgHeader)
	{
		delete mSipMsgHeader;
	}

	if (mSipMsgBody)
	{
		delete mSipMsgBody;
	}
}

int SipMsgObj::GetLocalPlatformInfo()
{
	return 0;
}

int SipMsgObj::GetRemotePlatformInfo(std::string remoteDeviceID)
{
	return 0;
}

int SipMsgObj::CreateRegisterMsg(osip_message_t *& dstSipMsg, const std::string remoteSipID, const std::string expires)
{
	int result = -1;

	if (dstSipMsg != nullptr)
	{
		osip_message_free(dstSipMsg);
		dstSipMsg = nullptr;
	}

	osip_message_init(&dstSipMsg);
	GetLocalPlatformInfo();
	GetRemotePlatformInfo(remoteSipID);

	// start-line
	result = SetSipMsgRequestStartLine(dstSipMsg, "REGISTER");
	if (!result)
	{
		// via
		mSipMsgHeader->SetSipMsgVia(dstSipMsg, mLocalIP, mLocalPort, SipUtilities::CreatUuid());
		// from
		mSipMsgHeader->SetSipMsgFrom(dstSipMsg, mLocalSipID, mLocalSipDomain, SipUtilities::CreatUuid());
		// to
		mSipMsgHeader->SetSipMsgTo(dstSipMsg, mLocalSipID, mLocalSipDomain, "");
		// callid
		mSipMsgHeader->SetSipMsgCallID(dstSipMsg, mLocalIP, SipUtilities::CreatUuid());
		// Cseq
		mSipMsgHeader->SetSipMsgCseq(dstSipMsg, "1", dstSipMsg->sip_method);
		// contact
		mSipMsgHeader->SetSipMsgContact(dstSipMsg, mLocalSipID, mLocalIP, mLocalPort);
		// maxForward
		mSipMsgHeader->SetSipMsgMaxForward(dstSipMsg);
		// Expires
		mSipMsgHeader->SetSipMsgExpires(dstSipMsg, expires);
		// User-Agent
		mSipMsgHeader->SetSipMsgUserAgent(dstSipMsg);
	}
	else
	{
		osip_message_free(dstSipMsg);
		dstSipMsg = nullptr;
	}
	return result;
}

int SipMsgObj::CreateRegMsgAgain(osip_message_t *& dstSipMsg, const std::string remoteSipID, const std::string callID, const std::string fromTag, const std::string toTag, const std::string cesq)
{
	CreateRegisterMsg(dstSipMsg, remoteSipID);

	mSipMsgHeader->SetSipMsgFromTag(dstSipMsg, fromTag);
	mSipMsgHeader->SetSipMsgToTag(dstSipMsg, toTag);
	mSipMsgHeader->SetSipMsgCallID(dstSipMsg, callID);
	mSipMsgHeader->SetSipMsgCseq(dstSipMsg, cesq, dstSipMsg->sip_method);
	return 0;
}

int SipMsgObj::CreateUnRegistrtMsg(osip_message_t *& dstSipMsg, const std::string remoteSipID, const std::string expires)
{
	CreateRegisterMsg(dstSipMsg, remoteSipID, expires);
	return 0;
}

int SipMsgObj::CreateAuthRegisterMsg(const osip_message_t * orgRequest, const osip_message_t * lastResponse, osip_message_t *& dstSipMsg)
{
	int result = -1;
	if (orgRequest != nullptr)
	{		
		result = osip_message_clone(orgRequest, &dstSipMsg);// 复制SipMsg消息
	}

	if (!result)
	{		
		mSipMsgHeader->SetSipMsgViaBranch(dstSipMsg, SipUtilities::CreatUuid());// 修改branch		
		mSipMsgHeader->SetSipMsgTo(dstSipMsg, lastResponse->to);// toTag		
		
		// 修改CseqNumber
		int cseq = 2;
		if (dstSipMsg->cseq && dstSipMsg->cseq->number)
		{
			SipUtilities::Str2Int(dstSipMsg->cseq->number, cseq);
			osip_free(dstSipMsg->cseq->number);
		}
		dstSipMsg->cseq->number = osip_strdup(std::to_string(cseq).c_str());

		// Authorization
		std::string password = mSipDB->GetUpperPassword(orgRequest->req_uri->username);
		result = mSipMsgHeader->SetSipMsgAuthorization(dstSipMsg, lastResponse->www_authenticates, password);
	}
	return 0;
}

int SipMsgObj::CreateUnauthrizedMsg(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg, const std::string toTag)
{
	if (srcSipMsg == nullptr)
	{
		return -1;
	}

	if (osip_message_init(&dstSipMsg))
	{
		return -1;
	}

	// start-line
	SetSipMsgResponseStartLine(dstSipMsg, 401, "Unauthorized");

	// via、from、to、call-id、CSeq、Contact、max-forward
	CreateNewSipMsg(srcSipMsg, dstSipMsg, NIST);

	// toTag
	if (toTag.size() != 0)
	{
		mSipMsgHeader->SetSipMsgToTag(dstSipMsg, toTag);
	}

	// www-Authenticate
	std::string realm = "";
	realm = "\"" + mLocalSipDomain + "\"";
	std::string nonce = "";
	nonce = "\"" + SipUtilities::RandLengthStr(10) + "\"";
	mSipMsgHeader->SetSipMsgWWW_Authrnticate(dstSipMsg, realm, nonce);

	return 0;
}

int SipMsgObj::CreateSipMsgXml(osip_message_t *& dstSipMsg, std::string remoteDeviceID)
{
	if (dstSipMsg)
	{
		osip_message_free(dstSipMsg);
		dstSipMsg = nullptr;
	}

	osip_message_init(&dstSipMsg);

	// 获取本平台相关信息
	GetLocalPlatformInfo();

	// 获取对方平台相关信息
	GetRemotePlatformInfo(remoteDeviceID);

	// start-line
	SetSipMsgRequestStartLine(dstSipMsg, "MESSAGE");

	// via、from、to、call-id、CSeq、Contact、max-forward
	CreateNewSipMsg(nullptr, dstSipMsg, NICT);

	// Content_Type
	mSipMsgHeader->SetSipMsgContentType(dstSipMsg, "Application/MANSCDP+xml");

	// Body
	mXmlParam.localSipID = mLocalSipID;
	mXmlParam.remoteSipID = mRemoteSipID;

	mSipMsgBody = new SipMsgXmlBody;
	mSipMsgBody->CreateSipMsgBody(&mXmlParam, mXmlParam.bodyString);
	delete mSipMsgBody;
	mSipMsgBody = nullptr;
	return 0;
}

int SipMsgObj::SetSipMsgProtol(osip_message_t * dstSipMsg, std::string protol)
{
	return mSipMsgHeader->SetSipMsgViaProtocal(dstSipMsg, protol);;
}

int SipMsgObj::ParserSipMsgBody(const osip_message_t* sipMsg)
{
	int result = -1;

	// 判断消息体是否存在类型
	if (sipMsg == nullptr || sipMsg->bodies.node == nullptr ||
		sipMsg->content_type == nullptr || sipMsg->content_type->subtype == nullptr)
	{
		return result;
	}

	// 获取消息体指针，body指向sipMsg的body第一个元素，不可释放
	osip_body_t* body = nullptr;
	body = (osip_body_t*)osip_list_get(&sipMsg->bodies, 0);
	
	std::string subType = sipMsg->content_type->subtype;// 获取消息体类型
	if (SipUtilities::CompareStrCase(subType.c_str(), "Application/MANSCDP+xml"))
	{
		mSipMsgBody = new SipMsgXmlBody;
		result = mSipMsgBody->ParserSipMsgBody(body->body, &mXmlParam);
	} 
	else if (SipUtilities::CompareStrCase(subType.c_str(), "APPLICATION/SDP"))
	{
		mSipMsgBody = new SipMsgSdpBody;
		result = mSipMsgBody->ParserSipMsgBody(body->body, &mSdpParam);
	} 
	else if (SipUtilities::CompareStrCase(subType.c_str(), "APPLICATION/MANSRTSP"))
	{
		mSipMsgBody = new SipMsgRtspBody;
		result = mSipMsgBody->ParserSipMsgBody(body->body, &mRtspParam);
	}
	delete mSipMsgBody;
	mSipMsgBody = nullptr;
	return result;
}

int SipMsgObj::CreateBaseResponseSipMsg(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg)
{
	int result = -1;
	if (srcSipMsg != nullptr)
	{
		if (dstSipMsg)
		{
			osip_message_free(dstSipMsg);
			dstSipMsg = nullptr;
		}

		result = osip_message_init(&dstSipMsg);
		if (result == 0)
		{
			// via、from、to、call-id、CSeq、Contact、max-forward
			result = CreateNewSipMsg(srcSipMsg, dstSipMsg, NIST);
		}
	}
	return result;
}

int SipMsgObj::Create200OKSipMsg(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg)
{
	int result = -1;
	result = CreateBaseResponseSipMsg(srcSipMsg, dstSipMsg);
	if (result == 0)
	{
		result = SetSipMsgResponseStartLine(dstSipMsg, 200, "OK");
	}
	return result;
}

int SipMsgObj::CreateRegister200OKSipMsg(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg)
{
	int result = -1;
	result = Create200OKSipMsg(srcSipMsg, dstSipMsg);
	if (result == 0)
	{
		result = mSipMsgHeader->SetSipMsgDate(dstSipMsg, SipUtilities::GetLocalTime());
	}
	return result;
}

int SipMsgObj::CreateBadRequestSipMsg(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg)
{
	int result = -1;
	result = CreateBaseResponseSipMsg(srcSipMsg, dstSipMsg);
	if (result == 0)
	{
		result = SetSipMsgResponseStartLine(dstSipMsg, 400, "Bad Request");
	}
	return result;
}

int SipMsgObj::CreateForbiddenSipMsg(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg)
{
	int result = -1;
	result = CreateBaseResponseSipMsg(srcSipMsg, dstSipMsg);
	if (result == 0)
	{
		result = SetSipMsgResponseStartLine(dstSipMsg, 403, "Forbidden");
	}
	return result;
}

int SipMsgObj::CreateNotFoundSipMsg(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg)
{
	int result = -1;
	result = CreateBaseResponseSipMsg(srcSipMsg, dstSipMsg);
	if (result == 0)
	{
		result = SetSipMsgResponseStartLine(dstSipMsg, 404, "Not Found");
	}
	return result;
}

int SipMsgObj::GetSipMsgTagAndCallID(const osip_message_t * sipMsg, std::string & fromTag, std::string & toTag, std::string & callID)
{
	mSipMsgHeader->GetSipMsgCallID(sipMsg, callID);
	mSipMsgHeader->GetSipMsgFromTag(sipMsg, fromTag);
	mSipMsgHeader->GetSipMsgToTag(sipMsg, toTag);
	return 0;
}

int SipMsgObj::GetSipMsgExpires(const osip_message_t * sipMsg, std::string & expires)
{
	return mSipMsgHeader->GetSipMsgExpires(sipMsg, expires);
}

int SipMsgObj::SetSipMsgRequestStartLine(osip_message_t *& dstSipMsg, std::string method)
{
	// start-line
	int result = 0;
	if (dstSipMsg != nullptr)
	{
		result = mSipMsgHeader->SetSipMsgVersion(dstSipMsg);
		result = result + mSipMsgHeader->SetSipMsgMethod(dstSipMsg, method);
		result = result + mSipMsgHeader->SetSipMsgReqUri(dstSipMsg, mRemoteSipID, mRemoteIP, mRemotePort);
	}
	return result;
}

int SipMsgObj::SetSipMsgResponseStartLine(osip_message_t *& dstMessage, int statusCode, std::string reasonPhrase)
{
	int result = -1;
	if (dstMessage != nullptr)
	{
		mSipMsgHeader->SetSipMsgStatusCode(dstMessage, statusCode);
		result = mSipMsgHeader->SetSipMsgReasonPhrase(dstMessage, reasonPhrase);
	}
	return result;
}

int SipMsgObj::CreateNewSipMsg(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg, osip_fsm_type_t msgType)
{
	int result = -1;
	switch (msgType)
	{
	case ICT:
		result = CreateNewSipMsg_ICT(srcSipMsg, dstSipMsg);
		break;
	case IST:
		result = CreateNewSipMsg_IST(srcSipMsg, dstSipMsg);
		break;
	case NICT:
		result = CreateNewSipMsg_NICT(srcSipMsg, dstSipMsg);
		break;
	case NIST:
		result = CreateNewSipMsg_NIST(srcSipMsg, dstSipMsg);
		break;
	default:
		break;
	}
	return result;
}

int SipMsgObj::CreateNewSipMsg_ICT(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg)
{
	return 0;
}

int SipMsgObj::CreateNewSipMsg_IST(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg)
{
	// via
	mSipMsgHeader->CloneSipMsgVias(dstSipMsg, &srcSipMsg->vias);

	// from
	mSipMsgHeader->SetSipMsgFrom(dstSipMsg, srcSipMsg->from);

	// to
	mSipMsgHeader->SetSipMsgTo(dstSipMsg, srcSipMsg->to);
	if (!srcSipMsg->to->gen_params.nb_elt)
	{
		mSipMsgHeader->SetSipMsgToTag(dstSipMsg, SipUtilities::CreatUuid());
	}

	// call-ID
	mSipMsgHeader->SetSipMsgCallID(dstSipMsg, srcSipMsg->call_id);

	// CSeq
	mSipMsgHeader->SetSipMsgCseq(dstSipMsg, srcSipMsg->cseq);

	// Contact
	mSipMsgHeader->SetSipMsgContact(dstSipMsg, mLocalSipID, mLocalIP, mLocalPort);

	// max-forward
	mSipMsgHeader->SetSipMsgMaxForward(dstSipMsg);

	return 0;
}

int SipMsgObj::CreateNewSipMsg_NICT(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg)
{
	return 0;
}

int SipMsgObj::CreateNewSipMsg_NIST(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg)
{
	// via
	mSipMsgHeader->CloneSipMsgVias(dstSipMsg, &srcSipMsg->vias);

	// from
	mSipMsgHeader->SetSipMsgFrom(dstSipMsg, srcSipMsg->from);

	// to
	mSipMsgHeader->SetSipMsgTo(dstSipMsg, srcSipMsg->to);
	if (!srcSipMsg->to->gen_params.nb_elt)
	{
		mSipMsgHeader->SetSipMsgToTag(dstSipMsg, SipUtilities::CreatUuid());
	}

	// call-ID
	mSipMsgHeader->SetSipMsgCallID(dstSipMsg, srcSipMsg->call_id);

	// CSeq
	mSipMsgHeader->SetSipMsgCseq(dstSipMsg, srcSipMsg->cseq);

	// Contact
	mSipMsgHeader->SetSipMsgContact(dstSipMsg, mLocalSipID, mLocalIP, mLocalPort);

	// max-forward
	mSipMsgHeader->SetSipMsgMaxForward(dstSipMsg);
	return 0;
}
