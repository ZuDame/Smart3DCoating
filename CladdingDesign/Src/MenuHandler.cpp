//===================================================================================================
// Summary:
//		CladdingDesign的菜单以及命令响应函数
// Date:
//		2016-02-20
// Author:
//		ZhouXuan(zhouxuan_joe@163.com)
//===================================================================================================

#include "stdafx.h"
#include "CladdingDesign.h"
#include "MenuHandler.h"
#include "PreGlobal.h"
#include "DlgConfirm.h"

//===================================================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//===================================================================================================

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
	ProFileName Msg = L"Msg.txt";

	char szCladdingDesignMenuName[] = "CladdingDesignMenuName";
	ProMenubarMenuAdd(szCladdingDesignMenuName, "CladdingDesign", "Utilities", PRO_B_TRUE, Msg);

	AddMenuItem(szCladdingDesignMenuName, "CD_ShortCutMenu", ShowShortCutMenu, UserAccessDefault, Msg);
	AddMenuItem(szCladdingDesignMenuName, "CD_Clean", OnCleanActFn, UserAccessDefault, Msg);

	char szCreateChildMenuName[] = "CladdingDesignCreateMenuName";
	ProMenubarmenuMenuAdd(szCladdingDesignMenuName, szCreateChildMenuName, "CD_Create", NULL, PRO_B_TRUE, Msg);
	{
		AddMenuItem(szCreateChildMenuName, "CD_RapidCreate", OnRapidCreatActFn, UserAccessDefault, Msg);
		AddMenuItem(szCreateChildMenuName, "CD_CreateUseEdge", OnCreateByEdgeActFn, UserAccessDefault, Msg);
	}
	AddMenuItem(szCladdingDesignMenuName, "CD_Trim", OnTrimActFn, UserAccessDefault, Msg);
	AddMenuItem(szCladdingDesignMenuName, "CD_Merge", OnMergeActFn, UserAccessDefault, Msg);

	char szOffsetChildMenuName[] = "CladdingDesignOffsetMenuName";
	ProMenubarmenuMenuAdd(szCladdingDesignMenuName, szOffsetChildMenuName, "CD_Offset", NULL, PRO_B_TRUE, Msg);
	{
		AddMenuItem(szOffsetChildMenuName, "CD_OffsetGeneral", OnOffsetGeneralActFn, UserAccessDefault, Msg);
		AddMenuItem(szOffsetChildMenuName, "CD_OffsetSingle", OnOffsetSingleActFn, UserAccessDefault, Msg);
	}

	char szBottomChildMenuName[] = "CladdingDesignBottomMenuName";
	ProMenubarmenuMenuAdd(szCladdingDesignMenuName, szBottomChildMenuName, "CD_Bottom", NULL, PRO_B_TRUE, Msg);
	{
		AddMenuItem(szBottomChildMenuName, "CD_BottomCut", OnBottomCutActFn, UserAccessDefault, Msg);
		AddMenuItem(szBottomChildMenuName, "CD_BottomExpand", OnBottomExpandActFn, UserAccessDefault, Msg);
	}
	AddMenuItem(szCladdingDesignMenuName, "CD_Export", OnExportActFn, UserAccessDefault, Msg);


	// 初始化快捷菜单
	int menuid, menuid_create, menuid_edit, menuid_export;
	ProMenufileName Mnu = "ShortCut.mnu";
	ProMenufileName MnuCreate = "ShortCut_Create.mnu";
	ProMenufileName MnuEdit = "ShortCut_Edit.mnu";
	ProMenufileName MnuExport = "ShortCut_Export.mnu";

	// 注册菜单文件
	char szMnuName[] = "CladdingDesign_ShortCut";
	char szMnuNameCreate[] = "CD_Create";
	char szMnuNameEdit[] = "CD_Edit";
	char szMnuNameExport[] = "CD_Export";

	ProMenuFileRegister(szMnuName, Mnu, &menuid);
	ProMenuFileRegister(szMnuNameCreate, MnuCreate, &menuid_create);
	ProMenuFileRegister(szMnuNameEdit, MnuEdit, &menuid_edit);
	ProMenuFileRegister(szMnuNameExport, MnuExport, &menuid_export);

	// 设置菜单动作函数
	ProMenubuttonActionSet(szMnuName, "CD_Clean", (ProMenubuttonAction)OnCleanActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameCreate, "CD_RapidCreate", (ProMenubuttonAction)OnRapidCreatActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameCreate, "CD_CreateUseEdge", (ProMenubuttonAction)OnCreateByEdgeActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameEdit, "CD_Trim", (ProMenubuttonAction)OnTrimActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameEdit, "CD_Merge", (ProMenubuttonAction)OnMergeActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameEdit, "CD_OffsetGeneral", (ProMenubuttonAction)OnOffsetGeneralActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameEdit, "CD_OffsetSingle", (ProMenubuttonAction)OnOffsetSingleActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameEdit, "CD_BottomCut", (ProMenubuttonAction)OnBottomCutActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameEdit, "CD_BottomExpand", (ProMenubuttonAction)OnBottomExpandActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameExport, "CD_Export_STL", (ProMenubuttonAction)OnExportActFn, NULL, 0);
	ProMenubuttonActionSet(szMnuNameExport, "CD_Close", (ProMenubuttonAction)OnMenuQuit, NULL, 0);
	ProMenubuttonActionSet(szMnuName, szMnuName, (ProMenubuttonAction)ProMenuDelete, NULL, 0);
	ProMenubuttonActionSet(szMnuNameCreate, szMnuNameCreate, (ProMenubuttonAction)ProMenuDelete, NULL, 0);
	ProMenubuttonActionSet(szMnuNameEdit, szMnuNameEdit, (ProMenubuttonAction)ProMenuDelete, NULL, 0);
	ProMenubuttonActionSet(szMnuNameExport, szMnuNameExport, (ProMenubuttonAction)ProMenuDelete, NULL, 0);

	return TRUE;
}

void CMenuHandler::UnInit()
{
}

//===================================================================================================

// 释出所有对话框
void DestroyAllDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
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
	static char *compoundmenu[] = {"CladdingDesign_ShortCut", "CD_Create", "CD_Edit", "CD_Export", ""};
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
	int nWnd;
	if (ProWindowCurrentGet(&nWnd) == PRO_TK_NO_ERROR)
	{
		ProMenuDelete();
		ProWindowActivate(nWnd);
	}
}

// 表面清理
void OnCleanActFn()
{
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
	while (SelectObject(arrSelSrf, "surface", MAX_SELECTION))
	{
		BOOL bError = FALSE;
		for (int i=0; i<(int)arrSelSrf.size(); i++)
		{
			ProModelitem itemSurf;
			ProSelectionModelitemGet(arrSelSrf[i], &itemSurf);

			// 第3步：创建基准面
			ProModelitem itemPlane;
			int nDatumFeatID = CreateDatumPlane(pMdl, arrSelSrf[i], itemPlane);
			if (nDatumFeatID > 0)
			{
				// 第4步：创建实体化特征
				ProSelection selPlane;
				ProSelectionAlloc(NULL, &itemPlane, &selPlane);
				if (CreateSolidify(pMdl, selPlane) < 0)
				{
					bError = TRUE;
				}
				SAFE_DELETE_SELECTION(selPlane);
			}
			else
			{
				bError = TRUE;
			}
		}

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
		if (CreateBlendSurf(pMdl, arrSelEdge[0], arrSelEdge[1], itemQlt) > 0)
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

// 裁剪面
void OnTrimActFn()
{
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
			int nFeatID = TrimSurface(pMdl, selSrf, arrSelEdge, nEdgeDir);
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

// 拼接包覆面
void OnMergeActFn()
{
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
		if (MergeSurfs(pMdl, arrSelQuilts, FALSE) < 0)
		{
			MessageBox(NULL, L"拼接失败，详见模型特征树！", L"提示", MB_OK);
		}
	}

	InvalidateDrawing();
	ProMessageClear();
}

// 整体放量
void OnOffsetGeneralActFn()
{
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
			int nOffsetID = OffsetSurf(pMdl, arrSelSrf[0], dOffset, itemQuilt);
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
			int nOffsetID = OffsetSingleSurf(pMdl, arrSelQlt[0], dOffset);
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

// 导出
void OnExportActFn()
{
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
}

// 底面切除
void OnBottomCutActFn()
{
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
		vector<ProSelection> arrSelSrf;
		if (SelectObject(arrSelSrf, "surface"))
		{
			status = ProModelitemUnhide(&itemQuilt);

			// 第5步：以底面为参照创建基准面
			ProModelitem itemPlane;
			int nDatumFeatID = CreateDatumPlane(pMdl, arrSelSrf[0], itemPlane);
			if (nDatumFeatID > 0)
			{
				ProAsmcomppath comppath;
				status = ProSelectionAsmcomppathGet(arrSelSrf[0], &comppath);
				ProSelection selRef;
				status = ProSelectionAlloc(&comppath, &itemPlane, &selRef);
				int nFeatID = TrimSurface(pMdl, selQlt, selRef, 1);
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
			if (ExtendQuiltToSrf(pMdl, selQlt, arrSelBottom[0]) < 0)
			{
				MessageBox(NULL, L"处理失败，详见模型特征树！", L"提示", MB_OK);
			}
		}
	}
	InvalidateDrawing();
	ProMessageClear();
}