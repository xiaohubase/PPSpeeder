
// PPSpeederDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PPSpeeder.h"
#include "PPSpeederDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <TlHelp32.h>
#include "AOBScan.h"
#include "VMProtectSDK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPPSpeederDlg �Ի���



CPPSpeederDlg::CPPSpeederDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPPSpeederDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPPSpeederDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPPSpeederDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()

	ON_STN_CLICKED(IDC_STATIC_tip, &CPPSpeederDlg::OnStnClickedStatictip)
	ON_BN_CLICKED(IDC_BUTTON1, &CPPSpeederDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPPSpeederDlg ��Ϣ�������

BOOL CPPSpeederDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	//if (FindWindow(_T("�Ż긨��_Client"), NULL) == NULL)
	//{
	//	AfxMessageBox(_T("δ�ҵ�����,�޷�ʹ�ã�"));
	//	ExitProcess(NULL);
	//	return FALSE;
	//}


	SetDlgItemInt(IDC_EDIT1, 1, FALSE);
	SetDlgItemInt(IDC_EDIT2, 1, FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�


void CPPSpeederDlg::OnOK()
{

}

HBRUSH CPPSpeederDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_tip)
	{
		pDC->SetTextColor(RGB(0, 0, 255));

	}

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}



void CPPSpeederDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPPSpeederDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//BOOL ����Ƿ�����(DWORD ProcessId)
//{
//	BOOL bRet = FALSE;
//	WCHAR MutexName[MAX_PATH] = { 0 };
//
//	wsprintf(MutexName, _T("Global\\Mutex.In%d"), ProcessId);
//
//	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, MutexName);
//
//	if (hMutex)
//	{
//		bRet = TRUE;
//		CloseHandle(hMutex);
//	}
//	else
//	{
//		bRet = FALSE;
//	}
//	return bRet;
//}

struct SpeederInfo
{
	DWORD ProcessId;
	PVOID AllocSpeeder;
	DWORD Speeder;
	DWORD Speeder2;
	WCHAR GameName[MAX_PATH];
};

using namespace std;
vector <LPCWSTR> g_Client = { 
	VMProtectDecryptStringW(TEXT("PopKart Client")),
	VMProtectDecryptStringW(TEXT("KartRider Client")),
	VMProtectDecryptStringW(TEXT(" Client")) };

vector <SpeederInfo> si;



DWORD GetCallAddress(HANDLE hProcess, DWORD Address)
{
	BYTE buffer[10] = { 0 };
	if (ReadProcessMemory(hProcess,
		(PVOID)Address,
		buffer,
		sizeof(buffer),
		NULL)) {
		if (buffer[0] == 0xE8)
			return Address + *reinterpret_cast<PDWORD>(&buffer[1]) + 5;
	}
	return -1;
}


LONG __ReadInt(HANDLE hProcess, DWORD Address)
{
	LONG buffer = NULL;
	BOOL res =
		ReadProcessMemory(hProcess,
		(PVOID)Address,
		&buffer,
		sizeof(LONG),
		NULL
		);
	if (res)	
		return buffer;	
	return -1;
}

HMODULE GetModuleHandleByAddress(HANDLE hProcess, LONG Address)
{
	
	MEMORY_BASIC_INFORMATION mbi = {0};
	memset(&mbi, 0, sizeof(MEMORY_BASIC_INFORMATION));
	if (VirtualQueryEx(hProcess, (LPVOID)Address, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) != sizeof(MEMORY_BASIC_INFORMATION))
		return NULL;
	return  (HMODULE)mbi.AllocationBase;
}

MODULEENTRY32 GetGameModuleEnter(LONG pid)
{
	MODULEENTRY32 me32 = { 0 };
	memset(&me32, 0, sizeof(MODULEENTRY32));
	HANDLE hSnapshot=
		CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);

	if (hSnapshot == INVALID_HANDLE_VALUE)
		return me32;

	me32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapshot, &me32))
	{
		do {
			if (!lstrcmpi(me32.szModule, TEXT("KartRider.exe")))
				break;
		} while (Module32Next(hSnapshot, &me32));
	}
	CloseHandle(hSnapshot);
	return me32;
}


BYTE ShellCode[] = { 
	0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x90, 0x8B, 0x15, 0x32, 0x00, 
	0x01, 0x06, 0xA3, 0x32, 0x00, 0x01, 0x06, 0x85, 0xD2, 0x74, 
	0x13, 0x8B, 0xC8, 0x2B, 0xCA, 0x0F, 0xAF, 0x0D, 0x36, 0x00, 
	0x01, 0x06, 0x01, 0x0D, 0x3A, 0x00, 0x01, 0x06, 0xEB, 0x05, 
	0xA3, 0x3A, 0x00, 0x01, 0x06, 0xA1, 0x3A, 0x00, 0x01, 0x06, 
	0xC3, 0x90
};


BOOL CPPSpeederDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	VMProtectBegin("OnCommand");
	WORD wID = LOWORD(wParam);
	WORD wNF = HIWORD(wParam);
	switch (wNF)
	{
	case CBN_DROPDOWN:
	{

						 if (wID == IDC_COMBO1)
						 {
							 wchar_t wc_class[MAX_PATH] = { 0 };
							 HWND tempwnd = NULL;
							 CComboBox* cbx = reinterpret_cast<CComboBox*> (GetDlgItem(IDC_COMBO1));
							 cbx->ResetContent();//���
							 do
							 {
								 tempwnd = ::FindWindowEx(NULL, tempwnd, NULL, NULL);
								 memset(wc_class, 0, MAX_PATH);
								 ::GetClassName(tempwnd, wc_class, MAX_PATH);
								 for (size_t i = 0; i < g_Client.size(); i++)
								 {
									 if (!lstrcmpi(wc_class, g_Client[i]))
									 {
										 if (!::IsWindowVisible(tempwnd))
										 	goto _____End;
										 DWORD ProcessId, AddIndex;
										 GetWindowThreadProcessId(tempwnd, &ProcessId);
										 
										 for (size_t i = 0; i < si.size(); i++)
										 {
											 if (si[i].ProcessId == ProcessId)
											 {
												 AddIndex = cbx->AddString(si[i].GameName);
												 cbx->SetItemData(AddIndex, ProcessId);
												 goto _____End;
											 }
										 }
										 auto hProcess = OpenProcess(
											 PROCESS_VM_OPERATION |
											 PROCESS_VM_WRITE |
											 PROCESS_VM_READ,  FALSE,
											 ProcessId);
										 if (!hProcess)
										 {
											 AfxMessageBox(VMProtectDecryptStringW(L"�򿪽���ʧ��"));
											 goto _____End;
										 }

										 auto me32 = GetGameModuleEnter(ProcessId);

										 DWORD ScanBegin = reinterpret_cast<DWORD>(me32.modBaseAddr), ScanEnd = reinterpret_cast<DWORD>(me32.modBaseAddr + me32.modBaseSize);

										 DWORD Address;
										 Address = _AOBScan(ProcessId, VMProtectDecryptStringA("32 C0 EB 11 8B"), ScanBegin, ScanEnd);
										 if (!Address)
										 {
											 AfxMessageBox(VMProtectDecryptStringW(L"����������ʧ��"));
											 goto _____End;
										 }
										 Address -= 18;



										 TRACE("Address:%08X\n", Address);
										 Address = GetCallAddress(hProcess, Address) + 8;
										 TRACE("Address:%08X\n", Address);
										 Address = __ReadInt(hProcess, Address);
										 TRACE("Address:%08X\n", Address);
										 Address += 0x7c;
										 TRACE("Address:%08X\n", Address);
										 Address = __ReadInt(hProcess, Address);
										 TRACE("Address:%08X\n", Address);
										 WCHAR GameName[MAX_PATH] = { 0 };
										 ReadProcessMemory(hProcess, reinterpret_cast<PVOID>(Address), GameName, sizeof(GameName), NULL);
										 AddIndex = cbx->AddString(GameName);
										 cbx->SetItemData(AddIndex, ProcessId);
										 auto speeder =
											 VirtualAllocEx(hProcess, NULL, 128, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
										 if (!speeder)
										 {
											 AfxMessageBox(VMProtectDecryptStringW(L"�����ڴ�ʧ�ܣ�����ʧ��"));
											 CloseHandle(hProcess);
											 goto _____End;
										 }
										 auto speedaddr = _AOBScan(ProcessId, VMProtectDecryptStringA("E9 9C 00 00 00 6A 00"), ScanBegin, ScanEnd) + 0x46;
										 TRACE("speedaddr:%08X\n", speedaddr);

										 speedaddr = __ReadInt(hProcess, speedaddr);
										 TRACE("speedaddr:%08X\n", speedaddr);
										 DWORD cyaddr, cyoffset;
										 cyoffset = _AOBScan(ProcessId, VMProtectDecryptStringA("33 D2 B9 E8 03 00 00 F7 F1 50 6A 00 6A 00 6A 00"), ScanBegin, ScanEnd) + 0x54;
										 TRACE("cyoffset:%08X\n", cyoffset);
										 cyoffset = __ReadInt(hProcess, cyoffset);
										 TRACE("cyoffset:%08X\n", cyoffset);
										 cyaddr = _AOBScan(ProcessId, VMProtectDecryptStringA("8B 41 14 EB 7F 8D 55"), ScanBegin, ScanEnd) + 0xE;
										 TRACE("cyaddr:%08X\n", cyaddr);
										 cyaddr = GetCallAddress(hProcess, cyaddr) + 0x8;
										 TRACE("cyaddr:%08X\n", cyaddr);
										 cyaddr = __ReadInt(hProcess, cyaddr);
										 TRACE("cyaddr:%08X\n", cyaddr);
										 cyaddr += cyoffset;
										 TRACE("cyaddr:%08X\n", cyaddr);

										 SpeederInfo speederInfo = { ProcessId ,speeder ,speedaddr ,cyaddr };


										 DWORD paramaddr = reinterpret_cast<DWORD>(speeder) + sizeof(ShellCode);
										 TRACE("paramaddr:%08X\n", paramaddr);
										 ReadProcessMemory(hProcess, reinterpret_cast<PVOID>(__ReadInt(hProcess, speedaddr)), &ShellCode, 5, NULL);

										 paramaddr += 4;
										 
										 memcpy(&ShellCode[0x8], &paramaddr, sizeof(DWORD_PTR));//ԭʼCount
										 memcpy(&ShellCode[0xd], &paramaddr, sizeof(DWORD_PTR));//ԭʼCount
										 paramaddr += 4;
										 memcpy(&ShellCode[0x1c], &paramaddr, sizeof(DWORD_PTR));//�����ٶ�
										 paramaddr += 4;
										 memcpy(&ShellCode[0x22], &paramaddr, sizeof(DWORD_PTR));//�µ�Count
										 memcpy(&ShellCode[0x29], &paramaddr, sizeof(DWORD_PTR));//�µ�Count
										 memcpy(&ShellCode[0x2e], &paramaddr, sizeof(DWORD_PTR));//�µ�Count
										 paramaddr -= 0xc;
										 DWORD oldtick = __ReadInt(hProcess, speedaddr);
										 WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(paramaddr), &oldtick, sizeof(DWORD), NULL);
										 DWORD Speed = 0x1;
										 WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(paramaddr + 0x8), &Speed, sizeof(DWORD), NULL);
										 WriteProcessMemory(hProcess, speeder, &ShellCode, sizeof(ShellCode), NULL);
										 WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(speedaddr), &speeder, sizeof(DWORD), NULL);
										 wsprintf(speederInfo.GameName, TEXT("%ws"), GameName);
										 si.push_back(speederInfo);
										 CloseHandle(hProcess);
									 }
								 }
							 _____End:;

							 } while (tempwnd != NULL);

						 }

	}
		break;
	case CBN_SELCHANGE:
		//AfxMessageBox(L"�޸�ѡ��");
		break;
	case BN_CLICKED:
	{
					   if (wID == IDC_BUTTON1)
					   {
						   CComboBox *cbx = (CComboBox*)GetDlgItem(IDC_COMBO1);
						   DWORD pid = cbx->GetItemData(cbx->GetCurSel());
						   for (size_t i = 0; i < si.size(); i++)
						   {
							   TRACE("GetItemData��%d\n", pid);

							   if (si[i].ProcessId == pid)
							   {

								   //if (!����Ƿ�����(pid))
								   //{
									  // AfxMessageBox(_T("����,�����Ϸû�й����"));
									  // break;
								   //}

								   HANDLE hProcess = OpenProcess(PROCESS_VM_OPERATION |
									   PROCESS_VM_WRITE |
									   PROCESS_VM_READ, NULL, pid);
								   if (!hProcess)
								   {
									   AfxMessageBox(VMProtectDecryptStringW(TEXT("�޷��򿪽���")));
									   break;
								   }
								   LONG speed = GetDlgItemInt(IDC_EDIT1);
								   if (speed <= -8)								   
									   speed = 1;
								   WriteProcessMemory(hProcess, (LPVOID)((DWORD)si[i].AllocSpeeder + sizeof(ShellCode)+8), &speed, sizeof(DWORD), NULL);
								   CloseHandle(hProcess);
								   break;
							   }
						   }
						   break;
					   }
					   else if (wID == IDC_BUTTON2)
					   {
						   CComboBox *cbx = (CComboBox*)GetDlgItem(IDC_COMBO1);
						   DWORD pid = cbx->GetItemData(cbx->GetCurSel());
						   for (size_t i = 0; i < si.size(); i++)
						   {
							   TRACE("GetItemData��%d\n", pid);

							   if (si[i].ProcessId == pid)
							   {
								   //if (!����Ƿ�����(pid))
								   //{
									  // AfxMessageBox(_T("����,�����Ϸû�й����"));
									  // break;
								   //}
								   HANDLE hProcess = OpenProcess(PROCESS_VM_OPERATION |
									   PROCESS_VM_WRITE |
									   PROCESS_VM_READ,  NULL, pid);
								   if (!hProcess)
								   {
									   AfxMessageBox(VMProtectDecryptStringW(TEXT("�޷��򿪽���")));
									   break;
								   }
								   DWORD __speed = 0x1;
								   WriteProcessMemory(hProcess, (LPVOID)((DWORD)si[i].AllocSpeeder + sizeof(ShellCode)+8), &__speed, sizeof(DWORD), NULL);
								   CloseHandle(hProcess);
								   break;
							   }
						   }
						   break;

					   }
					   else if (wID == IDC_BUTTON3)
					   {
						   CComboBox *cbx = (CComboBox*)GetDlgItem(IDC_COMBO1);
						   DWORD pid = cbx->GetItemData(cbx->GetCurSel());
						   for (size_t i = 0; i < si.size(); i++)
						   {
							   if (si[i].ProcessId == pid)
							   {
								   //if (!����Ƿ�����(pid))
								   //{
									  // AfxMessageBox(_T("����,�����Ϸû�й����"));
									  // break;
								   //}
								   HANDLE hProcess = OpenProcess(PROCESS_VM_OPERATION |
									   PROCESS_VM_WRITE |
									   PROCESS_VM_READ, NULL, pid);
								   if (!hProcess)
								   {
									   AfxMessageBox(VMProtectDecryptStringW(TEXT("�޷��򿪽���")));
									   break;
								   }
								   INT __cyday = GetDlgItemInt(IDC_EDIT2);
								   if (__cyday > 29999)
								   {
									   AfxMessageBox(VMProtectDecryptStringW(TEXT("��Խ��ֵ����29999����")));
									   CloseHandle(hProcess);
									   break;
								   }
								   WORD oldday = NULL;
								   ReadProcessMemory(hProcess, (LPVOID)si[i].Speeder2, &oldday, sizeof(WORD), NULL);
								   __cyday += oldday;
								   WriteProcessMemory(hProcess, (LPVOID)si[i].Speeder2, &__cyday, sizeof(WORD), NULL);
								   CloseHandle(hProcess);
								   break;
							   }
						   }
						   break;


					   }
					   break;
	}
	default:
		break;
	}
	VMProtectEnd();
	return CDialog::OnCommand(wParam, lParam);
}


void CPPSpeederDlg::OnStnClickedStatictip()
{
	ShellExecute(NULL, TEXT("Open"), VMProtectDecryptStringW(TEXT("https://github.com/xiaohubase/PPSpeeder")), NULL, NULL, SW_HIDE);

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPPSpeederDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
