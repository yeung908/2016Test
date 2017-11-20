#ifndef _APP_CONTEXT_H_
#define _APP_CONTEXT_H_
#include <iostream>
#include <string>
#include <osip2/osip.h>
class AppContext
{
public:
	virtual ~AppContext();
	static AppContext* CreateAppContext();
	static int DestroyAppContext(AppContext* appContext);

	osip_t* sip;
	osip_transaction_t *callTransaction;

	std::string mRecvIP;
	int mRecvPort;
	std::string mSendIP;
	int mSendPort;

	int mHttpsHandleId;
private:
	AppContext();
};
#endif // _APP_CONTEXT_H_

