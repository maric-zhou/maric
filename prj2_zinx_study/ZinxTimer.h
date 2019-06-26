#pragma once
#include <zinx.h>

class ZinxTimerChannel :
	public Ichannel
{
	int m_TimerFd = -1;
public:
	ZinxTimerChannel();
	virtual ~ZinxTimerChannel();

	// ͨ�� Ichannel �̳�
	virtual bool Init() override;

	virtual bool ReadFd(std::string & _input) override;
	virtual bool WriteFd(std::string & _output) override;
	virtual void Fini() override;
	virtual int GetFd() override;
	virtual std::string GetChannelInfo() override;
	virtual AZinxHandler * GetInputNextStage(BytesMsg & _oInput) override;
};

class TimerOutProc {
public:
	virtual void Proc() = 0;
	virtual int GetTimeSec() = 0;
	int iCount = -1;
};

class TimerOutMng :public AZinxHandler {
	std::list<TimerOutProc *> m_task_list;
	static TimerOutMng single;
public:
	/*����ʱ�¼�,�������г�ʱ����*/
	virtual IZinxMsg * InternelHandle(IZinxMsg & _oInput) override;

	virtual AZinxHandler * GetNextHandler(IZinxMsg & _oNextMsg) override;
	void AddTask(TimerOutProc *_ptask);
	void DelTask(TimerOutProc *_ptask);
	static TimerOutMng &GetInstance() {
		return single;
	}

};

