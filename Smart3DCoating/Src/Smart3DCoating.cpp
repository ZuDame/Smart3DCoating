//===================================================================================================
// Summary:
//		Smart3DCoating����ں���
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

	// ��ʼ��RichEdit
	if (!AfxInitRichEdit())
		return -1;

	// ��ʼ��ͨ�Ŷ˿�
	if (!AfxSocketInit())
		return -1;

	CoInitialize(NULL);

	// ��ʼ����Ȩ�ļ�
	CString strMainPath;
	GetMainModulePath(strMainPath, (HMODULE)theApp.m_hInstance);
	LicenseInit(strMainPath, PT_3DCOATING);

	if (g_pMenuHandler == NULL)
	{
		g_pMenuHandler = new CMenuHandler();
		g_pMenuHandler->Init();
	}
	
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);

	// ��ע����ж�ȡѡ����Ϣ
	ReadOptionInfo();

	// �Զ����ӷ�����
	if (g_bAutoConServer)
	{
		SClientInfo clientInfo;
		LoadServerData(clientInfo);
		ConSmartServer(TRUE, clientInfo);
	}

	return 0;
}

extern "C" void user_terminate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// �ر���������
	UnLicenseInit();

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
	GetMainModulePath(m_strAppPath, (HMODULE)this->m_hInstance);
	return TRUE;
}

BOOL CSmart3DCoatingApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
