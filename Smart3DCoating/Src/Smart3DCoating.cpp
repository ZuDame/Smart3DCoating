//===================================================================================================
// Summary:
//		Smart3DCoating的入口函数
// Date:
//		2017-02-20
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#include "stdafx.h"
#include "MenuHandler.h"
#include "Global.h"
#include "Smart3DCoating.h"

//===================================================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//===================================================================================================

CSmart3DCoatingApp theApp;
CMenuHandler *g_pMenuHandler = NULL;
ULONG_PTR g_gdiplusToken = 0;
BOOL g_bSelectDlgOpen = FALSE;

//===================================================================================================

BEGIN_MESSAGE_MAP(CSmart3DCoatingApp, CWinApp)
END_MESSAGE_MAP()

//===================================================================================================

extern "C" int user_initialize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// 初始化RichEdit
	if (!AfxInitRichEdit())
		return -1;

	CoInitialize(NULL);

	if (g_pMenuHandler == NULL)
	{
		g_pMenuHandler = new CMenuHandler();
		g_pMenuHandler->Init();
	}
	
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);

	return 0;
}

extern "C" void user_terminate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DestroyAllDialog();

	if (g_pMenuHandler != NULL)
	{
		g_pMenuHandler->UnInit();
		SAFE_DELETE_POINTER(g_pMenuHandler);
	}
	
	if (g_gdiplusToken != 0)
	{
		GdiplusShutdown(g_gdiplusToken);
		g_gdiplusToken = 0;
	}
	CoUninitialize();
}

//===================================================================================================

CSmart3DCoatingApp::CSmart3DCoatingApp()
{
}

BOOL CSmart3DCoatingApp::InitInstance()
{
	CWinApp::InitInstance();
	GetMainModulePath((HMODULE)this->m_hInstance, m_strAppPath);
	return TRUE;
}

BOOL CSmart3DCoatingApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
