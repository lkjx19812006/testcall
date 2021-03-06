
// testcallDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "testcall.h"
#include "testcallDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestcallDlg 对话框



CtestcallDlg::CtestcallDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTCALL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestcallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestcallDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CtestcallDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CtestcallDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CtestcallDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CtestcallDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CtestcallDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CtestcallDlg 消息处理程序

BOOL CtestcallDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	// TODO: 在此添加控件通知处理程序代码
	/*
	HWND hwnd = ::FindWindow(L"ConsoleWindowClass", L"C:\\Users\\Administrator\\source\\repos\\测试项目\\Release\\测试项目.exe");
	if (!hwnd) {
		MessageBox(L"没找到对应的窗口");		
	}
	else
	{
		DWORD dwPID = 0;//进程id
		DWORD dwThreadID = ::GetWindowThreadProcessId(hwnd, &dwPID); //线程id

	    m_baseHandle = new base(dwPID);
	}
	*/
	// 开启控制台窗口
	AllocConsole();//控制台调试窗口开启
	freopen("CONOUT$", "w", stdout);//开启中文控制台输出支持
	//FreeConsole();// 释放控制台资源

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestcallDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestcallDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 提升进程权限
bool enableDebugPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		return false;
	}
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{
		CloseHandle(hToken);
		return false;
	}
	return true;
}




void CtestcallDlg::OnBnClickedButton1()
{
	//1 获取窗口句柄
	HWND hwnd = ::FindWindow(L"ConsoleWindowClass",L"C:\\Users\\Administrator\\source\\repos\\测试项目\\Debug\\测试项目.exe");
	if (!hwnd) {
		MessageBox(L"没找到对应的窗口");
		return;
	}
	DWORD dwPID = 0;//进程id
	DWORD dwThreadID = ::GetWindowThreadProcessId(hwnd, &dwPID); //线程id
	TRACE("窗口句柄=%d\n", hwnd);
	TRACE("进程id=%d\n", dwPID);
	TRACE("线程id=%d\n", dwThreadID);

	//2 通过窗口句柄获取进程ID	
	if (!dwPID || !dwThreadID)
	{
		MessageBox(L"进程获取失败");
	}

	//3 提升当前进程权限
	enableDebugPriv();
	
	//4 打开进程 获取进程句柄
	HANDLE targetHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (!targetHandle) {
		MessageBox(L"进程打开失败");
	}

	//5 在打开的进程中创建动态线程 执行改进程中的函数
	DWORD tempThread;
	HANDLE hRemoteThread = CreateRemoteThread(targetHandle, NULL, 0, (LPTHREAD_START_ROUTINE)0x01321398, NULL, 0, &tempThread);
	
	//6 等待远程线程执行完毕
	WaitForSingleObject(hRemoteThread, INFINITE); //等待线程结束 

	//7 关闭远程线程 打开的线程
	CloseHandle(hRemoteThread);
	CloseHandle(targetHandle);
}

// 自定义函数
void  CallAddhp()
{
	_asm {
		mov eax, 0x01321398;
		call eax;
	}
	return;
}

void CtestcallDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//1 获取窗口句柄
	HWND hwnd = ::FindWindow(L"ConsoleWindowClass", L"C:\\Users\\Administrator\\source\\repos\\测试项目\\Debug\\测试项目.exe");
	if (!hwnd) {
		MessageBox(L"没找到对应的窗口");
		return;
	}
	DWORD dwPID = 0;//进程id
	DWORD dwThreadID = ::GetWindowThreadProcessId(hwnd, &dwPID); //线程id
	TRACE("窗口句柄=%d\n", hwnd);
	TRACE("进程id=%d\n", dwPID);
	TRACE("线程id=%d\n", dwThreadID);

	//2 通过窗口句柄获取进程ID	
	if (!dwPID || !dwThreadID)
	{
		MessageBox(L"进程获取失败");
	}

	//3 提升当前进程权限
	enableDebugPriv();


	//4 打开进程 获取进程句柄
	HANDLE targetHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (!targetHandle) {
		MessageBox(L"进程打开失败");
	}

	//5 申请内存
	// 函数内存地址
	LPVOID mFuncAddr = VirtualAllocEx(targetHandle, NULL, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	
	DWORD NumberOfByte;
	WriteProcessMemory(targetHandle, mFuncAddr, CallAddhp, 4096, &NumberOfByte);

	//6 在打开的进程中创建动态线程 执行改进程中的函数
	DWORD tempThread;
	HANDLE hRemoteThread = CreateRemoteThread(targetHandle, NULL, 0, (LPTHREAD_START_ROUTINE)mFuncAddr, NULL, 0, &tempThread);

	//7 等待远程线程执行完毕
	WaitForSingleObject(hRemoteThread, INFINITE); //等待线程结束 

	//8 释放内存
	VirtualFreeEx(targetHandle, mFuncAddr, 128, MEM_RELEASE);

	//9 关闭远程线程 打开的线程
	CloseHandle(hRemoteThread);
	CloseHandle(targetHandle);

}



// 远程call 有参数
void CtestcallDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	//1 获取窗口句柄
	HWND hwnd = ::FindWindow(L"ConsoleWindowClass", L"C:\\Users\\Administrator\\source\\repos\\测试项目\\Release\\测试项目.exe");
	if (!hwnd) {
		MessageBox(L"没找到对应的窗口");
		return;
	}
	DWORD dwPID = 0;//进程id
	DWORD dwThreadID = ::GetWindowThreadProcessId(hwnd, &dwPID); //线程id
	TRACE("窗口句柄=%d\n", hwnd);
	TRACE("进程id=%d\n", dwPID);
	TRACE("线程id=%d\n", dwThreadID);

	//2 通过窗口句柄获取进程ID	
	if (!dwPID || !dwThreadID)
	{
		MessageBox(L"进程获取失败");
	}

	//3 提升当前进程权限
	enableDebugPriv();

	//4 打开进程 获取进程句柄
	HANDLE targetHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (!targetHandle) {
		MessageBox(L"进程打开失败");
	}

	// 参数地址
	LPVOID mParamAddr = VirtualAllocEx(targetHandle, NULL, 4, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	int a = 4;
	WriteProcessMemory(targetHandle, mParamAddr, &a, 4, NULL);

	//5 在打开的进程中创建动态线程 执行改进程中的函数
	DWORD tempThread;
	HANDLE hRemoteThread = CreateRemoteThread(targetHandle, NULL, 0, (LPTHREAD_START_ROUTINE)0x010A1050, mParamAddr, 0, &tempThread);

	//6 等待远程线程执行完毕
	WaitForSingleObject(hRemoteThread, INFINITE); //等待线程结束 

	//7 关闭远程线程 打开的线程
	CloseHandle(hRemoteThread);
	CloseHandle(targetHandle);



}



// 多个参数call
struct params
{
	int a;
	int b;
};










//************************************************************************************** 
//函数名：InjectCallByPid 
//功能  ：注入call 
//参数 1：进程ID  
//参数 2：被注入函数指针<函数名>  
//参数 3：参数  
//参数 4：参数长度  
//**************************************************************************************
void InjectCallByPid(DWORD pid, LPVOID mFunc, LPVOID Param, DWORD ParamSize) {
	//1 提升当前进程权限
	enableDebugPriv();

	//2 打开进程 获取进程句柄
	HANDLE targetHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!targetHandle) {
		AfxMessageBox(L"进程打开失败");
		return;
	}

	// 函数地址
	LPVOID mfunAddr = VirtualAllocEx(targetHandle, NULL, 128, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(targetHandle, mfunAddr, mFunc, 128, NULL);

	HANDLE hRemoteThread;
	LPVOID mParamAddr = NULL;
	// 参数地址
	if (Param) {
		mParamAddr = VirtualAllocEx(targetHandle, NULL, ParamSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		WriteProcessMemory(targetHandle, mParamAddr, Param, ParamSize, NULL);
		//3 在打开的进程中创建动态线程 执行改进程中的函数
		hRemoteThread = CreateRemoteThread(targetHandle, NULL, 0, (LPTHREAD_START_ROUTINE)mfunAddr, mParamAddr, 0, NULL);
	}
	else
	{
		//3 在打开的进程中创建动态线程 执行改进程中的函数
		hRemoteThread = CreateRemoteThread(targetHandle, NULL, 0, (LPTHREAD_START_ROUTINE)mfunAddr, NULL, 0, NULL);
	}
		

	//6 等待远程线程执行完毕
	WaitForSingleObject(hRemoteThread, INFINITE); //等待线程结束 

	
	int a;
	//读取高度
	ReadProcessMemory(hRemoteThread, mParamAddr, &a, ParamSize, NULL);

	//释放申请有内存 
	VirtualFreeEx(targetHandle, mfunAddr, 128, MEM_RELEASE);

	if (Param) {
	  VirtualFreeEx(targetHandle, mParamAddr, ParamSize, MEM_RELEASE);
	}
	
	//7 关闭远程线程 打开的线程
	CloseHandle(hRemoteThread);
	CloseHandle(targetHandle);
}

// 有参call 调用
void CtestcallDlg::OnBnClickedButton4()
{
	
}





// 无参call调用
void  CallAddhp1()
{
	DWORD dwAddr = 0x01231050;
	_asm {
		call dwAddr;
	}
}

// 有参call调用
void CallAddhp2(LPVOID lparam)
{
	int a = *(int *)lparam;
	DWORD dwAddr = 0x01231060;
	_asm
	{
		push a;
		call dwAddr;
	}
}



// 多参call调用
void CallAddhp3(LPVOID lparam)
{
	struct params* ptr = (struct params *)lparam;
	int a = ptr->a;
	int b = ptr->b;
	DWORD dwAddr = 0x01231070;
	_asm
	{
		push a;
		push b;
		call dwAddr;
	}
}

// 带返回值的call调用
void getNum(LPVOID lres)
{
	int * const p = (int *)lres;
	DWORD dwAddr = 0x01231080;
	int a;
	_asm {
		call dwAddr;
		mov a, eax;
	}
	*p = a;
}

// 测试获取字符串类型的返回值
void getchar1(LPVOID lres)
{
	DWORD dwAddr = 0x01231090;
	LPCVOID strs = NULL;
	_asm {
		call dwAddr;
		mov strs, eax;
	}
	strcpy((char *)lres, (const char*)strs);
}


void CtestcallDlg::OnBnClickedButton5()
{

	// 读内存测试
	//DWORD dwAddr =  0X01235018;
	//int a = 0;
	//a = m_baseHandle->readmemoryInt(dwAddr);
	//std::cout << a << std::endl;

	// 写内存测试
	//Sleep(1000);
	//m_baseHandle->writememoryInt(0X01235018, 150);
	//std::cout << m_baseHandle->readmemoryInt(0X01235018) << std::endl;

	//无参call测试
	//m_baseHandle->InjectCall(CallAddhp1);
	//Sleep(1000);

	//有参数call测试
	//int num = 100;
	//m_baseHandle->InjectCall(CallAddhp2, &num, 4);
	//Sleep(1000);


	//多个参数call测试
	//struct params pams = { 999, 888 };
	//m_baseHandle->InjectCall(CallAddhp3, &pams, sizeof(pams));

	//参数call 返回一个整数型
	//m_baseHandle->InjectCall(getNum, (LPVOID)m_baseHandle->m_paramAddr, sizeof(int)); //调用call后
	//std::cout << m_baseHandle->readmemoryInt((DWORD)m_baseHandle->m_paramAddr) << std::endl; //读取参数地址的值

	//Sleep(1000);

	//参数call 返回一个字符串
	//m_baseHandle->InjectCall(getchar1,(LPVOID)m_baseHandle->m_paramAddr, 128); //调用call后
	//std::cout << m_baseHandle->readmemoryChar((DWORD)m_baseHandle->m_paramAddr) << std::endl; //读取参数地址的值

	HWND hwnd = ::FindWindow(L"CabinetWClass", 0);
	if (!hwnd) {
		MessageBox(L"没找到对应的窗口");
		return;
	}
	else
	{
		DWORD dwPID = 0;//进程id
		DWORD dwThreadID = ::GetWindowThreadProcessId(hwnd, &dwPID); //线程id
		m_baseHandle = new base(dwPID);
		HWND subHwnd = m_baseHandle->findSubWindow(hwnd, L"ShellTabWindowClass", L"C:\\Users\\Administrator\\Desktop\\小程序 封面_slices");
		std::cout << "测试查找窗口=" << subHwnd << std::endl;
	}
	
}
