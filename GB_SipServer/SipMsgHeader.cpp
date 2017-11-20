#include "SipMsgHeader.h"


int SipMsgHeader::SetUriScheme(osip_uri_t * uri, const std::string scheme)
{
	int result = -1;

	if (scheme.size())
	{
		if (uri == nullptr)
		{
			osip_uri_init(&uri);
		}

		if (uri->scheme)
		{
			osip_free(uri->scheme);
			uri->scheme = nullptr;
		}
		uri->scheme = osip_strdup(scheme.c_str());
		result = 0;
	}
	return result;
}

int SipMsgHeader::SetUriUsername(osip_uri_t * uri, const std::string username)
{
	int result = -1;

	if (username.size())
	{
		if (uri == nullptr)
		{
			osip_uri_init(&uri);
		}

		if (uri->username)
		{
			osip_free(uri->username);
			uri->username = nullptr;
		}
		uri->username = osip_strdup(username.c_str());
		result = 0;
	}
	return result;
}

int SipMsgHeader::SetUriHost(osip_uri_t* uri, const std::string host)
{
	int result = -1;

	if (host.size())
	{
		if (uri == nullptr)
		{
			osip_uri_init(&uri);
		}

		if (uri->host)
		{
			osip_free(uri->host);
			uri->host = nullptr;
		}
		uri->host = osip_strdup(host.c_str());
		result = 0;
	}
	return result;
}

int SipMsgHeader::SetUriPort(osip_uri_t * uri, const std::string port)
{
	int result = -1;

	if (port.size())
	{
		if (uri == nullptr)
		{
			osip_uri_init(&uri);
		}

		if (uri->port)
		{
			osip_free(uri->port);
			uri->port = nullptr;
		}
		uri->port = osip_strdup(port.c_str());
		result = 0;
	}
	return result;
}

int SipMsgHeader::SetUri(osip_uri_t ** uri, const std::string username, const std::string host, const std::string port, const std::string scheme)
{
	int result = 0;

	if (*uri == nullptr)
	{
		osip_uri_init(uri);
	}

	SetUriScheme(*uri, scheme);
	SetUriPort(*uri, port);

	result += SetUriUsername(*uri, username);
	result += SetUriHost(*uri, host);
	return result;
}

int SipMsgHeader::SetSipMsgVersion(osip_message_t * sipMsg, const std::string version)
{
	int result = -1;
	if (sipMsg != nullptr && version.size())
	{
		if (sipMsg->sip_version)
		{
			osip_free(sipMsg->sip_version);
			sipMsg->sip_version = nullptr;
		}
		sipMsg->sip_version = osip_strdup(version.c_str());
		result = 0;
	}
	return result;
}

int SipMsgHeader::SetSipMsgReqUri(osip_message_t * sipMsg, const std::string sipID, const std::string host, const std::string port)
{
	return SetUri(&sipMsg->req_uri, sipID, host, port);
}

int SipMsgHeader::SetSipMsgMethod(osip_message_t * sipMsg, const std::string method)
{
	int result = -1;

	if (sipMsg != nullptr && method.size())
	{
		if (sipMsg->sip_method)
		{
			osip_free(sipMsg->sip_method);
			sipMsg->sip_method = nullptr;
		}
		sipMsg->sip_method = osip_strdup(method.c_str());
		result = 0;
	}
	return result;
}

int SipMsgHeader::SetSipMsgStatusCode(osip_message_t * sipMsg, int statusCode)
{
	sipMsg->status_code = statusCode;
	return 0;
}

int SipMsgHeader::SetSipMsgReasonPhrase(osip_message_t* sipMsg, const std::string reasonPhrase)
{
	int result = -1;
	if (sipMsg != nullptr && reasonPhrase.size())
	{
		if (sipMsg->reason_phrase)
		{
			osip_free(sipMsg->reason_phrase);
			sipMsg->reason_phrase = nullptr;
		}

		sipMsg->reason_phrase = osip_strdup(reasonPhrase.c_str());
		result = 0;
	}
	return result;
}

int SipMsgHeader::SetSipMsgVia(osip_message_t * sipMsg, const std::string host, const std::string port, const std::string branchValue, const std::string protocol, const std::string version)
{
	int reslut = -1;
	if (sipMsg != nullptr && protocol.size() && host.size() && port.size())
	{
		osip_via_t* via = nullptr;
		osip_via_init(&via);

		via->version = osip_strdup(version.c_str());
		via->protocol = osip_strdup(protocol.c_str());
		via->host = osip_strdup(host.c_str());
		via->port = osip_strdup(port.c_str());

		// 设置branch
		osip_generic_param_t *viaBranch;
		osip_generic_param_init(&viaBranch);
		std::string branch = "z9hG4bK-";
		branch = branch + branchValue;
		viaBranch->gname = osip_strdup("branch");
		viaBranch->gvalue = osip_strdup(branch.c_str());

		int count = via->via_params.nb_elt;
		osip_list_special_free(&via->via_params, (void(*)(void*))osip_uri_param_freelist);
		osip_list_add(&via->via_params, viaBranch, count);

		// 不可释放内存
		osip_list_special_free(&sipMsg->vias, (void(*)(void *))&osip_via_free);
		osip_list_add(&sipMsg->vias, via, -1);

		reslut = 0;
	}
	return reslut;
}

int SipMsgHeader::SetSipMsgVia(osip_message_t * sipMsg, const osip_via_t * via)
{
	int result = -1;
	if (sipMsg != nullptr && via != nullptr)
	{
		osip_via_t* sipVia = nullptr;
		result = osip_via_clone(via, &sipVia);
		if (!result)
		{
			osip_list_special_free(&sipMsg->vias, (void(*)(void *))&osip_via_free);
			osip_list_add(&sipMsg->vias, sipVia, -1);
		}
	}
	return result;
}

int SipMsgHeader::CloneSipMsgVias(osip_message_t * sipMsg, const osip_list_t * vias)
{
	int result = -1;
	if (sipMsg != nullptr && vias != nullptr)
	{
		result = osip_list_clone(vias, &sipMsg->vias, (clone_func)osip_via_clone);
	}
	return result;
}

int SipMsgHeader::SetSipMsgViaProtocal(osip_message_t * sipMsg, const std::string protocol)
{
	int result = -1;
	if (sipMsg != nullptr || sipMsg->vias.node != nullptr)
	{
		osip_via_t * sipVia = nullptr;

		//sipVia = (osip_via_t*)osip_list_get(&sipMsg->vias, -1);
		sipVia = (osip_via_t*)sipMsg->vias.node->element;
		if (sipVia->protocol)
		{
			osip_free(sipVia->protocol);
			sipVia->protocol = nullptr;
		}
		sipVia->protocol = osip_strdup(protocol.c_str());
		result = 0;
	}	
	return result;
}

int SipMsgHeader::SetSipMsgViaBranch(osip_message_t * sipMsg, std::string branchValue)
{
	int result = -1;
	if (sipMsg != nullptr && branchValue.size())
	{
		osip_generic_param_t *viaBranch = nullptr;
		osip_generic_param_init(&viaBranch);

		std::string branch = "z9hG4bK-";
		branch = branch + branchValue;
		viaBranch->gname = osip_strdup("branch");
		viaBranch->gvalue = osip_strdup(branch.c_str());

		osip_via_t* via = (osip_via_t*)osip_list_get(&sipMsg->vias, 0);

		osip_list_special_free(&via->via_params, (void(*)(void*))osip_uri_param_free);
		result = osip_list_add(&via->via_params, viaBranch, -1);
	}
	return result;
}

int SipMsgHeader::SetTag(osip_list_t * gen_params, const std::string tag)
{
	int result = -1;
	if (gen_params != nullptr && tag.size())
	{
		// 必须释放已存在tag所占内存
		osip_generic_param_freelist(gen_params);

		// 创建新的tag字段，并进行初始化
		osip_generic_param_t *sipTag = nullptr;
		osip_generic_param_init(&sipTag);

		// 设置新tag字面值
		sipTag->gname = osip_strdup("tag");
		sipTag->gvalue = osip_strdup(tag.c_str());

		// 将新tag添加到sipMsg->from->gen_params或sipMsg->to->gen_params
		int count = gen_params->nb_elt;
		result = osip_list_add(gen_params, sipTag, count);
	}
	return 0;
}

int SipMsgHeader::GetTag(osip_list_t * gen_params, std::string & tag)
{
	int result = -1;
	osip_uri_param_t* url_param = nullptr;
	if (gen_params != nullptr)
	{
		result = osip_uri_param_get_byname(gen_params, "tag", &url_param);
		if (url_param)
		{
			tag = url_param->gvalue;
		}
	}
	return result;
}

// int SipMsgHeader::GetSipMsgFromTag(const osip_message_t * sipMsg, std::string & tag)
// {
// 	int result = -1;
// 	if (sipMsg != nullptr && sipMsg->from != nullptr)
// 	{
// 		result = GetTag(&sipMsg->from->gen_params, tag);
// 	}
// 	return result;
// }

int SipMsgHeader::GetSipMsgFromTag(const osip_message_t * sipMsg, std::string & tag)
{
	int result = -1;
	osip_uri_param_t* url_param = nullptr;
	if (sipMsg != nullptr && sipMsg->from != nullptr)
	{
		result = osip_from_get_tag(sipMsg->from, &url_param);
		if (url_param)
		{
			tag = url_param->gvalue;
		}
	}
	return result;
}

// int SipMsgHeader::GetSipMsgToTag(const osip_message_t * sipMsg, std::string & tag)
// {
// 	int result = -1;
// 	if (sipMsg != nullptr && sipMsg->to != nullptr)
// 	{
// 		result = GetTag(&sipMsg->to->gen_params, tag);
// 	}
// 	return result;
// }
int SipMsgHeader::GetSipMsgToTag(const osip_message_t * sipMsg, std::string & tag)
{
	int result = -1;
	osip_uri_param_t* url_param = nullptr;
	if (sipMsg != nullptr && sipMsg->from != nullptr)
	{
		result = osip_to_get_tag(sipMsg->to, &url_param);
		if (url_param)
		{
			tag = url_param->gvalue;
		}
	}
	return result;
}


int SipMsgHeader::SetSipMsgFrom(osip_message_t * sipMsg, const std::string username, const std::string host, const std::string tag)
{
	int result = -1;
	if (sipMsg != nullptr)
	{

		if (sipMsg->from)
		{
			osip_from_free(sipMsg->from);
			sipMsg->from = nullptr;
		}

		osip_from_t * from = nullptr;
		osip_from_init(&from);
		result = SetUri(&from->url, username, host, "", "");
		if (!result)
		{
			SetTag(&from->gen_params, tag);
			sipMsg->from = from;
		}
	}
	return result;
}

int SipMsgHeader::SetSipMsgFrom(osip_message_t * sipMsg, const osip_from_t * from)
{
	int result = -1;
	if (sipMsg != nullptr && from != nullptr)
	{
		if (sipMsg->from)
		{
			osip_from_free(sipMsg->from);
			sipMsg->from = nullptr;
		}

		result = osip_from_clone(from, &sipMsg->from);
	}
	return result;
}

int SipMsgHeader::SetSipMsgFromTag(osip_message_t * sipMsg, const std::string tag)
{
	int result = -1;
	if (sipMsg != nullptr && sipMsg->from != nullptr)
	{
		result = SetTag(&sipMsg->from->gen_params, tag);
	}
	return result;
}

int SipMsgHeader::SetSipMsgTo(osip_message_t * sipMsg, const std::string username, const std::string host, const std::string tag)
{
	int result = -1;
	if (sipMsg != nullptr)
	{

		if (sipMsg->to)
		{
			osip_from_free(sipMsg->to);
			sipMsg->to = nullptr;
		}

		osip_to_t * to = nullptr;
		osip_from_init(&to);
		result = SetUri(&to->url, username, host, "", "");
		if (!result)
		{
			SetTag(&to->gen_params, tag);
			sipMsg->to = to;
		}
	}
	return result;
}

int SipMsgHeader::SetSipMsgTo(osip_message_t * sipMsg, const osip_to_t * to)
{
	int result = -1;
	if (sipMsg != nullptr && to != nullptr)
	{
		if (sipMsg->to)
		{
			osip_from_free(sipMsg->to);
			sipMsg->to = nullptr;
		}

		result = osip_to_clone(to, &sipMsg->to);
	}
	return result;
}

int SipMsgHeader::SetSipMsgToTag(osip_message_t * sipMsg, const std::string tag)
{
	int result = -1;
	if (sipMsg != nullptr && sipMsg->to != nullptr)
	{
		result = SetTag(&sipMsg->to->gen_params, tag);
	}
	return result;
}

int SipMsgHeader::SetSipMsgCallID(osip_message_t * sipMsg, const osip_call_id_t * call_id)
{
	int result = -1;
	if (sipMsg != nullptr && call_id != nullptr)
	{
		if (sipMsg->call_id)
		{
			osip_call_id_free(sipMsg->call_id);
			sipMsg->call_id = nullptr;
		}

		result = osip_call_id_clone(call_id, &sipMsg->call_id);
	}
	return result;
}

int SipMsgHeader::SetSipMsgCallID(osip_message_t * sipMsg, const std::string host, const std::string number)
{
	int result = -1;

	if (sipMsg != nullptr && number.size() && host.size())
	{
		if (sipMsg->call_id)
		{
			osip_call_id_free(sipMsg->call_id);
			sipMsg->call_id = nullptr;
		}

		result = osip_call_id_init(&sipMsg->call_id);
		if (!result)
		{
			sipMsg->call_id->number = osip_strdup(number.c_str());
			sipMsg->call_id->host = osip_strdup(host.c_str());
		}
	}
	return result;
}

int SipMsgHeader::SetSipMsgCallID(osip_message_t* sipMsg, const std::string callID)
{
	int result = -1;

	if (sipMsg != nullptr)
	{
		if (sipMsg->call_id)
		{
			osip_call_id_free(sipMsg->call_id);
			sipMsg->call_id = nullptr;
		}
		result = osip_message_set_call_id(sipMsg, callID.c_str());
	}
	return result;
}

int SipMsgHeader::GetSipMsgCallID(const osip_message_t * sipMsg, std::string & callID)
{
	int result = -1;
	if (sipMsg != nullptr && sipMsg->call_id != nullptr)
	{
		char* callId = nullptr;
		result = osip_call_id_to_str(sipMsg->call_id, &callId);
		if (callId)
		{
			callID = callId;
			osip_free(callId);
			callId = nullptr;
		}
	}
	return result;
}

int SipMsgHeader::SetSipMsgContact(osip_message_t * sipMsg, const std::string username, const std::string host, const std::string port)
{
	int result = -1;
	if (sipMsg != nullptr)
	{
		osip_contact_t* contact = nullptr;
		osip_contact_init(&contact);
		result = SetUri(&contact->url, username, host, port);

		if (!result)
		{
			osip_list_special_free(&sipMsg->contacts, (void(*)(void *)) &osip_contact_free);
			result = osip_list_add(&sipMsg->contacts, contact, 0);
		}
		
		if (result)
		{
			osip_contact_free(contact);
			contact = nullptr;
		}
	}
	return result;
}

int SipMsgHeader::SetSipMsgContact(osip_message_t* sipMsg, const osip_contact_t* contact)
{
	int result = -1;
	if (sipMsg != nullptr && contact != nullptr)
	{
		osip_contact_t* sipContact = nullptr;
		result = osip_contact_clone(contact, &sipContact);
		if (!result)
		{
			osip_list_special_free(&sipMsg->contacts, (void(*)(void *))&osip_contact_free);
			result = osip_list_add(&sipMsg->contacts, sipContact, 0);
		}

		if (result)
		{
			osip_contact_free(sipContact);
			sipContact = nullptr;
		}
	}
	return result;
}

int SipMsgHeader::SetSipMsgCseq(osip_message_t * sipMsg, const std::string num, const std::string method)
{
	int result = -1;

	if (sipMsg != nullptr && num.size() && method.size())
	{
		if (sipMsg->cseq)
		{
			osip_cseq_free(sipMsg->cseq);
			sipMsg->cseq = nullptr;
		}

		osip_cseq_t* cseq = nullptr;
		osip_cseq_init(&cseq);

		cseq->number = osip_strdup(num.c_str());
		cseq->method = osip_strdup(method.c_str());

		sipMsg->cseq = cseq;
		result = 0;
	}
	return result;
}

int SipMsgHeader::SetSipMsgCseq(osip_message_t * sipMsg, const osip_cseq_t * cseq)
{
	int result = -1;
	if (sipMsg != nullptr && cseq != nullptr)
	{
		if (sipMsg->cseq)
		{
			osip_cseq_free(sipMsg->cseq);
			sipMsg->cseq = nullptr;
		}
		osip_cseq_clone(cseq, &sipMsg->cseq);
		result = 0;
	}
	return result;
}

int SipMsgHeader::SetSipMsgMaxForward(osip_message_t* sipMsg, const std::string maxForward /*= "70"*/)
{
	int result = -1;
	if (sipMsg != nullptr && maxForward.size())
	{
		result = osip_message_set_max_forwards(sipMsg, maxForward.c_str());
	}
	return result;
}

int SipMsgHeader::GetSipMsgMaxForward(const osip_message_t * sipMsg, int & maxForward)
{
	int result = -1;
	if (sipMsg != nullptr)
	{
		osip_header_t* sipMaxForwards = nullptr;
		result = osip_message_get_max_forwards(sipMsg, 0, &sipMaxForwards);
		if (!result && sipMaxForwards)
		{
			std::string max_forward = sipMaxForwards->hvalue;
			maxForward = stoi(max_forward);
		}
	}
	return result;
}

int SipMsgHeader::SetSipMsgExpires(osip_message_t * sipMsg, const std::string expires)
{
	int result = -1;
	if (sipMsg != nullptr && expires.size())
	{
		result = osip_message_set_expires(sipMsg, expires.c_str());
	}
	return result;
}

int SipMsgHeader::GetSipMsgExpires(const osip_message_t * sipMsg, std::string & expires)
{
	int result = -1;
	if (sipMsg != nullptr)
	{
		osip_header_t* hExpires = nullptr;
		result = osip_message_get_expires(sipMsg, 0, &hExpires);
		expires = hExpires->hvalue;
	}
	return result;
}

int SipMsgHeader::SetSipMsgContentType(osip_message_t * sipMsg, const std::string contentType)
{
	int result = -1;
	if (sipMsg != nullptr && contentType.size())
	{
		osip_content_type_t *content_type = nullptr;
		osip_content_type_init(&content_type);
		result = osip_content_type_parse(content_type, contentType.c_str());

		if (!result)
		{
			if (sipMsg->content_type)
			{
				osip_content_type_free(sipMsg->content_type);
				sipMsg->content_type = nullptr;
			}

			if (sipMsg->content_type == nullptr)
			{
				sipMsg->content_type = content_type;
			}
		}
		
	}
	return result;
}

int SipMsgHeader::SetSipMsgContentType(osip_message_t * sipMsg, const std::string type, const std::string subType)
{
	int result = -1;
	if (sipMsg != nullptr && type.size() && subType.size())
	{
		osip_content_type_t *content_type = nullptr;
		osip_content_type_init(&content_type);
		content_type->type = osip_strdup(type.c_str());
		content_type->subtype = osip_strdup(subType.c_str());
		
		if (sipMsg->content_type)
		{
			osip_content_type_free(sipMsg->content_type);
			sipMsg->content_type = nullptr;
		}

		if (sipMsg->content_type == nullptr)
		{
			sipMsg->content_type = content_type;
			result = 0;
		}
	}
	return result;
}

int SipMsgHeader::SetSipMsgUserAgent(osip_message_t * sipMsg, const std::string user)
{
	int result = -1;
	if (sipMsg != nullptr && user.size())
	{
		result = osip_message_set_user_agent(sipMsg, user.c_str());
	}
	return result;
}

int SipMsgHeader::SetSipMsgSubject(osip_message_t * sipMsg, const std::string subject)
{
	int result = -1;
	if (sipMsg != nullptr && subject.size())
	{
		result = osip_message_set_subject(sipMsg, subject.c_str());
	}
	return result;
}

int SipMsgHeader::SetSipMsgSubject(osip_message_t * sipMsg, std::string senderSipID, std::string senderNum, std::string receiverSipID, std::string receiverNum)
{
	std::string subject = senderSipID + ":" + senderNum + "," + receiverSipID + ":" + receiverNum;
	return SetSipMsgSubject(sipMsg, subject);
}

int SipMsgHeader::SetSipMsgDate(osip_message_t * sipMsg, const std::string date)
{
	int result = -1;
	if (sipMsg != nullptr && date.size())
	{
		osip_message_set_date(sipMsg, date.c_str());
		result = 0;
	}
	return result;
}

int SipMsgHeader::SetSipMsgWWW_Authrnticate(osip_message_t * sipMsg, const std::string realm, const std::string nonce, const std::string type)
{
	int result = -1;

	if (sipMsg != nullptr && type.size() && realm.size() && nonce.size())
	{
		osip_www_authenticate_t *www_authenticate = nullptr;
		osip_www_authenticate_init(&www_authenticate);

		www_authenticate->nonce = osip_strdup(nonce.c_str());
		www_authenticate->realm = osip_strdup(realm.c_str());
		www_authenticate->auth_type = osip_strdup(type.c_str());

		result = osip_list_add(&sipMsg->www_authenticates, www_authenticate, -1);
	}
	return result;
}

int SipMsgHeader::SetSipMsgAuthorization(osip_message_t * sipMsg, const osip_list_t www_authenticates, const std::string password)
{
	int result = -1;
	if (sipMsg != nullptr && sipMsg->sip_method != nullptr && www_authenticates.node != nullptr && password.size())
	{
		osip_list_iterator_t iterator;
		osip_www_authenticate_t* www_authenticate = nullptr;
		www_authenticate = (osip_www_authenticate_t*)osip_list_get_first(&www_authenticates, &iterator);
		osip_authorization_t* authorization = nullptr;

		// usename
		std::string username = "\"";
		username = username + sipMsg->from->url->username + "\"";

		// uri
		char* uri = nullptr;
		osip_uri_to_str(sipMsg->req_uri, &uri);
		std::string strUri = "\"";
		strUri = strUri + uri + "\"";
		osip_free(uri);

		osip_authorization_init(&authorization);
		authorization->auth_type = osip_strdup(www_authenticate->auth_type);
		authorization->username = osip_strdup(username.c_str());
		authorization->realm = osip_strdup(www_authenticate->realm);
		authorization->nonce = osip_strdup(www_authenticate->nonce);
		authorization->uri = osip_strdup(strUri.c_str());
		authorization->algorithm = osip_strdup("MD5");

		// response
		std::string response = "\"";
		response = response + SipCredentials::GetCredentials(sipMsg->sip_method, authorization, password) + "\"";
		authorization->response = osip_strdup(response.c_str());

		if (sipMsg->authorizations.nb_elt)
		{
			osip_list_special_free(&sipMsg->authorizations, (void(*)(void *))&osip_authorization_free);
		}
		result = osip_list_add(&sipMsg->authorizations, authorization, -1);
	}
	return result;
}
