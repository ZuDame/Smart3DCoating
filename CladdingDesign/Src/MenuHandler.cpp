//===================================================================================================
// Summary:
//		CladdingDesign�Ĳ˵��Լ�������Ӧ����
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


	// ��ʼ����ݲ˵�
	int menuid, menuid_create, menuid_edit, menuid_export;
	ProMenufileName Mnu = "ShortCut.mnu";
	ProMenufileName MnuCreate = "ShortCut_Create.mnu";
	ProMenufileName MnuEdit = "ShortCut_Edit.mnu";
	ProMenufileName MnuExport = "ShortCut_Export.mnu";

	// ע��˵��ļ�
	char szMnuName[] = "CladdingDesign_ShortCut";
	char szMnuNameCreate[] = "CD_Create";
	char szMnuNameEdit[] = "CD_Edit";
	char szMnuNameExport[] = "CD_Export";

	ProMenuFileRegister(szMnuName, Mnu, &menuid);
	ProMenuFileRegister(szMnuNameCreate, MnuCreate, &menuid_create);
	ProMenuFileRegister(szMnuNameEdit, MnuEdit, &menuid_edit);
	ProMenuFileRegister(szMnuNameExport, MnuExport, &menuid_export);

	// ���ò˵���������
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

// �ͳ����жԻ���
void DestroyAllDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
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
	static char *compoundmenu[] = {"CladdingDesign_ShortCut", "CD_Create", "CD_Edit", "CD_Export", ""};
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
	int nWnd;
	if (ProWindowCurrentGet(&nWnd) == PRO_TK_NO_ERROR)
	{
		ProMenuDelete();
		ProWindowActivate(nWnd);
	}
}

// ��������
void OnCleanActFn()
{
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
	while (SelectObject(arrSelSrf, "surface", MAX_SELECTION))
	{
		BOOL bError = FALSE;
		for (int i=0; i<(int)arrSelSrf.size(); i++)
		{
			ProModelitem itemSurf;
			ProSelectionModelitemGet(arrSelSrf[i], &itemSurf);

			// ��3����������׼��
			ProModelitem itemPlane;
			int nDatumFeatID = CreateDatumPlane(pMdl, arrSelSrf[i], itemPlane);
			if (nDatumFeatID > 0)
			{
				// ��4��������ʵ�廯����
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
		if (CreateBlendSurf(pMdl, arrSelEdge[0], arrSelEdge[1], itemQlt) > 0)
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

// �ü���
void OnTrimActFn()
{
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
			int nFeatID = TrimSurface(pMdl, selSrf, arrSelEdge, nEdgeDir);
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

// ƴ�Ӱ�����
void OnMergeActFn()
{
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
		if (MergeSurfs(pMdl, arrSelQuilts, FALSE) < 0)
		{
			MessageBox(NULL, L"ƴ��ʧ�ܣ����ģ����������", L"��ʾ", MB_OK);
		}
	}

	InvalidateDrawing();
	ProMessageClear();
}

// �������
void OnOffsetGeneralActFn()
{
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
			int nOffsetID = OffsetSurf(pMdl, arrSelSrf[0], dOffset, itemQuilt);
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
			int nOffsetID = OffsetSingleSurf(pMdl, arrSelQlt[0], dOffset);
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

// ����
void OnExportActFn()
{
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
}

// �����г�
void OnBottomCutActFn()
{
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
		vector<ProSelection> arrSelSrf;
		if (SelectObject(arrSelSrf, "surface"))
		{
			status = ProModelitemUnhide(&itemQuilt);

			// ��5�����Ե���Ϊ���մ�����׼��
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
			if (ExtendQuiltToSrf(pMdl, selQlt, arrSelBottom[0]) < 0)
			{
				MessageBox(NULL, L"����ʧ�ܣ����ģ����������", L"��ʾ", MB_OK);
			}
		}
	}
	InvalidateDrawing();
	ProMessageClear();
}