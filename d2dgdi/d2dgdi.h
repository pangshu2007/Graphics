
// d2dgdi.h : d2dgdi Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// Cd2dgdiApp:
// �йش����ʵ�֣������ d2dgdi.cpp
//

class Cd2dgdiApp : public CWinAppEx
{
public:
	Cd2dgdiApp();


private:
	ULONG_PTR m_gdiplusToken;

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Cd2dgdiApp theApp;
