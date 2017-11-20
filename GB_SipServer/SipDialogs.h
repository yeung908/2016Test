#ifndef _SIP_DIALOGS_H_
#define _SIP_DIALOGS_H_
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <mutex>

#include <osip2/osip_dialog.h>

#define SipDialogList std::list<osip_dialog_t*>
#define IntList std::list<int>

typedef enum DialogType
{
	REGISTER_DIALOG       = 0, // 注册
	PLAY_INVITE_DIALOG    = 1, // 实时视频
	PLAYBACK_DIALOG       = 2, // 历史回放
	DOWNLOAD_DIALOG       = 3, // 历史下载
	THIRD_PARTY_DIALOG    = 4, // 第三方呼叫
	AUDIOBROADCAST_DIALOG = 5, // 语音广播
	SUBSCRIBE_DIALOG      = 6  // 事件订阅
}DialogType;

typedef enum SocketType
{
	UDP = 0,
	TCP = 1
}SocketType;

typedef enum DialogHint
{
	UAC = 0,
	UAS = 1
}DialogHint;

typedef enum StreamDirection
{
	SENDER = 0,
	RECEIVER = 1
}StreamDirection;

typedef enum StreamType
{
	AVSS = 0,
	GB28181 = 1
}StreamType;

typedef struct DialogInfo
{
	int dialogID;
	int sn;
	int httpsHandleID;

	DialogHint hint;		// UAC,UAS
	DialogType dialogType;	// dialog类型：注册、实时、历史、下载、第三方、语音广播、订阅

	time_t lastResponseTime;// 最后收到心跳响应时间，用于判断是否超时
	time_t birthTime;		// dialog的创建时间，用于判断注册dialog是否在有效期内

	std::string remoteSipID;
	std::string localSipID;
	std::string dstDeviceID; // 目标设备或平台DeviceID

	int callerDialogID;	    // 上级dialogID
	int calleeDialogID;	    // 下级dialogID

	std::string recvIP;	    // 接收码流的ip
	int recvPort;		    // 接收码流的port
	std::string sendIP;	    // 发送码流的ip
	int sendPort;		    // 发送码流的port

	SocketType socketType;	// TCP、UDP
	StreamType streamType;  // 国标28181对接、还是私有协议对接
	StreamDirection streamDirection; // 在该dialog中本级发送码流还是接收码流
	IntList inviteDialogIDs;         // 平台调用视频dialogID,只有registerDialog使用该字段

	DialogInfo()
	{
		sn = 0;
		dialogID = 0;
		httpsHandleID = 0;
		callerDialogID = 0;
		calleeDialogID = 0;

		recvIP = "";
		recvPort = 0;
		sendIP = "";
		sendPort = 0;
	}
}DialogInfo;

class SipDialogs
{
public:
	virtual ~SipDialogs();
	static SipDialogs* GetDialogsInstance();

	// 创建、销毁dialog
	osip_dialog_t* CreateSipDialog(osip_transaction_t* trn, osip_message_t* respSipMsg, DialogType dialogType);
	void DestorySipDialog(osip_dialog_t* &dialog);

	// 插入、移除dialog
	void InsertSipDialog(osip_dialog_t* dialog, DialogType dialogType);
	int RemoveSipDialogByDialogID(int dialogID, DialogType dialogType);
	int RemoveDialogIdFromRegisterDialog(int dialogID, osip_dialog_t* &registerDialog);

	// 查找dialog
	osip_dialog_t* FindRegisterDialog(std::string remoteDeviceID, DialogHint hint); // 通过对方DeviceID查找注册Dialog
	osip_dialog_t* FindSipDialogByDialogID(int dialogID, DialogType dialogType);	// 通过DialogID和dialogType查找dialog
	osip_dialog_t* FindSipDialogByDialogID(int dialogID);	// 通过DialogID查找dialog
	osip_dialog_t* FindInviteDialog(std::string fromTag, std::string callId, DialogHint hint); // 通过fromTag和callID查找InviteDialog
	osip_dialog_t* FindInviteDialog(int httpsHandle); // 通过httpsHandle查找InviteDialog
	osip_dialog_t* FindAudioDialog(std::string audioDeviceID);

private:
	SipDialogs();					// 设置为private防止实例化对象
	SipDialogs(const SipDialogs&);	// 设置为private防止拷贝构造实例化对象
	static SipDialogs* mInstance;	// 唯一实例

	// dialogID和dialogID锁
	static int mDialogID;
	std::mutex mDialogIdMutex;

	// register Dialog相关变量
	std::mutex mRegisterMutex;
	SipDialogList mRegisterDialogs;

	std::mutex mInviteMutex;
	SipDialogList mInviteDialogs;

	std::mutex mSubcribeMutex;
	SipDialogList mSubcribeDialogs;

	// 内部使用
	osip_dialog_t* FindDialogByDialogID(SipDialogList dialogList, int dialogID); // 查找dialog	
	int RemoveDialogByDialogID(SipDialogList &dialogList, int dialogID);// 将dialog从dialogs列表中删除
};
#endif // _SIP_DIALOGS_H_

