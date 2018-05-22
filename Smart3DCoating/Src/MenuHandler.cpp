//===================================================================================================
// Summary:
//		Smart3DCoating�Ĳ˵��Լ�������Ӧ����
// Date:
//		2016-02-20
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#include "stdafx.h"
#include "Smart3DCoating.h"
#include "MenuHandler.h"
#include "PreGlobal.h"
#include "DlgConfirm.h"
#include "DlgAbout.h"
#include "DlgConServer.h"
#include "DlgMergeDir.h"

//===================================================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//===================================================================================================

CDlgSetThick* g_pDlgThick = NULL;

uiCmdAccessState ConServerAccessDefault(uiCmdAccessMode access_mode)
{
	access_mode;
	// ���ڵ�������Ȩ������Ҫ���ӷ�����
	if (GetSingleLicense())
		return ACCESS_UNAVAILABLE;

	return ACCESS_AVAILABLE;
}

uiCmdAccessState DisConServerAccessDefault(uiCmdAccessMode access_mode)
{
	access_mode;
	// ���ڵ�������Ȩ������Ҫ���ӷ�����
	if (GetSingleLicense())
		return ACCESS_UNAVAILABLE;

	return ACCESS_AVAILABLE;
}

uiCmdAccessState MainAccessAvailable(uiCmdAccessMode access_mode)
{
	access_mode;
	return ACCESS_AVAILABLE;
}

uiCmdAccessState UserAccessDefault(uiCmdAccessMode access_mode)
{
	UNUSED_ALWAYS(access_mode);
	ProMdl curmdl = NULL;
	if (ProMdlCurrentGet(&curmdl) != PRO_TK_NO_ERROR)
		return ACCESS_UNAVAILABLE;
	ProMdlType type;
	ProMdlTypeGet(curmdl, &type);
	if (type == PRO_MDL_PART || type == PRO_ASSEMBLY)
		return ACCESS_AVAILABLE;
	else
		return ACCESS_UNAVAILABLE;
}

// �˵����ɼ�
uiCmdAccessState UserNoShow(uiCmdAccessMode access_mode)
{
	UNUSED_ALWAYS(access_mode);
	return ACCESS_INVISIBLE;
}

// ��Ӳ˵���
void AddMenuItem(char *menuName, char *itemName, actionFunc menuitemAction, accessFunc menuitemAccess, 
				 ProFileName msgFile, char *iconName/* = NULL*/)
{
	string strCmd, strName, strLabel, strHelp, strTip;
	uiCmdCmdId uiMenuBottonId;
	strLabel = string(itemName);
	strCmd = string("cmd") + strLabel;
	ProError status = ProCmdActionAdd((char*)strCmd.c_str(), (uiCmdCmdActFn)menuitemAction, uiProeImmediate, menuitemAccess, PRO_B_TRUE, PRO_B_TRUE, &uiMenuBottonId);
	strName = string("Item_") + strLabel + string("Name");
	strHelp = strLabel + string(" Operator");
	strTip = string("Show ") + strHelp;
	status = ProMenubarmenuPushbuttonAdd(menuName, (char*)strName.c_str(), (char*)strLabel.c_str(), (char*)strHelp.c_str(), NULL, PRO_B_TRUE, uiMenuBottonId, msgFile);
	if (iconName != NULL)
	{
		status = ProCmdIconSet(uiMenuBottonId, iconName);
		status = ProCmdDesignate(uiMenuBottonId, (char*)strLabel.c_str(), (char*)strHelp.c_str(), (char*)strTip.c_str(), msgFile);
	}
}

//===================================================================================================

// ��ʼ���˵�
BOOL CMenuHandler::Init()
{
	ProFileName Msg = L"Msg_Smart3DCoating.txt";

	char szSmart3DCoatingMenuName[] = "Smart3DCoatingMenuName";
	ProMenubarMenuAdd(szSmart3DCoatingMenuName, "Smart3DCoating", "Utilities", PRO_B_TRUE, Msg);

	AddMenuItem(szSmart3DCoatingMenuName, "CD_LoadSolid", OnLoadSolidActFn, MainAccessAvailable, Msg);
	AddMenuItem(szSmart3DCoatingMenuName, "CD_Clean", OnCleanActFn, UserAccessDefault, Msg);

	char szCreateChildMenuName[] = "Smart3DCoatingCreateMenuName";
	ProMenubarmenuMenuAdd(szSmart3DCoatingMenuName, szCreateChildMenuName, "CD_Create", NULL, PRO_B_TRUE, Msg);
	{
		AddMenuItem(szCreateChildMenuName, "CD_RapidCreate", OnRapidCreatActFn, UserAccessDefault, Msg, "CD_RapidCreate.gif");
		AddMenuItem(szCreateChildMenuName, "CD_CreateByEdge", OnCreateByEdgeActFn, UserAccessDefault, Msg, "CD_CreateByEdge.gif");
		AddMenuItem(szCreateChildMenuName, "CD_CreateByChain", OnCreateByChainActFn, UserAccessDefault, Msg, "CD_CreateByChain.gif");
		AddMenuItem(szCreateChildMenuName, "CD_CreateBySketch", OnCreateBySketchActFn, UserAccessDefault, Msg, "CD_CreateBySketch.gif");
	}
	char szTrimChildMenuName[] = "Smart3DCoatingTrimMenuName";
	ProMenubarmenuMenuAdd(szSmart3DCoatingMenuName, szTrimChildMenuName, "CD_Trim", NULL, PRO_B_TRUE, Msg);
	{
		AddMenuItem(szTrimChildMenuName, "CD_TrimByEdge", OnTrimByEdgeActFn, UserAccessDefault, Msg, "CD_TrimByEdge.gif");
		AddMenuItem(szTrimChildMenuName, "CD_TrimBySketch", OnTrimBySketchActFn, UserAccessDefault, Msg, "CD_TrimBySketch.gif");
	}

	char szMergeChildMenuName[] = "Smart3DCoatingMergeMenuName";
	ProMenubarmenuMenuAdd(szSmart3DCoatingMenuName, szMergeChildMenuName, "CD_Merge", NULL, PRO_B_TRUE, Msg);
	{
		AddMenuItem(szMergeChildMenuName, "CD_Merge_Manual", OnMergeManualActFn, UserAccessDefault, Msg, "CD_Merge_Manual.gif");
		AddMenuItem(szMergeChildMenuName, "CD_Merge_Auto", OnMergeAutoActFn, UserAccessDefault, Msg, "CD_Merge_Auto.gif");
	}

	char szOffsetChildMenuName[] = "Smart3DCoatingOffsetMenuName";
	ProMenubarmenuMenuAdd(szSmart3DCoatingMenuName, szOffsetChildMenuName, "CD_Offset", NULL, PRO_B_TRUE, Msg);
	{
		AddMenuItem(szOffsetChildMenuName, "CD_OffsetGeneral", OnOffsetGeneralActFn, UserAccessDefault, Msg, "CD_OffsetGeneral.gif");
		AddMenuItem(szOffsetChildMenuName, "CD_OffsetSingle", OnOffsetSingleActFn, UserAccessDefault, Msg, "CD_OffsetSingle.gif");
	}

	char szBottomChildMenuName[] = "Smart3DCoatingBottomMenuName";
	ProMenubarmenuMenuAdd(szSmart3DCoatingMenuName, szBottomChildMenuName, "CD_Bottom", NULL, PRO_B_TRUE, Msg);
	{
		AddMenuItem(szBottomChildMenuName, "CD_BottomCut", OnBottomCutActFn, UserAccessDefault, Msg, "CD_BottomCut.gif");
		AddMenuItem(szBottomChildMenuName, "CD_BottomExpand", OnBottomExpandActFn, UserAccessDefault, Msg, "CD_BottomExpand.gif");
	}
	AddMenuItem(szSmart3DCoatingMenuName, "CD_Create_Body", OnCreateBodyActFn, UserAccessDefault, Msg, "CD_Create_Body.gif");

	char szExportChildMenuName[] = "Smart3DCoatingExportMenuName";
	ProMenubarmenuMenuAdd(szSmart3DCoatingMenuName, szExportChildMenuName, "CD_Export", NULL, PRO_B_TRUE, Msg);
	{
		AddMenuItem(szExportChildMenuName, "CD_Export_STL", OnExportSTLActFn, UserAccessDefault, Msg, "CD_Export_STL.gif");
		//AddMenuItem(szExportChildMenuName, "CD_Export_STP", OnExportSTPActFn, UserAccessDefault, Msg);
	}

	AddMenuItem(szSmart3DCoatingMenuName, "CD_ShortCutMenu", ShowShortCutMenu, UserAccessDefault, Msg);

	// ���֤������
	char szLicenseServerChildMenuName[] = "CD_LicServerMenuName";
	ProMenubarmenuMenuAdd(szSmart3DCoatingMenuName, szLicenseServerChildMenuName, "CD_LicenseServer", NULL, PRO_B_TRUE, Msg);
	{
		// ����
		AddMenuItem(szLicenseServerChildMenuName, "CD_ConnectServer", ConServerActFn, ConServerAccessDefault, Msg);
		// �Ͽ�
		AddMenuItem(szLicenseServerChildMenuName, "CD_DisconnectServer", DisConServerActFn, DisConServerAccessDefault, Msg);
	}

	// ����
	AddMenuItem(szSmart3DCoatingMenuName, "CD_Help", OnHelp, MainAccessAvailable, Msg);
	// ����
	AddMenuItem(szSmart3DCoatingMenuName, "CD_About", OnAbout, MainAccessAvailable, Msg);

	// ��ʼ����ݲ˵�
	int menuid, menuid_create, menuid_edit, menuid_export;
	ProMenufileName Mnu = "ShortCut.mnu";
	ProMenufileName MnuCreate = "ShortCut_Create.mnu";
	ProMenufileName MnuEdit = "ShortCut_Edit.mnu";
	ProMenufileName MnuExport = "ShortCut_Export.mnu";

	// ע��˵��ļ�
	char szMnuName[] = "Smart3DCoating_ShortCut";
	char szMnuNameCreate[] = "ShortCut_Create";
	char szMnuNameEdit[] = "ShortCut_Edit";
	char szMnuNameExport[] = "ShortCut_Export";

	ProMenuFileRegister(szMnuName, Mnu, &menuid);
	ProMenuFileRegister(szMnuNameCreate, MnuCreate, &menuid_create);
	ProMenuFileRegister(szMnuNameEdit, MnuEdit, &menuid_edit);
	ProMenuFileRegister(szMnuNameExport, MnuExport, &menuid_export);

	// ���ò˵���������
	ProMenubuttonActionSet(szMnuName, "CD_Clean", (ProMenubuttonAction)OnCleanActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameCreate, "CD_RapidCreate", (ProMenubuttonAction)OnRapidCreatActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameCreate, "CD_CreateByEdge", (ProMenubuttonAction)OnCreateByEdgeActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameCreate, "CD_CreateByChain", (ProMenubuttonAction)OnCreateByChainActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameCreate, "CD_CreateBySketch", (ProMenubuttonAction)OnCreateBySketchActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameEdit, "CD_TrimByEdge", (ProMenubuttonAction)OnTrimByEdgeActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameEdit, "CD_TrimBySketch", (ProMenubuttonAction)OnTrimBySketchActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameEdit, "CD_Merge_Manual", (ProMenubuttonAction)OnMergeManualActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameEdit, "CD_Merge_Auto", (ProMenubuttonAction)OnMergeAutoActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameEdit, "CD_OffsetGeneral", (ProMenubuttonAction)OnOffsetGeneralActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameEdit, "CD_OffsetSingle", (ProMenubuttonAction)OnOffsetSingleActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameEdit, "CD_BottomCut", (ProMenubuttonAction)OnBottomCutActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameEdit, "CD_BottomExpand", (ProMenubuttonAction)OnBottomExpandActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameEdit, "CD_Create_Body", (ProMenubuttonAction)OnCreateBodyActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameExport, "CD_Export_STL", (ProMenubuttonAction)OnExportSTLActFn, NULL, 0);
	//ProMenubuttonActionSet(szMnuNameExport, "CD_Export_STP", (ProMenubuttonAction)OnExportSTPActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameExport, "CD_Close", (ProMenubuttonAction)OnMenuQuit, NULL, 0);
	ProMenubuttonActionSet(szMnuName, szMnuName, (ProMenubuttonAction)ProMenuDelete, NULL, 0);
	ProMenubuttonActionSet(szMnuNameCreate, szMnuNameCreate, (ProMenubuttonAction)ProMenuDelete, NULL, 0);
	ProMenubuttonActionSet(szMnuNameEdit, szMnuNameEdit, (ProMenubuttonAction)ProMenuDelete, NULL, 0);
	ProMenubuttonActionSet(szMnuNameExport, szMnuNameExport, (ProMenubuttonAction)ProMenuDelete, NULL, 0);

	return TRUE;
}

void CMenuHandler::UnInit()
{
	DestroyAllDialog();
}

//===================================================================================================

// �ͳ����жԻ���
void DestroyAllDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (g_pDlgThick != NULL)
	{
		if (g_pDlgThick->GetSafeHwnd() != NULL)
			g_pDlgThick->DestroyWindow();

		SAFE_DELETE_POINTER(g_pDlgThick);
	}
}

// ȡ����ǰ����
void CancelOperator()
{
	ProWindowRepaint(PRO_VALUE_UNUSED);
}

// ȡ��ѡ��״̬
void CancelSelectDialog()
{
	if (g_bSelectDlgOpen)
		ProMacroLoad(L"~ Activate `GET SELECT` `quit_sel`;");
}

// ɾ����Ч����ָ��
void DeleteInvalidOperator()
{

}

// �ͷŲ�������
void DeleteOperator()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProWindowRepaint(PRO_VALUE_UNUSED);
	//DestroyAllDialog();
	//ActivateCurrentPreWindow();
}

//===================================================================================================

// ��ʾ��ݲ˵�
void ShowShortCutMenu()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	static char *compoundmenu[] = {"Smart3DCoating_ShortCut", "ShortCut_Create", "ShortCut_Edit", "ShortCut_Export", ""};
	int nMenuid;
	ProError status =ProCompoundmenuCreate(compoundmenu, &nMenuid);
	if (status == PRO_TK_NO_ERROR)
	{
		ProMenuProcess(compoundmenu[0], &nMenuid);
	}
}

// �˳���ݲ˵�
void OnMenuQuit()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	int nWnd;
	if (ProWindowCurrentGet(&nWnd) == PRO_TK_NO_ERROR)
	{
		ProMenuDelete();
		ProWindowActivate(nWnd);
	}
}

// ���ذ�������
void OnLoadSolidActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// ѡ��һ�����صĴ�����ģ��
	ProPath pathSelModel;
	if (ProFileOpen(L"ѡ�������ģ��", L"*.prt,*.asm,*.stp,*.stl", NULL, NULL, NULL, NULL, pathSelModel) == PRO_TK_NO_ERROR)
	{
		ProPath pathFolder;
		ProName nameModel, nameExt;
		int nVersion;
		ProFilenameParse(pathSelModel, pathFolder, nameModel, nameExt, &nVersion);

		ProMdl pMdlToCoating;
		if (CString(nameExt).CompareNoCase(L"stp") == 0 || CString(nameExt).CompareNoCase(L"step") == 0)
		{
			LoadStep(CString(pathSelModel), pMdlToCoating);
		}
		else
		{
			ProMdlLoad(pathSelModel, PRO_MDL_UNUSED, PRO_B_FALSE, &pMdlToCoating);
		}


		// ����һ��װ����
		CString strSolidName;
		strSolidName = nameModel;

		strSolidName.Format(L"%s_3DCoat", strSolidName);

		ProName nameSolid;
		wcsncpy_s(nameSolid, PRO_NAME_SIZE, strSolidName, _TRUNCATE);

		ProSolid solid;
		ProSolidCreate(nameSolid, PRO_ASSEMBLY, &solid);

		// ����Ĭ������ϵ
		ProError status;
		ProElement elemTree = NULL;
		ProElementAlloc(PRO_E_FEATURE_TREE, &elemTree);
		{
			// ��������PRO_E_FEATURE_TYPE
			ProElement elemType;
			status = ProElementAlloc(PRO_E_FEATURE_TYPE, &elemType);
			status = ProElemtreeElementAdd(elemTree, NULL, elemType);
			ProValueData valdataType;
			valdataType.type = PRO_VALUE_TYPE_INT;
			valdataType.v.i = PRO_FEAT_CSYS;
			ProValue valueType = NULL;
			status = ProValueAlloc(&valueType);
			status = ProValueDataSet(valueType, &valdataType);
			status = ProElementValueSet(elemType, valueType);
		}
		ProModelitem itemAssmble;
		ProMdlToModelitem(ProAssemblyToMdl(solid), &itemAssmble);
		ProSelection selAssmble;
		ProSelectionAlloc(NULL, &itemAssmble, &selAssmble);

		ProErrorlist errorsList;
		ProFeatureCreateOptions opts[1] = {PRO_FEAT_CR_NO_OPTS};
		ProFeature feat;
		status = ProFeatureCreate(selAssmble, elemTree, opts, 1, &feat, &errorsList);


		// ��ѡ��Ĵ�װ��ģ��װ�䵽װ������
		ProMatrix initPos = {
			{1.0, 0.0, 0.0, 0.0},
			{0.0, 1.0, 0.0, 0.0},
			{0.0, 0.0, 1.0, 0.0},
			{0.0, 0.0, 0.0, 1.0}
		};
		ProAsmcomp comp;
		ProAsmcompAssemble(ProAssembly(solid), ProMdlToSolid(pMdlToCoating), initPos, &comp);
		ProAsmcompconstraint compConstr;
		ProAsmcompconstraintAlloc(&compConstr);
		ProAsmcompconstraintTypeSet(compConstr, PRO_ASM_DEF_PLACEMENT);
		ProAsmcompconstraint* compConstrs;
		ProArrayAlloc (0, sizeof (ProAsmcompconstraint), 1, (ProArray*)&compConstrs);
		status = ProArrayObjectAdd ((ProArray*)&compConstrs, -1, 1, &compConstr);
		ProAsmcompConstraintsSet(NULL, &comp, compConstrs);

		ProMdlDisplay(solid);
		int nWnd;
		ProWindowCurrentGet(&nWnd);
		ProWindowActivate(nWnd);
	}

	InvalidateDrawing();
	ProMessageClear();
}

// ��������
void OnCleanActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// ��1������鵱ǰģ�ͻ���
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"��ǰ����δ����ģ�ͣ�", L"��ʾ", MB_OK|MB_ICONWARNING);
		return;
	}

	// ��2����ѡ��ο�
	ShowMessageTip(L"Tips.ѡ�񼸺�ƽ����������ɶ�ѡ...");
	vector<ProSelection> arrSelSrf;
	vector<ProModelitem> arrItemSrf;
	while (SelectObject(arrSelSrf, "surface", MAX_SELECTION))
	{
		BOOL bError = FALSE;
		// ��3����������׼��
		for (int i=0; i<(int)arrSelSrf.size(); i++)
		{
			ProModelitem itemSurf;
			ProSelectionModelitemGet(arrSelSrf[i], &itemSurf);

			ProModelitem itemPlane;
			int nDatumFeatID = CreateDatumPlane(pMdl, arrSelSrf[i], itemPlane);
			if (nDatumFeatID > 0)
			{
				arrItemSrf.push_back(itemPlane);
			}
		}
		// ��4��������ʵ�廯����
		for (int i=0; i<(int)arrItemSrf.size(); i++)
		{
			ProSelection selPlane;
			ProSelectionAlloc(NULL, &arrItemSrf[i], &selPlane);
			if (CreateSolidify(pMdl, selPlane) < 0)
			{
				bError = TRUE;
			}
			SAFE_DELETE_SELECTION(selPlane);
		}

		arrSelSrf.clear();
		arrItemSrf.clear();
				
		// ��5������ʾ������
		if (bError && MessageBox(NULL, L"������������ʧ�ܣ����ģ�����������Ƿ������", L"��ʾ", MB_YESNO) == IDNO)
		{
			break;
		}
	}
	InvalidateDrawing();
	ProMessageClear();
}

// ���ٴ���������
void OnRapidCreatActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// ��1������鵱ǰģ�ͻ���
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"��ǰ����δ����ģ�ͣ�", L"��ʾ", MB_OK|MB_ICONWARNING);
		return;
	}

	// ��2����ѡ��ο�
	ShowMessageTip(L"Tips.ѡ�񼸺�����д������ɶ�ѡ...");
	vector<ProSelection> arrSelSrf;
	while (SelectObject(arrSelSrf, "surface", MAX_SELECTION))
	{
		// ��3����������
		BOOL bError = FALSE;
		for (int i=0; i<(int)arrSelSrf.size(); i++)
		{
			ProModelitem itemQlt;
			if (CreateSurface(pMdl, arrSelSrf[i], itemQlt) > 0)
			{
				SetSurfColor(itemQlt, 154, 205, 7);
			}
			else
				bError = TRUE;
		}
		// ��4������ʾ������
		if (bError && MessageBox(NULL, L"������������ʧ�ܣ����ģ�����������Ƿ������", L"��ʾ", MB_YESNO) == IDNO)
		{
			break;
		}
	}
	InvalidateDrawing();
	ProMessageClear();
}

// �ӱ߽紴��
void OnCreateByEdgeActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);
	
	// ��1������鵱ǰģ�ͻ���
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"��ǰ����δ����ģ�ͣ�", L"��ʾ", MB_OK|MB_ICONWARNING);
		return;
	}
	// ��2����ѡ��ο�
	ShowMessageTip(L"Tips.ѡ�������߽���д���...");
	vector<ProSelection> arrSelEdge;
	while (SelectObject(arrSelEdge, "edge", 2))
	{
		int nSelCount = (int)arrSelEdge.size();
		if (nSelCount < 2)
			break;
		// ��3����������
		ProModelitem itemQlt;
		if (CreateSurfByTwoEdge(pMdl, arrSelEdge[0], arrSelEdge[1], itemQlt) > 0)
		{
			SetSurfColor(itemQlt, 154, 205, 7);
		}
		else if (MessageBox(NULL, L"������������ʧ�ܣ����ģ�����������Ƿ������", L"��ʾ", MB_YESNO) == IDNO)
		{
			break;
		}
	}
	
	InvalidateDrawing();
	ProMessageClear();
}

// �ӱ߽�(��)����
void OnCreateByChainActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// ��鵱ǰģ�ͻ���
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"��ǰ����δ����ģ�ͣ�", L"��ʾ", MB_OK|MB_ICONWARNING);
		return;
	}

	int nFeatID[3]={0};
	ProModelitem itemCurve[2];
	
	// ��1����ѡȡ��һ��������������
	ShowMessageTip(L"Tips.ѡȡͬһ�����ϵ������ߴ�����һ������...");
	vector<ProSelection> arrSelEdge;
	if (!SelectObject(arrSelEdge, "edge", 2))
	{
		return;
	}
	
	// ��2����������һ������
	int nSelCount = (int)arrSelEdge.size();
	if (nSelCount == 1)
		arrSelEdge.push_back(arrSelEdge[0]);
	nFeatID[0] = CreateFromToCurveByCopy(pMdl, arrSelEdge[0], arrSelEdge[1], itemCurve[0]);
	if (nFeatID[0] <= 0)
	{
		MessageBox(NULL, L"��һ����������ʧ�ܣ����ģ����������", L"��ʾ", MB_OK);
		return;
	}

	// �Ƿ���
	InvalidateDrawing();
	if (MessageBox(NULL, L"�Ƿ���", L"��ʾ", MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		int nfeatArray[] = {nFeatID[0]};
		ProFeatureDeleteOptions opt[] = {PRO_FEAT_DELETE_NO_OPTS};
		ProFeatureDelete(ProMdlToSolid(pMdl), nfeatArray, 1, opt, 1);
		nFeatID[0] = CreateFromToCurveByCopy(pMdl, arrSelEdge[1], arrSelEdge[0], itemCurve[0]);
		if (nFeatID[0] <= 0)
		{
			MessageBox(NULL, L"��һ����������ʧ�ܣ����ģ����������", L"��ʾ", MB_OK);
			return;
		}
		InvalidateDrawing();
	}

	// ��3����ѡȡ���ϵ�������
	ShowMessageTip(L"Tips.ѡȡͬһ�����ϵ������ߴ����ڶ�������...");
	vector<ProSelection> arrSelEdge2;
	if (!SelectObject(arrSelEdge2, "edge", 2))
	{
		return;
	}

	// ��4���������ڶ�������
	int nSelCount2 = (int)arrSelEdge2.size();
	if (nSelCount2 == 1)
		arrSelEdge2.push_back(arrSelEdge2[0]);
	nFeatID[1] = CreateFromToCurveByCopy(pMdl, arrSelEdge2[0], arrSelEdge2[1], itemCurve[1]);
	if (nFeatID[1] <= 0)
	{
		MessageBox(NULL, L"�ڶ�����������ʧ�ܣ����ģ����������", L"��ʾ", MB_OK);
		return;
	}

	// �Ƿ���
	InvalidateDrawing();
	if (MessageBox(NULL, L"�Ƿ���", L"��ʾ", MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		int nfeatArray[] = {nFeatID[1]};
		ProFeatureDeleteOptions opt[] = {PRO_FEAT_DELETE_NO_OPTS};
		ProFeatureDelete(ProMdlToSolid(pMdl), nfeatArray, 1, opt, 1);
		nFeatID[1] = CreateFromToCurveByCopy(pMdl, arrSelEdge2[1], arrSelEdge2[0], itemCurve[1]);
		if (nFeatID[1] <= 0)
		{
			MessageBox(NULL, L"�ڶ�����������ʧ�ܣ����ģ����������", L"��ʾ", MB_OK);
			return;
		}
		InvalidateDrawing();
	}

	// ��5���������߽�����
	ProSelection selCurve[2];
	ProSelectionAlloc(NULL, &itemCurve[0], &selCurve[0]);
	ProSelectionAlloc(NULL, &itemCurve[1], &selCurve[1]);
	ProModelitem itemQuilt;
	nFeatID[2] = CreateSurfByTwoCurve(pMdl, selCurve[0], selCurve[1], itemQuilt);
	if (nFeatID[2] > 0)
	{
		SetSurfColor(itemQuilt, 154, 205, 7);
		ProGroup group;
		ProLocalGroupCreate(ProMdlToSolid(pMdl), nFeatID, 3, L"������_����", &group);
	}
	else
	{
		MessageBox(NULL, L"����ʧ�ܣ����ģ����������", L"��ʾ", MB_OK);
	}

	//ExportElemTreeXML();

	/*ProChaincollUIControl ui[1]={PRO_CHAINCOLLUI_FROM_TO};
	ProCollection collect1, collect2;
	status = ProCrvcollectionAlloc(&collect1);
	status = ProCrvcollectionAlloc(&collect2);
	
	ProCurvesCollect(ui, 1, NULL, NULL, &collect1, NULL, NULL);
	ProCurvesCollect(ui, 1, NULL, NULL, &collect2, NULL, NULL);

	ProSelection* r_result_sellist1, *r_result_sellist2;
	int nNum1, nNum2;
	status = ProCrvcollectionRegenerate(collect1, &r_result_sellist1, &nNum1);
	status = ProCrvcollectionRegenerate(collect2, &r_result_sellist2, &nNum2);

	ProModelitem itemQuilt;
	CreateBlendSurfByChain(pMdl, r_result_sellist1[0], r_result_sellist2[0], itemQuilt);*/

	/*vector<ProSelection> arrSelEdges;
	ProSelection selSrf = NULL;
	if (SelectObject(arrSelEdges, "edge", 2))
	{
		ProCollection collect;
		status = ProCrvcollectionAlloc(&collect);
		ProCrvcollinstr instCurv;
		status = ProCrvcollinstrAlloc(PRO_CURVCOLL_FROM_TO_INSTR, &instCurv);

		for (int i=0; i<(int)arrSelEdges.size(); i++)
		{
			ProReference refCurve;
			status = ProSelectionToReference(arrSelEdges[i], &refCurve);
			status = ProCrvcollinstrReferenceAdd(instCurv, refCurve);
		}
		status = ProCrvcollinstrAttributeSet(instCurv, PRO_CURVCOLL_ALL);
		status = ProCrvcollectionInstructionAdd(collect, instCurv);*/
		
		//ProSelection* r_result_sellist;
		//int nNum;
		//ProCrvcollectionRegenerate(collect, &r_result_sellist, &nNum);

		//ProSelectionHighlight(selSrf, PRO_COLOR_WARNING);

		///*status = ProCollectionFree(&collect);
		//status = ProCrvcollinstrFree(instCurv);*/
	/*}*/

	InvalidateDrawing();
	ProMessageClear();
}

// ��ָ�����򴴽�
void OnCreateBySketchActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// ��鵱ǰģ�ͻ���
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"��ǰ����δ����ģ�ͣ�", L"��ʾ", MB_OK|MB_ICONWARNING);
		return;
	}

	// ѡȡ�ݻ�
	ShowMessageTip(L"Tips.ѡȡһ���ݻ�����...");
	vector<ProSelection> arrSelFeat;
	if (SelectObject(arrSelFeat, "feature"))
	{
		// ������������Ƿ�Ϊ�ݻ�
		// ...������

		ProModelitem itemQuilt;
		int nFeatID = CreateSurfByFillSketch(pMdl, arrSelFeat[0], itemQuilt);
		if (nFeatID > 0)
		{
			SetSurfColor(itemQuilt, 154, 205, 7);
			ShowMessageTip(L"Tips.�����洴���ɹ���");
			return;
		}
	}
	ShowMessageTip(L"Tips.�����洴��ʧ�ܣ�");
}

// ���߲ü�
void OnTrimByEdgeActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// ��1������鵱ǰģ�ͻ���
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"��ǰ����δ����ģ�ͣ�", L"��ʾ", MB_OK|MB_ICONWARNING);
		return;
	}
	// ��2����ѡ��Ҫ�ü�����
	ShowMessageTip(L"Tips.ѡ����Ҫ�ü�����...");
	vector<ProSelection> arrSelSrf;
	ProSelection selSrf = NULL;
	while (SelectObject(arrSelSrf, "dtmqlt"))
	{
		ProSelectionCopy(arrSelSrf[0], &selSrf);
		// ��3������ʱ����ѡ����棬ѡȡ��ɺ�ָ���ʾ
		ProModelitem itemQuilt;
		ProSelectionModelitemGet(arrSelSrf[0], &itemQuilt);
		ProModelitemHide(&itemQuilt);

		// ��4����ѡ�����ϵĻ�����ѡȡ����һ���ߣ�
		int nEdgeDir = PRO_EDGE_SAME_DIRECTION;
		ProSelection *pArrSelInit = NULL;
		ProArrayAlloc(0, sizeof(ProSelection), 1, (ProArray*)&pArrSelInit);
		vector<ProSelection> arrSelEdge;
		while (SelectObject(arrSelEdge, "sldedge"))
		{
			// ��5������ȡ�����ڻ���ȫ����
			vector<ProSelection> arrSelContourEdges;
			if (GetContourByEdge(arrSelEdge[0], arrSelContourEdges, nEdgeDir))
			{
				for (int i=0; i<arrSelContourEdges.size(); i++)
				{
					ProArrayObjectAdd((ProArray*)&pArrSelInit, 0, 1, &arrSelContourEdges[i]);
				}
				break;
			}
			else
			{
				int nMsgResult = MessageBox(NULL, L"ѡȡ�ı߲�������������ɱߣ��Ƿ��ֶ�ѡȡ��\n�ǣ��ֶ�ѡȡ��������ѡȡ; ȡ�����˳�", L"��ʾ", MB_YESNOCANCEL|MB_ICONWARNING);
				if (nMsgResult == IDYES)	// �ֶ�ѡ��
				{
					ProArrayObjectAdd((ProArray*)&pArrSelInit, 0, 1, &arrSelEdge[0]);
					break;
				}
				else if (nMsgResult == IDCANCEL)	// �˳�
				{
					ProModelitemUnhide(&itemQuilt);
					InvalidateDrawing();
					return;
				}
			}
		}
		// ��6�����Զ�ѡ�ж�Ӧ��������ȫ����
		arrSelEdge.clear();
		if (SelectObject(arrSelEdge, "edge", MAX_SELECTION, pArrSelInit))
		{
			// �ж���ķ���
			/*ProQuilt quilt;
			BOOL bSurfDirOut = FALSE;
			ProGeomitemToQuilt(&itemQuilt, &quilt);
			vector<ProSurface> arrSurfaces;
			ProQuiltSurfaceVisit(quilt, QuiltSurfacesGetAction, NULL, &arrSurfaces);
			ProGeomitemdata* psrfdata;
			ProSurfaceDataGet(arrSurfaces[0], &psrfdata);
			if (psrfdata->data.p_surface_data->orient == PRO_SURF_ORIENT_OUT)
				bSurfDirOut = TRUE;
			ProGeomitemdataFree(&psrfdata);*/
			
			// ��7�����޼�����
			int nFeatID = TrimQuiltByEdgeGroup(pMdl, selSrf, arrSelEdge, nEdgeDir);
			ProModelitemUnhide(&itemQuilt);
			SAFE_DELETE_PROARRAY(pArrSelInit);
			if (nFeatID > 0)
			{
				// ��8������ʾ�Ƿ��������޼�
				CDlgConfirm dlg;
				while (1)
				{
					int nResult = (int)dlg.DoModal();
					if (nResult == IDOK)
					{
						break;
					}
					else if (nResult == IDRETRY) 
					{
						// �޸Ĳü�����
						ProFeature feat;
						feat.id = nFeatID;
						feat.type = PRO_FEATURE;
						feat.owner = pMdl;
						ReverseTrimDirection(feat);
					}
					else
					{
						InvalidateDrawing();
						ProMessageClear();
						return;
					}
				}
			}
			else if (MessageBox(NULL, L"�ü�ʧ�ܣ����ģ�����������Ƿ������", L"��ʾ", MB_YESNO) == IDNO)
			{
				break;
			}
		}
		else
		{
			// �ָ���ʾ
			ProModelitemUnhide(&itemQuilt);
			break;
		}
	}
	
	InvalidateDrawing();
	ProMessageClear();
}

// �ݻ�ü�
void OnTrimBySketchActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	ProError status;
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"��ǰ����δ����ģ�ͣ�", L"��ʾ", MB_OK|MB_ICONWARNING);
		return;
	}
	
	// ѡ��ݻ�
	vector<ProSelection> arrSel;
	ShowMessageTip(L"Tips.ѡ��ݻ�����...");
	while (SelectObject(arrSel, "feature"))
	{
		// ����Ƿ�Ϊ�ݻ�����
		ProFeature featSketch;
		status = ProSelectionModelitemGet(arrSel[0], &featSketch);
		ProFeattype featType;
		status = ProFeatureTypeGet(&featSketch, &featType);
		if (featType == PRO_FEAT_CURVE)
		{
			ProAsmcomppath acomppath;
			ProSelectionAsmcomppathGet(arrSel[0], &acomppath);

			ProSelection selQuilt;	// ���ü���ƽ��
			BOOL bGetQuilt = FALSE;

			// ��ȡ�ݻ��������ڵ�ƽ��
			ProElement elemTreeCurve = NULL;
			ProFeatureElemtreeExtract(&featSketch, NULL, PRO_FEAT_EXTRACT_NO_OPTS, &elemTreeCurve);
			ProElempathItem pathItemType[1];
			pathItemType[0].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
			pathItemType[0].path_item.elem_id = PRO_E_CURVE_TYPE;
			ProElempath elemPathCurveType;
			ProElempathAlloc(&elemPathCurveType);
			ProElempathDataSet(elemPathCurveType, pathItemType, 1);
			ProElement elemType;
			status = ProElemtreeElementGet(elemTreeCurve, elemPathCurveType, &elemType);
			if (status == PRO_TK_NO_ERROR)
			{
				ProValue valType;
				status = ProElementValueGet(elemType, &valType);
				ProValueData valdataType;
				status = ProValueDataGet(valType, &valdataType);
				if (valdataType.v.i == PRO_CURVE_TYPE_SKETCHED)
				{
					// ȷ��Ϊ�ݻ����ߣ�������ȡ�ݻ�ƽ��
					ProElempathItem pathItemSketchPlane[3];
					pathItemSketchPlane[0].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
					pathItemSketchPlane[0].path_item.elem_id = PRO_E_STD_SECTION;
					pathItemSketchPlane[1].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
					pathItemSketchPlane[1].path_item.elem_id = PRO_E_STD_SEC_SETUP_PLANE;
					pathItemSketchPlane[2].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
					pathItemSketchPlane[2].path_item.elem_id = PRO_E_STD_SEC_PLANE;
					ProElempath elemPathPlane;
					ProElempathAlloc(&elemPathPlane);
					ProElempathDataSet(elemPathPlane, pathItemSketchPlane, 3);
					ProElement elemPlane;
					status = ProElemtreeElementGet(elemTreeCurve, elemPathPlane, &elemPlane);
					if (status == PRO_TK_NO_ERROR)
					{
						ProValue valPlane;
						status = ProElementValueGet(elemPlane, &valPlane);
						ProValueData valdataPlane;
						status = ProValueDataGet(valPlane, &valdataPlane);
						ProGeomitem itemSurf;
						ProSelectionModelitemGet(valdataPlane.v.r, &itemSurf);
						ProSurface surf;
						ProGeomitemToSurface(&itemSurf, &surf);

						// ��ȡ����������
						ProQuilt quilt;
						status = ProSurfaceQuiltGet(ProMdlToSolid(pMdl), surf, &quilt);
						if (status == PRO_TK_NO_ERROR)
						{
							ProGeomitem itemQuilt;
							status = ProQuiltToGeomitem(ProMdlToSolid(pMdl), quilt, &itemQuilt);
							status = ProSelectionAlloc(&acomppath, &itemQuilt, &selQuilt);
							bGetQuilt = TRUE;
						}
					}
				}
			}
			
			if (!bGetQuilt)
			{
				ShowMessageTip(L"Tips.�ݻ�ķ�����ǿɲü��棬���ֶ�ѡ����ü�����...");
				// �ֶ�ѡȡ���ü�����
				vector<ProSelection> arrSelSrf;
				if (SelectObject(arrSelSrf, "dtmqlt"))
				{
					ProSelectionCopy(arrSelSrf[0], &selQuilt);
				}
				else
				{
					break;
				}
			}
	
			// ������������ȡ�����е�ȫ����������
			vector<ProGeomitem> arrItemCurves;
			status = ProFeatureGeomitemVisit(&featSketch, PRO_CURVE, FeatureGeomsGetAction, NULL, &arrItemCurves);
			vector<int> arrFeatID;
			for (int i=0; i<arrItemCurves.size(); i++)
			{
				ProCurve curve;
				status = ProGeomitemToCurve(&arrItemCurves[i], &curve);
				// ����Ƿ�Ϊ��������
				ProEnttype curveType;
				status = ProCurveTypeGet(curve, &curveType);
				if (curveType == PRO_ENT_CMP_CRV)
				{
					// ���вü�
					ProSelection selCurve;
					status = ProSelectionAlloc(&acomppath, &arrItemCurves[i], &selCurve);
					int nFeatID = TrimQuiltByCurve(pMdl, selQuilt, selCurve, PRO_EDGE_SAME_DIRECTION);
					if (nFeatID > 0)
					{
						// �����ɹ�����¼����ID
						arrFeatID.push_back(nFeatID);

						// ��8������ʾ�Ƿ��������޼�
						CDlgConfirm dlg;
						while (1)
						{
							int nResult = (int)dlg.DoModal();
							if (nResult == IDOK)
							{
								break;
							}
							else if (nResult == IDRETRY) 
							{
								// �޸Ĳü�����
								ProFeature feat;
								feat.id = nFeatID;
								feat.type = PRO_FEATURE;
								feat.owner = pMdl;
								ReverseTrimDirection(feat);
							}
							else
							{
								InvalidateDrawing();
								ProMessageClear();
								return;
							}
						}
					}
				}
			}
			// �ϲ�����
			CreateFeatGroup(pMdl, arrFeatID, L"������_�ü�");
			break;
		}
		else
		{
			// 
			MessageBox(NULL, L"��ѡ�����ǲݻ�������������ѡ��", L"��ʾ", MB_OK);
		}
	}

	InvalidateDrawing();
	ProMessageClear();
}

// ƴ�Ӱ�����
void OnMergeManualActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// ��1������鵱ǰģ�ͻ���
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"��ǰ����δ����ģ�ͣ�", L"��ʾ", MB_OK|MB_ICONWARNING);
		return;
	}
	// ��2����ѡ��Ҫƴ�ӵ���
	ShowMessageTip(L"Tips.ѡ����Ҫƴ�ӵİ����棬�ɶ�ѡ...");
	vector<ProSelection> arrSelQuilts;
	if (SelectObject(arrSelQuilts, "dtmqlt", MAX_SELECTION))
	{
		// ��3����ִ��ƴ��
		ProFeature featMerge;
		int nFeatID = MergeQuilts(pMdl, arrSelQuilts, PRO_SRF_MRG_INTSCT, featMerge);
		if (nFeatID < 0)
		{
			MessageBox(NULL, L"ƴ��ʧ�ܣ����ģ����������", L"��ʾ", MB_OK);
		}
		else if (arrSelQuilts.size() == 2)
		{
			// �޸ĺϲ��ķ���
			ProFeature featMerge;
			featMerge.id = nFeatID;
			featMerge.type = PRO_FEATURE;
			featMerge.owner = pMdl;
			CDlgMergeDir dlgDir;
			dlgDir.IntiFeature(featMerge);
			dlgDir.DoModal();
		}
	}

	InvalidateDrawing();
	ProMessageClear();
}

// ƴ�ӣ�����ѡȡ��
void OnMergeAutoActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"��ǰ����δ����ģ�ͣ�", L"��ʾ", MB_OK|MB_ICONWARNING);
		return;
	}
	ProError status;

	// ѡȡһ��������
	ShowMessageTip(L"Tips.ѡ����Ҫƴ�ӵİ�����...");
	vector<ProSelection> arrSelQuilts;
	if (SelectObject(arrSelQuilts, "dtmqlt"))
	{
		ProModelitem itemQuiltStart;
		status = ProSelectionModelitemGet(arrSelQuilts[0], &itemQuiltStart);
		ProQuilt quiltStart;
		status = ProGeomitemToQuilt(&itemQuiltStart, &quiltStart);

		vector<ProQuilt> arrQuiltMerge;
		arrQuiltMerge.push_back(quiltStart);

		// ��ȡȫ��Quilt
		vector<ProQuilt> arrQuilt;
		status = ProSolidQuiltVisit(ProMdlToSolid(pMdl), SolidQuiltsGetAction, NULL, &arrQuilt);
		ProQuilt quiltCurr;
		for (int i=0; i<arrQuiltMerge.size(); i++)
		{
			quiltCurr = arrQuiltMerge[i];
			for (int j=0; j<arrQuilt.size(); )
			{
				if (quiltCurr == arrQuilt[j])
				{
					// �޳�����
					arrQuilt.erase(arrQuilt.begin()+j);
					continue;
				}
				if (CheckTwoQuiltNeighbor(pMdl, quiltCurr, arrQuilt[j]))
				{
					arrQuiltMerge.push_back(arrQuilt[j]);
					arrQuilt.erase(arrQuilt.begin()+j);
					continue;
				}
				j++;
			}
		}
		
		vector<ProSelection> arrSelMergeQuilt;
		for (int i=0; i<arrQuiltMerge.size(); i++)
		{
			ProModelitem itemQuilt;
			ProQuiltToGeomitem(ProMdlToSolid(pMdl), arrQuiltMerge[i], &itemQuilt);
			ProSelection selQuilt = NULL;
			ProSelectionAlloc(NULL, &itemQuilt, &selQuilt);
			arrSelMergeQuilt.push_back(selQuilt);
		}
		
		ProFeature featMerge;
		MergeQuilts(pMdl, arrSelMergeQuilt, PRO_SRF_MRG_INTSCT, featMerge);
	}

	InvalidateDrawing();
	ProMessageClear();
}

// �������
void OnOffsetGeneralActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// ��1������鵱ǰģ�ͻ���
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"��ǰ����δ����ģ�ͣ�", L"��ʾ", MB_OK|MB_ICONWARNING);
		return;
	}

	// ��2����ѡ��Ҫƫ�Ƶ���
	vector<ProSelection> arrSelSrf;
	ShowMessageTip(L"Tips.ѡ��Ҫƫ�Ƶ���...");
	if (SelectObject(arrSelSrf, "dtmqlt"))
	{
		// ��3��������ƫ��ֵ
		double dOffset;
		ShowMessageTip(L"����ƫ����������Ϊ0��������Ϊ��ֵ��");
		if (ProMessageDoubleRead(NULL, &dOffset) == PRO_TK_NO_ERROR)
		{
			// ��4��������ƫ������
			ProModelitem itemQuilt;
			int nOffsetID = OffsetQuilt(pMdl, arrSelSrf[0], dOffset, itemQuilt);
			if (nOffsetID > 0)
			{
				SetSurfColor(itemQuilt, 255, 215, 0);
				// ��5��������ԭ������
				ProModelitem itemQuiltSel;
				ProSelectionModelitemGet(arrSelSrf[0], &itemQuiltSel);
				ProModelitemHide(&itemQuiltSel);
			}
			else
			{
				MessageBox(NULL, L"����ʧ�ܣ����ģ����������", L"��ʾ", MB_OK);
			}
		}
	}
	InvalidateDrawing();
	ProMessageClear();
}

// �������
void OnOffsetSingleActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"��ǰ����δ����ģ�ͣ�", L"��ʾ", MB_OK|MB_ICONWARNING);
		return;
	}

	// ѡ����
	vector<ProSelection> arrSelQlt;
	ShowMessageTip(L"Tips.ѡ��Ҫƫ�Ƶ���...");
	if (SelectObject(arrSelQlt, "surface"))
	{
		// ����ƫ��ֵ
		double dOffset;
		ShowMessageTip(L"����ƫ����������Ϊ0��������Ϊ��ֵ��");
		if (ProMessageDoubleRead(NULL, &dOffset) == PRO_TK_NO_ERROR)
		{
			// �ж���ķ���
			ProModelitem itemSurf;
			ProSelectionModelitemGet(arrSelQlt[0], &itemSurf);
			ProSurface surf;
			ProGeomitemToSurface(&itemSurf, &surf);
			ProGeomitemdata* psrfdata;
			ProSurfaceDataGet(surf, &psrfdata);
			if (psrfdata->data.p_surface_data->orient == PRO_SURF_ORIENT_IN)
				dOffset = dOffset*(-1);
			ProGeomitemdataFree(&psrfdata);
			// ����ƫ������
			int nOffsetID = OffsetSurfaceWithSide(pMdl, arrSelQlt[0], dOffset);
			if (nOffsetID > 0)
			{
				// �޸�����ɫ
				ProFeature feat;
				feat.id = nOffsetID;
				feat.type = PRO_FEATURE;
				feat.owner = pMdl;
				vector<ProModelitem> arrQuilts;
				ProFeatureGeomitemVisit(&feat, PRO_QUILT, FeatureGeomsGetAction, NULL, &arrQuilts);
				for (int i=0; i<arrQuilts.size(); i++)
					SetSurfColor(arrQuilts[i], 255, 215, 0);
			}
		}
	}
	InvalidateDrawing();
	ProMessageClear();
}

// ������STL
void OnExportSTLActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// ��1������鵱ǰģ�ͻ���
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"��ǰ����δ����ģ�ͣ�", L"��ʾ", MB_OK|MB_ICONWARNING);
		return;
	}

	// ѡȡ����
	vector<ProSelection> arrSelQlt;
	if (SelectObject(arrSelQlt, "dtmqlt"))
	{
		ProName mdlName;
		ProMdlNameGet(pMdl, mdlName);
		CString strMdlName(mdlName);
		ProFileName filename;
		wcsncpy_s(filename,  PRO_FILE_NAME_SIZE, strMdlName, _TRUNCATE);

		// ѡ�񱣴�λ��
		ProPath stlpath;
		if (ProFileSave(NULL, L"*.stl", NULL, NULL, NULL, filename, stlpath) == PRO_TK_NO_ERROR)
		{
			CString strStlPath(stlpath);
			ProGeomitem qltitem;
			ProSelectionModelitemGet(arrSelQlt[0], &qltitem);
			ProQuilt quilt;
			ProGeomitemToQuilt(&qltitem, &quilt);
			vector<ProSurface> arrSurfs;
			ProQuiltSurfaceVisit(quilt, QuiltSurfacesGetAction, NULL, &arrSurfs);
			vector<ProGeomitem> arrSurfsItems;
			for (int i=0; i<(int)arrSurfs.size(); i++)
			{
				ProGeomitem srfitem;
				ProSurfaceToGeomitem(ProMdlToSolid(pMdl), arrSurfs[i], &srfitem);
				arrSurfsItems.push_back(srfitem);
			}
			if (ExportStl(arrSurfsItems, strStlPath, 0.5, 1.0))
			{
				MessageBox(NULL, L"����stl�ɹ�", L"��ʾ", MB_OK);
			}
			else
				MessageBox(NULL, L"����stlʧ��", L"��ʾ", MB_OK|MB_ICONWARNING);
		}
	}

	InvalidateDrawing();
	ProMessageClear();
}

// ������STP
void OnExportSTPActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// ��鵱ǰģ�ͻ���
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"��ǰ����δ����ģ�ͣ�", L"��ʾ", MB_OK|MB_ICONWARNING);
		return;
	}

	InvalidateDrawing();
	ProMessageClear();
}

// �����г�
void OnBottomCutActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);
	ProError status;

	// ��1������鵱ǰģ�ͻ���
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"��ǰ����δ����ģ�ͣ�", L"��ʾ", MB_OK|MB_ICONWARNING);
		return;
	}
	// ��2����ѡ��ο�
	ShowMessageTip(L"Tips.ѡ��ƴ�Ӻ�İ���������г�...");
	vector<ProSelection> arrSelQlt;
	ProSelection selQlt = NULL;
	if (SelectObject(arrSelQlt, "dtmqlt"))
	{
		status = ProSelectionCopy(arrSelQlt[0], &selQlt);

		// ��3������ʱ����ѡ����棬ѡȡ��ɺ�ָ���ʾ
		ProModelitem itemQuilt;
		status = ProSelectionModelitemGet(arrSelQlt[0], &itemQuilt);
		status = ProModelitemHide(&itemQuilt);
		
		// ��4����ѡ�����
		ShowMessageTip(L"Tips.ѡ����Ҫƽ��ĵ���...");
		vector<ProSelection> arrSelSrf;
		if (SelectObject(arrSelSrf, "surface"))
		{
			status = ProModelitemUnhide(&itemQuilt);

			// ��5�����Ե���Ϊ���մ�����׼��
			ProModelitem itemPlane;
			int nDatumFeatID = CreateDatumPlane(pMdl, arrSelSrf[0], itemPlane);
			if (nDatumFeatID > 0)
			{
				ProSelection selRef;
				status = ProSelectionAlloc(NULL, &itemPlane, &selRef);
				int nFeatID = TrimSurfaceBySurf(pMdl, selQlt, selRef, 1);
				if (nFeatID > 0)
				{
					// ��8������ʾ�Ƿ��������޼�
					CDlgConfirm dlg;
					while (1)
					{
						int nResult = (int)dlg.DoModal();
						if (nResult == IDRETRY) 
						{
							// �޸Ĳü�����
							ProFeature feat;
							feat.id = nFeatID;
							feat.type = PRO_FEATURE;
							feat.owner = pMdl;
							ReverseTrimDirection(feat);
						}
						else
						{
							break;
						}
					}
				}
				else
				{
					MessageBox(NULL, L"�г�ʧ�ܣ����ģ����������", L"��ʾ", MB_OK);
				}
			}
		}
		else
			status = ProModelitemUnhide(&itemQuilt);
	}
	InvalidateDrawing();
	ProMessageClear();
}

// ��������
void OnBottomExpandActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);
	ProError status;

	// ��1������鵱ǰģ�ͻ���
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"��ǰ����δ����ģ�ͣ�", L"��ʾ", MB_OK|MB_ICONWARNING);
		return;
	}

	// ��2����ѡ������
	vector<ProSelection> arrSelQlt;
	ProSelection selQlt;
	if (SelectObject(arrSelQlt, "dtmqlt"))
	{
		status = ProSelectionCopy(arrSelQlt[0], &selQlt);
		
		// ��3����ѡ�����
		vector<ProSelection> arrSelBottom;
		if (SelectObject(arrSelBottom, "surface"))
		{
			if (ExtendQuiltToSurf(pMdl, selQlt, arrSelBottom[0]) < 0)
			{
				MessageBox(NULL, L"����ʧ�ܣ����ģ����������", L"��ʾ", MB_OK);
			}
		}
	}
	InvalidateDrawing();
	ProMessageClear();
}

// ���ɰ�����
void OnCreateBodyActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// ��鵱ǰģ�ͻ���
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"��ǰ����δ����ģ�ͣ�", L"��ʾ", MB_OK|MB_ICONWARNING);
		return;
	}

	ProMdlType typeMdl;
	ProMdlTypeGet(pMdl, &typeMdl);
	if (typeMdl != PRO_MDL_ASSEMBLY)
	{
		MessageBox(NULL, L"�ù��ܽ���װ�价������Ч��", L"��ʾ", MB_OK|MB_ICONWARNING);
		return;
	}

	//ExportElemTreeXML();

	// ѡ��һ������
	vector<ProSelection> arrSelQlt;
	ShowMessageTip(L"Tips.ѡ��Ҫ�Ӻ�İ�����...");
	if (SelectObject(arrSelQlt, "dtmqlt"))
	{
		// ��������Ӻ�ֵ
		double dThickValue;
		ShowMessageTip(L"ָ�����ֵ������Ϊ0����ֵ��ʾ���ڼӺ�");
		if (ProMessageDoubleRead(NULL, &dThickValue) == PRO_TK_NO_ERROR)
		{
			// ������������
			int nPubFeatID = CreatePublishGeom(pMdl, arrSelQlt[0]);

			ProName nameAsm;
			ProMdlNameGet(pMdl, nameAsm);
			CString strNamePart = nameAsm;
			ProName namePart;
			for (int i=101; ;i++)
			{
				CString strTemp;
				strTemp.Format(L"%s_%d", nameAsm, i);
				wcsncpy_s(namePart, PRO_NAME_SIZE, strTemp, _TRUNCATE);
				ProMdl pMdlTemp;
				if (ProMdlInit(namePart, PRO_MDL_PART, &pMdlTemp) == PRO_TK_E_NOT_FOUND)
					break;
			}

			// �½�һ�����
			ProAsmcomp compPart;
			ProAsmcompCreateCopy(ProMdlToAssembly(pMdl), namePart, PRO_MDL_PART, NULL, PRO_B_FALSE, &compPart);
			//ProSolidRegenerate(pMdl);
			ProMdl pMdlPart;
			ProAsmcompMdlGet(&compPart, &pMdlPart);

			ProModelitem itemAsm;
			ProMdlToModelitem(pMdl, &itemAsm);
			ProSelection selAsm;
			ProSelectionAlloc(NULL, &itemAsm, &selAsm);

			ProFeature featPub;
			featPub.id = nPubFeatID;
			featPub.owner = pMdl;
			featPub.type = PRO_FEATURE;
			ProSelection selPubFeat;
			ProSelectionAlloc(NULL, &featPub, &selPubFeat);

			// �������Ƽ���
			ProModelitem itemCopyQuilt;
			CreateCopyGeom(pMdlPart, selAsm, selPubFeat, itemCopyQuilt);
			ProSelection selCopyQuilt;
			ProSelectionAlloc(NULL, &itemCopyQuilt, &selCopyQuilt);

			// ������ʾ���Ƿ�Ҫ�Ե��������ú��
			if (MessageBox(NULL, L"�Ƿ�Ҫ�Ե��������ú��", L"��ʾ", MB_YESNO|MB_ICONQUESTION) == IDYES)
			{
				ProMdlDisplay(pMdlPart);
				int nWndIDPart;
				ProMdlWindowGet(pMdlPart, &nWndIDPart);
				ProWindowActivate(nWndIDPart);

				// �����Ի���
				if (g_pDlgThick != NULL)
				{
					if (g_pDlgThick->GetSafeHwnd() != NULL)
						g_pDlgThick->DestroyWindow();

					SAFE_DELETE_POINTER(g_pDlgThick);
				}
				g_pDlgThick = new CDlgSetThick();
				g_pDlgThick->InitSurfData(pMdl, selCopyQuilt, dThickValue);
				g_pDlgThick->Create(IDD_DLG_SETTHICK);
				g_pDlgThick->ShowWindow(SW_SHOW);
			}
			else
			{
				ThickQuilt(pMdlPart, selCopyQuilt, dThickValue);
			}
		}
	}

	InvalidateDrawing();
	ProMessageClear();
}

// �������ӵ���Ӧ����
void ConServerActFn()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();

	// ������������ӣ�У���Ƿ���ȷ���ɡ�
	CDlgConServer serverDlg;
	SClientInfo clientInfo;
	LoadServerData(clientInfo);
	serverDlg.m_clientInfo = clientInfo;
	if (serverDlg.DoModal() == IDCANCEL)
	{
		ProWindowRepaint(PRO_VALUE_UNUSED);
		return;
	}

	ProWindowRepaint(PRO_VALUE_UNUSED);

	// �������÷���������Ϣ
	ConSmartServer(FALSE, serverDlg.m_clientInfo);
}

// ����Ͽ�����Ӧ����
void DisConServerActFn()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();

	// �Ͽ�������
	DisConSmartServer(FALSE);
}

// ����
void OnHelp()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();

	CString strMainModulePath;
	GetMainModulePath(strMainModulePath, (HMODULE)theApp.m_hInstance);
	if (strMainModulePath.IsEmpty())
		return;

	CString strHelpFileName = strMainModulePath + L"\\Help\\Smart3DCoating.pdf";
	ShellExecute(NULL, TEXT("open"), strHelpFileName, NULL, NULL, SW_SHOW);
}

// ����...
void OnAbout()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();

	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
	ProWindowRepaint(PRO_VALUE_UNUSED);
}
