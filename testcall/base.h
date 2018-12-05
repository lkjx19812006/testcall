#pragma once

class base
{
public:
	base();
	base(DWORD pid);//构造函数
	~base();
	DWORD m_pid;//进程id
	HWND m_hwnd;//进程的窗口句柄
	HANDLE m_handle;//可操作的进程句柄

	LPVOID m_funcAddr;//函数的内存地址
	LPVOID m_paramAddr;//参数的内存地址
	DWORD m_funcSize = 4096;
	DWORD m_paramSize = 4096;


	HANDLE m_hRemoteThread;//动态线程id


	bool enableDebugPriv();

	// 自定义申请内存地址
	LPVOID myVirtualAllocEx(
		HANDLE hProcess,
		LPVOID lpAddress,
		SIZE_T dwSize,
		DWORD flAllocationType,
		DWORD flProtect
	);


	//读内存整数型
	int readmemoryInt(DWORD dwAddr);
	//读内存浮点数型
	float readmemoryFloat(DWORD dwAddr);
	//读内存文本型
	char* readmemoryChar(DWORD dwAddr);

	//写内存整数型
	void writememoryInt(DWORD dwAddr, int num);

	//写内存浮点数型
	void writememoryFloat(DWORD dwAddr, float flnum);

	// mFunc:要注入的函数地址
	// Param：要注入的参数地址
	// ParamSize：参数大小
	// ParamNum: 参数个数
	void InjectCall(LPVOID mFunc);//不带参数
	void InjectCall(LPVOID mFunc, LPVOID Param, DWORD ParamSize);//带参数


	//------------------窗口相关-------------------------------
	// parentHwnd 父级窗口
	// lpClassName 要寻找的窗口类名
	// lpWindowName 要寻找窗口的标题名
	HWND findSubWindow(HWND parentHwnd);

};

