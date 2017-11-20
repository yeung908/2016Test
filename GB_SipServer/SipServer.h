#ifndef _SIP_SERVER_H_
#define _SIP_SERVER_H_
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <thread>
#include <mutex>

#include "SipLog.h"
#include "SipMsgObj.h"
#include "SipCallback.h"
#include "SipDialogs.h"
#include "SipSocketServer.h"
#include "AppContext.h"

class SipServer
{
public:
	static SipServer* GetInstance();// 获取SipServer唯一接口
	virtual ~SipServer();

	int StartSipServer();			// 启动sip服务的入口函数

	osip_t* sip; // 全局唯一
 	SipLog* log;
	SipDialogs* mSipDialogs;				// 全局唯一dialog集合
 	SipSocketServer* mSocketServer;			// SocketServer唯一实例
 	SipDatabase* mSipDB;
 	std::vector<osip_event_t*> mSipEvents;	// 需要处理的event全局唯一

private:
	SipServer();				// 设置为private防止实例化对象
	SipServer(const SipServer&);// 设置为private防止拷贝构造实例化对象
	static SipServer* mInstance;// 唯一实例

	int InitSipServer();// 初始化sip服务
	int AutoRegister();	// 启动时自动向上级注册

	// 收发sip消息处理函数
	int ProcessInComingMessage(std::string message);
	int ProcessOutGoingMessage(osip_transaction_t* callTransaction, osip_event_t* &event);
	int CreateTransaction(osip_transaction_t* &transaction, osip_event_t* event);
	
	// 注册回调函数
	void RegisterSipMsgCallbacks();
	void RegisterCommonCallbacks();

	// 独立线程，负责链路管理
	int CheckPlatformStatus();
	void CheckUpperStatus(int timeout);
	void CheckLowerStatus(int timeout);
};

#endif // _SIP_SERVER_H_
