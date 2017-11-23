#ifndef _SIP_MSG_BODY_H_
#define _SIP_MSG_BODY_H_
#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <map>

#include "SipDefine.h"
#include "SipUtilities.h"

////////////////////////////////RTSP//////////////////////////////////////////
enum RTSP_TYPE
{
	PLAY,		// 播放
	PAUSE,		// 暂停
	SCALE,		// 快进、快退
	RANGE,		// 随机拖放
	TEARDOWN	// 停止
};

typedef struct RtspControlParam
{
	RTSP_TYPE rtspType; // 命令类型
	std::string method; // "PLAY", "PAUSE"
	int cseq;
	float scale;        // 倍速播放
	long  startTime;    // 拖放开始时间
	long  endTime;      // 拖放结束时间

	std::string bodyString;

	RtspControlParam()
	{
		rtspType = RTSP_TYPE::PLAY;
		method = "PAUSE";
		cseq = 0;
		scale = 0;
		startTime = 0;
		endTime = 0;
	}
}RtspParam;

////////////////////////////////SDP//////////////////////////////////////////
typedef struct O_Param
{
	std::string userName;
	std::string sessID;
	std::string sessVersion;
	std::string netType;
	std::string addrType;
	std::string addr;
	O_Param()
	{
		userName = "";
		sessID = "0";
		sessVersion = "0";
		netType = "IN";
		addrType = "IP4";
		addr = "";
	}
}O_Param;

typedef struct U_Param
{
	std::string deviceID;
	int num;
	U_Param()
	{
		deviceID = "";
		num = 3;
	}
}U_Param;

typedef struct C_Param
{
	std::string netType;
	std::string addrType;
	std::string addr;
	C_Param()
	{
		netType = "IN";
		addrType = "IP4";
		addr = "";
	}
}C_Param;

typedef struct T_Param
{
	std::string startTime;
	std::string endTime;
	T_Param()
	{
		startTime = "0";
		endTime = "0";
	}
}T_Param;

typedef struct M_Param
{
	//bool isAudio;
	std::string mediaType;
	//bool isRecvOnly;
	std::string onlyType;
	int port;
	std::string rtpType;
	std::string transType;
	std::vector<std::string> payloads;
	std::vector<std::string> attributes;
	M_Param()
	{
		port = 0;
		//isAudio = false;
		mediaType = "video";
		//isRecvOnly = false;
		onlyType = "recvonly";
		rtpType = "RTP/AVP";
		transType = "";
	}
}M_Param;

typedef struct SdpMediaParam
{
	std::string version;
	O_Param oParam;
	std::string sName; // “Play”、“Playback”、“Download”
	U_Param uParam;
	C_Param cParam;
	T_Param tParam;
	M_Param mParam;
	std::string y;
	std::string f;

	std::string bodyString;
	SdpMediaParam()
	{
		version = "0";
		sName = "Play";
		y = "";
		f = "v/////a///";
	}
}SdpParam;

////////////////////////////////XML//////////////////////////////////////////
// Control
typedef struct PtzControlParam
{
	std::string ptzCmdValue;
};

typedef struct AlarmInfoParam
{
	std::string alarmMethod;
	std::string alarmType;
}AlarmInfoParam;

typedef struct ResetAlarmParam
{
	std::string resetAlarm;
	std::string alarmMethod;
	std::string alarmType;
	//std::vector<AlarmInfoParam> alarmInfoParam;
	ResetAlarmParam()
	{
		resetAlarm = "ResetAlarm";
	}
}ResetAlarmParam;

typedef struct DragZoomParam
{
	bool isZoomOut;
	int length;
	int width;
	int midPointX;
	int midPointY;
	int lengthX;
	int lengthY;
	DragZoomParam()
	{
		isZoomOut = true;
	}
}DragZoomParam;

typedef struct HomePositionParam
{
	int enabled;
	int resetTime;
	int presetIndex;
}HomePositionParam;

typedef struct DeviceConfigParam
{
	// deviceConfig基本参数配置
	std::string name;		// 设备名称
	std::string expiration;			// 注册过期时间
	std::string heartBeatInterval;	// 心跳间隔时间
	std::string heartBeatCount;		// 心跳超时次数

	// svac 编码配置
	// svac 解码配置
}DeviceConfigParam;

typedef struct ControlCmdParam
{
	PtzControlParam   ptzParam;        // Ptz控制命令
	std::string       teleBootParam;   // 远程启动命令
	std::string       recordType;      // 录像控制类型，Record/StopRecord
	std::string       guardType;       // 布防、撤防控制类型，SetGuard/ResetGuard
	ResetAlarmParam   alarmParam;       // 报警复位命令及其扩展参数
	std::string       iFame;           // 强制关键帧
	DragZoomParam     dragZoom;        // 拉框放大缩小
	HomePositionParam homePositionParam;	// 看守卫控制命令
	DeviceConfigParam deviceConfigParam;	// 设备配置
}ControlParam;

// Query
typedef struct QueryCatalog
{
	// 增加设备起始、终止时间（可选）
	std::string startTime;
	std::string endTime;
}QueryCatalog;

typedef struct QueryRecordInfoParam
{
	std::string startTime;	// 录像起始时间
	std::string endTime;	// 录像终止时间
	std::string filePath;	// 文件路径，可选
	std::string address;	// 录像地址，可选
	std::string secrecy;	// 录像保密属性，可选
	std::string type;		// 录像产生类型，可选
	std::string recorderID;	// 录像触发者id，可选
}QueryRecordInfoParam;

typedef struct QueryAlarmParam
{
	std::string startAlarmPriority; // 报警起始级别
	std::string endAlarmPriority;	// 报警结束级别
	std::string alarmMethod;		// 报警方式条件
	std::string alarmType;			// 报警类型
	std::string startAlarmTime;		// 报警开始时间
	std::string endAlarmTime;		// 报警结束时间
}QueryAlarmParam;

typedef struct QueryCmdParam
{
	// ???设备状态查询，无需参数	
	QueryCatalog queryCatalogParam;// 设备目录信息查询
									// ???设备信息查询，无需参数
	QueryRecordInfoParam queryRecordInfoParam;// 文件目录检索请求
	QueryAlarmParam queryAlarmParam;	// 报警查询
	std::string configType;			// 设备配置信息查询
									// 设备预置位查询，无需参数
	int mobileInterval;		// 移动设备位置数据查询
	QueryCmdParam()
	{
		mobileInterval = 5;
		configType = "BasicParam";
	}
}QueryParam;

// Notify
typedef struct NotifyKeepaliveDevList // 心跳可选参数
{
	std::vector<std::string> info; // 故障设备列表
}NotifyKeepaliveDevList;

typedef struct NotifyAlarmInfo
{
	std::string alarmType;
	std::string eventType;
}NotifyAlarmInfo;

typedef struct NotifyAlarmParam // 报警通知
{
	std::string alarmPriority;	// 报警级别，必选
	std::string alarmMethod;	// 报警方式，必选
	std::string alarmTime;		// 报警时间，必选
	std::string alarmType;
	std::string alarmTypeParam;
	std::string deviceID;
	std::string alarmDescription;// 报警内容描述，可选
	std::string longitude;	// 经度，可选
	std::string latitude;	// 纬度，可选
	std::vector<NotifyAlarmInfo> alarmInfo; // 扩展，携带报警类型，报警参数
}NotifyAlarmParam;

typedef struct NotifyBroadcastParam
{
	std::string sourceID;
	std::string targetID;
}NotifyBroadcastParam;

struct NotifyCatalogInfo
{
	std::string deviceID;
	std::string event;
};

typedef struct NotifyMobilePositionParam
{
	std::string time;		// 产生通知时间，必选
	std::string longtitude;	// 经度，必选
	std::string latitude;	// 纬度，必选
	std::string speed;		// 速度，可选
	std::string direction;	// 方向，可选
	std::string altitude;	// 海拔高度，可选
}NotifyMobilePositionParam;

typedef struct NotifyCmdParam
{
	NotifyKeepaliveDevList keepalive;	// 状态信息报送，心跳信息
	NotifyAlarmParam alarm;			// 报警通知
	std::string mediaStatus;// "121",表示历史媒体文件发送结束
	NotifyBroadcastParam broadcast;	// 语音广播通知
	NotifyMobilePositionParam mobilePosition; // 移动设备位置数据通知
	std::vector<NotifyCatalogInfo> deviceList; // 目录变更通知
	NotifyCmdParam()
	{
		mediaStatus = "121";
	}
}NotifyParam;

// Response

// 从下级获取目录解析时和向上级汇报目录时使用
typedef struct CatalogInformation
{
	std::string tableName;
	std::map<std::string, std::string> valuesMap;
}CatalogItem, CatalogInfo;

typedef struct CatalogItemAndInfo			// 向上级汇报资源点使用
{
	CatalogItem resItem;					// 摄像机/设备信息
	CatalogInfo cameraInfo;					// 摄像机扩展信息
}CatalogItemAndInfo;
typedef  std::vector<CatalogItemAndInfo> RespCatalogParam;

typedef struct RespDeviceInfoParam
{
	std::string deviceName;	  // 目标设备名称
	std::string result;		  // 查询结果
	std::string manufacturer; // 设备生产商
	std::string model;		  // 设备型号
	std::string firmware;	  // 设备固件版本
	std::string channel;	  // 视频输入通道数
	std::string info;		  // 扩展信息
}RespDeviceInfoParam;

typedef struct ResDevStatusInfoAlarmstatus
{
	std::string deviceID;
	std::string dutyStatus; // "ONDUTY" "OFFDUTY" "ALARM"
}Alarmstatus;

typedef struct RespDeviceStatusParam
{
	std::string result;	// 查询结果"OK"或"ERROR"
	std::string online;	// "ONLINE" "OFFLINE"
	std::string status;	// 是否正常工作"OK"或"ERROR"
	std::string reason;	// 原因
	std::string encode;	// 是否编码 "ON"或"OFF"
	std::string record;	// 是否录像 "ON"或"OFF"
	std::string deviceTime; // 设备时间或日期
	std::vector<Alarmstatus> alarmStatus; // 报警设备状态列表
	std::string info;       // 扩展信息
}RespDeviceStatusParam;

typedef QueryRecordInfoParam RespRecordInfoItem;
typedef struct RespRecordInfoParam
{
	std::string name;	// 摄像机名称
	std::string sumNum;	// 总历史视频数
	int recordListNum;	// 该次收到的历史视频片段数
	std::vector<RespRecordInfoItem> recordInfoItems;
}RespRecordInfoParam;

typedef struct RespConfigDownloadParam
{
	std::string result;

	// deviceConfig基本参数配置
	std::string name;		        // 设备名称
	std::string expiration;			// 注册过期时间
	std::string heartBeatInterval;	// 心跳间隔时间
	std::string heartBeatCount;		// 心跳超时次数
	std::string positionCapaility;
	std::string Longitude;
	std::string Latitude;
}RespConfigDownloadParam;

typedef struct RespPresetParam
{
	std::string presetID;
	std::string presetName;
}RespPresetParam;
typedef std::vector<RespPresetParam> RespPresetParams;

typedef struct ResponseCmdParam
{
	std::string result; // 设备控制命令应答"OK"或"ERROR"
						// 报警通知应答"OK"或"ERROR"
						// 目录信息查询收到应答"OK"或"ERROR"
						// 设备配置应答"OK"或"ERROR"
						// 语音广播应答"OK"或"ERROR"

	RespCatalogParam        catalogParam;		 // 设备目录查询应答
	RespDeviceInfoParam     devInfoParam;		 // 设备信息查询应答
	RespDeviceStatusParam   devStatusParam;		 // 设备状态信息查询应答
	RespRecordInfoParam     recordInfoParam;	 // 文件目录检索应答
	RespConfigDownloadParam configDownloadParam; // 设备配置查询应答
	RespPresetParams        presetList;          // 设备预置位查询应答

	ResponseCmdParam()
	{
		result = "OK";
	}
}ResponseParam;


typedef struct XmlCmdParam
{
	MESSAGE_MANSCDP_TYPE manscdpType;
	std::string reqRespType; // Control, Query, Notify, Response
	std::string cmdType;
	int sn;
	std::string deviceID;

	ControlParam  controlParam;
	QueryParam    queryParam;
	NotifyParam   notifyParam;
	ResponseParam responseParam;

	std::string localSipID;
	std::string remoteSipID;

	std::string bodyString;
	XmlCmdParam()
	{

	}
}XmlParam;
//////////////////////////////////////////////////////////////////////////

class SipMsgBody
{
public:
	SipMsgBody();
	virtual ~SipMsgBody();

	virtual int CreateSipMsgBody(void* bodyParameter, std::string &bodyString) = 0;
	virtual int ParserSipMsgBody(const std::string bodyString, void* bodyParameter) = 0;
};

#endif // _SIP_MSG_BODY_H_
