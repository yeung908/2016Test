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
	return 0;
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
	SetCreateCallback(RESPONSE_DEVICE_CONFIG_DOWNLOAD, ResponseDeviceDownload);
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
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement(); // 获取根节点指针	
	InsertChildElement(xmlDoc, rootElement, "TeleBoot", "Boot");     // 添加远程启动命令字段
	return 0;
}

int SipMsgXmlBody::ControlRecordCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement(); // 获取根节点指针	
	//InsertChildElement(xmlDoc, rootElement, "Record", xmlParam.controlParam);     // 添加远程启动命令字段
	return 0;
}

int SipMsgXmlBody::ControlGuardCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ControlAlarmCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ControlIFameCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ControlDragZoom(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ControlHomePosition(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ControlDeviceConfig(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::QueryDeviceStatus(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::QueryCatalog(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam)
{
	return 0;
}

int SipMsgXmlBody::QueryDeviceInfo(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam)
{
	return 0;
}

int SipMsgXmlBody::QueryRecordInfo(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::QueryAlarm(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::QueryConfigDownload(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::QueryPreset(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::QueryMobilePosition(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam)
{
	return 0;
}

int SipMsgXmlBody::NotifyKeepalive(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::NotifyAlarm(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::NotifyMediaStutas(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::NotifyBroadcast(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::NotifyMobilePosition(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::NotifyCatalog(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ResponseDeviceControl(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ResponseAlarmNotify(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ResponseCatalog(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ResponseRcvCatalogQuery(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ResponseRcvCatalog(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ResponseDeviceInfoQuery(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ResponseDeviceStatusQuery(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ResponseRecordInfo(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ResponseDeviceConfig(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ResponseDeviceDownload(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ResponsePresetQuery(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

int SipMsgXmlBody::ResponseBroadcast(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	return 0;
}

void SipMsgXmlBody::InsertChildElement(tinyxml2::XMLDocument *& xmlDoc, tinyxml2::XMLElement *& parentElement, std::string childElementName, std::string childElementValue)
{
	if (xmlDoc)
	{
		tinyxml2::XMLElement* childElement = xmlDoc->NewElement(childElementName.c_str());
		parentElement->InsertEndChild(childElement);
		if (childElementValue.size() != 0)
		{
			childElement->SetText(childElementValue.c_str());
		}
	}	
}

// void SipMsgXmlBody::InsertChildElement(tinyxml2::XMLDocument *& xmlDoc, tinyxml2::XMLElement *& parentElement, std::string childElementName, int childElementValue)
// {
// 	tinyxml2::XMLElement* childElement = xmlDoc->NewElement(childElementName.c_str());
// 	parentElement->InsertEndChild(childElement);
// 	childElement->SetText(childElementValue);
// }

void SipMsgXmlBody::InsertChildElementWithAttr(tinyxml2::XMLDocument *& xmlDoc, tinyxml2::XMLElement* &parentElement, std::string childElementName, int childElementAttribute)
{
	if (xmlDoc)
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

