
// PPSpeederDlg.h : 头文件
//

#pragma once


// CPPSpeederDlg 对话框
class CPPSpeederDlg : public CDialog
{
// 构造
public:
	CPPSpeederDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PPSPEEDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;


	virtual void OnOK();
	// 生成的消息映射函数



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
