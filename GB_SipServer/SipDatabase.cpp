#include "SipDatabase.h"



SipDatabase::SipDatabase()
{
}


SipDatabase::~SipDatabase()
{
}

int SipDatabase::LoginDatabase()
{
	int result = -1;
	try
	{
		char* dbInfo = "DSN=AVSS";
		db.rlogon(dbInfo);

		std::cout << "登录数据库成功！！！" << std::endl;
		result = 0;
	}
	catch (otl_exception& p)
	{
		std::cout << "登录数据库失败！！！" << std::endl;
	}

	return result;
}

int SipDatabase::LogoutDatabase()
{
	db.logoff();
	//std::cout << "注销数据库成功！！！" << std::endl;
	return 0;
}

int SipDatabase::CheckDB()
{
	bool isLogout = false;
	try
	{
		otl_stream o(1, "select PlatformID from tbl_platform", db);
		db.commit();
	}
	catch (otl_exception& p)
	{
		isLogout = true;
	}

	if (isLogout)
	{
		LogoutDatabase();
		LoginDatabase();
	}
	return 0;
}

int SipDatabase::GetIntData(const std::string sql, int & data)
{
	CheckDB();
	int result = -1;
	try
	{
		otl_stream o(1, sql.c_str(), db);
		o >> data;
		result = 0;
	}
	catch (otl_exception& p)
	{
		printf("%s\n", p.msg);
	}
	return result;
}

std::string SipDatabase::GetStringData(const std::string sql)
{
	std::string data = "";
	GetStringData(sql, data);
	return data;
}

int SipDatabase::GetStringData(const std::string sql, std::string & data)
{
	CheckDB();
	int result = -1;
	char value[128] = { 0 };
	try
	{
		otl_stream o(1, sql.c_str(), db);
		o >> value;
		data = value;
		result = 0;
	}
	catch (otl_exception& p)
	{
		printf("%s\n", p.msg);
	}
	return result;
}

int SipDatabase::GetLocalPort()
{
	std::string sqlString = "select PlatformPort from tbl_Platform where PlatformID = 1";
	int port = -1;
	GetIntData(sqlString, port);
	return port;
}

std::string SipDatabase::GetLocalIP()
{
	std::string sqlStr = "select IPAddress from tbl_Platform where PlatformID = 1";
	std::string localIP = GetStringData(sqlStr);
	return localIP;
}

std::string SipDatabase::GetLocalSipID()
{
	std::string sqlStr = "select SipID from tbl_Platform where PlatformID = 1";
	std::string localSipID = GetStringData(sqlStr);
	return localSipID;
}

std::string SipDatabase::GetLocalSipDomain()
{
	std::string sqlStr = "select SipDomain from tbl_Platform where PlatformID = 1";
	std::string localDomain = GetStringData(sqlStr);
	return localDomain;
}

int SipDatabase::GetLocalPlatformParam(std::string & sipId, std::string & sipIP, std::string & sipPort, std::string & sipDomain)
{
	int result = -1;
	CheckDB();

	std::string sqlString = "select SipID, IPAddress, PlatformPort, SipDomain from tbl_Platform where PlatformID = 1";

	char id[100] = { 0 };
	char ip[100] = { 0 };
	int port = 0;
	char domain[100] = { 0 };

	try
	{
		otl_stream o(1, sqlString.c_str(), db);
		while (!o.eof())
		{
			o >> id;
			o >> ip;
			o >> port;
			o >> domain;

			sipId = id;
			sipIP = ip;
			sipPort = std::to_string(port);
			sipDomain = domain;

			result = 0;
		}
	}
	catch (otl_exception& p)
	{
		printf("%s\n", p.msg);		
	}
	return 0;
}

int SipDatabase::GetUpperPlatformIDs(VecStrings & sipIds)
{
	std::string sql = "SELECT SipID FROM TBL_Platform WHERE ConnectState = \'1\' OR ConnectState = \'3\'";
	return GetStringVec(sipIds, sql);
}

int SipDatabase::GetLowerPlatformIDs(VecStrings & sipIds)
{
	std::string sql = "SELECT SipID FROM TBL_Platform WHERE ConnectState = 2";
	return GetStringVec(sipIds, sql);
}

std::string SipDatabase::GetUpperPassword(const std::string sipID)
{
	std::string sqlString = "select UpPassword from tbl_Platform where SipID = '" + sipID + "'";
	return GetStringData(sqlString);
}

std::string SipDatabase::GetLowerPassword(const std::string sipID)
{
	std::string sqlString = "select DownPassword from tbl_Platform where SipID = '" + sipID + "'";
	return GetStringData(sqlString);
}

int SipDatabase::ResetPlatformStatus()
{
	std::string sqlStr = "update TBL_PlatForm set status = \'OFF\' where PlatformID != \'1\'";
	return ExecuteSQL(sqlStr);
}

int SipDatabase::SetDeviceStatus(const std::string deviceID, const std::string ststus)
{
	return 0;
}

int SipDatabase::SetGroupParentID()
{
	std::string sql = "";
	// 1 设置行政区划的ParentID
	// 顶级行政区划
	sql = "UPDATE tbl_group b\
		SET b.ParentID = b.GroupID\
		WHERE(LEFT(b.VirtualOrgID, CHAR_LENGTH(b.VirtualOrgID) - 2)NOT IN(SELECT\
			VirtualOrgID\
			FROM(SELECT *\
				FROM tbl_group) a))\
		AND b.Type = 1;";
	ExecuteSQL(sql);

	// 一般行政区划
	sql = "UPDATE tbl_group b\
		SET b.ParentID = (SELECT\
			a.GroupID\
			FROM(SELECT *\
				FROM tbl_group) a\
			WHERE a.virtualOrgID = LEFT(b.VirtualOrgID, CHAR_LENGTH(b.VirtualOrgID) - 2))\
		WHERE(LEFT(b.VirtualOrgID, CHAR_LENGTH(b.VirtualOrgID) - 2)IN(SELECT\
			VirtualOrgID\
			FROM(SELECT *\
				FROM tbl_group) a))\
		AND b.Type = 1;";
	ExecuteSQL(sql);

	// 2 设置业务分组的ParentID
	sql = "UPDATE tbl_group a,\
		tbl_platform b\
		SET a.ParentID = b.PlatformID\
		WHERE a.ParentOrgID = b.SipID\
		AND a.Type = 2;";
	ExecuteSQL(sql);

	// 3 设置虚拟组织的ParentID
	// #3.1设置顶级虚拟组织的ParentID
	sql = "UPDATE tbl_group b\
		SET b.ParentID = (SELECT\
			a.GroupID\
			FROM(SELECT *\
				FROM tbl_group) a\
			WHERE a.VirtualOrgID = b.BusinessGroupID)\
		WHERE(SELECT\
			a.GroupID\
			FROM(SELECT *\
				FROM tbl_group) a\
			WHERE a.VirtualOrgID = b.BusinessGroupID)IS NOT NULL\
		AND(b.ParentOrgID IS NULL\
			AND b.Type = 3);";
	ExecuteSQL(sql);
	// #3.2设置虚拟组织分组的ParentID
	sql = "UPDATE tbl_group b\
		SET b.ParentID = (SELECT\
			a.groupid\
			FROM(SELECT *\
				FROM tbl_group) a\
			WHERE a.virtualOrgID = b.ParentOrgID)\
		WHERE(SELECT\
			a.groupid\
			FROM(SELECT *\
				FROM tbl_group) a\
			WHERE a.virtualOrgID = b.ParentOrgID)IS NOT NULL\
		AND b.Type = 3;";
	ExecuteSQL(sql);
	return 0;
}

int SipDatabase::InsertData(std::string sqlStr)
{
	return ExecuteSQL(sqlStr);
}

int SipDatabase::DeleteData(std::string sqlStr)
{
	return ExecuteSQL(sqlStr);
}

int SipDatabase::UpdateData(std::string sqlStr)
{
	return ExecuteSQL(sqlStr);
}

int SipDatabase::ExecuteSQL(const std::string sql)
{
	CheckDB();
	int result = -1;
	try
	{
		otl_stream o(1, sql.c_str(), db);
		result = 0;
	}
	catch (otl_exception& p)
	{
		printf("%s\n", p.msg);
	}
	return result;
}

int SipDatabase::GetStringVec(VecStrings & vecStringDatas, std::string sql)
{
	CheckDB();
	int result = -1;

	char value[128] = { 0 };
	try
	{
		otl_stream o(1, sql.c_str(), db);
		while (!o.eof())
		{
			o >> value;
			vecStringDatas.push_back(value);
			result = 0;
		}
	}
	catch (otl_exception& p)
	{
		printf("%s\n", p.msg);
	}
	return result;
}
