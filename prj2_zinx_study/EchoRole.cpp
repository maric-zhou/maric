#include "EchoRole.h"
#include "CmdMsg.h"
#include "CmdCheck.h"

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
	/*写出去*/
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
	return true;
}

UserData * OutputCtrl::ProcMsg(UserData & _poUserData)
{
	/*读取用户设置的开关标志*/
	GET_REF2DATA(CmdMsg, input, _poUserData);
	if (true == input.isOpen)
	{
		//开输出
		ZinxKernel::Zinx_Add_Channel(*(pOut));
	}
	else
	{
		//关输出
		auto pchannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		ZinxKernel::Zinx_Del_Channel(*(pchannel));
		pOut = pchannel;
	}

	return nullptr;
}

void OutputCtrl::Fini()
{
}
