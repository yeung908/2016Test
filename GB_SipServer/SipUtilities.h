#ifndef _SIP_UTILITIES_H_
#define _SIP_UTILITIES_H_
#include <string>
#include <random>
#include <chrono>
#include <bitset>
#include <time.h>

#ifdef WIN32
#include <objbase.h>
#else
//#include <uuid/uuid.h>
#endif

namespace SipUtilities
{
	bool CompareStrCase(const char* src, const char* dst);

	int Str2Int(std::string str, int &num);       // 将字符串转换为int，返回值: 0成功，-1失败
	int Str2Double(std::string str, double &num); // 将字符串转换为double，返回值: 0成功，-1失败

	std::string CreatUuid();                     // 生成字符串8-4-4-4-12
	std::string RandLengthStr(const int length); // 生成随机字符长度字符串

	std::string BitsetToHex(std::bitset<8> bite);            // 将bitset<8>转换为16进制表示,每一位为两个字符共16个字符
	int HexToBitset(std::bitset<8> &bitset, std::string hex);// 将16进制表示的1或2个字符转换为bitset<8>类型
	int HexStr2Int(int &num, std::string hex);               // 将字符表示的16进制转换为10进制类型
	int GetBiteByNumber(const std::string cmd, const int pos, std::bitset<8> &bitset);// 从字符串获取pos位置的字符

	int GBTime2NptTime(std::string gbTime, std::string & nptTime);

	std::string GetLocalTime();

};
#endif // _SIP_UTILITIES_H_

