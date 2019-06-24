#include "CmdCheck.h"
#include "CmdMsg.h"

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
	/*赋值原始数据字符串到用户数据中字符串字段*/
	auto pret = new CmdMsg();
	pret->szUserData = _szInput;
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
	
	return rolelist.front();
}

Ichannel * CmdCheck::GetMsgSender(BytesMsg & _oBytes)
{
	/*指定数据要通过标准输出输出*/
	return ZinxKernel::Zinx_GetChannel_ByInfo("stdout");;
}
