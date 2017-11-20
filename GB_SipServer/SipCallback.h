#ifndef _SIP_CALLBACK_H_
#define _SIP_CALLBACK_H_
#include "SipServer.h"

namespace SipCallback
{
	// 发送sip消息
	int osip_send_message_cb(osip_transaction_t* trn,
		                     osip_message_t* sip_msg,
		                     char* dest_socket_str,
		                     int   dest_prot,
		                     int   send_sock);

	// sip消息后回调处理
	void osip_nict_rcv_2xx_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_nict_rcv_3xx_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_nict_rcv_4xx_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_nict_send_register_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_nict_send_notify_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_nict_send_subscribe_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);

	void osip_ict_rcv_2xx_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_ict_rcv_3xx_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_ict_rcv_45xx_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_ict_send_invite_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);

	void osip_ist_rcv_invite_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_ist_rcv_ack_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);

	void osip_nist_rcv_register_cb(int type, osip_transaction_t *trn, osip_message_t *sipMsg);
	void osip_nist_rcv_notify_cb(int type, osip_transaction_t *trn, osip_message_t *sipMsg);
	void osip_nist_rcv_subscribe_cb(int type, osip_transaction_t *trn, osip_message_t *sipMsg);
	void osip_nist_rcv_bye_cb(int type, osip_transaction_t *trn, osip_message_t *sipMsg);
	void osip_nist_rcv_info_cb(int type, osip_transaction_t *trn, osip_message_t *sipMsg);

	void osip_rcv_message_cb(int type, osip_transaction_t *trn, osip_message_t *sipMsg);

	// 状态超时处理
	void osip_nict_status_timeout_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);

	// transaction 回调处理
	void osip_ict_kill_transaction_cb(int type, osip_transaction_t *trn);
	void osip_ist_kill_transaction_cb(int type, osip_transaction_t *trn);
	void osip_nict_kill_transaction_cb(int type, osip_transaction_t *trn);
	void osip_nist_kill_transaction_cb(int type, osip_transaction_t *trn);
	void osip_ict_transport_error_cb(int type, osip_transaction_t *, int error);
	void osip_ist_transport_error_cb(int type, osip_transaction_t *, int error);
	void osip_nict_transport_error_cb(int type, osip_transaction_t *, int error);
	void osip_nist_transport_error_cb(int type, osip_transaction_t *, int error);
};
#endif // _SIP_CALLBACK_H_

