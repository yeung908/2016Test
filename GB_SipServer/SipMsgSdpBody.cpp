#include "SipMsgSdpBody.h"



SipMsgSdpBody::SipMsgSdpBody()
{
}


SipMsgSdpBody::~SipMsgSdpBody()
{
}

int SipMsgSdpBody::CreateSipMsgBody(void * bodyParameter, std::string & bodyString)
{
	SdpMediaParam& sdpMediaParam = *(SdpMediaParam*)bodyParameter;

	sdp_message_t* sdpMsg = nullptr;
	sdp_message_init(&sdpMsg);

	

	return 0;
}

int SipMsgSdpBody::ParserSipMsgBody(const std::string bodyString, void * bodyParameter)
{
	return 0;
}

int SipMsgSdpBody::SetV_Version(sdp_message_t * sdpMsg, const std::string version)
{
	char* sipVersion = osip_strdup(version.c_str());

	if (sdpMsg->v_version != nullptr)
	{
		osip_free(sdpMsg->v_version);
	}
	sdp_message_v_version_set(sdpMsg, sipVersion);
	return 0;
}

int SipMsgSdpBody::SetO_UserName(sdp_message_t * sdpMsg, const std::string userName)
{
	char* sipUsername = osip_strdup(userName.c_str());

	if (sdpMsg->o_username != nullptr)
	{
		osip_free(sdpMsg->o_username);
	}
	sdpMsg->o_username = sipUsername;

	return 0;
}

int SipMsgSdpBody::SetO_SessID(sdp_message_t * sdpMsg, const std::string sessID)
{
	char* sipSessID = osip_strdup(sessID.c_str());

	if (sdpMsg->o_sess_id != nullptr)
	{
		osip_free(sdpMsg->o_sess_id);
	}

	sdpMsg->o_sess_id = sipSessID;

	return 0;
}

int SipMsgSdpBody::SetO_SessVersion(sdp_message_t * sdpMsg, const std::string sessVersion)
{
	char* sipSessVision = osip_strdup(sessVersion.c_str());

	if (sdpMsg->o_sess_version != nullptr)
	{
		osip_free(sdpMsg->o_sess_version);
	}

	sdpMsg->o_sess_version = sipSessVision;

	return 0;
}

int SipMsgSdpBody::SetO_NetType(sdp_message_t * sdpMsg, const std::string netType)
{
	char* sipNetType = osip_strdup(netType.c_str());

	if (sdpMsg->o_nettype != nullptr)
	{
		osip_free(sdpMsg->o_nettype);
	}

	sdpMsg->o_nettype = sipNetType;

	return 0;
}

int SipMsgSdpBody::SetO_AddrType(sdp_message_t* sdpMsg, const std::string addrType /*= "IP4"*/)
{
	char* sipAddrType = osip_strdup(addrType.c_str());

	if (sdpMsg->o_addrtype != nullptr)
	{
		osip_free(sdpMsg->o_addrtype);
	}

	sdpMsg->o_addrtype = sipAddrType;

	return 0;
}

int SipMsgSdpBody::SetO_Addr(sdp_message_t * sdpMsg, const std::string addr)
{
	char* sipAddr = osip_strdup(addr.c_str());

	if (sdpMsg->o_addr != nullptr)
	{
		osip_free(sdpMsg->o_addr);
	}

	sdpMsg->o_addr = sipAddr;

	return 0;
}

int SipMsgSdpBody::SetO_Origin(sdp_message_t * sdpMsg, const SdpMediaParam & sdpMediaParam)
{
	int result = -1;
	if (sdpMsg != nullptr)
	{
		SetO_UserName(sdpMsg, sdpMediaParam.oParam.userName);
		SetO_SessID(sdpMsg, sdpMediaParam.oParam.sessID);
		SetO_SessVersion(sdpMsg, sdpMediaParam.oParam.sessVersion);
		SetO_NetType(sdpMsg, sdpMediaParam.oParam.netType);
		SetO_AddrType(sdpMsg, sdpMediaParam.oParam.addrType);
		SetO_Addr(sdpMsg, sdpMediaParam.oParam.addr);
		result = 0;

// 		char* userName = osip_strdup(sdpMediaParam.oParam.userName.c_str());
// 		char* sessID = osip_strdup(sdpMediaParam.oParam.sessID.c_str());
// 		char* sessVersion = osip_strdup(sdpMediaParam.oParam.sessVersion.c_str());
// 		char* netType = osip_strdup(sdpMediaParam.oParam.netType.c_str());
// 		char* addrType = osip_strdup(sdpMediaParam.oParam.addrType.c_str());
// 		char* addr = osip_strdup(sdpMediaParam.oParam.addr.c_str());
// 		result = sdp_message_o_origin_set(sdpMsg, userName, sessID, sessVersion, netType, addrType, addr);
	}
	
	return 0;
}

int SipMsgSdpBody::SetS_Name(sdp_message_t * sdpMsg, const std::string name)
{
	char* sipName = osip_strdup(name.c_str());

	if (sdpMsg->s_name != nullptr)
	{
		osip_free(sdpMsg->s_name);
	}

	sdp_message_s_name_set(sdpMsg, sipName);

	return 0;
}

int SipMsgSdpBody::GetS_Name(sdp_message_t * sdpMsg, SdpMediaParam & sdpMediaParam)
{
	if (sdpMsg)
	{
		sdpMediaParam.sName = sdpMsg->s_name;
	}
	return 0;
}

int SipMsgSdpBody::SetC_NetType(sdp_message_t* sdpMsg, const std::string netType /*= "IN"*/)
{
	char* sipNetType = osip_strdup(netType.c_str());

	if (sdpMsg->c_connection == nullptr)
	{
		sdp_connection_init(&sdpMsg->c_connection);
	}

	if (sdpMsg->c_connection->c_nettype != nullptr)
	{
		osip_free(sdpMsg->c_connection->c_nettype);
	}

	sdpMsg->c_connection->c_nettype = sipNetType;

	return 0;
}

int SipMsgSdpBody::SetC_AddrType(sdp_message_t* sdpMsg, const std::string addrType /*= "IP4"*/)
{
	char* sipAddrType = osip_strdup(addrType.c_str());

	if (sdpMsg->c_connection->c_addrtype != nullptr)
	{
		osip_free(sdpMsg->c_connection->c_addrtype);
	}

	sdpMsg->c_connection->c_addrtype = sipAddrType;

	return 0;
}

int SipMsgSdpBody::SetC_Addr(sdp_message_t* sdpMsg, const std::string addr)
{
	char* sipAddr = osip_strdup(addr.c_str());

	if (sdpMsg->c_connection->c_addr != nullptr)
	{
		osip_free(sdpMsg->c_connection->c_addr);
	}

	sdpMsg->c_connection->c_addr = sipAddr;

	return 0;
}

int SipMsgSdpBody::SetC_Conection(sdp_message_t* sdpMsg, const SdpMediaParam &sdpMediaParam)
{
	if (sdpMsg->c_connection != nullptr)
	{
		osip_free(sdpMsg->c_connection);
	}
	SetC_NetType(sdpMsg, sdpMediaParam.cParam.netType);
	SetC_AddrType(sdpMsg, sdpMediaParam.cParam.addrType);
	SetC_Addr(sdpMsg, sdpMediaParam.cParam.addr);

	return 0;
}

int SipMsgSdpBody::SetT_Descrs(sdp_message_t * sdpMsg, const std::string startTime, const std::string endTime)
{
	char* sipStart = osip_strdup(startTime.c_str());
	char* sipEnd = osip_strdup(endTime.c_str());

	osip_list_special_free(&sdpMsg->t_descrs, (void(*)(void*))sdp_time_descr_free);

	sdp_message_t_time_descr_add(sdpMsg, sipStart, sipEnd);
	return 0;
}

int SipMsgSdpBody::GetT_Descrs(sdp_message_t * sdpMsg, SdpMediaParam & sdpMediaParam)
{
	sdpMediaParam.tParam.startTime = sdp_message_t_start_time_get(sdpMsg, 0);
	sdpMediaParam.tParam.endTime = sdp_message_t_stop_time_get(sdpMsg, 0);
	return 0;
}

int SipMsgSdpBody::SetM_Medias(sdp_message_t* sdpMsg, const SdpMediaParam &sdpMediaParam)
{
	osip_list_special_free(&sdpMsg->m_medias, (void(*)(void*))sdp_media_free);

	sdp_media_t* sdpMedia = nullptr;
	sdp_media_init(&sdpMedia);

	sdpMedia->m_media = osip_strdup(sdpMediaParam.mParam.mediaType.c_str());
	sdpMedia->m_port = osip_strdup(std::to_string(sdpMediaParam.mParam.port).c_str());
	sdpMedia->m_proto = osip_strdup(sdpMediaParam.mParam.rtpType.c_str());

	char* sipPlayload = nullptr;
	for each (std::string payload in sdpMediaParam.mParam.payloads)
	{
		sipPlayload = osip_strdup(payload.c_str());
		osip_list_add(&sdpMedia->m_payloads, sipPlayload, -1);
	}

	// 添加m中的a字段
	SetM_Attributes(sdpMedia, sdpMediaParam);
	osip_list_add(&sdpMsg->m_medias, sdpMedia, -1);
	return 0;
}

int SipMsgSdpBody::SetM_Attributes(sdp_media_t * sdpMedia, const SdpMediaParam & sdpMediaParam)
{
	osip_list_special_free(&sdpMedia->a_attributes, (void(*)(void*))sdp_attribute_free);

	sdp_attribute_t* attribute = nullptr;

	// recvonly、sendonly
	SetAttribute(attribute, sdpMediaParam.mParam.onlyType, "");
	osip_list_add(&sdpMedia->a_attributes, attribute, -1);

	// tcp连接主动、被动方式
	if (sdpMediaParam.mParam.transType == "active" || sdpMediaParam.mParam.transType == "passive")
	{
		SetAttribute(attribute, "setup", sdpMediaParam.mParam.transType);
		osip_list_add(&sdpMedia->a_attributes, attribute, -1);

		SetAttribute(attribute, "connection", "new");
		osip_list_add(&sdpMedia->a_attributes, attribute, -1);
	}

	// "96 PS/90000"、"97 MPEG4/90000"、"rtpmap", "98 H264/90000"
	for each (std::string payload in sdpMediaParam.mParam.attributes)
	{
		SetAttribute(attribute, "rtpmap", payload);
		osip_list_add(&sdpMedia->a_attributes, attribute, -1);
	}

	return 0;
}

int SipMsgSdpBody::SetAttribute(sdp_attribute_t*& attribute, const std::string field, const std::string value)
{
	if (attribute)
	{
		sdp_attribute_free(attribute);
	}
	
	sdp_attribute_init(&attribute);

// 	char* sipField = nullptr;;
// 	char* sipValue = nullptr;
// 
// 	if (field != "")
// 	{
// 		sipField = osip_strdup(field.c_str());
// 	}
// 	if (value != "")
// 	{
// 		sipValue = osip_strdup(value.c_str());
// 	}

	attribute->a_att_field = osip_strdup(field.c_str());
	attribute->a_att_value = osip_strdup(value.c_str());

	return 0;
}

int SipMsgSdpBody::GetM_Medias(sdp_message_t * sdpMsg, SdpMediaParam & sdpMediaParam)
{
	int result = -1;

	sdp_media_t* sdpMedia = (sdp_media_t*)osip_list_get(&sdpMsg->m_medias, 0);

	result = SipUtilities::Str2Int(sdpMedia->m_port, sdpMediaParam.mParam.port);
	if (result)
	{
		return result;
	}
	

	// 解析media中的payload
	osip_list_t m_payloads = sdpMedia->m_payloads;
	while (m_payloads.node)
	{
		sdpMediaParam.mParam.payloads.push_back((char*)m_payloads.node->element);
		m_payloads.node = m_payloads.node->next;
	}

	// 解析media中的attribute
	osip_list_t m_attributes = sdpMedia->a_attributes;
	while (m_attributes.node)
	{
		sdp_attribute_t* attribute = (sdp_attribute_t*)m_attributes.node->element;

		if (!strcmp(attribute->a_att_field, "recvonly") || !strcmp(attribute->a_att_field, "sendonly"))
		{
			sdpMediaParam.mParam.onlyType = attribute->a_att_field;
		}
		else if (!strcmp(attribute->a_att_field, "setup"))
		{
			sdpMediaParam.mParam.transType = attribute->a_att_value;
		}
		else if (!strcmp(attribute->a_att_field, "rtpmap"))
		{
			sdpMediaParam.mParam.attributes.push_back(attribute->a_att_value);
		}

		m_attributes.node = m_attributes.node->next;
	}

	return 0;
}

int SipMsgSdpBody::SetY(std::string &sdpMsg, std::string y)
{
	if (!y.empty())
	{
		sdpMsg = sdpMsg + "y=" + y + "\r\n";
	}
	return 0;
}

int SipMsgSdpBody::GetY(std::string sdpMsg, std::string & y)
{
	std::string ssrcY = "";
	int ssrc = 0;
	for (int i = 0; i < sdpMsg.size(); i++)
	{
		if (sdpMsg[i] == 'y' || sdpMsg[i] == 'Y')
		{
			if (sdpMsg.size() - i > 2 && (sdpMsg[i + 1] == '=' || sdpMsg[i + 2] == '='))
			{
				ssrcY = sdpMsg.substr(i, sdpMsg.size() - i);
				break;
			}
		}
	}
	sscanf_s(ssrcY.c_str(), "y=%d", &ssrc);
	y = std::to_string(ssrc);
	return 0;
}


int SipMsgSdpBody::SetF(std::string &sdpMsg, std::string f)
{
	if (!f.empty())
	{
		sdpMsg = sdpMsg + "f=" + f + "\r\n";
	}
	return 0;
}

int SipMsgSdpBody::GetF(std::string & sdpMsg, std::string f)
{
	return 0;
}

