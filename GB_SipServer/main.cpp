#include "SipServer.h"

int main()
{
	SipServer* sipServer = SipServer::GetInstance();
	return sipServer->StartSipServer();
}