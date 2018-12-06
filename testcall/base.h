#include <vector>
#pragma once
class base
{
public:
	base();
	base(DWORD pid);//���캯��
	~base();

	DWORD m_pid;//����id
	HWND m_hwnd;//���̵Ĵ��ھ��
	HANDLE m_handle;//�ɲ����Ľ��̾��

	LPVOID m_funcAddr;//�������ڴ��ַ
	LPVOID m_paramAddr;//�������ڴ��ַ
	DWORD m_funcSize = 4096;
	DWORD m_paramSize = 4096;
	HANDLE m_hRemoteThread;//��̬�߳�id

	bool enableDebugPriv();

	// �Զ��������ڴ��ַ
	LPVOID myVirtualAllocEx(
		HANDLE hProcess,
		LPVOID lpAddress,
		SIZE_T dwSize,
		DWORD flAllocationType,
		DWORD flProtect
	);


	//���ڴ�������
	int readmemoryInt(DWORD dwAddr);
	//���ڴ渡������
	float readmemoryFloat(DWORD dwAddr);
	//���ڴ��ı���
	char* readmemoryChar(DWORD dwAddr);

	//д�ڴ�������
	void writememoryInt(DWORD dwAddr, int num);

	//д�ڴ渡������
	void writememoryFloat(DWORD dwAddr, float flnum);

	// mFunc:Ҫע��ĺ�����ַ
	// Param��Ҫע��Ĳ�����ַ
	// ParamSize��������С
	// ParamNum: ��������
	void InjectCall(LPVOID mFunc);//��������
	void InjectCall(LPVOID mFunc, LPVOID Param, DWORD ParamSize);//������



public:
	//����window ��Ϣ�Ľṹ��
	struct wndObject {
		WCHAR title[128];
		WCHAR className[128];
		HWND hwnd;
	};

	// ����һ�����洰�ڶ������������
	std::vector<wndObject>wndList;

	//------------------�������-------------------------------
	// parentHwnd ��������
	// lpClassName ҪѰ�ҵĴ�������
	// lpWindowName ҪѰ�Ҵ��ڵı�����
	static HWND findSubWindow(HWND parentHwnd, LPCTSTR lpClassName, LPCTSTR lpWindowName);

	//ö�ٵĻص�����
	static BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lparam);


};

