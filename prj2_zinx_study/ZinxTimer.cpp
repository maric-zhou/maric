#include "ZinxTimer.h"
#include <sys/timerfd.h>


ZinxTimerChannel::ZinxTimerChannel()
{
}


ZinxTimerChannel::~ZinxTimerChannel()
{
}

/*创建定时器文件描述符*/
bool ZinxTimerChannel::Init()
{
	bool bRet = false;
	/*创建文件描述符*/
	int iFd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (0 <= iFd)
	{
		/*设置定时周期*/
		struct itimerspec period = {
			{1,0}, {1,0}
		};
		if (0 == timerfd_settime(iFd, 0, &period, NULL))
		{
			bRet = true;
			m_TimerFd = iFd;
		}
	}

	return bRet;
}

/*读取超时次数*/
bool ZinxTimerChannel::ReadFd(std::string & _input)
{
	bool bRet = false;
	char buff[8] = { 0 };

	if (sizeof(buff) == read(m_TimerFd, buff, sizeof(buff)))
	{
		bRet = true;
		_input.assign(buff, sizeof(buff));
	}
	return bRet;
}

bool ZinxTimerChannel::WriteFd(std::string & _output)
{
	return false;
}
/*关闭文件描述符*/
void ZinxTimerChannel::Fini()
{
	close(m_TimerFd);
	m_TimerFd = -1;
}
/*返回当前的定时器文件描述符*/
int ZinxTimerChannel::GetFd()
{
	return m_TimerFd;
}

std::string ZinxTimerChannel::GetChannelInfo()
{
	return "TimerFd";
}

class output_hello :public AZinxHandler {
	// 通过 AZinxHandler 继承
	virtual IZinxMsg * InternelHandle(IZinxMsg & _oInput) override
	{
		auto pchannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		std::string output = "hello world";
		ZinxKernel::Zinx_SendOut(output, *pchannel);
		return nullptr;
	}
	virtual AZinxHandler * GetNextHandler(IZinxMsg & _oNextMsg) override
	{
		return nullptr;
	}
} *pout_hello = new output_hello();

/*返回处理超时事件的对象*/
AZinxHandler * ZinxTimerChannel::GetInputNextStage(BytesMsg & _oInput)
{
	return &TimerOutMng::GetInstance();
}
TimerOutMng TimerOutMng::single;
IZinxMsg * TimerOutMng::InternelHandle(IZinxMsg & _oInput)
{
	/*遍历任务列表，每个任务的计数减一*/
	for (auto task : m_task_list)
	{
		task->iCount--;
		/*若计数为0则调用超时处理函数,回复计数*/
		if (task->iCount <= 0)
		{
			task->Proc();
			task->iCount = task->GetTimeSec();
		}
	}
	return nullptr;
}

AZinxHandler * TimerOutMng::GetNextHandler(IZinxMsg & _oNextMsg)
{
	return nullptr;
}

void TimerOutMng::AddTask(TimerOutProc * _ptask)
{
	m_task_list.push_back(_ptask);
	_ptask->iCount = _ptask->GetTimeSec();
}

void TimerOutMng::DelTask(TimerOutProc * _ptask)
{
	m_task_list.remove(_ptask);
}
