#ifndef _SIP_CREDENTIALS_H_
#define _SIP_CREDENTIALS_H_
#include <string>
#include "osip2/osip.h"
#include "osipparser2/osip_md5.h"
namespace SipCredentials
{
	std::string GetCredentials(const std::string sipMsgMethod,
		                       const osip_authorization_t *authorization,
		                       const std::string passWord);
	std::string RemoveQutoes(const std::string srcStr);
	std::string Md5(const std::string srcData);
	std::string A1(const osip_authorization_t *authorization, const std::string & passWord);
	std::string A2(const std::string sipMsgMethod, const osip_authorization_t *authorization);
};
#endif //_SIP_CREDENTIALS_H_