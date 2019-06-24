#pragma once
#include <zinx.h>
class EchoRole :
	public Irole
{
public:
	EchoRole();
	virtual ~EchoRole();

	// ͨ�� Irole �̳�
	virtual bool Init() override;
	virtual UserData * ProcMsg(UserData & _poUserData) override;
	virtual void Fini() override;
};

