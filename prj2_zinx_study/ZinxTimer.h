#pragma once
#include <zinx.h>

class ZinxTimerChannel :
	public Ichannel
{
	int m_TimerFd = -1;
public:
	ZinxTimerChannel();
	virtual ~ZinxTimerChannel();

	// Í¨¹ý Ichannel ¼Ì³Ð
	virtual bool Init() override;

	virtual bool ReadFd(std::string & _input) override;
	virtual bool WriteFd(std::string & _output) override;
	virtual void Fini() override;
	virtual int GetFd() override;
	virtual std::string GetChannelInfo() override;
	virtual AZinxHandler * GetInputNextStage(BytesMsg & _oInput) override;
};

