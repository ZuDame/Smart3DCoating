//===================================================================================================
// Summary:
//		Smart3DCoating的菜单以及命令响应函数
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
	// 存在单机版授权，不需要连接服务器
	if (GetSingleLicense())
		return ACCESS_UNAVAILABLE;

	return ACCESS_AVAILABLE;
}

uiCmdAccessState DisConServerAccessDefault(uiCmdAccessMode access_mode)
{
	access_mode;
	// 存在单机版授权，不需要连接服务器
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

// 菜单不可见
uiCmdAccessState UserNoShow(uiCmdAccessMode access_mode)
{
	UNUSED_ALWAYS(access_mode);
	return ACCESS_INVISIBLE;
}

// 添加菜单项
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

// 初始化菜单
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

	// 许可证服务器
	char szLicenseServerChildMenuName[] = "CD_LicServerMenuName";
	ProMenubarmenuMenuAdd(szSmart3DCoatingMenuName, szLicenseServerChildMenuName, "CD_LicenseServer", NULL, PRO_B_TRUE, Msg);
	{
		// 连接
		AddMenuItem(szLicenseServerChildMenuName, "CD_ConnectServer", ConServerActFn, ConServerAccessDefault, Msg);
		// 断开
		AddMenuItem(szLicenseServerChildMenuName, "CD_DisconnectServer", DisConServerActFn, DisConServerAccessDefault, Msg);
	}

	// 帮助
	AddMenuItem(szSmart3DCoatingMenuName, "CD_Help", OnHelp, MainAccessAvailable, Msg);
	// 关于
	AddMenuItem(szSmart3DCoatingMenuName, "CD_About", OnAbout, MainAccessAvailable, Msg);

	// 初始化快捷菜单
	int menuid, menuid_create, menuid_edit, menuid_export;
	ProMenufileName Mnu = "ShortCut.mnu";
	ProMenufileName MnuCreate = "ShortCut_Create.mnu";
	ProMenufileName MnuEdit = "ShortCut_Edit.mnu";
	ProMenufileName MnuExport = "ShortCut_Export.mnu";

	// 注册菜单文件
	char szMnuName[] = "Smart3DCoating_ShortCut";
	char szMnuNameCreate[] = "ShortCut_Create";
	char szMnuNameEdit[] = "ShortCut_Edit";
	char szMnuNameExport[] = "ShortCut_Export";

	ProMenuFileRegister(szMnuName, Mnu, &menuid);
	ProMenuFileRegister(szMnuNameCreate, MnuCreate, &menuid_create);
	ProMenuFileRegister(szMnuNameEdit, MnuEdit, &menuid_edit);
	ProMenuFileRegister(szMnuNameExport, MnuExport, &menuid_export);

	// 设置菜单动作函数
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

// 释出所有对话框
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

// 取消当前操作
void CancelOperator()
{
	ProWindowRepaint(PRO_VALUE_UNUSED);
}

// 取消选择状态
void CancelSelectDialog()
{
	if (g_bSelectDlgOpen)
		ProMacroLoad(L"~ Activate `GET SELECT` `quit_sel`;");
}

// 删除无效操作指针
void DeleteInvalidOperator()
{

}

// 释放操作对象
void DeleteOperator()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProWindowRepaint(PRO_VALUE_UNUSED);
	//DestroyAllDialog();
	//ActivateCurrentPreWindow();
}

//===================================================================================================

// 显示快捷菜单
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

// 退出快捷菜单
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

// 加载包覆对象
void OnLoadSolidActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// 选择一个本地的待包覆模型
	ProPath pathSelModel;
	if (ProFileOpen(L"选择待包覆模型", L"*.prt,*.asm,*.stp,*.stl", NULL, NULL, NULL, NULL, pathSelModel) == PRO_TK_NO_ERROR)
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


		// 创建一个装配体
		CString strSolidName;
		strSolidName = nameModel;

		strSolidName.Format(L"%s_3DCoat", strSolidName);

		ProName nameSolid;
		wcsncpy_s(nameSolid, PRO_NAME_SIZE, strSolidName, _TRUNCATE);

		ProSolid solid;
		ProSolidCreate(nameSolid, PRO_ASSEMBLY, &solid);

		// 创建默认坐标系
		ProError status;
		ProElement elemTree = NULL;
		ProElementAlloc(PRO_E_FEATURE_TREE, &elemTree);
		{
			// 特征类型PRO_E_FEATURE_TYPE
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


		// 将选择的待装配模型装配到装配体下
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

// 表面清理
void OnCleanActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// 第1步：检查当前模型环境
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"当前环境未发现模型！", L"提示", MB_OK|MB_ICONWARNING);
		return;
	}

	// 第2步：选择参考
	ShowMessageTip(L"Tips.选择几何平面进行清理，可多选...");
	vector<ProSelection> arrSelSrf;
	vector<ProModelitem> arrItemSrf;
	while (SelectObject(arrSelSrf, "surface", MAX_SELECTION))
	{
		BOOL bError = FALSE;
		// 第3步：创建基准面
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
		// 第4步：创建实体化特征
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
				
		// 第5步：显示处理结果
		if (bError && MessageBox(NULL, L"部分特征创建失败，详见模型特征树。是否继续？", L"提示", MB_YESNO) == IDNO)
		{
			break;
		}
	}
	InvalidateDrawing();
	ProMessageClear();
}

// 快速创建包覆面
void OnRapidCreatActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// 第1步：检查当前模型环境
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"当前环境未发现模型！", L"提示", MB_OK|MB_ICONWARNING);
		return;
	}

	// 第2步：选择参考
	ShowMessageTip(L"Tips.选择几何面进行创建，可多选...");
	vector<ProSelection> arrSelSrf;
	while (SelectObject(arrSelSrf, "surface", MAX_SELECTION))
	{
		// 第3步：创建面
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
		// 第4步：显示处理结果
		if (bError && MessageBox(NULL, L"部分特征创建失败，详见模型特征树。是否继续？", L"提示", MB_YESNO) == IDNO)
		{
			break;
		}
	}
	InvalidateDrawing();
	ProMessageClear();
}

// 从边界创建
void OnCreateByEdgeActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);
	
	// 第1步：检查当前模型环境
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"当前环境未发现模型！", L"提示", MB_OK|MB_ICONWARNING);
		return;
	}
	// 第2步：选择参考
	ShowMessageTip(L"Tips.选择两条边界进行创建...");
	vector<ProSelection> arrSelEdge;
	while (SelectObject(arrSelEdge, "edge", 2))
	{
		int nSelCount = (int)arrSelEdge.size();
		if (nSelCount < 2)
			break;
		// 第3步：创建面
		ProModelitem itemQlt;
		if (CreateSurfByTwoEdge(pMdl, arrSelEdge[0], arrSelEdge[1], itemQlt) > 0)
		{
			SetSurfColor(itemQlt, 154, 205, 7);
		}
		else if (MessageBox(NULL, L"部分特征创建失败，详见模型特征树。是否继续？", L"提示", MB_YESNO) == IDNO)
		{
			break;
		}
	}
	
	InvalidateDrawing();
	ProMessageClear();
}

// 从边界(链)创建
void OnCreateByChainActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// 检查当前模型环境
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"当前环境未发现模型！", L"提示", MB_OK|MB_ICONWARNING);
		return;
	}

	int nFeatID[3]={0};
	ProModelitem itemCurve[2];
	
	// 第1步：选取第一条边链的两条边
	ShowMessageTip(L"Tips.选取同一个面上的两条边创建第一条边链...");
	vector<ProSelection> arrSelEdge;
	if (!SelectObject(arrSelEdge, "edge", 2))
	{
		return;
	}
	
	// 第2步：创建第一条边链
	int nSelCount = (int)arrSelEdge.size();
	if (nSelCount == 1)
		arrSelEdge.push_back(arrSelEdge[0]);
	nFeatID[0] = CreateFromToCurveByCopy(pMdl, arrSelEdge[0], arrSelEdge[1], itemCurve[0]);
	if (nFeatID[0] <= 0)
	{
		MessageBox(NULL, L"第一条边链创建失败，详见模型特征树！", L"提示", MB_OK);
		return;
	}

	// 是否反向
	InvalidateDrawing();
	if (MessageBox(NULL, L"是否反向？", L"提示", MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		int nfeatArray[] = {nFeatID[0]};
		ProFeatureDeleteOptions opt[] = {PRO_FEAT_DELETE_NO_OPTS};
		ProFeatureDelete(ProMdlToSolid(pMdl), nfeatArray, 1, opt, 1);
		nFeatID[0] = CreateFromToCurveByCopy(pMdl, arrSelEdge[1], arrSelEdge[0], itemCurve[0]);
		if (nFeatID[0] <= 0)
		{
			MessageBox(NULL, L"第一条边链创建失败，详见模型特征树！", L"提示", MB_OK);
			return;
		}
		InvalidateDrawing();
	}

	// 第3步：选取面上的两条边
	ShowMessageTip(L"Tips.选取同一个面上的两条边创建第二条边链...");
	vector<ProSelection> arrSelEdge2;
	if (!SelectObject(arrSelEdge2, "edge", 2))
	{
		return;
	}

	// 第4步：创建第二条边链
	int nSelCount2 = (int)arrSelEdge2.size();
	if (nSelCount2 == 1)
		arrSelEdge2.push_back(arrSelEdge2[0]);
	nFeatID[1] = CreateFromToCurveByCopy(pMdl, arrSelEdge2[0], arrSelEdge2[1], itemCurve[1]);
	if (nFeatID[1] <= 0)
	{
		MessageBox(NULL, L"第二条边链创建失败，详见模型特征树！", L"提示", MB_OK);
		return;
	}

	// 是否反向
	InvalidateDrawing();
	if (MessageBox(NULL, L"是否反向？", L"提示", MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		int nfeatArray[] = {nFeatID[1]};
		ProFeatureDeleteOptions opt[] = {PRO_FEAT_DELETE_NO_OPTS};
		ProFeatureDelete(ProMdlToSolid(pMdl), nfeatArray, 1, opt, 1);
		nFeatID[1] = CreateFromToCurveByCopy(pMdl, arrSelEdge2[1], arrSelEdge2[0], itemCurve[1]);
		if (nFeatID[1] <= 0)
		{
			MessageBox(NULL, L"第二条边链创建失败，详见模型特征树！", L"提示", MB_OK);
			return;
		}
		InvalidateDrawing();
	}

	// 第5步：创建边界混合面
	ProSelection selCurve[2];
	ProSelectionAlloc(NULL, &itemCurve[0], &selCurve[0]);
	ProSelectionAlloc(NULL, &itemCurve[1], &selCurve[1]);
	ProModelitem itemQuilt;
	nFeatID[2] = CreateSurfByTwoCurve(pMdl, selCurve[0], selCurve[1], itemQuilt);
	if (nFeatID[2] > 0)
	{
		SetSurfColor(itemQuilt, 154, 205, 7);
		ProGroup group;
		ProLocalGroupCreate(ProMdlToSolid(pMdl), nFeatID, 3, L"包覆面_创建", &group);
	}
	else
	{
		MessageBox(NULL, L"创建失败，详见模型特征树！", L"提示", MB_OK);
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

// 从指定区域创建
void OnCreateBySketchActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// 检查当前模型环境
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"当前环境未发现模型！", L"提示", MB_OK|MB_ICONWARNING);
		return;
	}

	// 选取草绘
	ShowMessageTip(L"Tips.选取一个草绘曲线...");
	vector<ProSelection> arrSelFeat;
	if (SelectObject(arrSelFeat, "feature"))
	{
		// 检查特征类型是否为草绘
		// ...待补充

		ProModelitem itemQuilt;
		int nFeatID = CreateSurfByFillSketch(pMdl, arrSelFeat[0], itemQuilt);
		if (nFeatID > 0)
		{
			SetSurfColor(itemQuilt, 154, 205, 7);
			ShowMessageTip(L"Tips.包覆面创建成功！");
			return;
		}
	}
	ShowMessageTip(L"Tips.包覆面创建失败！");
}

// 环边裁剪
void OnTrimByEdgeActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// 第1步：检查当前模型环境
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"当前环境未发现模型！", L"提示", MB_OK|MB_ICONWARNING);
		return;
	}
	// 第2步：选择要裁剪的面
	ShowMessageTip(L"Tips.选择需要裁剪的面...");
	vector<ProSelection> arrSelSrf;
	ProSelection selSrf = NULL;
	while (SelectObject(arrSelSrf, "dtmqlt"))
	{
		ProSelectionCopy(arrSelSrf[0], &selSrf);
		// 第3步：暂时隐藏选择的面，选取完成后恢复显示
		ProModelitem itemQuilt;
		ProSelectionModelitemGet(arrSelSrf[0], &itemQuilt);
		ProModelitemHide(&itemQuilt);

		// 第4步：选择面上的环（先选取环的一条边）
		int nEdgeDir = PRO_EDGE_SAME_DIRECTION;
		ProSelection *pArrSelInit = NULL;
		ProArrayAlloc(0, sizeof(ProSelection), 1, (ProArray*)&pArrSelInit);
		vector<ProSelection> arrSelEdge;
		while (SelectObject(arrSelEdge, "sldedge"))
		{
			// 第5步：获取边所在环的全部边
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
				int nMsgResult = MessageBox(NULL, L"选取的边不是完整环的组成边，是否手动选取？\n是：手动选取；否：重新选取; 取消：退出", L"提示", MB_YESNOCANCEL|MB_ICONWARNING);
				if (nMsgResult == IDYES)	// 手动选择
				{
					ProArrayObjectAdd((ProArray*)&pArrSelInit, 0, 1, &arrSelEdge[0]);
					break;
				}
				else if (nMsgResult == IDCANCEL)	// 退出
				{
					ProModelitemUnhide(&itemQuilt);
					InvalidateDrawing();
					return;
				}
			}
		}
		// 第6步：自动选中对应完整环的全部边
		arrSelEdge.clear();
		if (SelectObject(arrSelEdge, "edge", MAX_SELECTION, pArrSelInit))
		{
			// 判断面的方向
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
			
			// 第7步：修剪曲面
			int nFeatID = TrimQuiltByEdgeGroup(pMdl, selSrf, arrSelEdge, nEdgeDir);
			ProModelitemUnhide(&itemQuilt);
			SAFE_DELETE_PROARRAY(pArrSelInit);
			if (nFeatID > 0)
			{
				// 第8步：提示是否反向或继续修剪
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
						// 修改裁剪方向
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
			else if (MessageBox(NULL, L"裁剪失败，详见模型特征树。是否继续？", L"提示", MB_YESNO) == IDNO)
			{
				break;
			}
		}
		else
		{
			// 恢复显示
			ProModelitemUnhide(&itemQuilt);
			break;
		}
	}
	
	InvalidateDrawing();
	ProMessageClear();
}

// 草绘裁剪
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
		MessageBox(NULL, L"当前环境未发现模型！", L"提示", MB_OK|MB_ICONWARNING);
		return;
	}
	
	// 选择草绘
	vector<ProSelection> arrSel;
	ShowMessageTip(L"Tips.选择草绘特征...");
	while (SelectObject(arrSel, "feature"))
	{
		// 检查是否为草绘曲线
		ProFeature featSketch;
		status = ProSelectionModelitemGet(arrSel[0], &featSketch);
		ProFeattype featType;
		status = ProFeatureTypeGet(&featSketch, &featType);
		if (featType == PRO_FEAT_CURVE)
		{
			ProAsmcomppath acomppath;
			ProSelectionAsmcomppathGet(arrSel[0], &acomppath);

			ProSelection selQuilt;	// 待裁剪的平面
			BOOL bGetQuilt = FALSE;

			// 获取草绘曲线所在的平面
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
					// 确定为草绘曲线，进而获取草绘平面
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

						// 获取面所在面组
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
				ShowMessageTip(L"Tips.草绘的放置面非可裁剪面，请手动选择需裁剪的面...");
				// 手动选取待裁剪的面
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
	
			// 遍历特征，获取特征中的全部复合曲线
			vector<ProGeomitem> arrItemCurves;
			status = ProFeatureGeomitemVisit(&featSketch, PRO_CURVE, FeatureGeomsGetAction, NULL, &arrItemCurves);
			vector<int> arrFeatID;
			for (int i=0; i<arrItemCurves.size(); i++)
			{
				ProCurve curve;
				status = ProGeomitemToCurve(&arrItemCurves[i], &curve);
				// 检查是否为复合曲线
				ProEnttype curveType;
				status = ProCurveTypeGet(curve, &curveType);
				if (curveType == PRO_ENT_CMP_CRV)
				{
					// 进行裁剪
					ProSelection selCurve;
					status = ProSelectionAlloc(&acomppath, &arrItemCurves[i], &selCurve);
					int nFeatID = TrimQuiltByCurve(pMdl, selQuilt, selCurve, PRO_EDGE_SAME_DIRECTION);
					if (nFeatID > 0)
					{
						// 创建成功，记录特征ID
						arrFeatID.push_back(nFeatID);

						// 第8步：提示是否反向或继续修剪
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
								// 修改裁剪方向
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
			// 合并成组
			CreateFeatGroup(pMdl, arrFeatID, L"包覆面_裁剪");
			break;
		}
		else
		{
			// 
			MessageBox(NULL, L"所选特征非草绘特征，请重新选择！", L"提示", MB_OK);
		}
	}

	InvalidateDrawing();
	ProMessageClear();
}

// 拼接包覆面
void OnMergeManualActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// 第1步：检查当前模型环境
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"当前环境未发现模型！", L"提示", MB_OK|MB_ICONWARNING);
		return;
	}
	// 第2步：选择要拼接的面
	ShowMessageTip(L"Tips.选择需要拼接的包覆面，可多选...");
	vector<ProSelection> arrSelQuilts;
	if (SelectObject(arrSelQuilts, "dtmqlt", MAX_SELECTION))
	{
		// 第3步：执行拼接
		ProFeature featMerge;
		int nFeatID = MergeQuilts(pMdl, arrSelQuilts, PRO_SRF_MRG_INTSCT, featMerge);
		if (nFeatID < 0)
		{
			MessageBox(NULL, L"拼接失败，详见模型特征树！", L"提示", MB_OK);
		}
		else if (arrSelQuilts.size() == 2)
		{
			// 修改合并的方向
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

// 拼接（智能选取）
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
		MessageBox(NULL, L"当前环境未发现模型！", L"提示", MB_OK|MB_ICONWARNING);
		return;
	}
	ProError status;

	// 选取一个包覆面
	ShowMessageTip(L"Tips.选择需要拼接的包覆面...");
	vector<ProSelection> arrSelQuilts;
	if (SelectObject(arrSelQuilts, "dtmqlt"))
	{
		ProModelitem itemQuiltStart;
		status = ProSelectionModelitemGet(arrSelQuilts[0], &itemQuiltStart);
		ProQuilt quiltStart;
		status = ProGeomitemToQuilt(&itemQuiltStart, &quiltStart);

		vector<ProQuilt> arrQuiltMerge;
		arrQuiltMerge.push_back(quiltStart);

		// 获取全部Quilt
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
					// 剔除自身
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

// 整体放量
void OnOffsetGeneralActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// 第1步：检查当前模型环境
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"当前环境未发现模型！", L"提示", MB_OK|MB_ICONWARNING);
		return;
	}

	// 第2步：选择要偏移的面
	vector<ProSelection> arrSelSrf;
	ShowMessageTip(L"Tips.选择要偏移的面...");
	if (SelectObject(arrSelSrf, "dtmqlt"))
	{
		// 第3步：输入偏离值
		double dOffset;
		ShowMessageTip(L"输入偏移量，不能为0，但可以为负值：");
		if (ProMessageDoubleRead(NULL, &dOffset) == PRO_TK_NO_ERROR)
		{
			// 第4步：创建偏移特征
			ProModelitem itemQuilt;
			int nOffsetID = OffsetQuilt(pMdl, arrSelSrf[0], dOffset, itemQuilt);
			if (nOffsetID > 0)
			{
				SetSurfColor(itemQuilt, 255, 215, 0);
				// 第5步：隐藏原包覆面
				ProModelitem itemQuiltSel;
				ProSelectionModelitemGet(arrSelSrf[0], &itemQuiltSel);
				ProModelitemHide(&itemQuiltSel);
			}
			else
			{
				MessageBox(NULL, L"放量失败，详见模型特征树！", L"提示", MB_OK);
			}
		}
	}
	InvalidateDrawing();
	ProMessageClear();
}

// 单侧放量
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
		MessageBox(NULL, L"当前环境未发现模型！", L"提示", MB_OK|MB_ICONWARNING);
		return;
	}

	// 选择面
	vector<ProSelection> arrSelQlt;
	ShowMessageTip(L"Tips.选择要偏移的面...");
	if (SelectObject(arrSelQlt, "surface"))
	{
		// 输入偏离值
		double dOffset;
		ShowMessageTip(L"输入偏移量，不能为0，但可以为负值：");
		if (ProMessageDoubleRead(NULL, &dOffset) == PRO_TK_NO_ERROR)
		{
			// 判断面的方向
			ProModelitem itemSurf;
			ProSelectionModelitemGet(arrSelQlt[0], &itemSurf);
			ProSurface surf;
			ProGeomitemToSurface(&itemSurf, &surf);
			ProGeomitemdata* psrfdata;
			ProSurfaceDataGet(surf, &psrfdata);
			if (psrfdata->data.p_surface_data->orient == PRO_SURF_ORIENT_IN)
				dOffset = dOffset*(-1);
			ProGeomitemdataFree(&psrfdata);
			// 创建偏移特征
			int nOffsetID = OffsetSurfaceWithSide(pMdl, arrSelQlt[0], dOffset);
			if (nOffsetID > 0)
			{
				// 修改面颜色
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

// 导出至STL
void OnExportSTLActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// 第1步：检查当前模型环境
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"当前环境未发现模型！", L"提示", MB_OK|MB_ICONWARNING);
		return;
	}

	// 选取面组
	vector<ProSelection> arrSelQlt;
	if (SelectObject(arrSelQlt, "dtmqlt"))
	{
		ProName mdlName;
		ProMdlNameGet(pMdl, mdlName);
		CString strMdlName(mdlName);
		ProFileName filename;
		wcsncpy_s(filename,  PRO_FILE_NAME_SIZE, strMdlName, _TRUNCATE);

		// 选择保存位置
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
				MessageBox(NULL, L"导出stl成功", L"提示", MB_OK);
			}
			else
				MessageBox(NULL, L"导出stl失败", L"提示", MB_OK|MB_ICONWARNING);
		}
	}

	InvalidateDrawing();
	ProMessageClear();
}

// 导出至STP
void OnExportSTPActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// 检查当前模型环境
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"当前环境未发现模型！", L"提示", MB_OK|MB_ICONWARNING);
		return;
	}

	InvalidateDrawing();
	ProMessageClear();
}

// 底面切除
void OnBottomCutActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);
	ProError status;

	// 第1步：检查当前模型环境
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"当前环境未发现模型！", L"提示", MB_OK|MB_ICONWARNING);
		return;
	}
	// 第2步：选择参考
	ShowMessageTip(L"Tips.选择拼接后的包覆面进行切除...");
	vector<ProSelection> arrSelQlt;
	ProSelection selQlt = NULL;
	if (SelectObject(arrSelQlt, "dtmqlt"))
	{
		status = ProSelectionCopy(arrSelQlt[0], &selQlt);

		// 第3步：暂时隐藏选择的面，选取完成后恢复显示
		ProModelitem itemQuilt;
		status = ProSelectionModelitemGet(arrSelQlt[0], &itemQuilt);
		status = ProModelitemHide(&itemQuilt);
		
		// 第4步：选择底面
		ShowMessageTip(L"Tips.选择需要平齐的底面...");
		vector<ProSelection> arrSelSrf;
		if (SelectObject(arrSelSrf, "surface"))
		{
			status = ProModelitemUnhide(&itemQuilt);

			// 第5步：以底面为参照创建基准面
			ProModelitem itemPlane;
			int nDatumFeatID = CreateDatumPlane(pMdl, arrSelSrf[0], itemPlane);
			if (nDatumFeatID > 0)
			{
				ProSelection selRef;
				status = ProSelectionAlloc(NULL, &itemPlane, &selRef);
				int nFeatID = TrimSurfaceBySurf(pMdl, selQlt, selRef, 1);
				if (nFeatID > 0)
				{
					// 第8步：提示是否反向或继续修剪
					CDlgConfirm dlg;
					while (1)
					{
						int nResult = (int)dlg.DoModal();
						if (nResult == IDRETRY) 
						{
							// 修改裁剪方向
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
					MessageBox(NULL, L"切除失败，详见模型特征树！", L"提示", MB_OK);
				}
			}
		}
		else
			status = ProModelitemUnhide(&itemQuilt);
	}
	InvalidateDrawing();
	ProMessageClear();
}

// 底面延伸
void OnBottomExpandActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);
	ProError status;

	// 第1步：检查当前模型环境
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"当前环境未发现模型！", L"提示", MB_OK|MB_ICONWARNING);
		return;
	}

	// 第2步：选择面组
	vector<ProSelection> arrSelQlt;
	ProSelection selQlt;
	if (SelectObject(arrSelQlt, "dtmqlt"))
	{
		status = ProSelectionCopy(arrSelQlt[0], &selQlt);
		
		// 第3步：选择底面
		vector<ProSelection> arrSelBottom;
		if (SelectObject(arrSelBottom, "surface"))
		{
			if (ExtendQuiltToSurf(pMdl, selQlt, arrSelBottom[0]) < 0)
			{
				MessageBox(NULL, L"处理失败，详见模型特征树！", L"提示", MB_OK);
			}
		}
	}
	InvalidateDrawing();
	ProMessageClear();
}

// 生成包覆体
void OnCreateBodyActFn()
{
	/*if (!QuickCheckValidLicense(SMART_PROFESSIONAL))
		return;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();
	CancelSelectDialog();
	Sleep(50);

	// 检查当前模型环境
	ProMdl pMdl = GetCurrentMdl();
	if (pMdl == NULL)
	{
		MessageBox(NULL, L"当前环境未发现模型！", L"提示", MB_OK|MB_ICONWARNING);
		return;
	}

	ProMdlType typeMdl;
	ProMdlTypeGet(pMdl, &typeMdl);
	if (typeMdl != PRO_MDL_ASSEMBLY)
	{
		MessageBox(NULL, L"该功能仅在装配环境下有效！", L"提示", MB_OK|MB_ICONWARNING);
		return;
	}

	//ExportElemTreeXML();

	// 选择一个面组
	vector<ProSelection> arrSelQlt;
	ShowMessageTip(L"Tips.选择要加厚的包覆面...");
	if (SelectObject(arrSelQlt, "dtmqlt"))
	{
		// 输入整体加厚值
		double dThickValue;
		ShowMessageTip(L"指定厚度值，不能为0，负值表示向内加厚：");
		if (ProMessageDoubleRead(NULL, &dThickValue) == PRO_TK_NO_ERROR)
		{
			// 创建发布几何
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

			// 新建一个零件
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

			// 创建复制几何
			ProModelitem itemCopyQuilt;
			CreateCopyGeom(pMdlPart, selAsm, selPubFeat, itemCopyQuilt);
			ProSelection selCopyQuilt;
			ProSelectionAlloc(NULL, &itemCopyQuilt, &selCopyQuilt);

			// 弹出提示：是否要对单个面设置厚度
			if (MessageBox(NULL, L"是否要对单个面设置厚度", L"提示", MB_YESNO|MB_ICONQUESTION) == IDYES)
			{
				ProMdlDisplay(pMdlPart);
				int nWndIDPart;
				ProMdlWindowGet(pMdlPart, &nWndIDPart);
				ProWindowActivate(nWndIDPart);

				// 弹出对话框
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

// 网络连接的响应函数
void ConServerActFn()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();

	// 如果服务器连接，校验是否正确即可。
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

	// 重新设置服务器的信息
	ConSmartServer(FALSE, serverDlg.m_clientInfo);
}

// 网络断开的响应函数
void DisConServerActFn()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();

	// 断开服务器
	DisConSmartServer(FALSE);
}

// 帮助
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

// 关于...
void OnAbout()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DestroyAllDialog();

	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
	ProWindowRepaint(PRO_VALUE_UNUSED);
}
