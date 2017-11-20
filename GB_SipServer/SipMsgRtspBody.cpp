#include "SipMsgRtspBody.h"



SipMsgRtspBody::SipMsgRtspBody()
{
}

SipMsgRtspBody::~SipMsgRtspBody()
{
}

int SipMsgRtspBody::CreateSipMsgBody(void * bodyParameter, std::string & bodyString)
{
	RtspControlParam& rtspParam = *(RtspControlParam*)bodyParameter;

	rtspParam.bodyString = "";
	rtspParam.bodyString = rtspParam.method + " " + RTSP_VISION + "\r\n";
	rtspParam.bodyString = rtspParam.bodyString + "CSeq:" + std::to_string(rtspParam.cseq) + "\r\n";

	if (rtspParam.method == "PLAY") //播放
	{
		if (rtspParam.scale > 0.0) // 倍速播放
		{
			rtspParam.bodyString = rtspParam.bodyString + "Scale:" + std::to_string(rtspParam.scale) + "\r\n";
		}
		else // 原速播放
		{
			if (rtspParam.startTime > 0) // 拖拽播放
			{
				rtspParam.bodyString = rtspParam.bodyString + "Range:npt=" + std::to_string(rtspParam.startTime) + "-"; // 开始播放、恢复播放
				if (rtspParam.endTime > rtspParam.startTime) // 拖拽播放
				{
					rtspParam.bodyString = rtspParam.bodyString + "\r\n" + std::to_string(rtspParam.endTime);
				}
			}
			else // 继续播放
			{
				rtspParam.bodyString = rtspParam.bodyString + "Range:npt=now-";
			}
			rtspParam.bodyString = rtspParam.bodyString + "\r\n";
		}
	}
	else if (rtspParam.method == "PAUSE") //暂停
	{
		rtspParam.bodyString = rtspParam.bodyString + "PauseTime: now\r\n"; // GB28181测试now前必须有空格
	}
	bodyString = rtspParam.bodyString;

	return 0;
}

int SipMsgRtspBody::ParserSipMsgBody(const std::string bodyInfo, void * bodyParameter)
{
	RtspControlParam& rtspInfo = *(RtspControlParam*)bodyParameter;
	std::vector<std::string> dstLines;
	GetLines(bodyInfo, dstLines);
	if (dstLines.size() > 3)
	{
		return -1;
	}

	// 获取各字段属性值
	return GetRtspInfo(dstLines, rtspInfo);
}

int SipMsgRtspBody::GetLines(std::string src, std::vector<std::string> &dstLines)
{
	std::string line = "";
	for (size_t i = 0; i < src.size(); i++)
	{
		if (src[i] != '\r' && src[i] != '\n')
		{
			line = line + src[i];
		}

		if (src[i] == '\r' || src[i] == '\n' || i == src.size() - 1)
		{
			if (line.size() > 4)
			{
				dstLines.push_back(line);
				line = "";
			}
		}
	}
	return 0;
}

int SipMsgRtspBody::GetRtspInfo(std::vector<std::string> dstLines, RtspControlParam & rtspInfo)
{
	int result = 0;
	for (size_t i = 0; i < dstLines.size(); i++)
	{
		if (dstLines[i][0] == 'P')
		{
			if (dstLines[i][1] == 'L') // 播放 PLAY
			{
				rtspInfo.rtspType = RTSP_TYPE::PLAY;
				rtspInfo.method = "PLAY";
			}
			else if (dstLines[i][1] == 'a') // 暂停PauseTime
			{
				rtspInfo.rtspType = RTSP_TYPE::PAUSE;
				rtspInfo.method = "PAUSE";
			}
		}
		else if (dstLines[i][0] == 'C') // CSeq
		{
			result = GetCseq(dstLines[i], rtspInfo.cseq);
		}
		else if (dstLines[i][0] == 'R') // Range
		{
			result = GetRange(dstLines[i], rtspInfo);
		}
		else if (dstLines[i][0] == 'S') // Scale // 倍速播放
		{
			result = GetScale(dstLines[i], rtspInfo.scale);
			rtspInfo.rtspType = RTSP_TYPE::SCALE;
		}
		else if (dstLines[i][0] == 'T') // Teardown
		{
			result = rtspInfo.rtspType = RTSP_TYPE::TEARDOWN;
		}

		// 解析异常，退出循环
		if (result == -1)
		{
			break;
		}
	}
	return result;
}

int SipMsgRtspBody::GetScale(std::string scaleLine, float & scale)
{
	int result = -1;
	if (sscanf_s(scaleLine.c_str(), "Scale:%f", &scale) == 1)
	{
		result = 0;
	}
	return result;
}

int SipMsgRtspBody::GetCseq(std::string cseqLine, int & cseq)
{
	int result = -1;
	if (sscanf_s(cseqLine.c_str(), "CSeq:%d", &cseq) == 1)
	{
		result = 0;
	}
	return result;
}

int SipMsgRtspBody::GetRange(std::string rangeLine, RtspControlParam & rtspInfo)
{
	int result = -1;
	std::string paramStr = "";
	for (size_t i = 0; i < rangeLine.size(); i++)
	{
		if (rangeLine[i] == ':')
		{
			i++;
			paramStr = rangeLine.substr(i, rangeLine.size() - i);
			break;
		}
	}

	if (paramStr.size() < 5)
	{
		return result;
	}

	if (sscanf_s(paramStr.c_str(), "npt = %ld - %ld", &rtspInfo.startTime, &rtspInfo.endTime) == 2)
	{
		rtspInfo.rtspType = RTSP_TYPE::RANGE;
		result = 0;
	}
	else if (sscanf_s(paramStr.c_str(), "npt = %ld -", &rtspInfo.startTime) == 1)
	{
		rtspInfo.endTime = 0;
		rtspInfo.rtspType = RTSP_TYPE::RANGE;
		result = 0;
	}
	else if (paramStr == "npt=now-")
	{
		rtspInfo.startTime = 0;
		rtspInfo.endTime = 0;
		rtspInfo.rtspType = RTSP_TYPE::PLAY;
		result = 0;
	}
	else
	{
		int hour = 0;
		int minute = 0;
		int second = 0;
		if (sscanf_s(paramStr.c_str(), "smtpe = %d:%d:%d", &hour, &minute, &second) == 3)
		{
			result = 0;
		}
		rtspInfo.startTime = (hour * 60 + minute) * 60 + second;
	}
	/*else if (sscanf(paramStr.c_str(), "clock = %*s%n", &numCharsMatched) == 0 && numCharsMatched > 0)
	{
	// We accept "clock=" parameters, but currently do no interpret them.
	}
	else
	{
	return False; // The header is malformed
	}*/

	return result;
}
