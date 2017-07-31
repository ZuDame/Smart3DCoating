//===================================================================================================
// Summary:
//		Smart3DCoating的入口函数
// Date:
//		2017-02-20
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#ifndef __Smart3DCoatingApp_h__
#define __Smart3DCoatingApp_h__

//===================================================================================================

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

//===================================================================================================

#include "resource.h"

//===================================================================================================

class CSmart3DCoatingApp : public CWinApp
{
public:
	CSmart3DCoatingApp();

	CString m_strAppPath;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

private:
	DECLARE_MESSAGE_MAP()
};

//===================================================================================================

extern CSmart3DCoatingApp theApp;
extern BOOL g_bSelectDlgOpen;

//===================================================================================================

#endif
