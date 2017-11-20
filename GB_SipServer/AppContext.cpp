#include "AppContext.h"



AppContext::AppContext()
{
}


AppContext::~AppContext()
{
}

AppContext * AppContext::CreateAppContext()
{
	AppContext* appContext = new AppContext;
	appContext->sip = nullptr;
	appContext->callTransaction = nullptr;

	appContext->mRecvIP = "";
	appContext->mRecvPort = 0;
	appContext->mSendIP = "";
	appContext->mSendPort = 0;
	appContext->mHttpsHandleId = -1;
	return appContext;
}

int AppContext::DestroyAppContext(AppContext * appContext)
{
	if (appContext != nullptr)
	{
		delete appContext;
		appContext = nullptr;
	}
	return 0;
}
