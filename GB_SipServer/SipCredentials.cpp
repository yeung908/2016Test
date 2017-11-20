#include "SipCredentials.h"

std::string SipCredentials::GetCredentials(const std::string sipMsgMethod, const osip_authorization_t * authorization, const std::string passWord)
{
	// ¼øÈ¨²Î¿¼rfc 2617
	if (authorization == nullptr)
	{
		return "";
	}

	std::string nonce = RemoveQutoes(authorization->nonce);
	if (authorization->message_qop != nullptr)
	{
		nonce = nonce + ":" + RemoveQutoes(authorization->nonce_count);
		nonce = nonce + ":" + RemoveQutoes(authorization->cnonce);
		nonce = nonce + ":" + RemoveQutoes(authorization->message_qop);
	}

	std::string a1 = A1(authorization, passWord);
	std::string a2 = A2(sipMsgMethod, authorization);
	return Md5(a1 + ":" + nonce + ":" + a2);
}

std::string SipCredentials::RemoveQutoes(const std::string srcStr)
{
	std::string dstStr = "";
	for (int i = 0; i < srcStr.size(); i++)
	{
		if (srcStr[i] != '\"')
		{
			dstStr += srcStr[i];
		}
	}
	return dstStr;
}

std::string SipCredentials::Md5(const std::string srcData)
{
	char dst[128] = { '0' };
	strcpy_s(dst, srcData.c_str());

	unsigned char md5Result[16];
	osip_MD5_CTX md5Context;
	osip_MD5Init(&md5Context);
	osip_MD5Update(&md5Context, (unsigned char*)dst, strlen(dst));
	osip_MD5Final(md5Result, &md5Context);

	std::string dstData = "";
	char a[3] = { 0 };
	for (int i = 0; i < 16; i++)
	{
		sprintf_s(a, "%02x", md5Result[i]);
		dstData = dstData + a;
	}
	return dstData;
}

std::string SipCredentials::A1(const osip_authorization_t * authorization, const std::string & passWord)
{
	if (authorization == nullptr)
		return "";
	std::string userName = RemoveQutoes(authorization->username);
	std::string realm = RemoveQutoes(authorization->realm);
	std::string secret = RemoveQutoes(passWord);
	std::string a1 = userName + ":" + realm + ":" + secret;
	return Md5(a1);
}

std::string SipCredentials::A2(const std::string sipMsgMethod, const osip_authorization_t * authorization)
{
	if (authorization == nullptr)
		return "";
	char md5Result[16];
	std::string method = RemoveQutoes(sipMsgMethod);
	std::string uri = RemoveQutoes(authorization->uri);
	std::string a2 = method + ":" + uri;
	return Md5(a2);
}
