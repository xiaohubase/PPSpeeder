
// PPSpeeder.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPPSpeederApp: 
// �йش����ʵ�֣������ PPSpeeder.cpp
//

class CPPSpeederApp : public CWinApp
{
public:
	CPPSpeederApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPPSpeederApp theApp;