#include "ZinxTimer.h"
#include <sys/timerfd.h>


ZinxTimerChannel::ZinxTimerChannel()
{
}


ZinxTimerChannel::~ZinxTimerChannel()
{
}

/*������ʱ���ļ�������*/
bool ZinxTimerChannel::Init()
{
	bool bRet = false;
	/*�����ļ�������*/
	int iFd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (0 <= iFd)
	{
		/*���ö�ʱ����*/
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

/*��ȡ��ʱ����*/
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
/*�ر��ļ�������*/
void ZinxTimerChannel::Fini()
{
	close(m_TimerFd);
	m_TimerFd = -1;
}
/*���ص�ǰ�Ķ�ʱ���ļ�������*/
int ZinxTimerChannel::GetFd()
{
	return m_TimerFd;
}

std::string ZinxTimerChannel::GetChannelInfo()
{
	return "TimerFd";
}

class output_hello :public AZinxHandler {
	// ͨ�� AZinxHandler �̳�
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

/*���ش���ʱ�¼��Ķ���*/
AZinxHandler * ZinxTimerChannel::GetInputNextStage(BytesMsg & _oInput)
{
	return &TimerOutMng::GetInstance();
}
TimerOutMng TimerOutMng::single;
IZinxMsg * TimerOutMng::InternelHandle(IZinxMsg & _oInput)
{
	/*���������б�ÿ������ļ�����һ*/
	for (auto task : m_task_list)
	{
		task->iCount--;
		/*������Ϊ0����ó�ʱ������,�ظ�����*/
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
