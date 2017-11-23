#include "SipXmlDialogs.h"

int SipXmlDialogs::mSipXmlDialogID = 0;
SipXmlDialogs* SipXmlDialogs::mInstance = nullptr;

SipXmlDialogs::SipXmlDialogs()
{
}


SipXmlDialogs* SipXmlDialogs::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new SipXmlDialogs();
	}
	return mInstance;
}

SipXmlDialogs::~SipXmlDialogs()
{
}


void SipXmlDialogs::InsertSipXmlDialog(XmlDialogParam* sipXmlDialog)
{
	mDialogDequeMutex.lock();
	sipXmlDialog->xmlDialogID = mSipXmlDialogID;
	mSipXmlDialogDeque.push_back(sipXmlDialog);
	mDialogDequeMutex.unlock();
}

void SipXmlDialogs::RemoveSipXmlDialog(XmlDialogParam* sipXmlDialog)
{

}

XmlDialogParam* SipXmlDialogs::CreateSipXmlDialog()
{
	mDialogIdMutex.lock();
	mSipXmlDialogID++;
	mDialogIdMutex.unlock();

	XmlDialogParam* xmlDialog = new XmlDialogParam;
	xmlDialog->xmlDialogID = mSipXmlDialogID;
}

int SipXmlDialogs::DestroySipXmlDialog(XmlDialogParam* &sipXmlDialog)
{
	return 0;
}

XmlDialogParam* SipXmlDialogs::FindSipXmlDialog(int manscdpType, std::string dstDevice)
{
	XmlDialogParam* dialog = nullptr;

	return dialog;
}