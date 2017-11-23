#ifndef _SIP_XML_DIALOG_H_
#define _SIP_XML_DIALOG_H_
#include <iostream>
#include <string>
#include <deque>
#include <mutex>

typedef struct XmlDialogParam
{
	int xmlDialogID;
	int manscdpType; // XML命令类型
	std::string srcDeviceID; // 源设备
	std::string dstDeviceID; // 目的设备

	std::string fromTag;
	std::string toTag;
	std::string callID;

	// 订阅使用
	int diffTime; // 向上级汇报时间间隔
	time_t lastResponseTime;// 上次汇报时间

	// 历史视音频查询使用
	std::string startTime;
	std::string endTime;
	std::string recordStartTime;
	std::string recordEndTime;
	std::string recordTimes;

	// 有响应控制、查询、通知时使用
	time_t birthTime; // dialog的创建时间，用于判断注册dialog是否在有效期内
	int expires;

	XmlDialogParam()
	{
		xmlDialogID = 0;
		manscdpType = -1;
		expires = -1;
	}
}XmlDialogParam;


class SipXmlDialogs
{
public:
	SipXmlDialogs* GetInstance();
	virtual ~SipXmlDialogs();
	
	void InsertSipXmlDialog(XmlDialogParam* sipXmlDialog);
	void RemoveSipXmlDialog(XmlDialogParam* sipXmlDialog);

	XmlDialogParam* CreateSipXmlDialog();
	int DestroySipXmlDialog(XmlDialogParam* &sipXmlDialog);
	XmlDialogParam* FindSipXmlDialog(int manscdpType, std::string dstDevice);
	
private:
	std::mutex mDialogIdMutex;
	static int mSipXmlDialogID;

	std::mutex mDialogDequeMutex;
	std::deque<XmlDialogParam*> mSipXmlDialogDeque;

private:
	SipXmlDialogs();
	SipXmlDialogs(const SipXmlDialogs&);
	static SipXmlDialogs* mInstance;
};

#endif // _SIP_XML_DIALOG_H_


