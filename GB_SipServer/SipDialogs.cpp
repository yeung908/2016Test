#include "SipDialogs.h"

int SipDialogs::mDialogID = 1;
SipDialogs* SipDialogs::mInstance = nullptr;

SipDialogs::SipDialogs()
{
}

SipDialogs::~SipDialogs()
{
}

SipDialogs * SipDialogs::GetDialogsInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new SipDialogs();
	}
	return mInstance;
}

osip_dialog_t * SipDialogs::CreateSipDialog(osip_transaction_t * trn, osip_message_t * resSipMsg, DialogType dialogType)
{
	if (trn == nullptr || resSipMsg == nullptr)
	{
		return nullptr;
	}

	osip_dialog_t* dialog = nullptr;
	DialogHint hint;

	if (trn->ctx_type == ICT || trn->ctx_type == NICT)
	{
		hint = DialogHint::UAC;
		osip_dialog_init_as_uac(&dialog, resSipMsg);
// 		// UAC时，orig_request由本身发出
// 		// 且route字段为对方平台的contact信息
// 		if (!dialog->remote_contact_uri)
// 		{
// 		}
	}
	else
	{
		hint = DialogHint::UAS;
		osip_dialog_init_as_uas(&dialog, trn->orig_request, resSipMsg);
// 		if (!dialog->remote_contact_uri)
// 		{
// 		}
	}

	if (dialog != nullptr)
	{
		DialogInfo* dialogInfo = new DialogInfo;
		if (dialogInfo != nullptr)
		{
			dialogInfo->sn = 0;
			dialogInfo->hint = hint;
			dialogInfo->dialogType = dialogType;
			dialogInfo->birthTime = time(NULL);
			dialogInfo->lastResponseTime = time(NULL);

			// 目标设备DeviceID，可能是平台、设备
			dialogInfo->dstDeviceID = trn->orig_request->req_uri->username;

			if (hint == UAS) // 本级为下级、服务端
			{
				dialogInfo->localSipID = dialogInfo->dstDeviceID;
				dialogInfo->remoteSipID = resSipMsg->from->url->username;
			}
			else // 本级为下级、客户端
			{
				dialogInfo->localSipID = resSipMsg->from->url->username;;
				dialogInfo->remoteSipID = dialogInfo->dstDeviceID;
			}
		}
				
		std::lock_guard<std::mutex> lock(mDialogIdMutex);
		dialogInfo->dialogID = SipDialogs::mDialogID;
		SipDialogs::mDialogID++;
		mDialogIdMutex.unlock();

		dialog->your_instance = dialogInfo; // 将dialogInfo与dialog相关联
	}

	return dialog;
}

void SipDialogs::DestorySipDialog(osip_dialog_t *& dialog)
{
	if (dialog)
	{
		if (dialog->your_instance != nullptr)
		{
			DialogInfo* dialogInfo = (DialogInfo*)dialog->your_instance;
			delete dialogInfo;
			dialogInfo = nullptr;
		}

		dialog->your_instance = nullptr;
		osip_dialog_free(dialog);
		dialog = nullptr;
	}
}

void SipDialogs::InsertSipDialog(osip_dialog_t * dialog, DialogType dialogType)
{
	switch (dialogType)
	{
	case REGISTER_DIALOG:
		mRegisterMutex.lock();
		mRegisterDialogs.push_back(dialog);
		mRegisterMutex.unlock();
		break;
	case SUBSCRIBE_DIALOG:
		mSubcribeMutex.lock();
		mSubcribeDialogs.push_back(dialog);
		mSubcribeMutex.unlock();
		break;
	default:
		mInviteMutex.lock();
		mInviteDialogs.push_back(dialog);
		mInviteMutex.unlock();
		break;
	}
}

int SipDialogs::RemoveSipDialogByDialogID(int dialogID, DialogType dialogType)
{
	switch (dialogType)
	{
	case REGISTER_DIALOG:
		mRegisterMutex.lock();
		RemoveDialogByDialogID(mRegisterDialogs, dialogID);
		mRegisterMutex.unlock();
		break;
	case SUBSCRIBE_DIALOG:
		mSubcribeMutex.lock();
		RemoveDialogByDialogID(mSubcribeDialogs, dialogID);
		mSubcribeMutex.unlock();
		break;
	default:
		mInviteMutex.lock();
		RemoveDialogByDialogID(mInviteDialogs, dialogID);
		mInviteMutex.unlock();
		break;
	}
	return 0;
}

int SipDialogs::RemoveDialogIdFromRegisterDialog(int dialogID, osip_dialog_t *& registerDialog)
{
	DialogInfo* dialogInfo = (DialogInfo*)registerDialog->your_instance;
	IntList::iterator iter;
	for (iter = dialogInfo->inviteDialogIDs.begin(); iter != dialogInfo->inviteDialogIDs.end(); iter++)
	{
		if (dialogID == (*iter))
		{
			dialogInfo->inviteDialogIDs.erase(iter);
			break;
		}
	}
	return 0;
}

osip_dialog_t * SipDialogs::FindRegisterDialog(std::string remoteDeviceID, DialogHint hint)
{
	return nullptr;
}

osip_dialog_t * SipDialogs::FindSipDialogByDialogID(int dialogID, DialogType dialogType)
{
	return nullptr;
}

osip_dialog_t * SipDialogs::FindDialogByDialogID(SipDialogList dialogList, int dialogID)
{
	osip_dialog_t* dialog = nullptr;
	DialogInfo* dialogInfo = nullptr;

	for (SipDialogList::iterator iter = dialogList.begin(); iter != dialogList.end(); iter++)
	{
		dialogInfo = (DialogInfo*)(*iter)->your_instance;

		if (dialogInfo->dialogID == dialogID)
		{
			dialog = *iter;
		}
	}

	return dialog;
}

int SipDialogs::RemoveDialogByDialogID(SipDialogList & dialogList, int dialogID)
{
	for (SipDialogList::iterator iter = dialogList.begin(); iter != dialogList.end(); iter++)
	{
		DialogInfo* dialogInfo = (DialogInfo*)(*iter)->your_instance;
		if (dialogID == dialogInfo->dialogID)
		{
			dialogList.erase(iter);
			break;
		}
	}

	return 0;
}
