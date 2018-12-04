#include "stdafx.h"
#include "base.h"



base::base()
{

}

base::base(DWORD pid)
{
	m_pid = pid;
	if (!m_pid) {
		AfxMessageBox(L"base��ʼ��ʧ�� �봫�����ID");
		return;
	}
	//��������Ȩ��
	enableDebugPriv();
	//1 �򿪽��� ��ȡ���̾��
	m_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_pid);
	if (!m_handle) {
		AfxMessageBox(L"base���̴�ʧ��");
		return;
	}
	// 2 ����һ���ڴ��ַ�Ա� �������� Ĭ�ϴ�СΪ4096���ֽ� ���������Ҫ������ڴ��ַ �и��Զ��������ڴ�
	m_funcAddr = VirtualAllocEx(m_handle, NULL, m_funcSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	m_paramAddr = VirtualAllocEx(m_handle, NULL, m_paramSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	//������Խ���һЩ��ʼ������

}

base::~base()
{
	//�ͷ��������ڴ� 
	VirtualFreeEx(m_handle, m_funcAddr, m_funcSize, MEM_RELEASE);
	VirtualFreeEx(m_handle, m_paramAddr, m_paramSize, MEM_RELEASE);

	//�رյ�ǰ�Ľ��̾��
	CloseHandle(m_handle);
	CloseHandle(m_hRemoteThread);
}

// ��������Ȩ��
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

// ���ڴ�������
int base::readmemoryInt(DWORD dwAddr)
{
	int result;
	
	//��ȡ�߶�
	if (!ReadProcessMemory(m_handle, (LPCVOID)dwAddr, (LPVOID)&result, 4, NULL)) {
		return -1;
	}
	return result;
}

// ���ڸ�������
float base::readmemoryFloat(DWORD dwAddr)
{
	float result;
	//��ȡ�߶�
	if (!ReadProcessMemory(m_handle, (LPCVOID)dwAddr, (LPVOID)&result, 4, NULL)) {
		return -1.0;
	}
	return result;
}

// ע�뺯�� ��������
void base::InjectCall(LPVOID mFunc)
{
	//1.������д�뵽�ڴ��ַ
	WriteProcessMemory(m_handle, m_funcAddr, mFunc, m_funcSize, NULL);
	//2.������̬�߳�
	m_hRemoteThread = CreateRemoteThread(m_handle, NULL, 0, (LPTHREAD_START_ROUTINE)m_funcAddr, NULL, 0, NULL);
	//3.�ȴ��߳̽���
	WaitForSingleObject(m_hRemoteThread, INFINITE); //�ȴ��߳̽��� 
	//4.�ر��߳�
	CloseHandle(m_hRemoteThread);
}

// ע�뺯�� ������
void base::InjectCall(LPVOID mFunc, LPVOID Param, DWORD ParamSize, DWORD ParamNum)
{
	//1.������д�뵽�ڴ��ַ
	WriteProcessMemory(m_handle, m_funcAddr, mFunc, m_funcSize, NULL);
	WriteProcessMemory(m_handle, m_paramAddr, Param, ParamSize, NULL);

	//2.������̬�߳�
	m_hRemoteThread = CreateRemoteThread(m_handle, NULL, 0, (LPTHREAD_START_ROUTINE)m_funcAddr, m_paramAddr, 0, NULL);
	//3.�ȴ��߳̽���
	WaitForSingleObject(m_hRemoteThread, INFINITE); //�ȴ��߳̽��� 
	//4.�ر��߳�
	CloseHandle(m_hRemoteThread);
}