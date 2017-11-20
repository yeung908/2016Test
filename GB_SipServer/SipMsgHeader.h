#ifndef _SIP_MSG_HEADER_H_
#define _SIP_MSG_HEADER_H_

#include <iostream>
#include <string>
#include "osip2/osip.h"
#include "SipCredentials.h"

#define osip_message_set_event(sip,value)	 osip_message_set_header((osip_message_t *)sip,(const char *)"Event",value)
#define osip_message_get_event(sip,pos,dest) osip_message_header_get_byname(( osip_message_t *)sip,(const char *)"event",pos,(osip_header_t **)dest)

#define osip_message_set_Subscription_State(sip,value)	 osip_message_set_header((osip_message_t *)sip,(const char *)"Subscription-State",value)
#define osip_message_get_Subscription_State(sip,pos,dest) osip_message_header_get_byname(( osip_message_t *)sip,(const char *)"subscription-state",pos,(osip_header_t **)dest)

class SipMsgHeader
{
	typedef int(*clone_func)(void *, void **);
public:
	// Uri◊÷∂Œ Ùœ‡…Ë÷√
	int SetUriScheme(osip_uri_t* uri, const std::string scheme = "sip");
	int SetUriUsername(osip_uri_t* uri, const std::string username);
	int SetUriHost(osip_uri_t* uri, const std::string host);
	int SetUriPort(osip_uri_t* uri, const std::string port);
	int SetUri(osip_uri_t** uri,
		       const std::string username,
		       const std::string host,
		       const std::string port,
		       const std::string scheme = "sip");

	// SIP request only
	int SetSipMsgVersion(osip_message_t* sipMsg, const std::string version = "SIP/2.0");
	int SetSipMsgReqUri(osip_message_t* sipMsg,
		                const std::string sipID,
		                const std::string host,
		                const std::string port);
	int SetSipMsgMethod(osip_message_t *sipMsg, const std::string method);	// …Ë÷√sipMegµƒmethod◊÷∂Œ

	// SIP answer only
	int SetSipMsgStatusCode(osip_message_t* sipMsg, int statusCode);
	int SetSipMsgReasonPhrase(osip_message_t* sipMsg, const std::string reasonPhrase);

	// SIP via
	int SetSipMsgVia(osip_message_t* sipMsg,
		             const std::string host,
		             const std::string port,
		             const std::string branchValue,
		             const std::string protocol = "UDP",
		             const std::string version = "2.0");
	int SetSipMsgVia(osip_message_t* sipMsg, const osip_via_t * via);
	int CloneSipMsgVias(osip_message_t* sipMsg, const osip_list_t* vias);
	int SetSipMsgViaProtocal(osip_message_t* sipMsg, const std::string protocol = "TCP");
	int SetSipMsgViaBranch(osip_message_t* sipMsg, std::string branchValue);

	// Tag
	int SetTag(osip_list_t* gen_params, const std::string tagStr);
	int GetTag(osip_list_t* gen_params, std::string &tag);
	int GetSipMsgFromTag(const osip_message_t* sipMsg, std::string &tag);
	int GetSipMsgToTag(const osip_message_t* sipMsg, std::string &tag);

	// From
	int SetSipMsgFrom(osip_message_t* sipMsg,
		              const std::string username,
		              const std::string host,
		              const std::string tag = "");
	int SetSipMsgFrom(osip_message_t* sipMsg, const osip_from_t* from);
	int SetSipMsgFromTag(osip_message_t* sipMsg, const std::string tag);

	// To
	int SetSipMsgTo(osip_message_t* sipMsg,
		            const std::string username,
		            const std::string host,
		            const std::string tag = "");
	int SetSipMsgTo(osip_message_t* sipMsg, const osip_to_t* to);
	int SetSipMsgToTag(osip_message_t* sipMsg, const std::string tag);

	// CallID
	int SetSipMsgCallID(osip_message_t* sipMsg, const osip_call_id_t *call_id);
	int SetSipMsgCallID(osip_message_t* sipMsg, const std::string host, const std::string number);
	int SetSipMsgCallID(osip_message_t* sipMsg, const std::string callID);
	int GetSipMsgCallID(const osip_message_t* sipMsg, std::string &callID);

	// contact
	int SetSipMsgContact(osip_message_t* sipMsg,
		                 const std::string username,
		                 const std::string host,
		                 const std::string port);
	int SetSipMsgContact(osip_message_t* sipMsg, const osip_contact_t* contact);

	// Cseq
	int SetSipMsgCseq(osip_message_t* sipMsg, const std::string num, const std::string method);
	int SetSipMsgCseq(osip_message_t* sipMsg, const osip_cseq_t *cseq);

	// Header
	// Max-Forwards
	int SetSipMsgMaxForward(osip_message_t* sipMsg, const std::string maxForward = "70");
	int GetSipMsgMaxForward(const osip_message_t* sipMsg, int &maxForward);

	// Expires
	int SetSipMsgExpires(osip_message_t* sipMsg, const std::string expires);
	int GetSipMsgExpires(const osip_message_t* sipMsg, std::string & expires);

	// Content-Type
	int SetSipMsgContentType(osip_message_t* sipMsg, const std::string contentType);
	int SetSipMsgContentType(osip_message_t* sipMsg, 
		                      const std::string type,
		                      const std::string subType);

	// User-Agent
	int SetSipMsgUserAgent(osip_message_t* sipMsg, const std::string user = "ccf_AVSS");

	// subject
	int SetSipMsgSubject(osip_message_t* sipMsg, const std::string subject);
	int SetSipMsgSubject(osip_message_t* sipMsg,
		                 std::string senderSipID,
		                 std::string senderNum,
		                 std::string receiverSipID,
		                 std::string receiverNum);

	// Date
	int SetSipMsgDate(osip_message_t* sipMsg, const std::string date);

	// www-authenticate
	int SetSipMsgWWW_Authrnticate(osip_message_t* sipMsg,
		                          const std::string realm,
		                          const std::string nonce,
		                          const std::string type = "Digest");

	// Authorization
	int SetSipMsgAuthorization(osip_message_t* sipMsg, 
		                       const osip_list_t www_authenticates, 
		                       const std::string password);
};

#endif // _SIP_MSG_HEADER_H_
