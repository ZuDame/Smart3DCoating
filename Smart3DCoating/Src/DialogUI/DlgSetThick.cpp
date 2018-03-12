//===================================================================================================
// Summary:
//		�������ȷ�϶Ի���
// Date:
//		2017-04-11
// Author:
//		
//===================================================================================================

#include "stdafx.h"
#include "DlgSetThick.h"
#include "PreGlobal.h"
#include "DlgMergeDir.h"

//===================================================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//===================================================================================================

CDlgSetThick::CDlgSetThick(CWnd* pParent /*=NULL*/) : CDialog(CDlgSetThick::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetThick)
	m_dMaxThick = 0.0;
	m_pAsm = NULL;
	//}}AFX_DATA_INIT
}

void CDlgSetThick::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetThick)
	DDX_Control(pDX, IDC_LIST_SURF, m_listSurf);
	//}}AFX_DATA_MAP	
}

BEGIN_MESSAGE_MAP(CDlgSetThick, CDialog)
	//{{AFX_MSG_MAP(CDlgSetThick)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BTN_SEL, OnBnClickedSel)
	ON_LBN_SELCHANGE(IDC_LIST_SURF, OnLbnSelchangeListSurf)
	ON_LBN_DBLCLK(IDC_LIST_SURF, OnLbnDblclkListSurf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgSetThick::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
}

BOOL CDlgSetThick::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString str;
	for (int i=0; i<m_arrQuiltThickData.size(); i++)
	{
		str.Format(L"����:%d (thick=%.2f)", m_arrQuiltThickData[i].nSurfID, m_arrQuiltThickData[i].dThick);
		m_listSurf.AddString(str);
	}

	return TRUE;
}

void CDlgSetThick::OnOK() 
{
	ShowWindow(SW_HIDE);
	ProMdl pMdl = m_itemQuilt.owner;
	if (pMdl != NULL)
	{
		ProQuilt quiltToThick;				// ����������
		ProGeomitemToQuilt(&m_itemQuilt, &quiltToThick);

		vector<int> arrFeatID;				// ��¼�����в�����ȫ������ID
		MapQuiltSubData mapQuiltSub;		// ƫ��ǰ����ID��ƫ������֮���ӳ���ϵ
		map<int, int>mapExtendQuiltID;		// ƫ������ID��ƫ��ǰ����ID֮���ӳ���ϵ
		
		// Ϊ�����е��������洴��ƫ����
		for (int i=0; i<m_arrQuiltThickData.size(); i++)
		{
			QuiltSubData subData;
			subData.subSurf = m_arrQuiltThickData[i].surf;
			if (m_arrQuiltThickData[i].dThick < m_dMaxThick)
				subData.bSolidity = TRUE;

			ProModelitem itemSubSurf;
			ProSurfaceToGeomitem(ProMdlToSolid(pMdl), subData.subSurf, &itemSubSurf);
			ProSelection selSubSurf;
			ProSelectionAlloc(NULL, &itemSubSurf, &selSubSurf);
			int nFeatID = OffsetSurf(pMdl, selSubSurf, m_arrQuiltThickData[i].dThick, subData.itemExtendQuilt);
			if (nFeatID > 0)
				arrFeatID.push_back(nFeatID);	
			
			mapQuiltSub.insert(make_pair(m_arrQuiltThickData[i].nSurfID, subData));
			mapExtendQuiltID.insert(make_pair(subData.itemExtendQuilt.id, m_arrQuiltThickData[i].nSurfID));
		}

		// ��ƫ���������չ
		vector<int> arrQuiltToMerge;
		for (int i=0; i<m_arrQuiltThickData.size(); i++)
		{
			int nCurrSubID = m_arrQuiltThickData[i].nSurfID;

			// ����������ÿ�������������Ϣ
			vector<int> arrNeighborSubID;
			GetNeighborSurfInQuilt(pMdl, m_arrQuiltThickData[i].surf, quiltToThick, arrNeighborSubID);
			mapQuiltSub[nCurrSubID].arrNeighborID = arrNeighborSubID;

			ProQuilt quilt1;
			ProGeomitemToQuilt(&mapQuiltSub[nCurrSubID].itemExtendQuilt, &quilt1);
			vector<ProSurface> arrQltSrfs1;
			ProQuiltSurfaceVisit(quilt1, QuiltSurfacesGetAction, NULL, &arrQltSrfs1);
			ProModelitem itemSurf1;
			ProSurfaceToGeomitem(ProMdlToSolid(pMdl), arrQltSrfs1[0], &itemSurf1);

			// ��ȡƫ�������֮��������
			double dMaxDis = 0.0;
			Pro3dPnt pnt1, pnt2;
			for (int j=0; j<arrNeighborSubID.size(); j++)
			{
				ProQuilt quilt2;
				ProGeomitemToQuilt(&mapQuiltSub[arrNeighborSubID[j]].itemExtendQuilt, &quilt2);
				vector<ProSurface> arrQltSrfs2;
				ProQuiltSurfaceVisit(quilt2, QuiltSurfacesGetAction, NULL, &arrQltSrfs2);
				ProModelitem itemSurf2;
				ProSurfaceToGeomitem(ProMdlToSolid(pMdl), arrQltSrfs2[0], &itemSurf2);
				double dDis = 0.0;
				MeasureDistance(itemSurf1, itemSurf2, dDis, pnt1, pnt2);
				if (dDis > dMaxDis)
					dMaxDis = dDis;
			}
			// �������������չ
			// ��ȡ����⻷
			ProSelection selQuilt1;
			ProSelectionAlloc(NULL, &mapQuiltSub[nCurrSubID].itemExtendQuilt, &selQuilt1);
			int nID = ExtendSrfByContour(pMdl, selQuilt1, dMaxDis*1.2);
			if (nID > 0)
				arrFeatID.push_back(nID);

			// ɸѡ��ƫ�����Ȼ������������ཻ��ƫ����
			if (CheckTwoQuiltInstersect(pMdl, quilt1, quiltToThick))
			{
				arrQuiltToMerge.push_back(nCurrSubID);
			}
		}

		if (arrQuiltToMerge.size() > 0)
		{
			InvalidateDrawing();
			for (int i=0; i<arrQuiltToMerge.size(); i++)
			{
				ProSelection selTemp;
				ProSelectionAlloc(NULL, &mapQuiltSub[arrQuiltToMerge[i]].itemExtendQuilt, &selTemp);
				ProSelectionDisplay(selTemp);
			}
			// ��������Ҫ�˹����кϲ�����
			MessageBox(L"��������Ҫ�˹����кϲ�����", L"��ʾ", MB_OK|MB_ICONWARNING);

			// �˹��Բ�������кϲ�
			vector<ProSelection> arrSelQuilts;
			ShowMessageTip(L"Tips.ѡ��ƫ������ж�κϲ���ѡ�����������1��ȡ��ѡ�񽫼�����������...");
			while (1)
			{
				arrSelQuilts.clear();
				if (!SelectObject(arrSelQuilts, "dtmqlt", 2) || arrSelQuilts.size() == 1)
				{
					if (MessageBox(L"ȷ�Ͻ�����ϲ����������ɰ����壿", L"��ʾ", MB_OKCANCEL|MB_ICONWARNING) == IDOK)
						break;
					else
						continue;
				}
				int nMergeID = MergeSurfs(pMdl, arrSelQuilts);
				if (nMergeID > 0)
				{
					// �޸ĺϲ��ķ���
					ProFeature featMerge;
					featMerge.id = nMergeID;
					featMerge.type = PRO_FEATURE;
					featMerge.owner = pMdl;
					CDlgMergeDir dlgDir;
					dlgDir.IntiFeature(featMerge);
					if (dlgDir.DoModal() == IDCANCEL)
					{
						// ɾ������ѡ��
						int nFeatID[] = {nMergeID};
						ProFeatureDeleteOptions opt[] = {PRO_FEAT_DELETE_NO_OPTS};
						ProFeatureDelete(ProMdlToSolid(pMdl), nFeatID, 1, opt, 1);
						continue;
					}

					ProModelitem itemFirst;
					ProSelectionModelitemGet(arrSelQuilts[0], &itemFirst);
					int nFirstSurfaceID = mapExtendQuiltID[itemFirst.id];
					ProModelitem itemSecond;
					ProSelectionModelitemGet(arrSelQuilts[1], &itemSecond);
					int nSecondurfaceID = mapExtendQuiltID[itemSecond.id];
					if (mapQuiltSub[nSecondurfaceID].bSolidity)
					{
						mapQuiltSub[nFirstSurfaceID].bSolidity = TRUE;
					}
					mapQuiltSub.erase(nSecondurfaceID);
				}
				InvalidateDrawing();
			}
		}

		// �Բ���ƫ������з���
		//vector<vector<int>> groupSurfID;
		//for (int i=0; i<arrQuiltToMerge.size(); i++)
		//{
		//	BOOL bFind = FALSE;
		//	for (int j=0; j<groupSurfID.size(); j++)
		//	{
		//		for (int k=0; k<groupSurfID[j].size(); k++)
		//		{
		//			for (int m=0; m<mapQuiltSub[groupSurfID[j][k]].arrNeighborID.size(); m++)
		//			{
		//				if (mapQuiltSub[groupSurfID[j][k]].arrNeighborID[m] == arrQuiltToMerge[i])
		//				{
		//					bFind = TRUE;
		//					groupSurfID[j].push_back(arrQuiltToMerge[i]);
		//					break;
		//				}
		//			}
		//			if (bFind)
		//				break;
		//		}
		//		if (bFind)
		//			break;
		//	}
		//	if (!bFind)
		//	{
		//		vector<int> arrSurfID;
		//		arrSurfID.push_back(arrQuiltToMerge[i]);
		//		groupSurfID.push_back(arrSurfID);
		//	}
		//}

		//// �Է�����ƫ������кϲ�
		//for (int j=0; j<groupSurfID.size(); j++)
		//{
		//	if (groupSurfID[j].size() >= 2)
		//	{
		//		ProSelection selQuilt1;
		//		ProSelectionAlloc(NULL, &mapQuiltSub[groupSurfID[j][0]].itemExtendQuilt, &selQuilt1);

		//		vector<ProSelection> arrSelQuilts;
		//		for (int k=1; k<groupSurfID[j].size(); k++)
		//		{
		//			ProSelection selQuilt2;
		//			ProSelectionAlloc(NULL, &mapQuiltSub[groupSurfID[j][k]].itemExtendQuilt, &selQuilt2);

		//			arrSelQuilts.clear();
		//			arrSelQuilts.push_back(selQuilt1);
		//			arrSelQuilts.push_back(selQuilt2);
		//			int nFeatID = MergeSurfs(pMdl, arrSelQuilts, FALSE);
		//			if (nFeatID > 0)
		//			{
		//				arrFeatID.push_back(nFeatID);
		//				if (mapQuiltSub[groupSurfID[j][k]].bSolidity)
		//					mapQuiltSub[groupSurfID[j][0]].bSolidity = TRUE;
		//				mapQuiltSub.erase(groupSurfID[j][k]);
		//			}
		//		}
		//	}
		//}

		// ���尴���ƫ��ֵ���мӺ�
		ProSelection selQuiltToThick;
		ProSelectionAlloc(NULL, &m_itemQuilt, &selQuiltToThick);
		ThickQuilt(pMdl, selQuiltToThick, m_dMaxThick);

		// ʵ�廯���ü�ʵ�壩
		MapQuiltSubData::iterator iter;
		iter = mapQuiltSub.begin();
		while(iter != mapQuiltSub.end())
		{
			if (iter->second.bSolidity)
			{
				ProSelection selQuilt;
				ProSelectionAlloc(NULL, &iter->second.itemExtendQuilt, &selQuilt);

				int nFeatID = CreateSolidify(pMdl, selQuilt, PRO_SOLIDIFY_SIDE_TWO);
				if (nFeatID > 0)
				{
					arrFeatID.push_back(nFeatID);
				}
			}
			else
			{
				// ����������������
				ProModelitemHide(&iter->second.itemExtendQuilt);
			}
			iter++;
		}

		CreateFeatGroup(pMdl, arrFeatID, L"������");
	}

	if (m_pAsm != NULL)
	{
		ProMdlDisplay(m_pAsm);
		int nWndID;
		ProMdlWindowGet(m_pAsm, &nWndID);
		ProWindowActivate(nWndID);
	}
	CDialog::OnOK();
}

void CDlgSetThick::OnCancel() 
{
	if (MessageBox(L"ȷ���˳���ǰ������", L"��ʾ", MB_OKCANCEL|MB_ICONINFORMATION) == IDOK)
	{
		CDialog::OnCancel();
	}
}

void CDlgSetThick::OnBnClickedSel()
{
	if (m_itemQuilt.owner == NULL)
	{
		return;
	}

	this->ShowWindow(SW_HIDE);
	vector<ProSelection> arrSelSurf;
	if (SelectObject(arrSelSurf, "qltface", MAX_SELECTION))
	{
		double dThickValue;
		ShowMessageTip(L"ָ�����ֵ������Ϊ0��������Ϊ��ֵ��");
		if (ProMessageDoubleRead(NULL, &dThickValue) == PRO_TK_NO_ERROR)
		{
			ProQuilt quilt;
			ProGeomitemToQuilt(&m_itemQuilt, &quilt);

			for (int i=0; i<arrSelSurf.size(); i++)
			{
				ProModelitem itemSurf;
				ProSelectionModelitemGet(arrSelSurf[i], &itemSurf);
				ProSurface surf;
				ProGeomitemToSurface(&itemSurf, &surf);
				ProQuilt quiltOwner;
				ProSurfaceQuiltGet(ProMdlToSolid(m_itemQuilt.owner), surf, &quiltOwner);
				if (quiltOwner == quilt)
				{
					for (int j=0; j<m_arrQuiltThickData.size(); j++)
					{
						if (surf == m_arrQuiltThickData[j].surf)
						{
							CString str;
							str.Format(L"����:%d (thick=%.2f)", m_arrQuiltThickData[j].nSurfID, dThickValue);
							m_arrQuiltThickData[j].dThick = dThickValue;
							m_listSurf.DeleteString(j);
							m_listSurf.InsertString(j, str);
							m_listSurf.SetCurSel(j);
							break;
						}
					}
				}
			}
			if (dThickValue > m_dMaxThick)
				m_dMaxThick = dThickValue;
		}
	}
	this->ShowWindow(SW_SHOW);
}

void CDlgSetThick::InitSurfData(ProMdl pAsm, ProSelection selQuilt, double dThick)
{
	m_arrQuiltThickData.clear();

	// ��ȡȫ����
	m_pAsm = pAsm;
	ProSelectionModelitemGet(selQuilt, &m_itemQuilt);
	ProQuilt quilt;
	ProGeomitemToQuilt(&m_itemQuilt, &quilt);
	vector<ProSurface> arrSurf;
	ProQuiltSurfaceVisit(quilt, QuiltSurfacesGetAction, NULL, &arrSurf);

	for (int i=0; i<arrSurf.size(); i++)
	{
		SurfThickData data;
		data.surf = arrSurf[i];
		ProSurfaceIdGet(arrSurf[i], &data.nSurfID);
		data.dThick = dThick;
		m_arrQuiltThickData.push_back(data);
	}
	m_dMaxThick = dThick;
}

void CDlgSetThick::OnLbnSelchangeListSurf()
{
	ProWindowRepaint(PRO_VALUE_UNUSED);
	ProSelbufferClear();

	int nIndex = m_listSurf.GetCurSel();
	if (nIndex >= 0 && m_itemQuilt.owner != NULL)
	{
		ProGeomitem itemSurf;
		ProSurfaceToGeomitem(ProMdlToSolid(m_itemQuilt.owner), m_arrQuiltThickData[nIndex].surf, &itemSurf);
		ProSelection selSurf;
		ProSelectionAlloc(NULL, &itemSurf, &selSurf);
		//ProSelectionHighlight(selSurf, PRO_COLOR_HIGHLITE);
		ProSelectionDisplay(selSurf);
	}
}

void CDlgSetThick::OnLbnDblclkListSurf()
{
	int nIndex = m_listSurf.GetCurSel();
	if (nIndex >= 0 && m_itemQuilt.owner != NULL)
	{
		double dThickValue;
		ShowMessageTip(L"ָ�����ֵ������Ϊ0��������Ϊ��ֵ��");
		if (ProMessageDoubleRead(NULL, &dThickValue) == PRO_TK_NO_ERROR)
		{
			CString str;
			str.Format(L"����:%d (thick=%.2f)", m_arrQuiltThickData[nIndex].nSurfID, dThickValue);
			m_arrQuiltThickData[nIndex].dThick = dThickValue;
			m_listSurf.DeleteString(nIndex);
			m_listSurf.InsertString(nIndex, str);
			if (dThickValue > m_dMaxThick)
				m_dMaxThick = dThickValue;
		}
	}
}
