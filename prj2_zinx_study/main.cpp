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

		ZinxKernel::Zinx_SendOut(input.szData, *poOut);

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
		return poExit;
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