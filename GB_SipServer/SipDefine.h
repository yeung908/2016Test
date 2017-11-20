#ifndef _SIP_DEF_H
#define _SIP_DEF_H
/************************************************************************/
/* 本文件内容不能修改
/************************************************************************/
enum MESSAGE_MANSCDP_TYPE
{
	MESSAGE_MANSCDP_ERROR,	// 错误的命令

	//控制，10
	CONTROL_DEVICE_PTZ,		// 设备ptz控制
	CONTROL_TELE_BOOT,		// 远程启动控制
	CONTROL_RECORD,			// 录像控制，开始录像、停止录像
	CONTROL_GUARD,			// 布防、撤防
	CONTROL_ALARM,			// 报警复位命令
	CONTROL_IFAME,			// 强制关键帧
	CONTROL_DRAG_ZOOM,	    // 拉框放大\缩小
	CONTROL_HOME_POSITION,	// 看守卫控制命令，自动归位
	CONTROL_DEVICE_CONFIG,	// 设备配置

	//查询请求，8
	QUERY_DEVICE_STATUS,	// 设备状态查询请求
	QUERY_DEVICE_CATALOG,	// 设备目录信息查询请求
	QUERY_DEVICE_INFO,		// 设备信息查询请求
	QUERY_RECORD_INFO,		// 文件目录检索请求
	QUERY_ALARM,			// 报警查询
	QUERY_CONFIG_DOWNLOAD,	// 设备配置查询
	QUERY_PRESET_QUERY,		// 设备预置位查询
	QUERY_MOBILE_POSITION,	// 移动设备位置数据查询

	//通知,5
	NOTIFY_KEEPALIVE,		// 状态信息报送，心跳
	NOTIFY_ALARM,			// 报警通知
	NOTIFY_MEDIA_STATUS,	// 媒体通知，filetoend
	NOTIFY_BROADCAST,		// 语音广播通知
	NOTIFY_MOBILE_POSITION,	// 移动设备位置数据通知
	NOTIFY_CATALOG,

	//应答，12
	RESPONSE_DEVICE_CONTROL,			// 设备控制应答
	RESPONSE_ALARM_NOTIFY,				// 报警通知应答
	RESPONSE_DEVICE_CATALOG_QUERY,		// 设备目录信息查询应答（目录通知）
	RESPONSE_CATALOG_QUERY_RECEIVED,	// 目录信息查询收到应答（目录订阅应答）
	RESPONSE_CATALOG_RECEIVED,			// 目录收到应答（目录通知应答）
	RESPONSE_DEVICE_INFO_QUERY,			// 设备信息查询应答
	RESPONSE_DEVICE_STATUS_QUERY,		// 设备状态信息查询应答
	RESPONSE_DOCUMENT_RECORD_INFO,		// 文件目录检索应答
	RESPONSE_DEVICE_CONFIG,				// 设备配置应答
	RESPONSE_DEVICE_CONFIG_DOWNLOAD,	// 设备配置查询应答
	RESPONSE_DEVICE_PRESET_QUERY,		// 设备预置位查询应答
	RESPONSE_BROADCAST,					// 语音广播应答

	CATALOG_CHANGED,

	XML_COUNT
};

enum CONTROL_TYPE
{
	PTZ_STOP		= 0x1009,	//摄像机停止控制
	VIDEO_SWITCH	= 0x1010,	//摄像机切换
	PTZ_UP			= 0x1011,	//向上旋转摄像头
	PTZ_UP_STOP		= 0x1012,	//摄像头向上旋转停止
	PTZ_DOWN		= 0x1013,	//向下旋转摄像头
	PTZ_DOWN_STOP	= 0x1014,	//摄像头向下旋转停止
	PTZ_LEFT		= 0x1015,	//向左旋转摄像头
	PTZ_LEFT_STOP	= 0x1016,	//摄像头向左旋转停止
	PTZ_RIGHT		= 0x1017,	//向右旋转摄像头
	PTZ_RIGHT_STOP	= 0x1018,	//摄像头向右旋转停止
	ZOOM_IN			= 0x1019,	//镜头变倍放大
	ZOOM_IN_STOP	= 0x1020,	//镜头变倍放大停止
	ZOOM_OUT		= 0x1021,	//镜头变倍缩小
	ZOOM_OUT_STOP	= 0x1022,	//镜头变倍缩小停止

	FOCUS_FAR		= 0x1023,	//镜头焦距调远
	FOCUS_FAR_STOP	= 0x1024,	//镜头焦距调远停止
	FOCUS_NEAR		= 0x1025,	//镜头焦距调近
	FOCUS_NEAR_STOP = 0x1026,	//镜头焦距调近停止
	IRIS_OPEN		= 0x1027,	//镜头光圈放大
	IRIS_OPEN_STOP	= 0x1028,	//镜头光圈放大停止
	IRIS_CLOSE		= 0x1029,	//镜头光圈缩小
	IRIS_CLOSE_STOP = 0x1030,	//镜头光圈缩小停止

	GET_PRESET		= 0x1031,	//查询预置位,20170912添加
	GOTO_PRESET		= 0x1032,	//调用预置位
	ADD_PRESET		= 0x1033,	//添加预置位\shezhi
	DEL_PRESET		= 0x1034,	//删除预置位
	RENAME_PRESET	= 0x1035,	//重命名预置位	,20170912添加 

	//////////////////////////////////////////////////////////////////////////
// 	GET_CRUISE		= 0x1036,	// 查询巡航路径
 	RUN_CRUISE		= 0x1037,	// 调用巡航路径
 	STOP_CRUISE		= 0x1038,	// 停止巡航
// 	RENAME_CRUISE	= 0x1039,	// 重命名巡航路径
 	ADD_CRUISE		= 0x103A,	// 添加巡航
 	DEL_CRUISE		= 0x103B,	// 删除巡航		
// 	STA_MEM_TRACK	= 0x103C,	// 开始记录轨迹		
// 	STO_MEM_TRACK	= 0x103D,	// 停止记录轨迹		
// 	RUN_TRACK		= 0x103E,	// 开始轨迹		
// 	RUN_TRACK_STOP	= 0x103F,	// 停止轨迹		


	//ADD_CRUISE = 0x1A,		//加入巡航点
	//DEL_CRUISE = 0x1B,		//删除一个巡航点
	//RUN_CRUISE = 0x1E,		//开始巡航
	//STOP_CRUISE = 0x1F,		//停止巡航
	SET_NAVI_SPEED		= 0x1C,	//设置巡航速度
	SET_NAVI_STOP_TIME	= 0x1D, //设置巡航停留时间

	//////////////////////////////////////////////////////////////////////////
	PTZ_LEFT_UP			= 0x1040,	// 向左上旋转摄像头		 
	PTZ_LEFT_UP_STOP	= 0x1041,	// 摄像头向左上旋转停止	
	PTZ_LEFT_DOWN		= 0x1042,	// 向左下旋转摄像头		
	PTZ_LEFT_DOWN_STOP	= 0x1043,	// 摄像头向左下旋转停止		
	PTZ_RIGHT_UP		= 0x1044,	// 向右上旋转摄像头		
	PTZ_RIGHT_UP_STOP	= 0x1045,	// 摄像头向右上旋转停止		 
	PTZ_RIGHT_DOWN		= 0x1046,	// 向右下旋转摄像头		
	PTZ_RIGHT_DOWN_STOP = 0x1047,	// 摄像头向右下旋转停止	

	//////////////////////////////////////////////////////////////////////////
	//RUN_SCAN = 0x1051,			//开始自动扫描
	AUTO_SCANNING_LEFT  = 0x21,		//设置自动扫描左边界
	AUTO_SCANNING_RIGHT = 0x22,		//设置自动扫描右边界
	SET_AUTO_SCANNING_SPEED = 0x23,	//设置自动扫描速度
	//STOP_SCAN = 0x1052,			//停止扫描

// 	GET_SCAN = 0x1050,				// 查询扫描		
 	RUN_SCAN = 0x1051,				// 启动扫描		
 	STOP_SCAN = 0x1052,				// 停止扫描		
// 	RENAME_SCAN = 0x1053,			// 重命名扫描		
// 	ADD_SCAN = 0x1054,				// 添加扫描		
// 	DEL_SCAN = 0x1055,				// 删除扫描
	//////////////////////////////////////////////////////////////////////////

	SET_GUARD     = 0x1060,	// 布防		
	RESET_GUARD   = 0x1061,	// 撤防		
	REMOTE_START  = 0X1070,	// 远程启动
	IFAME         = 0X1071,	// 强制关键帧
	SYSTEM_ALARM  = 0X1072,	// 系统报警
	RESET_ALARM   = 0X1073,	// 报警复位
	DRAG_ZOON_IN  = 0X1074,	// 拉框放大
	DRAG_ZOON_OUT = 0X1075,	// 拉框缩小
	HOME_POSITION = 0X1076,	// 看守位控制

	OPEN_CAMERA = 0X25,
	CLOSE_CAMERA = 0X26,

};
#endif