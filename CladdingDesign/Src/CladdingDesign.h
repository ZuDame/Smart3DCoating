//===================================================================================================
// Summary:
//		CladdingDesign的入口函数
// Date:
//		2017-02-20
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#ifndef __CladdingDesignApp_h__
#define __CladdingDesignApp_h__

//===================================================================================================

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

//===================================================================================================

#include "resource.h"

//===================================================================================================

class CCladdingDesignApp : public CWinApp
{
public:
	CCladdingDesignApp();

	CString m_strAppPath;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

private:
	DECLARE_MESSAGE_MAP()
};

//===================================================================================================

extern CCladdingDesignApp theApp;
extern BOOL g_bSelectDlgOpen;

//===================================================================================================

#endif
