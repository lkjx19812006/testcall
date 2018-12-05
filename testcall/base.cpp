#include "stdafx.h"
#include "base.h"



base::base()
{

}

base::base(DWORD pid)
{
	m_pid = pid;
	if (!m_pid) {
		AfxMessageBox(L"base初始化失败 请传入进程ID");
		return;
	}
	//提升进程权限
	enableDebugPriv();
	//1 打开进程 获取进程句柄
	m_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_pid);
	if (!m_handle) {
		AfxMessageBox(L"base进程打开失败");
		return;
	}
	// 2 申请一段内存地址以便 反复调用 默认大小为4096个字节 后期如果需要更大的内存地址 有个自定义申请内存
	m_funcAddr = VirtualAllocEx(m_handle, NULL, m_funcSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	m_paramAddr = VirtualAllocEx(m_handle, NULL, m_paramSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	//下面可以进行一些初始化操作

}

base::~base()
{
	//释放申请有内存 
	VirtualFreeEx(m_handle, m_funcAddr, m_funcSize, MEM_RELEASE);
	VirtualFreeEx(m_handle, m_paramAddr, m_paramSize, MEM_RELEASE);

	//关闭当前的进程句柄
	CloseHandle(m_handle);
	CloseHandle(m_hRemoteThread);
}

// 提升进程权限
bool base::enableDebugPriv()
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

// 读内存整数型
int base::readmemoryInt(DWORD dwAddr)
{
	int result;
	
	//读取高度
	if (!ReadProcessMemory(m_handle, (LPCVOID)dwAddr, (LPVOID)&result, 4, NULL)) {
		return -1;
	}
	return result;
}

// 读内浮点数型
float base::readmemoryFloat(DWORD dwAddr)
{
	float result;
	//读取高度
	if (!ReadProcessMemory(m_handle, (LPCVOID)dwAddr, (LPVOID)&result, 4, NULL)) {
		return -1.0;
	}
	return result;
}

// 读内存字符型
char* base::readmemoryChar(DWORD dwAddr)
{
	char result[256];//最长256
	ReadProcessMemory(m_handle, (LPCVOID)m_paramAddr, (LPVOID)result, 256, NULL);
	return result;
}

//写内存整数型
void base::writememoryInt(DWORD dwAddr, int num)
{
	WriteProcessMemory(m_handle, (LPVOID)dwAddr, &num, 4, NULL);
}

//写内存浮点型
void base::writememoryFloat(DWORD dwAddr, float flnum)
{
	WriteProcessMemory(m_handle, (LPVOID)dwAddr, &flnum, 4, NULL);
}

// 注入函数 不带参数
void base::InjectCall(LPVOID mFunc)
{
	//1.将函数写入到内存地址
	WriteProcessMemory(m_handle, m_funcAddr, mFunc, m_funcSize, NULL);
	//2.创建动态线程
	m_hRemoteThread = CreateRemoteThread(m_handle, NULL, 0, (LPTHREAD_START_ROUTINE)m_funcAddr, NULL, 0, NULL);
	//3.等待线程结束
	WaitForSingleObject(m_hRemoteThread, INFINITE); //等待线程结束 
	//4.关闭线程
	CloseHandle(m_hRemoteThread);
}

// 注入函数 带参数
void base::InjectCall(LPVOID mFunc, LPVOID Param, DWORD ParamSize)
{
	//1.将函数写入到内存地址
	WriteProcessMemory(m_handle, m_funcAddr, mFunc, m_funcSize, NULL);
	WriteProcessMemory(m_handle, m_paramAddr, Param, ParamSize, NULL);

	//2.创建动态线程
	m_hRemoteThread = CreateRemoteThread(m_handle, NULL, 0, (LPTHREAD_START_ROUTINE)m_funcAddr, m_paramAddr, 0, NULL);
	//3.等待线程结束
	WaitForSingleObject(m_hRemoteThread, INFINITE); //等待线程结束 
	//4.关闭线程
	CloseHandle(m_hRemoteThread);
}



// -------------------窗口操作相关---------------------------

// 获取子窗口的句柄 递归查找
// hwndChild 子窗口句柄
// lParam 参数
BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam)
{
	WCHAR windowName[128];//定义一个宽字符接收名字
	memset(windowName, 0, sizeof(windowName));// 设置内存地址为0
	::GetWindowText(hwndChild, windowName, 128);
	std::cout << windowName << std::endl;
	if (wcscmp(windowName, L""))
	{
		
	}

	return TRUE;
}

HWND base::findSubWindow(HWND parentHwnd)
{
	//1.枚举当前父级下所有的子窗口
    ::EnumChildWindows(parentHwnd, EnumChildProc, 0);
	HWND hwnd = NULL;
	return hwnd;
}