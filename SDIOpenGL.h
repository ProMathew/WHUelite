
// SDIOpenGL.h : SDIOpenGL Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CSDIOpenGLApp:
// �йش����ʵ�֣������ SDIOpenGL.cpp
//

class CSDIOpenGLApp : public CWinApp
{
public:
	CSDIOpenGLApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSDIOpenGLApp theApp;
