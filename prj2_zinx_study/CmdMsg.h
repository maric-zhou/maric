#pragma once
#include <zinx.h>
#include <string>

class CmdMsg :
	public UserData
{
public:
	/*��Ա������ʾҪ���Ե��ַ���*/
	std::string szUserData;
	CmdMsg();
	virtual ~CmdMsg();
};

