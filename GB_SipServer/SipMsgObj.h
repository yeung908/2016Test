#ifndef _SIP_MSG_OBJ_H_
#define _SIP_MSG_OBJ_H_
#include "SipMsgHeader.h"
#include "SipMsgRtspBody.h"
#include "SipMsgSdpBody.h"
#include "SipMsgXmlBody.h"

#include "SipDatabase.h"
#include "SipUtilities.h"

typedef class SipMsgObj
{
public:
	SipMsgObj();
	~SipMsgObj();

	// 获取系统信息
	int GetLocalPlatformInfo();
	int GetRemotePlatformInfo(std::string remoteDeviceID);
	
	// Register
	int CreateRegisterMsg(osip_message_t* &dstSipMsg, const std::string remoteSipID, const std::string expires = "3600"); // 不带鉴权注册函数
	int CreateRegMsgAgain(osip_message_t* &dstSipMsg,
		                  const std::string remoteSipID,
		                  const std::string callID,
		                  const std::string fromTag,
		                  const std::string toTag,
		                  const std::string cesq); // 有效期到达重新注册函数
	int CreateUnRegistrtMsg(osip_message_t* &dstSipMsg, const std::string remoteSipID, const std::string expires = "0");	// 注销
	int CreateAuthRegisterMsg(const osip_message_t* orgRequest, const osip_message_t* lastResponse, osip_message_t* &dstSipMsg);
	int CreateUnauthrizedMsg(const osip_message_t* srcMessage, osip_message_t* &dstMessage, const std::string toTag = "");// 401 Unauthrized

	// Message、Subscribe、NotifyMessage, xml消息体，
	int CreateSipMsgXml(osip_message_t* &dstSipMsg, std::string remoteDeviceID);
	int SetSipMsgProtol(osip_message_t* dstSipMsg, std::string protol = "TCP");

	// Invite, sdp消息体
	int CreateInviteSipMsg();
	int CreateThirdInviteSipMsg();
	int CreateAckSipMsg();
	int CreateThirdAckSipMsg();
	int CreateByeSipMsg();

	// rtsp消息体
	int CreateSipMsgRtsp();

	// 1xx

	// 2xx
	int Create200OKSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg); // 200
	int CreateInvite200OkSipMsg();
	int CreateRegister200OKSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg); // 200

	// 3xx

	// 4xx
	int CreateBadRequestSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg); // 400
	int CreateForbiddenSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg); // 403
	int CreateNotFoundSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg); // 404

	// 解析SipMsg的常用头字段
	int GetSipMsgTagAndCallID(const osip_message_t* sipMsg, std::string &fromTag, std::string &toTag, std::string &callID);
	int GetSipMsgExpires(const osip_message_t* sipMsg, std::string & expires);

	// 解析SipMsg消息体
	int ParserSipMsgBody(const osip_message_t* sipMsg);
	XmlParam  mXmlParam;
	SdpParam  mSdpParam;
	RtspParam mRtspParam;

public:
	// 平台参数信息
	int mLocalPlatformID;
	std::string mLocalSipID;		// 本级平台28181国标编码
	std::string mLocalSipDomain;	// 本级平台所在域编码
	std::string mLocalIP;			// 本级平台sip信令IP地址
	std::string mLocalPort;			// 本级平台sip信令端口号
	std::string mLocalDeviceID;		// 本级平台/设备DeviceID

	int mRemotePlatformID;
	std::string mRemoteSipID;		// 级联平台28181国标编码
	std::string mRemoteSipDomain;	// 级联平台所在域编码
	std::string mRemoteIP;			// 级联平台sip信令IP地址
	std::string mRemotePort;		// 级联平台sip信令端口号
	std::string mRemoteDeviceID;	// 级联平台/设备DeviceID（可能为平台SipID）

private:
	// 数据库连接
	static SipDatabase* mSipDB;

	SipMsgHeader* mSipMsgHeader;
	SipMsgBody* mSipMsgBody;// 消息体

	// 设置消息Start-Line
	int SetSipMsgRequestStartLine(osip_message_t* &dstMessage, std::string method);
	int SetSipMsgResponseStartLine(osip_message_t* &dstMessage, int statusCode, std::string reasonPhrase);

	int CreateBaseResponseSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);

	// CreateNewSipMsg
	int CreateNewSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg, osip_fsm_type_t msgType);
	int CreateNewSipMsg_ICT(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);
	int CreateNewSipMsg_IST(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);
	int CreateNewSipMsg_NICT(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);
	int CreateNewSipMsg_NIST(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);

}SipMsgBuilder, SipMsgParser;
#endif // _SIP_MSG_OBJ_H_
