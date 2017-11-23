#include "SipMsgXmlBody.h"

SipDatabase* SipMsgXmlBody::mSipDB = new SipDatabase;

bool SipMsgXmlBody::mIsRegistered = false;
CreateBodyCallback SipMsgXmlBody::CreateBodyCallbacks[XML_COUNT];
ParserBodyCallback SipMsgXmlBody::ParserCallbacks[XML_COUNT];

SipMsgXmlBody::SipMsgXmlBody()
{
}


SipMsgXmlBody::~SipMsgXmlBody()
{
}

int SipMsgXmlBody::CreateSipMsgBody(void * bodyParameter, std::string & bodyString)
{
	if (!mIsRegistered)
	{
		RegisterCreateCallback();
	}

	XmlCmdParam & xmlCmdParam = *(XmlCmdParam*)bodyParameter;

	// xml文档指针
	tinyxml2::XMLDocument* xmlDocument = new tinyxml2::XMLDocument;

	// 创建xml声明
	tinyxml2::XMLDeclaration* declaration = xmlDocument->NewDeclaration();
	xmlDocument->InsertFirstChild(declaration);

	// xml根节点
	tinyxml2::XMLElement* rootElement = xmlDocument->NewElement(xmlCmdParam.reqRespType.c_str());
	xmlDocument->InsertEndChild(rootElement);

	// 通用节点
	InsertChildElement(xmlDocument, rootElement, "CmdType", xmlCmdParam.cmdType);
	InsertChildElement(xmlDocument, rootElement, "SN", xmlCmdParam.sn);
	InsertChildElement(xmlDocument, rootElement, "DeviceID", xmlCmdParam.deviceID);

	tinyxml2::XMLElement* cmdTypeElement = rootElement->FirstChildElement("CmdType");

	// 调用注册回调函数,设置xml文档
	int result = -1;
	result = CreateBodyCallbacks[xmlCmdParam.manscdpType](xmlDocument, xmlCmdParam);
	if (!result)
	{
		// 设置节点名称和值，element: <value>text</value>
		rootElement->SetValue(xmlCmdParam.reqRespType.c_str());
		cmdTypeElement->SetText(xmlCmdParam.cmdType.c_str());

		// 将xml文件转换为字符串
		tinyxml2::XMLPrinter printer;
		xmlDocument->Accept(&printer);
		xmlCmdParam.bodyString = printer.CStr();
		bodyString = xmlCmdParam.bodyString;
	}
	else
	{
		bodyString = "";
	}

	// 释放内存
	delete xmlDocument;
	xmlDocument = nullptr;

	return result;
}

int SipMsgXmlBody::ParserSipMsgBody(const std::string bodyString, void * bodyParameter)
{
	XmlParam & xmlCmdParam = *(XmlParam*)bodyParameter;

	tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument;
	xmlDoc->Parse(bodyString.c_str());

	int result = -1;
	tinyxml2::XMLElement* xmlRootElement = xmlDoc->RootElement();
	if (xmlRootElement != nullptr)
	{		
		// 获取消息类型
		xmlCmdParam.reqRespType = xmlRootElement->Value();// Control、Query、Notify、Response

		// sip消息所携带的消息体具体分类解析
		if (xmlCmdParam.reqRespType == "Control") // 控制
		{
			//result = ParserControl(xmlRootElement, xmlCmdInfo);
		}
		else if (xmlCmdParam.reqRespType == "Query") // 查询
		{
			//result = ParserQuery(xmlRootElement, xmlCmdInfo);
		}
		else if (xmlCmdParam.reqRespType == "Notify") // 通知
		{
			//result = ParserNotify(xmlRootElement, xmlCmdInfo);
		}
		else if (xmlCmdParam.reqRespType == "Response") // 应答
		{
			//result = ParserResponse(xmlRootElement, xmlCmdInfo);
		}
	}

	// 释放内存
	delete xmlDoc;
	xmlDoc = nullptr;

	return result;
}

void SipMsgXmlBody::SetCreateCallback(int type, CreateBodyCallback cb)
{
	CreateBodyCallbacks[type] = cb;
}

void SipMsgXmlBody::SetParserCallback(int type, ParserBodyCallback cb)
{
	ParserCallbacks[type] = cb;
}

void SipMsgXmlBody::RegisterCreateCallback()
{
	// Control
	SetCreateCallback(CONTROL_DEVICE_PTZ, ControlPtzCmd);
	SetCreateCallback(CONTROL_TELE_BOOT, ControlTeleBoot);
	SetCreateCallback(CONTROL_RECORD, ControlRecordCmd);
	SetCreateCallback(CONTROL_GUARD, ControlGuardCmd);
	SetCreateCallback(CONTROL_ALARM, ControlAlarmCmd);
	SetCreateCallback(CONTROL_IFAME, ControlIFameCmd);
	SetCreateCallback(CONTROL_DRAG_ZOOM, ControlDragZoom);
	SetCreateCallback(CONTROL_HOME_POSITION, ControlHomePosition);
	SetCreateCallback(CONTROL_DEVICE_CONFIG, ControlDeviceConfig);

	// Query
	SetCreateCallback(QUERY_DEVICE_STATUS, QueryDeviceStatus);
	SetCreateCallback(QUERY_DEVICE_CATALOG, QueryCatalog);
	SetCreateCallback(QUERY_DEVICE_INFO, QueryDeviceInfo);
	SetCreateCallback(QUERY_RECORD_INFO, QueryRecordInfo);
	SetCreateCallback(QUERY_ALARM, QueryAlarm);
	SetCreateCallback(QUERY_CONFIG_DOWNLOAD, QueryConfigDownload);
	SetCreateCallback(QUERY_PRESET_QUERY, QueryPreset);
	SetCreateCallback(QUERY_MOBILE_POSITION, QueryMobilePosition);

	// Notify
	SetCreateCallback(NOTIFY_KEEPALIVE, NotifyKeepalive);
	SetCreateCallback(NOTIFY_ALARM, NotifyAlarm);
	SetCreateCallback(NOTIFY_MEDIA_STATUS, NotifyMediaStutas);
	SetCreateCallback(NOTIFY_BROADCAST, NotifyBroadcast);
	SetCreateCallback(NOTIFY_MOBILE_POSITION, NotifyMobilePosition);
	SetCreateCallback(NOTIFY_CATALOG, NotifyCatalog);

	// Response
	SetCreateCallback(RESPONSE_DEVICE_CONTROL, ResponseDeviceControl);
	SetCreateCallback(RESPONSE_ALARM_NOTIFY, ResponseAlarmNotify);
	SetCreateCallback(RESPONSE_DEVICE_CATALOG_QUERY, ResponseCatalog);
	SetCreateCallback(RESPONSE_CATALOG_QUERY_RECEIVED, ResponseRcvCatalogQuery);
	SetCreateCallback(RESPONSE_CATALOG_RECEIVED, ResponseRcvCatalog);
	SetCreateCallback(RESPONSE_DEVICE_INFO_QUERY, ResponseDeviceInfoQuery);
	SetCreateCallback(RESPONSE_DEVICE_STATUS_QUERY, ResponseDeviceStatusQuery);
	SetCreateCallback(RESPONSE_DOCUMENT_RECORD_INFO, ResponseRecordInfo);
	SetCreateCallback(RESPONSE_DEVICE_CONFIG, ResponseDeviceConfig);
	SetCreateCallback(RESPONSE_DEVICE_CONFIG_DOWNLOAD, ResponseConfigDownload);
	SetCreateCallback(RESPONSE_DEVICE_PRESET_QUERY, ResponsePresetQuery);
	SetCreateCallback(RESPONSE_BROADCAST, ResponseBroadcast);

	mIsRegistered = true;
}

int SipMsgXmlBody::ControlPtzCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ControlTeleBoot(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Control";
	xmlParam.cmdType = "DeviceControl";

	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();   // 获取根节点指针	
	InsertChildElement(xmlDoc, rootElement, "TeleBoot", "Boot"); // 添加远程启动命令字段
	return 0;
}

int SipMsgXmlBody::ControlRecordCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Control";
	xmlParam.cmdType = "DeviceControl";

	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement(); // 获取根节点指针	
	InsertChildElement(xmlDoc, rootElement, "Record", xmlParam.controlParam.recordType); // 添加手动录像命令字段
	return 0;
}

int SipMsgXmlBody::ControlGuardCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Control";
	xmlParam.cmdType = "DeviceControl";

	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement(); // 获取根节点指针	
	InsertChildElement(xmlDoc, rootElement, "GuardCmd", xmlParam.controlParam.guardType); // 添加***命令字段
	return 0;
}

int SipMsgXmlBody::ControlAlarmCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Control";
	xmlParam.cmdType = "DeviceControl";

	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement(); // 获取根节点指针	
	InsertChildElement(xmlDoc, rootElement, "AlarmCmd", xmlParam.controlParam.alarmParam.resetAlarm); // 添加***命令字段

	tinyxml2::XMLElement* info = xmlDoc->NewElement("Info");
	InsertChildElement(xmlDoc, info, "AlarmMethod", xmlParam.controlParam.alarmParam.alarmMethod);
	InsertChildElement(xmlDoc, info, "AlarmType", xmlParam.controlParam.alarmParam.alarmType);

	return 0;
}

int SipMsgXmlBody::ControlIFameCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Control";
	xmlParam.cmdType = "DeviceControl";

	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针
	InsertChildElement(xmlDoc, rootElement, "IFameCmd", "Send");// 添加强制关键帧控制命令
	return 0;
}

int SipMsgXmlBody::ControlDragZoom(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Control";
	xmlParam.cmdType = "DeviceControl";

	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	tinyxml2::XMLElement* dragZoom = nullptr;
	if (xmlParam.controlParam.dragZoom.isZoomOut)
	{
		dragZoom = xmlDoc->NewElement("DragZoomOut");
	}
	else
	{
		dragZoom = xmlDoc->NewElement("DragZoomIn");
	}
	rootElement->InsertEndChild(dragZoom);

	InsertChildElement(xmlDoc, dragZoom, "Length", std::to_string(xmlParam.controlParam.dragZoom.length));// 添加拉框放大参数
	InsertChildElement(xmlDoc, dragZoom, "Width", std::to_string(xmlParam.controlParam.dragZoom.width));// 添加拉框放大参数
	InsertChildElement(xmlDoc, dragZoom, "MidPointX", std::to_string(xmlParam.controlParam.dragZoom.midPointX));// 添加拉框放大参数
	InsertChildElement(xmlDoc, dragZoom, "MidPointY", std::to_string(xmlParam.controlParam.dragZoom.midPointY));// 添加拉框放大参数
	InsertChildElement(xmlDoc, dragZoom, "LengthX", std::to_string(xmlParam.controlParam.dragZoom.lengthX));// 添加拉框放大参数
	InsertChildElement(xmlDoc, dragZoom, "LengthY", std::to_string(xmlParam.controlParam.dragZoom.lengthY));// 添加拉框放大参数
	return 0;
}

int SipMsgXmlBody::ControlHomePosition(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Control";
	xmlParam.cmdType = "DeviceControl";

	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	tinyxml2::XMLElement* homePosition = xmlDoc->NewElement("HomePosition");
	rootElement->InsertEndChild(homePosition);

	InsertChildElement(xmlDoc, homePosition, "Enabled", xmlParam.controlParam.homePositionParam.enabled);
	InsertChildElement(xmlDoc, homePosition, "ResetTime", xmlParam.controlParam.homePositionParam.resetTime);
	InsertChildElement(xmlDoc, homePosition, "PresetIndex", xmlParam.controlParam.homePositionParam.presetIndex);
	return 0;
}

int SipMsgXmlBody::ControlDeviceConfig(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Control";
	xmlParam.cmdType = "DeviceControl";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	tinyxml2::XMLElement* basicParam = xmlDoc->NewElement("BasicParam");
	rootElement->InsertEndChild(basicParam);

	// 设备配置
	InsertChildElement(xmlDoc, basicParam, "Name", xmlParam.controlParam.deviceConfigParam.name);
	InsertChildElement(xmlDoc, basicParam, "Expiration", xmlParam.controlParam.deviceConfigParam.expiration);
	InsertChildElement(xmlDoc, basicParam, "HeartBeatInterval", xmlParam.controlParam.deviceConfigParam.heartBeatInterval);
	InsertChildElement(xmlDoc, basicParam, "HeartBeatCount", xmlParam.controlParam.deviceConfigParam.heartBeatCount);

	return 0;
}

int SipMsgXmlBody::QueryDeviceStatus(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Query";
	xmlParam.cmdType = "DeviceStatus";

	return 0;
}

int SipMsgXmlBody::QueryCatalog(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam)
{
	xmlParam.reqRespType = "Query";
	xmlParam.cmdType = "Catalog";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	InsertChildElement(xmlDoc, rootElement, "StartTime", xmlParam.queryParam.queryCatalogParam.startTime);
	InsertChildElement(xmlDoc, rootElement, "EndTime", xmlParam.queryParam.queryCatalogParam.endTime);

	return 0;
}

int SipMsgXmlBody::QueryDeviceInfo(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam)
{
	xmlParam.reqRespType = "Query";
	xmlParam.cmdType = "DeviceInfo";

	return 0;
}

int SipMsgXmlBody::QueryRecordInfo(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Query";
	xmlParam.cmdType = "RecordInfo";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	QueryRecordInfoParam &queryRecordInfoParam = xmlParam.queryParam.queryRecordInfoParam;// 文件目录检索请求
	InsertChildElement(xmlDoc, rootElement, "StartTime", queryRecordInfoParam.startTime);
	InsertChildElement(xmlDoc, rootElement, "EndTime", queryRecordInfoParam.endTime);
	InsertChildElement(xmlDoc, rootElement, "FilePath", queryRecordInfoParam.filePath);
	InsertChildElement(xmlDoc, rootElement, "Address", queryRecordInfoParam.address);
	InsertChildElement(xmlDoc, rootElement, "Secrdcy", queryRecordInfoParam.secrecy);
	InsertChildElement(xmlDoc, rootElement, "Type", queryRecordInfoParam.type);
	InsertChildElement(xmlDoc, rootElement, "RecorderID", queryRecordInfoParam.recorderID);

	return 0;
}

int SipMsgXmlBody::QueryAlarm(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Query";
	xmlParam.cmdType = "Alarm";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	QueryAlarmParam &queryAlarmParam = xmlParam.queryParam.queryAlarmParam;		// 报警查询
	InsertChildElement(xmlDoc, rootElement, "StartAlarmPriority", queryAlarmParam.startAlarmPriority);
	InsertChildElement(xmlDoc, rootElement, "EndAlarmProitity", queryAlarmParam.endAlarmPriority);
	InsertChildElement(xmlDoc, rootElement, "AlarmMethod", queryAlarmParam.alarmMethod);
	InsertChildElement(xmlDoc, rootElement, "AlarmType", queryAlarmParam.alarmType);
	InsertChildElement(xmlDoc, rootElement, "StartAlarmTime", queryAlarmParam.startAlarmTime);
	InsertChildElement(xmlDoc, rootElement, "EndAlarmTime", queryAlarmParam.endAlarmTime);

	return 0;
}

int SipMsgXmlBody::QueryConfigDownload(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Query";
	xmlParam.cmdType = "ConfigDownload";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	InsertChildElement(xmlDoc, rootElement, "ConfigType", xmlParam.queryParam.configType);

	return 0;
}

int SipMsgXmlBody::QueryPreset(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Query";
	xmlParam.cmdType = "PresetQuery";

	return 0;
}

int SipMsgXmlBody::QueryMobilePosition(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam)
{
	xmlParam.reqRespType = "Query";
	xmlParam.cmdType = "MobilePosition";

	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针
	InsertChildElement(xmlDoc, rootElement, "Interval", xmlParam.queryParam.mobileInterval);

	return 0;
}

int SipMsgXmlBody::NotifyKeepalive(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Notify";
	xmlParam.cmdType = "Keepalive";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针
	InsertChildElement(xmlDoc, rootElement, "Status", "OK");
	xmlParam.notifyParam.keepalive.info;

	return 0;
}

int SipMsgXmlBody::NotifyAlarm(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Notify";
	xmlParam.cmdType = "Alarm";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	NotifyAlarmParam &alarm = xmlParam.notifyParam.alarm;			// 报警通知// 添加报警通知信息命令字段																	
	InsertChildElement(xmlDoc, rootElement, "AlarmPriority", alarm.alarmPriority);
	InsertChildElement(xmlDoc, rootElement, "AlarmMethod", alarm.alarmTime);
	InsertChildElement(xmlDoc, rootElement, "AlarmTime", alarm.alarmTime);
	InsertChildElement(xmlDoc, rootElement, "AlarmDescription", alarm.alarmDescription);
	InsertChildElement(xmlDoc, rootElement, "Longitude", alarm.longitude);
	InsertChildElement(xmlDoc, rootElement, "Latitude", alarm.latitude);

	alarm.alarmInfo;

	return 0;
}

int SipMsgXmlBody::NotifyMediaStutas(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Notify";
	xmlParam.cmdType = "MediaStutas";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针
	InsertChildElement(xmlDoc, rootElement, "NotifyType", xmlParam.notifyParam.mediaStatus); // 121 表示历史媒体文件发送结束
	return 0;
}

int SipMsgXmlBody::NotifyBroadcast(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Notify";
	xmlParam.cmdType = "Broadcast";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针
																	
	rootElement->DeleteChild(rootElement->FirstChildElement("DeviceID"));// 需要删除deviceID字段
	InsertChildElement(xmlDoc, rootElement, "SourceID", xmlParam.notifyParam.broadcast.sourceID);
	InsertChildElement(xmlDoc, rootElement, "TargetID", xmlParam.notifyParam.broadcast.targetID);
	return 0;
}

int SipMsgXmlBody::NotifyMobilePosition(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Notify";
	xmlParam.cmdType = "MobilePosition";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	NotifyMobilePositionParam &mobilePosition = xmlParam.notifyParam.mobilePosition; // 移动设备位置数据通知
	InsertChildElement(xmlDoc, rootElement, "Time", mobilePosition.time);
	InsertChildElement(xmlDoc, rootElement, "Longitude", mobilePosition.longtitude);
	InsertChildElement(xmlDoc, rootElement, "Latitude", mobilePosition.latitude);
	InsertChildElement(xmlDoc, rootElement, "Speed", mobilePosition.speed);
	InsertChildElement(xmlDoc, rootElement, "Direction", mobilePosition.direction);
	InsertChildElement(xmlDoc, rootElement, "Altitude", mobilePosition.altitude);

	return 0;
}

int SipMsgXmlBody::NotifyCatalog(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Notify";
	xmlParam.cmdType = "Catalog";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	InsertChildElement(xmlDoc, rootElement, "Status", "OK");
	InsertChildElement(xmlDoc, rootElement, "SumNum", (int)xmlParam.notifyParam.deviceList.size());

	tinyxml2::XMLElement* deviceList = xmlDoc->NewElement("DeviceList");
	deviceList->SetAttribute("Num", (int)xmlParam.notifyParam.deviceList.size());
	rootElement->InsertEndChild(deviceList);
	InsertChildElementWithAttr(xmlDoc, rootElement, "DeviceList", (int)xmlParam.notifyParam.deviceList.size());

	for (int i = 0; i < xmlParam.notifyParam.deviceList.size(); i++)
	{
		tinyxml2::XMLElement* item = xmlDoc->NewElement("Item");
		deviceList->InsertEndChild(item);

		InsertChildElement(xmlDoc, item, "DeviceID", xmlParam.notifyParam.deviceList[i].deviceID);
		InsertChildElement(xmlDoc, item, "Event", xmlParam.notifyParam.deviceList[i].event);
	}

	return 0;
}

int SipMsgXmlBody::ResponseDeviceControl(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "DeviceControl";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	InsertChildElement(xmlDoc, rootElement, "Result", xmlParam.responseParam.result);
	return 0;
}

int SipMsgXmlBody::ResponseAlarmNotify(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "Alarm";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	InsertChildElement(xmlDoc, rootElement, "Result", xmlParam.responseParam.result);
	return 0;
}

int SipMsgXmlBody::ResponseCatalog(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "Catalog";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针
	// ???
	return 0;
}

int SipMsgXmlBody::ResponseRcvCatalogQuery(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "DeviceControl";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	InsertChildElement(xmlDoc, rootElement, "Result", xmlParam.responseParam.result);

	return 0;
}

int SipMsgXmlBody::ResponseRcvCatalog(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "DeviceControl";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	InsertChildElement(xmlDoc, rootElement, "Result", xmlParam.responseParam.result);

	return 0;
}

int SipMsgXmlBody::ResponseDeviceInfoQuery(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "DeviceInfo";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	RespDeviceInfoParam &devInfo = xmlParam.responseParam.devInfoParam;		// 设备信息查询应答
	InsertChildElement(xmlDoc, rootElement, "DeviceName", devInfo.deviceName);
	InsertChildElement(xmlDoc, rootElement, "Result", devInfo.result);
	InsertChildElement(xmlDoc, rootElement, "Manufacturter", devInfo.manufacturer);
	InsertChildElement(xmlDoc, rootElement, "Model", devInfo.model);
	InsertChildElement(xmlDoc, rootElement, "Firmware", devInfo.firmware);
	InsertChildElement(xmlDoc, rootElement, "Channel", devInfo.channel);

	return 0;
}

int SipMsgXmlBody::ResponseDeviceStatusQuery(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "DeviceStatus";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	RespDeviceStatusParam &devStatus = xmlParam.responseParam.devStatusParam; // 设备状态信息查询应答
	InsertChildElement(xmlDoc, rootElement, "Result", devStatus.result);
	InsertChildElement(xmlDoc, rootElement, "Online", devStatus.online);
	InsertChildElement(xmlDoc, rootElement, "Status", devStatus.status);
	if (devStatus.status == "ERROR")
	{
		InsertChildElement(xmlDoc, rootElement, "Reason", devStatus.reason);
	}
	InsertChildElement(xmlDoc, rootElement, "Encode", devStatus.encode);
	InsertChildElement(xmlDoc, rootElement, "Record", devStatus.record);
	InsertChildElement(xmlDoc, rootElement, "DeviceTime", devStatus.deviceTime);

	// 报警状态列表
	if (devStatus.alarmStatus.size())
	{
		tinyxml2::XMLElement* alarmstatus = xmlDoc->NewElement("Alarmstatus");
		alarmstatus->SetAttribute("num", (int)devStatus.alarmStatus.size());
		for (size_t i = 0; i < devStatus.alarmStatus.size(); i++)
		{
			tinyxml2::XMLElement* item = xmlDoc->NewElement("Item");
			InsertChildElement(xmlDoc, item, "DeviceID", devStatus.alarmStatus[i].deviceID);
			InsertChildElement(xmlDoc, item, "DutyStatus", devStatus.alarmStatus[i].dutyStatus);
		}
	}

	// info扩展信息

	return 0;
}

int SipMsgXmlBody::ResponseRecordInfo(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "RecordInfo";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针
	
	RespRecordInfoParam &recordInfo = xmlParam.responseParam.recordInfoParam;// 文件目录检索应答
	InsertChildElement(xmlDoc, rootElement, "Name", recordInfo.name);
	InsertChildElement(xmlDoc, rootElement, "SumNum", recordInfo.sumNum);
	InsertChildElementWithAttr(xmlDoc, rootElement, "RecordList", (int)recordInfo.recordInfoItems.size());

	tinyxml2::XMLElement* recordList = rootElement->FirstChildElement("RecordList");
	for (size_t i = 0; i < recordInfo.recordInfoItems.size(); i++)
	{
		// 插入Item字段
		tinyxml2::XMLElement* item = xmlDoc->NewElement("Item");
		recordList->InsertEndChild(item);

		// 插入相机DeviceID和相机名称
		InsertChildElement(xmlDoc, item, "DeviceID", xmlParam.deviceID);
		InsertChildElement(xmlDoc, item, "Name", recordInfo.name);

		// 插入历史文件信息
		InsertChildElement(xmlDoc, rootElement, "StartTime", recordInfo.recordInfoItems[i].startTime);
		InsertChildElement(xmlDoc, rootElement, "EndTime", recordInfo.recordInfoItems[i].endTime);
		InsertChildElement(xmlDoc, rootElement, "FilePath", recordInfo.recordInfoItems[i].filePath);
		InsertChildElement(xmlDoc, rootElement, "Address", recordInfo.recordInfoItems[i].address);
		InsertChildElement(xmlDoc, rootElement, "Secrdcy", recordInfo.recordInfoItems[i].secrecy);
		InsertChildElement(xmlDoc, rootElement, "Type", recordInfo.recordInfoItems[i].type);
		InsertChildElement(xmlDoc, rootElement, "RecorderID", recordInfo.recordInfoItems[i].recorderID);
	}

	return 0;
}

int SipMsgXmlBody::ResponseDeviceConfig(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "DeviceConfig";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	InsertChildElement(xmlDoc, rootElement, "Result", xmlParam.responseParam.result);
	
	return 0;
}

int SipMsgXmlBody::ResponseConfigDownload(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "ConfigDownload";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	RespConfigDownloadParam &configDownload = xmlParam.responseParam.configDownloadParam;// 设备配置查询应答
	InsertChildElement(xmlDoc, rootElement, "Result", configDownload.result);
	tinyxml2::XMLElement* basicParam = xmlDoc->NewElement("BasicParam");
	rootElement->InsertEndChild(basicParam);
	InsertChildElement(xmlDoc, basicParam, "Name", configDownload.name);
	InsertChildElement(xmlDoc, basicParam, "Expiration", configDownload.expiration);
	InsertChildElement(xmlDoc, basicParam, "HeartBeatInterval", configDownload.heartBeatInterval);
	InsertChildElement(xmlDoc, basicParam, "HeartBeatCount", configDownload.heartBeatCount);
	InsertChildElement(xmlDoc, basicParam, "PositionCapability", configDownload.positionCapaility);
	InsertChildElement(xmlDoc, basicParam, "Longitude", configDownload.Longitude);
	InsertChildElement(xmlDoc, basicParam, "Latitude", configDownload.Latitude);

	return 0;
}

int SipMsgXmlBody::ResponsePresetQuery(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "PresetQuery";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针


	RespPresetParams &presetList = xmlParam.responseParam.presetList; // 设备预置位查询应答
	tinyxml2::XMLElement* presetListEle = xmlDoc->NewElement("PresetList");
	rootElement->InsertEndChild(presetListEle);
	presetListEle->SetAttribute("Num", (int)presetList.size());

	for (size_t i = 0; i < presetList.size(); i++)
	{
		tinyxml2::XMLElement* item = xmlDoc->NewElement("Item");
		rootElement->InsertEndChild(item);

		InsertChildElement(xmlDoc, item, "PresetID", presetList[i].presetID);
		InsertChildElement(xmlDoc, item, "presetName", presetList[i].presetName);
	}
	return 0;
}

int SipMsgXmlBody::ResponseBroadcast(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "Broadcast";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	InsertChildElement(xmlDoc, rootElement, "Result", xmlParam.responseParam.result);
	return 0;
}

int SipMsgXmlBody::ParserControl(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam & xmlParam)
{
	int result = -1;

	// 获取命令类型、sn和获取deviceID
	if (GetChildElementValue(xmlRootElement, "CmdType", xmlParam.cmdType) ||
		GetChildElementValue(xmlRootElement, "SN", xmlParam.sn) ||
		GetChildElementValue(xmlRootElement, "DeviceID", xmlParam.deviceID))
	{
		result = -1;
	}
	else
	{
		if (xmlParam.cmdType == "DeviceControl") // 设备控制
		{
			result = ParserDeviceControl(xmlRootElement, xmlParam);
		}
		else if (xmlParam.cmdType == "DeviceConfig")// 设备配置
		{
			result = ParserDeviceConfig(xmlRootElement, xmlParam);
		}
	}
	return result;
}

int SipMsgXmlBody::ParserDeviceControl(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam & xmlParam)
{
	int result = 0;

	// 判断命令类型
	if (!GetChildElementValue(xmlRootElement, "PTZCmd", xmlParam.controlParam.ptzParam.ptzCmdValue)) // Ptz控制命令
	{
		xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::CONTROL_DEVICE_PTZ;
		//ParserControlCmd(xmlRootElement, xmlCmdInfo);
	}
	else if (!GetChildElementValue(xmlRootElement, "TeleBoot", xmlParam.controlParam.teleBootParam)) // 远程重启命令
	{
		xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::CONTROL_TELE_BOOT;
	}
	else if (!GetChildElementValue(xmlRootElement, "RecordCmd", xmlParam.controlParam.recordType)) // 录像控制命令
	{
		xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::CONTROL_RECORD;
	}
	else if (!GetChildElementValue(xmlRootElement, "GuardCmd", xmlParam.controlParam.guardType)) // 布防/撤防命令
	{
		xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::CONTROL_GUARD;
	}
	else if (xmlRootElement->FirstChildElement("AlarmCmd")) // 报警复位命令
	{
		xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::CONTROL_ALARM;

		// 报警复位命令扩展项
		tinyxml2::XMLElement* info = xmlRootElement->FirstChildElement("Info");
		if (info)
		{
			GetChildElementValue(info, "AlarmMethod", xmlParam.controlParam.alarmParam.alarmMethod);
			GetChildElementValue(info, "AlarmType", xmlParam.controlParam.alarmParam.alarmType);
		}
	}
	else if (!GetChildElementValue(xmlRootElement, "IFameCmd", xmlParam.controlParam.iFame)) // 强制关键帧命令
	{
		xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::CONTROL_IFAME;
	}
	else if (xmlRootElement->FirstChildElement("HomePosition")) // 看守位控制命令
	{
		xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::CONTROL_HOME_POSITION;

		// 看守位控制命令扩展项
		tinyxml2::XMLElement* homePosition = xmlRootElement->FirstChildElement("HomePosition");
		GetChildElementValue(homePosition, "Enabled", xmlParam.controlParam.homePositionParam.enabled);
		GetChildElementValue(homePosition, "ResetTime", xmlParam.controlParam.homePositionParam.resetTime);
		GetChildElementValue(homePosition, "PresetIndex", xmlParam.controlParam.homePositionParam.presetIndex);
	}
	else if (xmlRootElement->FirstChildElement("DragZoomIn") || xmlRootElement->FirstChildElement("DragZoomOut")) // 看守位控制命令
	{		
		xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::CONTROL_DRAG_ZOOM;

		// 拉框放大
		tinyxml2::XMLElement* dragZoom = xmlRootElement->FirstChildElement("DragZoomIn");
		if (dragZoom)
		{
			xmlParam.controlParam.dragZoom.isZoomOut = false;
		} 
		else
		{
			xmlParam.controlParam.dragZoom.isZoomOut = true;
		}

		GetChildElementValue(dragZoom, "Length", xmlParam.controlParam.dragZoom.length);
		GetChildElementValue(dragZoom, "Width", xmlParam.controlParam.dragZoom.length);
		GetChildElementValue(dragZoom, "MidPointX", xmlParam.controlParam.dragZoom.length);
		GetChildElementValue(dragZoom, "MidPointY", xmlParam.controlParam.dragZoom.length);
		GetChildElementValue(dragZoom, "LengthX", xmlParam.controlParam.dragZoom.length);
		GetChildElementValue(dragZoom, "LengthY", xmlParam.controlParam.dragZoom.length);
	}
	else
	{
		xmlParam.manscdpType = MESSAGE_MANSCDP_ERROR;
		result = -1;
	}
	return result;
}

int SipMsgXmlBody::ParserDeviceConfig(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam & xmlParam)
{
	xmlParam.manscdpType = CONTROL_DEVICE_CONFIG;

	// 基本配置参数获取
	tinyxml2::XMLElement* basicParam = xmlRootElement->FirstChildElement("BasicParam");
	if (basicParam != nullptr)
	{
		GetChildElementValue(basicParam, "Name", xmlParam.controlParam.deviceConfigParam.name);
		GetChildElementValue(basicParam, "Expiration", xmlParam.controlParam.deviceConfigParam.expiration);
		GetChildElementValue(basicParam, "HeartBeatInterval", xmlParam.controlParam.deviceConfigParam.heartBeatInterval);
		GetChildElementValue(basicParam, "HeartBeatCount", xmlParam.controlParam.deviceConfigParam.heartBeatCount);
	}
	return 0;
}

int SipMsgXmlBody::ParserQuery(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam & xmlParam)
{
	int result = 0;

	// 获取命令类型、sn和获取deviceID
	if (GetChildElementValue(xmlRootElement, "CmdType", xmlParam.cmdType) ||
		GetChildElementValue(xmlRootElement, "SN", xmlParam.sn) ||
		GetChildElementValue(xmlRootElement, "DeviceID", xmlParam.deviceID))
	{
		result = -1;
	}
	else
	{
		if (xmlParam.cmdType == "DeviceStatus") // 设备状态查询
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::QUERY_DEVICE_STATUS;
		}
		else if (xmlParam.cmdType == "Catalog") // 设备目录查询
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::QUERY_DEVICE_CATALOG;
			GetChildElementValue(xmlRootElement, "StartTime", xmlParam.queryParam.queryCatalogParam.startTime);
			GetChildElementValue(xmlRootElement, "EndTime", xmlParam.queryParam.queryCatalogParam.endTime);
		}
		else if (xmlParam.cmdType == "DeviceInfo") // 设备信息查询
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::QUERY_DEVICE_INFO;
		}
		else if (xmlParam.cmdType == "RecordInfo") // 文件目录检索请求
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::QUERY_RECORD_INFO;

			if (GetChildElementValue(xmlRootElement, "StartTime", xmlParam.queryParam.queryRecordInfoParam.startTime) ||
				GetChildElementValue(xmlRootElement, "EndTime", xmlParam.queryParam.queryRecordInfoParam.endTime))
			{
				result = -1;
			}
			else
			{
				GetChildElementValue(xmlRootElement, "FilePath", xmlParam.queryParam.queryRecordInfoParam.filePath);
				GetChildElementValue(xmlRootElement, "Address", xmlParam.queryParam.queryRecordInfoParam.address);
				GetChildElementValue(xmlRootElement, "Secrecy", xmlParam.queryParam.queryRecordInfoParam.secrecy);
				GetChildElementValue(xmlRootElement, "Type", xmlParam.queryParam.queryRecordInfoParam.type);
				GetChildElementValue(xmlRootElement, "RecorderID", xmlParam.queryParam.queryRecordInfoParam.recorderID);
			}
		}
		else if (xmlParam.cmdType == "Alarm") // 报警查询
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::QUERY_ALARM;

			GetChildElementValue(xmlRootElement, "StartAlarmPriority", xmlParam.queryParam.queryAlarmParam.startAlarmPriority);
			GetChildElementValue(xmlRootElement, "EndAlarmPriority", xmlParam.queryParam.queryAlarmParam.endAlarmPriority);
			GetChildElementValue(xmlRootElement, "AlarmMethod", xmlParam.queryParam.queryAlarmParam.alarmMethod);
			GetChildElementValue(xmlRootElement, "AlarmType", xmlParam.queryParam.queryAlarmParam.alarmType);
			GetChildElementValue(xmlRootElement, "StartAlarmTime", xmlParam.queryParam.queryAlarmParam.startAlarmTime);
			GetChildElementValue(xmlRootElement, "EndAlarmTime", xmlParam.queryParam.queryAlarmParam.endAlarmTime);
		}
		else if (xmlParam.cmdType == "ConfigDownload") // 设备配置查询
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::QUERY_CONFIG_DOWNLOAD;
			if (GetChildElementValue(xmlRootElement, "ConfigType", xmlParam.queryParam.configType))
			{
				result = -1;
			}
		}
		else if (xmlParam.cmdType == "PresetQuery") // 设备预置位查询
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::QUERY_PRESET_QUERY;
		}
		else if (xmlParam.cmdType == "MobilePosition") // 移动设备位置数据查询
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::QUERY_MOBILE_POSITION;
			GetChildElementValue(xmlRootElement, "Interval", xmlParam.queryParam.mobileInterval);
		}
		else
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::MESSAGE_MANSCDP_ERROR;
			result = -1;
		}
	}
	return result;
}

int SipMsgXmlBody::ParserNotify(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam & xmlParam)
{
	int result = 0;

	// 获取命令类型、sn和获取deviceID
	if (GetChildElementValue(xmlRootElement, "CmdType", xmlParam.cmdType) ||
		GetChildElementValue(xmlRootElement, "SN", xmlParam.sn))
	{
		result = -1;
	}
	else
	{
		if (xmlParam.cmdType == "Broadcast") // 语音广播通知
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::NOTIFY_BROADCAST;
			if (GetChildElementValue(xmlRootElement, "SourceID", xmlParam.notifyParam.broadcast.sourceID) ||
				GetChildElementValue(xmlRootElement, "TargetID", xmlParam.notifyParam.broadcast.targetID))
			{
				result = -1;
			}
		}
		else if (!GetChildElementValue(xmlRootElement, "DeviceID", xmlParam.deviceID))
		{
			if (xmlParam.cmdType == "Keepalive") // 状态信息报送
			{
				xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::NOTIFY_KEEPALIVE;
				// Info扩展暂时未使用
			}
			else if (xmlParam.cmdType == "Alarm") // 报警通知
			{
				xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::NOTIFY_ALARM;
				GetChildElementValue(xmlRootElement, "AlarmPriority", xmlParam.notifyParam.alarm.alarmPriority);
				GetChildElementValue(xmlRootElement, "AlarmMethod", xmlParam.notifyParam.alarm.alarmMethod);
				GetChildElementValue(xmlRootElement, "AlarmTime", xmlParam.notifyParam.alarm.alarmTime);
				GetChildElementValue(xmlRootElement, "AlarmType", xmlParam.notifyParam.alarm.alarmType);
				GetChildElementValue(xmlRootElement, "AlarmTypeParam", xmlParam.notifyParam.alarm.alarmTypeParam);
				GetChildElementValue(xmlRootElement, "AlarmDescription", xmlParam.notifyParam.alarm.alarmDescription);
				GetChildElementValue(xmlRootElement, "AlarmLongitude", xmlParam.notifyParam.alarm.longitude);
				GetChildElementValue(xmlRootElement, "AlarmLatitude", xmlParam.notifyParam.alarm.latitude);
				GetChildElementValue(xmlRootElement, "DeviceID", xmlParam.notifyParam.alarm.deviceID);

				tinyxml2::XMLElement* firstInfo = xmlRootElement->FirstChildElement("Info");
				NotifyAlarmInfo alarmInfo;
				while (firstInfo)
				{
					GetChildElementValue(firstInfo, "AlarmType", alarmInfo.alarmType);

					tinyxml2::XMLElement* alarmTypeParam = firstInfo->FirstChildElement("AlarmTypeParam");
					if (alarmTypeParam)
					{
						GetChildElementValue(firstInfo, "EventType", alarmInfo.eventType);
					}
					xmlParam.notifyParam.alarm.alarmInfo.push_back(alarmInfo);
					firstInfo = firstInfo->NextSiblingElement();
				}
			}
			else if (xmlParam.cmdType == "MediaStatus") // 媒体通知,
			{
				xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::NOTIFY_MEDIA_STATUS;
				GetChildElementValue(xmlRootElement, "NotifyType", xmlParam.notifyParam.mediaStatus);
			}
			else if (xmlParam.cmdType == "MobilePosition") // 移动设备位置数据通知
			{
				xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::NOTIFY_MOBILE_POSITION;
				if (GetChildElementValue(xmlRootElement, "Time", xmlParam.notifyParam.mobilePosition.time) ||
					GetChildElementValue(xmlRootElement, "Longitude", xmlParam.notifyParam.mobilePosition.longtitude) ||
					GetChildElementValue(xmlRootElement, "Latitude", xmlParam.notifyParam.mobilePosition.latitude))
				{
					result = -1;
				}
				else
				{
					GetChildElementValue(xmlRootElement, "Speed", xmlParam.notifyParam.mobilePosition.speed);
					GetChildElementValue(xmlRootElement, "Direction", xmlParam.notifyParam.mobilePosition.direction);
					GetChildElementValue(xmlRootElement, "Altitude", xmlParam.notifyParam.mobilePosition.altitude);
				}
			}
			else if (xmlParam.cmdType == "Catalog")
			{
				xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::NOTIFY_CATALOG;

				tinyxml2::XMLElement* deviceList = xmlRootElement->FirstChildElement("DeviceList");
				if (deviceList)
				{
					NotifyCatalogInfo catalogInfo;
					tinyxml2::XMLElement* firstInfo = deviceList->FirstChildElement("Item");
					while (firstInfo)
					{
						GetChildElementValue(firstInfo, "DeviceID", catalogInfo.deviceID);
						GetChildElementValue(firstInfo, "Event", catalogInfo.event);
						xmlParam.notifyParam.deviceList.push_back(catalogInfo);
						firstInfo = firstInfo->NextSiblingElement();
					}
				}
			}
		}
		else
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::MESSAGE_MANSCDP_ERROR;
			result = -1;
		}
	}
	return result;
}

int SipMsgXmlBody::ParserResponse(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam & xmlParam)
{
	int result = 0;

	// 获取命令类型、sn和获取deviceID
	if (GetChildElementValue(xmlRootElement, "CmdType", xmlParam.cmdType) ||
		GetChildElementValue(xmlRootElement, "SN", xmlParam.sn) ||
		GetChildElementValue(xmlRootElement, "DeviceID", xmlParam.deviceID))
	{
		result = -1;
	}
	else
	{
		if (xmlParam.cmdType == "DeviceControl")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONTROL;
			GetChildElementValue(xmlRootElement, "Result", xmlParam.responseParam.result);
		}
		else if (xmlParam.cmdType == "Alarm")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_ALARM_NOTIFY;
			GetChildElementValue(xmlRootElement, "Result", xmlParam.responseParam.result);
		}
		else if (xmlParam.cmdType == "Catalog") // 设备目录信息查询应答、目录信息查询收到应答、目录收到应答
		{
			if (xmlParam.deviceID == xmlParam.localSipID)// 发送目录后收到下级回复的200OK
			{
				xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_CATALOG_RECEIVED;
			}
			else
			{
				xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CATALOG_QUERY;
				ParserResponseCatalog(xmlRootElement, xmlParam);
			}
		}
		else if (xmlParam.cmdType == "DeviceInfo")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_INFO_QUERY;

			RespDeviceInfoParam &devInfo = xmlParam.responseParam.devInfoParam;		// 设备信息查询应答
			if (GetChildElementValue(xmlRootElement, "Result", devInfo.result))
			{
				result = -1;
			}
			else
			{
				GetChildElementValue(xmlRootElement, "DeviceName", devInfo.deviceName);
				GetChildElementValue(xmlRootElement, "Manufacturer", devInfo.manufacturer);
				GetChildElementValue(xmlRootElement, "Model", devInfo.model);
				GetChildElementValue(xmlRootElement, "Fireware", devInfo.firmware);
				GetChildElementValue(xmlRootElement, "Channel", devInfo.channel);
				GetChildElementValue(xmlRootElement, "Info", devInfo.info);
			}

		}
		else if (xmlParam.cmdType == "DeviceStatus")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_STATUS_QUERY;

			RespDeviceStatusParam &devStatus = xmlParam.responseParam.devStatusParam; // 设备状态信息查询应答
			if (GetChildElementValue(xmlRootElement, "Result", devStatus.result) ||
				GetChildElementValue(xmlRootElement, "Online", devStatus.online) ||
				GetChildElementValue(xmlRootElement, "Status", devStatus.status))
			{
				result = -1;
			}
			else
			{
				GetChildElementValue(xmlRootElement, "Reason", devStatus.reason);
				GetChildElementValue(xmlRootElement, "Encode", devStatus.encode);
				GetChildElementValue(xmlRootElement, "Record", devStatus.record);
				GetChildElementValue(xmlRootElement, "DeviceTime", devStatus.deviceTime);

				tinyxml2::XMLElement* alarmstatusEle = xmlRootElement->FirstChildElement("Alarmstatus");
				if (alarmstatusEle)
				{
					tinyxml2::XMLElement* firstItem = alarmstatusEle->FirstChildElement("Item");
					while (firstItem)
					{
						Alarmstatus alarmStaues;
						GetChildElementValue(firstItem, "DeviceID", alarmStaues.deviceID);
						GetChildElementValue(firstItem, "DutyStatus", alarmStaues.dutyStatus);
						devStatus.alarmStatus.push_back(alarmStaues);
						firstItem = firstItem->NextSiblingElement();
					}
				}
			}
		}
		else if (xmlParam.cmdType == "RecordInfo")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_DOCUMENT_RECORD_INFO;

			RespRecordInfoParam &recordInfo = xmlParam.responseParam.recordInfoParam;		// 文件目录检索应答	

			if (GetChildElementValue(xmlRootElement, "Name", recordInfo.name))// 必选字段，必须验证,2017.09.20测试临时屏蔽
			{
				result = -1;
			}
			else
			{
				GetChildElementValue(xmlRootElement, "SumNum", recordInfo.sumNum);
				tinyxml2::XMLElement* recordList = xmlRootElement->FirstChildElement("RecordList");
				if (recordList)
				{
					tinyxml2::XMLElement* recordListFirstItem = recordList->FirstChildElement("Item"); // 获取RecordList的第一个Item
					RespRecordInfoItem recordInfoItems;
					while (recordListFirstItem)
					{
						GetChildElementValue(recordListFirstItem, "FilePath", recordInfoItems.filePath);
						GetChildElementValue(recordListFirstItem, "Address", recordInfoItems.address);
						GetChildElementValue(recordListFirstItem, "StartTime", recordInfoItems.startTime);
						GetChildElementValue(recordListFirstItem, "EndTime", recordInfoItems.endTime);
						GetChildElementValue(recordListFirstItem, "Secrecy", recordInfoItems.secrecy);
						GetChildElementValue(recordListFirstItem, "Type", recordInfoItems.type);
						GetChildElementValue(recordListFirstItem, "RecorderID", recordInfoItems.recorderID);

						// 保存解析的内容
						recordInfo.recordInfoItems.push_back(recordInfoItems);

						// 指向下一个元素
						recordListFirstItem = recordListFirstItem->NextSiblingElement();
					}
				}
			}			
		}
		else if (xmlParam.cmdType == "DeviceConfig")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONFIG;
			if (GetChildElementValue(xmlRootElement, "Result", xmlParam.responseParam.result))
			{
				result = -1;
			}
		}
		else if (xmlParam.cmdType == "ConfigDownload")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONFIG_DOWNLOAD;

			RespConfigDownloadParam &configDownload = xmlParam.responseParam.configDownloadParam;
			if (GetChildElementValue(xmlRootElement, "Result", configDownload.result))
			{
				result = -1;
			}
			else
			{
				tinyxml2::XMLElement* basicParam = xmlRootElement->FirstChildElement("BasicParam");
				GetChildElementValue(basicParam, "Name", configDownload.name);
				GetChildElementValue(basicParam, "Expiration", configDownload.expiration);
				GetChildElementValue(basicParam, "HeartBeatInterval", configDownload.heartBeatInterval);
				GetChildElementValue(basicParam, "HeartBeatCount", configDownload.heartBeatCount);
				GetChildElementValue(basicParam, "PositionCapability", configDownload.positionCapaility);
				GetChildElementValue(basicParam, "Longitude", configDownload.Longitude);
				GetChildElementValue(basicParam, "Latitude", configDownload.Latitude);
			}
		}
		else if (xmlParam.cmdType == "PresetQuery")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_PRESET_QUERY;

			RespPresetParam presetItem;

			tinyxml2::XMLElement* presetList = xmlRootElement->FirstChildElement("PresetList");
			if (presetList)
			{
				tinyxml2::XMLElement* presetListFirstItem = presetList->FirstChildElement("Item"); // 获取RecordList的第一个Item
				while (presetListFirstItem)
				{
					GetChildElementValue(presetListFirstItem, "PresetID", presetItem.presetID);
					GetChildElementValue(presetListFirstItem, "PresetName", presetItem.presetName);

					// 保存解析的内容
					xmlParam.responseParam.presetList.push_back(presetItem);

					// 指向下一个元素
					presetListFirstItem = presetListFirstItem->NextSiblingElement();
				}
			}
		}
		else if (xmlParam.cmdType == "Broadcast")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_BROADCAST;
			GetChildElementValue(xmlRootElement, "Result", xmlParam.responseParam.result);
		}
		else
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::MESSAGE_MANSCDP_ERROR;
			return -1;
		}
	}
	return result;
}

int SipMsgXmlBody::ParserResponseCatalog(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam & xmlParam)
{
	int  result = 0;

	tinyxml2::XMLElement* xmlDeviceList = xmlRootElement->FirstChildElement("DeviceList");	// 获取目录信息项列表指针
	if (xmlDeviceList == nullptr)
	{
		result = -1;
	}
	else
	{
		CatalogItemAndInfo catalogItemInfo;
		tinyxml2::XMLElement* firstItem = xmlDeviceList->FirstChildElement(); // 获取第一个item指针
		while (firstItem)
		{
			std::string itemDeviceID = "";
			result = GetChildElementValue(firstItem, "DeviceID", itemDeviceID);
			if (result) // 若条目中不存在deviceID资源直接跳过本条目其它字段
			{
				firstItem = firstItem->NextSiblingElement();
				continue;
			}
			else
			{
				// 判断itemDeviceID类型
				if (itemDeviceID.size() <= 10 && itemDeviceID.size() >= 2) // 行政区域编码，数据插入tbl_group
				{
					// 需要获取DeviceID, Name			
					std::string itemName = "";
					if (GetChildElementValue(firstItem, "Name", itemName))
					{
						firstItem = firstItem->NextSiblingElement();
						continue;
					}

					// 插入数据队列
					catalogItemInfo.resItem.tableName = TBL_Group;
					catalogItemInfo.resItem.valuesMap["VirtualOrgID"] = itemDeviceID;
					catalogItemInfo.resItem.valuesMap["Name"] = itemName;
					catalogItemInfo.resItem.valuesMap["Type"] = "1";// 表示行政区划
				}
				else if (itemDeviceID.size() <= 20 && itemDeviceID.size() >= 18)
				{
					int type = 0;
					SipUtilities::Str2Int(itemDeviceID.substr(10, 3), type);
					if (type == 0)
					{
						firstItem = firstItem->NextSiblingElement();
						continue;
					}
					else if (type >= 200 && type <= 211) // 服务
					{
						
					} 
					else if (type == 215)// 业务分组
					{
						// 需要获取DeviceID、Name、ParentID				
						std::string itemName = "";
						std::string itemParentID = "";

						if (GetChildElementValue(firstItem, "Name", itemName) ||
							GetChildElementValue(firstItem, "ParentID", itemParentID))
						{
							firstItem = firstItem->NextSiblingElement();
							continue;
						}

						// 插入数据队列
						catalogItemInfo.resItem.tableName = TBL_Group;
						catalogItemInfo.resItem.valuesMap["VirtualOrgID"] = itemDeviceID;
						catalogItemInfo.resItem.valuesMap["Name"] = itemName;
						catalogItemInfo.resItem.valuesMap["ParentOrgID"] = itemParentID;
						catalogItemInfo.resItem.valuesMap["Type"] = "2";// 表示业务分组
					} 
					else if (type == 216)// 虚拟组织
					{
						// 需要获取DeviceID 、Name、ParentID、BusinessGroupID
						std::string itemName = "";				// 虚拟组织名称
						std::string itemParentID = "";			// 父节点虚拟组织ID			
						std::string itemBusinessGroupID = "";	// 虚拟组织所属业务分组

						if (GetChildElementValue(firstItem, "Name", itemName) ||
							GetChildElementValue(firstItem, "BusinessGroupID", itemBusinessGroupID))
						{
							firstItem = firstItem->NextSiblingElement();
							continue;;
						}

						GetChildElementValue(firstItem, "ParentID", itemParentID);

						// 插入数据队列
						catalogItemInfo.resItem.tableName = TBL_Group;
						catalogItemInfo.resItem.valuesMap["VirtualOrgID"] = itemDeviceID;
						catalogItemInfo.resItem.valuesMap["Name"] = itemName;
						catalogItemInfo.resItem.valuesMap["BusinessGroupID"] = itemBusinessGroupID;
						catalogItemInfo.resItem.valuesMap["ParentOrgID"] = itemParentID;
						catalogItemInfo.resItem.valuesMap["Type"] = "3";// 表示虚拟组织
					} 
					else if (type >= 111 && type <= 199)// 设备
					{
						std::string elementValue = "";

						// 解析相机基本信息
						GetChildElementValue(firstItem, "DeviceID", elementValue);
						catalogItemInfo.resItem.valuesMap["DeviceID"] = elementValue;
						GetChildElementValue(firstItem, "Name", elementValue);
						catalogItemInfo.resItem.valuesMap["Name"] = elementValue;
						GetChildElementValue(firstItem, "Manufacturer", elementValue);
						catalogItemInfo.resItem.valuesMap["Manufacturer"] = elementValue;
						GetChildElementValue(firstItem, "Model", elementValue);
						catalogItemInfo.resItem.valuesMap["Model"] = elementValue;
						GetChildElementValue(firstItem, "Owner", elementValue);
						catalogItemInfo.resItem.valuesMap["Owner"] = elementValue;
						GetChildElementValue(firstItem, "CivilCode", elementValue);
						catalogItemInfo.resItem.valuesMap["CivilCode"] = elementValue;
						GetChildElementValue(firstItem, "Block", elementValue);
						catalogItemInfo.resItem.valuesMap["Block"] = elementValue;
						GetChildElementValue(firstItem, "Address", elementValue);
						catalogItemInfo.resItem.valuesMap["Address"] = elementValue;
						GetChildElementValue(firstItem, "Parental", elementValue);
						catalogItemInfo.resItem.valuesMap["Parental"] = elementValue;
						GetChildElementValue(firstItem, "ParentID", elementValue);
						catalogItemInfo.resItem.valuesMap["ParentID"] = elementValue;
						GetChildElementValue(firstItem, "SafetyWay", elementValue);
						catalogItemInfo.resItem.valuesMap["SafetyWay"] = elementValue;
						GetChildElementValue(firstItem, "RegisterWay", elementValue);
						catalogItemInfo.resItem.valuesMap["RegisterWay"] = elementValue;
						GetChildElementValue(firstItem, "CertNum", elementValue);
						catalogItemInfo.resItem.valuesMap["CertNum"] = elementValue;
						GetChildElementValue(firstItem, "Certifiable", elementValue);
						catalogItemInfo.resItem.valuesMap["Certifiable"] = elementValue;
						GetChildElementValue(firstItem, "ErrCode", elementValue);
						catalogItemInfo.resItem.valuesMap["ErrCode"] = elementValue;
						GetChildElementValue(firstItem, "EndTime", elementValue);
						catalogItemInfo.resItem.valuesMap["EndTime"] = elementValue;
						GetChildElementValue(firstItem, "Secrecy", elementValue);
						catalogItemInfo.resItem.valuesMap["Secrecy"] = elementValue;
						GetChildElementValue(firstItem, "IPAddress", elementValue);
						catalogItemInfo.resItem.valuesMap["IPAddress"] = elementValue;
						GetChildElementValue(firstItem, "Port", elementValue);
						catalogItemInfo.resItem.valuesMap["Port"] = elementValue;
						GetChildElementValue(firstItem, "Password", elementValue);
						catalogItemInfo.resItem.valuesMap["Password"] = elementValue;
						GetChildElementValue(firstItem, "Status", elementValue);
						catalogItemInfo.resItem.valuesMap["Status"] = elementValue;
						GetChildElementValue(firstItem, "Longitude", elementValue);
						catalogItemInfo.resItem.valuesMap["Longitude"] = elementValue;
						GetChildElementValue(firstItem, "Latitude", elementValue);
						catalogItemInfo.resItem.valuesMap["Latitude"] = elementValue;

						// 解析相机扩展信息
						catalogItemInfo.cameraInfo.tableName = TBL_Camera;
						tinyxml2::XMLElement* info = firstItem->FirstChildElement("Info");
						if (info)
						{
							tinyxml2::XMLElement* infoFirstChild = info->FirstChildElement();
							while (infoFirstChild)
							{
								const char* infoName = infoFirstChild->Value();		// 条目名称
								const char* infoValue = infoFirstChild->GetText();	// 条目属性值
								if (infoValue == nullptr)
								{
									infoValue = "";
								}
								catalogItemInfo.cameraInfo.valuesMap[infoName] = infoValue;
								infoFirstChild = infoFirstChild->NextSiblingElement();
							}
							catalogItemInfo.cameraInfo.valuesMap["ProtocolType"] = "SIP";
						}

						// 非28181字段，自定义补充字段
						GetChildElementValue(firstItem, "DeviceID", elementValue);
						catalogItemInfo.resItem.valuesMap["ResType"] = elementValue.substr(10, 3);// 非28181字段，自定义补充字段

						//std::string sqlString = "select PlatformID from tbl_platform where sipid = '" + rootDeviceID + "\'";
						//catalogItemInfo.resItem.valuesMap["PlatformID"] = std::to_string(mSipDB->GetIntData(sqlString));
						catalogItemInfo.resItem.tableName = TBL_Res_Attr;
					}
				}
			}

			// 将数据加入插入数据队列
			xmlParam.responseParam.catalogParam.push_back(catalogItemInfo);

			// 清空数据
			catalogItemInfo.resItem.tableName = "";
			catalogItemInfo.resItem.valuesMap.clear();
			catalogItemInfo.cameraInfo.tableName = "";
			catalogItemInfo.cameraInfo.valuesMap.clear();

			// 获取下一个Item
			firstItem = firstItem->NextSiblingElement();
		}
	}

	return result;
}

int SipMsgXmlBody::ParserResponseRecordInfo(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam & xmlParam)
{
	int result = 0;

	// 必选字段，必须验证
	if (GetChildElementValue(xmlRootElement, "DeviceID", xmlParam.deviceID)||
	    GetChildElementValue(xmlRootElement, "Name", xmlParam.deviceID))
	{
		result = -1;
	}
	else
	{
		
		RespRecordInfoItem recordListItem;
		tinyxml2::XMLElement* recordList = xmlRootElement->FirstChildElement("RecordList");
		//GetElementAttr(recordList, "Num", )
		tinyxml2::XMLElement* recordListFirstItem = recordList->FirstChildElement("Item"); // 获取RecordList的第一个Item
		while (recordListFirstItem)
		{
			GetChildElementValue(recordListFirstItem, "FilePath", recordListItem.filePath);
			GetChildElementValue(recordListFirstItem, "Address", recordListItem.address);
			GetChildElementValue(recordListFirstItem, "StartTime", recordListItem.startTime);
			GetChildElementValue(recordListFirstItem, "EndTime", recordListItem.endTime);
			GetChildElementValue(recordListFirstItem, "Secrecy", recordListItem.secrecy);
			GetChildElementValue(recordListFirstItem, "Type", recordListItem.type);
			GetChildElementValue(recordListFirstItem, "RecorderID", recordListItem.recorderID);

			// 保存解析的内容
			xmlParam.responseParam.recordInfoParam.recordInfoItems.push_back(recordListItem);

			// 指向下一个元素
			recordListFirstItem = recordListFirstItem->NextSiblingElement();
		}
	}	
	
	return result;
}

void SipMsgXmlBody::InsertChildElement(tinyxml2::XMLDocument *& xmlDoc, tinyxml2::XMLElement *& parentElement, std::string childElementName, std::string childElementValue)
{
	if (xmlDoc && parentElement)
	{
		tinyxml2::XMLElement* childElement = xmlDoc->NewElement(childElementName.c_str());
		parentElement->InsertEndChild(childElement);
		if (childElementValue.size() != 0)
		{
			childElement->SetText(childElementValue.c_str());
		}
	}	
}


void SipMsgXmlBody::InsertChildElementWithAttr(tinyxml2::XMLDocument *& xmlDoc, tinyxml2::XMLElement* &parentElement, std::string childElementName, int childElementAttribute)
{
	if (xmlDoc && parentElement)
	{
		tinyxml2::XMLElement* childElement = xmlDoc->NewElement(childElementName.c_str());
		parentElement->InsertEndChild(childElement);
		childElement->SetAttribute("Num", childElementAttribute);
	}
}

int SipMsgXmlBody::GetChildElementValue(tinyxml2::XMLElement* parentElement, std::string elementName, std::string &elementValue)
{
	int result = -1;
	tinyxml2::XMLElement* element = parentElement->FirstChildElement(elementName.c_str());
	if (element && element->GetText())
	{
		elementValue = element->GetText();
		result = 0;
	}
	return result;
}

int SipMsgXmlBody::GetChildElementValue(tinyxml2::XMLElement * parentElement, std::string elementName, int & elementValue)
{
	int result = -1;
	tinyxml2::XMLElement* element = parentElement->FirstChildElement(elementName.c_str());
	if (element)
	{
		result = element->QueryIntText(&elementValue);
	}
	return result;
}

int SipMsgXmlBody::GetChildElementValue(tinyxml2::XMLElement * parentElement, std::string elementName, double & elementValue)
{
	int result = -1;
	tinyxml2::XMLElement* element = parentElement->FirstChildElement(elementName.c_str());
	if (element)
	{
		result = element->QueryDoubleText(&elementValue);
	}
	return result;
}

int SipMsgXmlBody::GetElementAttr(tinyxml2::XMLElement * element, std::string elementAttrName, int & elementAttrValue)
{
	int result = -1;
	if (element)
	{
		result = element->QueryAttribute(elementAttrName.c_str(), &elementAttrValue);
	}
	return 0;
}

