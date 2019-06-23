#include <zinx.h>
#include <iostream>

using namespace std;
/*读标准输入，回显到标准输出*/

/*2-写功能处理类*/
class Echo :public AZinxHandler {

	virtual IZinxMsg * InternelHandle(IZinxMsg & _oInput) override
	{
		/*回显到标准输出*/
		GET_REF2DATA(BytesMsg, input, _oInput);

		cout << input.szData << endl;

		return nullptr;
	}
	virtual AZinxHandler * GetNextHandler(IZinxMsg & _oNextMsg) override
	{
		return nullptr;
	}
} *poEcho = new Echo();

/*3-写通道类*/
class TestStdin :public Ichannel {
	// 通过 Ichannel 继承
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
		return poEcho;
	}
};

int main()
{
	/*1-初始化框架*/
	ZinxKernel::ZinxKernelInit();

	/*4-将通道对象添加到框架*/
	TestStdin *poStdin = new TestStdin();
	ZinxKernel::Zinx_Add_Channel(*poStdin);

	/*5-运行框架*/
	ZinxKernel::Zinx_Run();

	ZinxKernel::ZinxKernelFini();
	return 0;
}