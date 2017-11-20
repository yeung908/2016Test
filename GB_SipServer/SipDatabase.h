#ifndef _SIP_DATABASE_H_
#define _SIP_DATABASE_H_

#include <iostream>
#include <string>
#include <vector>

#define OTL_ODBC_MYSQL
#include "otlv4.h"

typedef std::vector<std::string> VecStrings;

class SipDatabase
{
public:
	SipDatabase();
	virtual ~SipDatabase();

	int LoginDatabase();
	int LogoutDatabase();
	int CheckDB();

	int GetIntData(const std::string sql, int &data);
	std::string GetStringData(const std::string sql);
	int GetStringData(const std::string sql, std::string &data);

	// 获取本级平台信息
	int GetLocalPort();
	std::string GetLocalIP();
	std::string GetLocalSipID();
	std::string GetLocalSipDomain();
	int GetLocalPlatformParam(std::string &sipId, std::string &sipIP, std::string &sipPort, std::string &sipDomain);

	// 获取上级平台、下级平台SipID
	int GetUpperPlatformIDs(VecStrings &sipIds);
	int GetLowerPlatformIDs(VecStrings &sipIds);

	int GetPlatformIdByDeviceID(const std::string deviceID, int &platformID);
	bool IsGb28181Device(std::string deviceID);
	bool IsGb28181Device(int resID);

	// 获取平台密码
	std::string GetUpperPassword(const std::string sipID);
	std::string GetLowerPassword(const std::string sipID);

	// 平台状态复位
	int ResetPlatformStatus();

	// 设置平台/设备状态
	int SetDeviceStatus(const std::string deviceID, const std::string ststus);
	int GetDeviceStatus(const std::string deviceID, std::string &ststus);

	// 获取设备信息

	// 设置tbl_group表的ParentID字段
	int SetGroupParentID();
private:
	otl_connect db;

	int InsertData(std::string sqlStr); // 插入数据
	int DeleteData(std::string sqlStr); // 删除数据
	int UpdateData(std::string sqlStr); // 更新数据
	int ExecuteSQL(const std::string sql);

	int GetStringVec(VecStrings &vecStringDatas, std::string sql);
};
#endif