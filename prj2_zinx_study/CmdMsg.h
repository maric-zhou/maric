#pragma once
#include <zinx.h>
#include <string>

class CmdMsg :
	public UserData
{
public:
	/*成员变量表示要回显的字符串*/
	std::string szUserData;
	CmdMsg();
	virtual ~CmdMsg();
};

