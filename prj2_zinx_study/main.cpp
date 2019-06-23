#include <zinx.h>
#include <iostream>

using namespace std;
/*����׼���룬���Ե���׼���*/

/*������׼���ͨ����*/
class TestStdout :public Ichannel {
	// ͨ�� Ichannel �̳�
	virtual bool Init() override
	{
		return true;
	}
	virtual bool ReadFd(std::string & _input) override
	{
		return false;
	}
	virtual bool WriteFd(std::string & _output) override
	{
		cout << _output << endl;
		return true;
	}
	virtual void Fini() override
	{
	}
	virtual int GetFd() override
	{
		return 1;
	}
	virtual std::string GetChannelInfo() override
	{
		return "stdout";
	}
	virtual AZinxHandler * GetInputNextStage(BytesMsg & _oInput) override
	{
		return nullptr;
	}
} *poOut = new TestStdout();


/*2-д���ܴ�����*/
class Echo :public AZinxHandler {

	virtual IZinxMsg * InternelHandle(IZinxMsg & _oInput) override
	{
		/*���Ե���׼���*/
		GET_REF2DATA(BytesMsg, input, _oInput);
		Ichannel *poSendOut = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		if (NULL != poSendOut)
		{
			ZinxKernel::Zinx_SendOut(input.szData, *poSendOut);
		}

		return nullptr;
	}
	virtual AZinxHandler * GetNextHandler(IZinxMsg & _oNextMsg) override
	{
		return nullptr;
	}
} *poEcho = new Echo();

class ExitFramework :public AZinxHandler {
	// ͨ�� AZinxHandler �̳�
	virtual IZinxMsg * InternelHandle(IZinxMsg & _oInput) override
	{
		GET_REF2DATA(BytesMsg, obyte, _oInput);

		if (obyte.szData == "exit")
		{
			ZinxKernel::Zinx_Exit();
			return NULL;
		}
		/*����������һ�����ڴ��������*/
		return new BytesMsg(obyte);
	}
	virtual AZinxHandler * GetNextHandler(IZinxMsg & _oNextMsg) override
	{
		return poEcho;
	}
} *poExit = new ExitFramework();

class AddDate :public AZinxHandler {
	// ͨ�� AZinxHandler �̳�
	virtual IZinxMsg * InternelHandle(IZinxMsg & _oInput) override
	{
		GET_REF2DATA(BytesMsg, oBytes, _oInput);
		time_t tmp;
		time(&tmp);
		string szNew = string(ctime(&tmp)) + oBytes.szData;
		BytesMsg *pret = new BytesMsg(oBytes);
		pret->szData = szNew;
		return pret;
	}
	virtual AZinxHandler * GetNextHandler(IZinxMsg & _oNextMsg) override
	{
		return poEcho;
	}
} *poAddDate = new AddDate();
class CmdHandler :public AZinxHandler {
	int status = 0;
	// ͨ�� AZinxHandler �̳�
	virtual IZinxMsg * InternelHandle(IZinxMsg & _oInput) override
	{
		/*�ж������Ƿ���open��close��ִ�в�ͬ����*/
		GET_REF2DATA(BytesMsg, oBytes,_oInput);

		if (oBytes.szData == "open")
		{
			ZinxKernel::Zinx_Add_Channel(*poOut);
			return NULL;
		}
		else if (oBytes.szData == "close")
		{
			ZinxKernel::Zinx_Del_Channel(*poOut);
			return nullptr;
		}
		else if (oBytes.szData == "date")
		{
			status = 1;
			return nullptr;
		}
		else if (oBytes.szData == "cleardate")
		{
			status = 0;
		}

		return new BytesMsg(oBytes);
	}

	/*������Ϣ��ͬ��ѡ��ͬ�Ĵ�����*/
	virtual AZinxHandler * GetNextHandler(IZinxMsg & _oNextMsg) override
	{
		GET_REF2DATA(BytesMsg, oBytes, _oNextMsg);
		if (oBytes.szData == "exit")
		{
			return poExit;
		}
		else
		{
			if (0 == status)
			{
				return poEcho;
			}
			else
			{
				return poAddDate;
			}
		}
	}
} *poCmd = new CmdHandler();



/*3-дͨ����*/
class TestStdin :public Ichannel {
	// ͨ�� Ichannel �̳�
	virtual bool Init() override
	{
		return true;
	}
	virtual bool ReadFd(std::string & _input) override
	{
		cin >> _input;
		return true;
	}
	virtual bool WriteFd(std::string & _output) override
	{
		return false;
	}
	virtual void Fini() override
	{
	}
	virtual int GetFd() override
	{
		return 0;
	}
	virtual std::string GetChannelInfo() override
	{
		return "stdin";
	}
	virtual AZinxHandler * GetInputNextStage(BytesMsg & _oInput) override
	{
		return poCmd;
	}
};

int main()
{
	/*1-��ʼ�����*/
	ZinxKernel::ZinxKernelInit();

	/*4-��ͨ��������ӵ����*/
	TestStdin *poStdin = new TestStdin();
	ZinxKernel::Zinx_Add_Channel(*poStdin);
	ZinxKernel::Zinx_Add_Channel(*poOut);

	/*5-���п��*/
	ZinxKernel::Zinx_Run();

	ZinxKernel::ZinxKernelFini();
	return 0;
}