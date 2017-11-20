#ifndef _SIP_MSG_RTSP_BODY_H_
#define _SIP_MSG_RTSP_BODY_H_
#include "SipMsgBody.h"

#define RTSP_VISION "RTSP/1.0"

class SipMsgRtspBody :
	public SipMsgBody
{
public:
	SipMsgRtspBody();
	~SipMsgRtspBody();

	virtual int CreateSipMsgBody(void* bodyParameter, std::string &bodyString);
	virtual int ParserSipMsgBody(const std::string bodyString, void* bodyParameter);

private:
	int GetLines(std::string src, std::vector<std::string> &dstLines);// 由rtsp消息体获取每一行命令
	int GetRtspInfo(std::vector<std::string> dstLines, RtspControlParam & rtspInfo); // 解析每一行参数

	int GetScale(std::string scaleLine, float & scale);		// 获取播放倍速数值
	int GetCseq(std::string cseqLine, int & cseq);			// 获取cseq
	int GetRange(std::string rangeLine, RtspControlParam & rtspInfo);	// 获取拖放开始时间

};

#endif // _SIP_MSG_RTSP_BODY_H_
