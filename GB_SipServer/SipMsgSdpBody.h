#ifndef _SIP_MSG_SDP_BODY_H_
#define _SIP_MSG_SDP_BODY_H_
#include <osip2/osip.h>
#include <osipparser2/sdp_message.h>
#include "SipMsgBody.h"

class SipMsgSdpBody :
	public SipMsgBody
{
public:
	SipMsgSdpBody();
	~SipMsgSdpBody();

	virtual int CreateSipMsgBody(void* bodyParameter, std::string &bodyString);
	virtual int ParserSipMsgBody(const std::string bodyString, void* bodyParameter);

private:
	// v×Ö¶Î
	int SetV_Version(sdp_message_t* sdpMsg, const std::string version = "0");

	// o×Ö¶Î
	int SetO_UserName(sdp_message_t* sdpMsg, const std::string userName);
	int SetO_SessID(sdp_message_t* sdpMsg, const std::string sessID = "0");
	int SetO_SessVersion(sdp_message_t* sdpMsg, const std::string sessVersion = "0");
	int SetO_NetType(sdp_message_t* sdpMsg, const std::string netType = "IN");
	int SetO_AddrType(sdp_message_t* sdpMsg, const std::string addrType = "IP4");
	int SetO_Addr(sdp_message_t* sdpMsg, const std::string addr);
	int SetO_Origin(sdp_message_t* sdpMsg, const SdpMediaParam &sdpMediaParam);

	// s×Ö¶Î
	int SetS_Name(sdp_message_t* sdpMsg, const std::string name = "Play"); //Play¡¢Playback¡¢Download
	int GetS_Name(sdp_message_t* sdpMsg, SdpMediaParam &sdpMediaParam);

	// c×Ö¶Î
	int SetC_NetType(sdp_message_t* sdpMsg, const std::string netType = "IN");
	int SetC_AddrType(sdp_message_t* sdpMsg, const std::string addrType = "IP4");
	int SetC_Addr(sdp_message_t* sdpMsg, const std::string addr);
	int SetC_Conection(sdp_message_t* sdpMsg, const SdpMediaParam &sdpMediaParam);

	// t×Ö¶Î
	int SetT_Descrs(sdp_message_t* sdpMsg, const std::string startTime, const std::string endTime);
	int GetT_Descrs(sdp_message_t* sdpMsg, SdpMediaParam &sdpMediaParam);

	// m×Ö¶Î
	int SetM_Medias(sdp_message_t* sdpMsg, const SdpMediaParam &sdpMediaParam);
	int SetM_Attributes(sdp_media_t* sdpMedia, const SdpMediaParam &sdpMediaParam);

	int SetAttribute(sdp_attribute_t*& attribute, const std::string field, const std::string value);
	int GetM_Medias(sdp_message_t* sdpMsg, SdpMediaParam &sdpMediaParam);

	// y×Ö¶Î
	int SetY(std::string &sdpMsg, std::string y);
	int GetY(std::string sdpMsg, std::string &y);

	// k×Ö¶Î
	int SetF(std::string &sdpMsg, std::string f);
	int GetF(std::string &sdpMsg, std::string f);
};

#endif // _SIP_MSG_SDP_BODY_H_
