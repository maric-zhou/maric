#include "EchoRole.h"
#include "CmdMsg.h"
#include "CmdCheck.h"

using namespace std;

EchoRole::EchoRole()
{
}


EchoRole::~EchoRole()
{
}

bool EchoRole::Init()
{
	return true;
}

UserData * EchoRole::ProcMsg(UserData & _poUserData)
{
	/*д��ȥ*/
	GET_REF2DATA(CmdMsg, input, _poUserData);
	CmdMsg *pout = new CmdMsg(input);
	ZinxKernel::Zinx_SendOut(*pout, *(CmdCheck::GetInstance()));
	return nullptr;
}

void EchoRole::Fini()
{
}

bool OutputCtrl::Init()
{
	Irole *pRetRole = NULL;
	/*������һ����������dateǰ׺����*/
	for (auto pRole : ZinxKernel::Zinx_GetAllRole())
	{
		auto pDate = dynamic_cast<DatePreRole *>(pRole);
		if (NULL != pDate)
		{
			pRetRole = pDate;
			break;
		}
	}
	if (NULL != pRetRole)
	{
		SetNextProcessor(*pRetRole);
	}
	return true;
}

UserData * OutputCtrl::ProcMsg(UserData & _poUserData)
{
	/*��ȡ�û����õĿ��ر�־*/
	GET_REF2DATA(CmdMsg, input, _poUserData);
	if (true == input.isOpen)
	{
		if (NULL != pOut)
		{
			//�����
			ZinxKernel::Zinx_Add_Channel(*(pOut));
			pOut = NULL;
		}
	}
	else
	{
		//�����
		auto pchannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		ZinxKernel::Zinx_Del_Channel(*(pchannel));
		pOut = pchannel;

	}

	return new CmdMsg(input);
}

void OutputCtrl::Fini()
{
}

bool DatePreRole::Init()
{
	Irole *pRetRole = NULL;
	/*������һ����������echo����*/
	for (auto pRole : ZinxKernel::Zinx_GetAllRole())
	{
		auto pEcho = dynamic_cast<EchoRole *>(pRole);
		if (NULL != pEcho)
		{
			pRetRole = pEcho;
			break;
		}
	}
	if (NULL != pRetRole)
	{
		SetNextProcessor(*pRetRole);
	}
	
	return true;
}

UserData * DatePreRole::ProcMsg(UserData & _poUserData)
{
	/*����û�����������-->�ı��־λ*/
	CmdMsg *pret = NULL;
	GET_REF2DATA(CmdMsg, input, _poUserData);
	if (input.isCmd)
	{
		needAdd = input.needDatePre;
	}
	else
	/*�������Ҫ���Ե�����---������ַ������������Թ��ܶ���*/
	{
		if (true == needAdd)
		{
			time_t tmp;
			time(&tmp);
			string szNew = string(ctime(&tmp)) + input.szUserData;
			pret = new CmdMsg(input);
			pret->szUserData = szNew;
		}
		else
		{
			pret = new CmdMsg(input);
		}
	}
	
	return pret;
}

void DatePreRole::Fini()
{
}
