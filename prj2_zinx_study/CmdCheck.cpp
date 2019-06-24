#include "CmdCheck.h"
#include "CmdMsg.h"
#include "EchoRole.h"
using namespace std;

CmdCheck *CmdCheck::poSingle = new CmdCheck();

CmdCheck::CmdCheck()
{
}


CmdCheck::~CmdCheck()
{
}

UserData * CmdCheck::raw2request(std::string _szInput)
{
	if ("exit" == _szInput)
	{
		ZinxKernel::Zinx_Exit();
		return NULL;
	}

	/*��ֵԭʼ�����ַ������û��������ַ����ֶ�*/
	auto pret = new CmdMsg();
	pret->szUserData = _szInput;
	if ("open" == _szInput)
	{
		pret->isCmd = true;
		pret->isOpen = true;
	}
	if ("close" == _szInput)
	{
		pret->isCmd = true;
		pret->isOpen = false;
	}
	if ("date" == _szInput)
	{
		pret->needDatePre = true;
		pret->isCmd = true;
	}
	if ("cleardate" == _szInput)
	{
		pret->needDatePre = false;
		pret->isCmd = true;
	}
	return pret;
}

std::string * CmdCheck::response2raw(UserData & _oUserData)
{
	GET_REF2DATA(CmdMsg, output, _oUserData);
	return new string(output.szUserData);
}

Irole * CmdCheck::GetMsgProcessor(UserDataMsg & _oUserDataMsg)
{
	/*�������ͬ����������ͬ�Ĵ���role����*/
	auto rolelist = ZinxKernel::Zinx_GetAllRole();

	auto pCmdMsg = dynamic_cast<CmdMsg *>(_oUserDataMsg.poUserData);
	/*��ȡ��ǰ��Ϣ�Ƿ�������*/
	bool isCmd = pCmdMsg->isCmd;
	Irole *pRetRole = NULL;

	for (Irole *prole : rolelist)
	{
		if (isCmd)
		{
			auto pOutCtrl = dynamic_cast<OutputCtrl *>(prole);
			if (NULL != pOutCtrl)
			{
				pRetRole = pOutCtrl;
				break;
			}
		}
		else
		{
			auto pDate = dynamic_cast<DatePreRole *>(prole);
			if (NULL != pDate)
			{
				pRetRole = pDate;
				break;
			}
		}
	}
	
	return pRetRole;
}

Ichannel * CmdCheck::GetMsgSender(BytesMsg & _oBytes)
{
	/*ָ������Ҫͨ����׼������*/
	return ZinxKernel::Zinx_GetChannel_ByInfo("stdout");;
}
