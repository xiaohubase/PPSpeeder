
// PPSpeederDlg.h : ͷ�ļ�
//

#pragma once


// CPPSpeederDlg �Ի���
class CPPSpeederDlg : public CDialog
{
// ����
public:
	CPPSpeederDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PPSPEEDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;


	virtual void OnOK();
	// ���ɵ���Ϣӳ�亯��



	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();




	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:


	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnStnClickedStatictip();
	afx_msg void OnBnClickedButton1();
};
