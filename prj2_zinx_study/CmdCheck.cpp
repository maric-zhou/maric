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

	/*赋值原始数据字符串到用户数据中字符串字段*/
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
	return pret;
}

std::string * CmdCheck::response2raw(UserData & _oUserData)
{
	GET_REF2DATA(CmdMsg, output, _oUserData);
	return new string(output.szUserData);
}

Irole * CmdCheck::GetMsgProcessor(UserDataMsg & _oUserDataMsg)
{
	/*根据命令不同，交个给不同的处理role对象*/
	auto rolelist = ZinxKernel::Zinx_GetAllRole();

	auto pCmdMsg = dynamic_cast<CmdMsg *>(_oUserDataMsg.poUserData);
	/*读取当前消息是否是命令*/
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
			auto pEcho = dynamic_cast<EchoRole *>(prole);
			if (NULL != pEcho)
			{
				pRetRole = pEcho;
				break;
			}
		}
	}
	
	return pRetRole;
}

Ichannel * CmdCheck::GetMsgSender(BytesMsg & _oBytes)
{
	/*指定数据要通过标准输出输出*/
	return ZinxKernel::Zinx_GetChannel_ByInfo("stdout");;
}
