//===================================================================================================
// Summary:
//		操作结果确认对话框
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
	for (int i=0; i<m_arrQltfaceData.size(); i++)
	{
		str.Format(L"曲面:%d (thick=%.2f)", m_arrQltfaceData[i].nSurfID, m_arrQltfaceData[i].dOffset);
		m_listSurf.AddString(str);
	}

	return TRUE;
}



// 第二步：两两相交
// 第三步：创建相交线
// 第四步：裁剪
// 第五步：合并
// 第六步：实体化

void CDlgSetThick::OnOK()
{
	ShowWindow(SW_HIDE);
	ProMdl pMdlPart = m_itemQuiltBase.owner;
	if (pMdlPart != NULL)
	{
		int nQltfaceCount = (int)m_arrQltfaceData.size();
		ProQuilt quiltBase;				// 基础包覆面
		ProGeomitemToQuilt(&m_itemQuiltBase, &quiltBase);

		vector<int> arrFeatID;				// 记录过程中产生的全部特征ID
		MapQuiltSubData mapQuiltSub;		// 偏离前的面ID和偏离后的面之间的映射关系
		map<int, int>mapExtendQuiltID;		// 偏离后的面ID与偏离前的面ID之间的映射关系
		
		// 第一步：为面组中的所有子面创建偏离面，并与偏离前的面建立关联
		for (int i=0; i<nQltfaceCount; i++)
		{
			// 获取面组中每个子面的邻面信息
			GetNeighborSurfInQuilt(pMdlPart, m_arrQltfaceData[i].qltface, quiltBase, m_arrQltfaceData[i].arrNeighborface);

			// 对每个子面进行偏离
			ProModelitem itemQltface;
			ProSurfaceToGeomitem(ProMdlToSolid(pMdlPart), m_arrQltfaceData[i].qltface, &itemQltface);
			ProSelection selQltface = NULL;
			ProSelectionAlloc(NULL, &itemQltface, &selQltface);
			int nFeatID = OffsetSurf(pMdlPart, selQltface, m_arrQltfaceData[i].dOffset, m_arrQltfaceData[i].itemOffsetQlt);
			if (nFeatID > 0)
			{
				arrFeatID.push_back(nFeatID);

				// 获取偏离后的面上的一个点
				ProModelitem itemOffsetSurf;
				QuiltToSurf(m_arrQltfaceData[i].itemOffsetQlt, itemOffsetSurf);
				ProSurface surfOffset;
				ProGeomitemToSurface(&itemOffsetSurf, &surfOffset);
				GetPointOnSurface(pMdlPart, surfOffset, m_arrQltfaceData[i].pntOnOffset);
			}
			
			/*mapQuiltSub.insert(make_pair(m_arrQltfaceData[i].nSurfID, subData));
			mapExtendQuiltID.insert(make_pair(subData.itemExtendQuilt.id, m_arrQltfaceData[i].nSurfID));*/
		}

		// 第二步：对偏离面进行延展，两两相交
		for (int i=0; i<nQltfaceCount; i++)
		{
			// 偏离后的子面
			for (int j=0, nNeighborIndex = -1; j<m_arrQltfaceData[i].arrNeighborface.size(); j++)
			{
				// 偏离后的子面的邻面
				nNeighborIndex = m_mapQltfaceIndex[m_arrQltfaceData[i].arrNeighborface[j]];

				int nFeatID1, nFeatID2;
				if (ExtendToInterSect(pMdlPart, m_arrQltfaceData[i].itemOffsetQlt, 
					m_arrQltfaceData[nNeighborIndex].itemOffsetQlt, nFeatID1, nFeatID2))
				{
					arrFeatID.push_back(nFeatID1);
					arrFeatID.push_back(nFeatID2);
				}
			}
		}

		// 第三步：每两个相交面之间创建相交线，并进行裁剪
		vector<vector<int>> arrIntersectFlag(nQltfaceCount, vector<int>(nQltfaceCount));
		ProFeature featTrim;
		featTrim.type = PRO_FEATURE;
		featTrim.owner = pMdlPart;
		for (int i=0; i<nQltfaceCount; i++)
		{
			// 偏离后的子面
			ProSelection selOffsetQlt;
			ProSelectionAlloc(NULL, &m_arrQltfaceData[i].itemOffsetQlt, &selOffsetQlt);
			ProModelitem itemOffsetSurf;
			QuiltToSurf(m_arrQltfaceData[i].itemOffsetQlt, itemOffsetSurf);
			ProSurface surfOffset;
			ProGeomitemToSurface(&itemOffsetSurf, &surfOffset);

			for (int j=0, nNeighborIndex = -1; j<m_arrQltfaceData[i].arrNeighborface.size(); j++)
			{
				nNeighborIndex = m_mapQltfaceIndex[m_arrQltfaceData[i].arrNeighborface[j]];
				if (arrIntersectFlag[i][nNeighborIndex] != 1)
				{
					// 偏离后的子面的邻面
					ProSelection selNeighborQlt;
					ProSelectionAlloc(NULL, &m_arrQltfaceData[nNeighborIndex].itemOffsetQlt, &selNeighborQlt);
					ProModelitem itemCurve;
					int nFeatIntersect = CreateIntersect(pMdlPart, selOffsetQlt, selNeighborQlt, itemCurve);
					if (nFeatIntersect > 0)
					{
						arrIntersectFlag[i][nNeighborIndex] = arrIntersectFlag[nNeighborIndex][i] = 1;

						// 以相交线进行裁剪，裁剪时判断基准点是否在裁剪后的面上，如果不在，反向
						ProSelection selCurve;
						ProSelectionAlloc(NULL, &itemCurve, &selCurve);
						int nFeatTrim1 = TrimSurfaceByCurve(pMdlPart, selOffsetQlt, selCurve, 0);
						if (nFeatTrim1 > 0)
						{
							featTrim.id = nFeatTrim1;
							if (!IsPntInsideSurf(pMdlPart, surfOffset, m_arrQltfaceData[i].pntOnOffset))
							{
								ReverseTrimDirection(featTrim);
							}
						}
						
						int nFeatTrim2 = TrimSurfaceByCurve(pMdlPart, selNeighborQlt, selCurve, 0);
						if (nFeatTrim2 > 0)
						{
							ProModelitem itemNerighborSurf;
							QuiltToSurf(m_arrQltfaceData[nNeighborIndex].itemOffsetQlt, itemNerighborSurf);
							ProSurface surfNeighbor;
							ProGeomitemToSurface(&itemNerighborSurf, &surfNeighbor);
							featTrim.id = nFeatTrim2;
							if (!IsPntInsideSurf(pMdlPart, surfNeighbor, m_arrQltfaceData[nNeighborIndex].pntOnOffset))
							{
								ReverseTrimDirection(featTrim);
							}
						}
					}
				}
			}
		}

		// 第四步：将裁剪后的面进行合并（按相邻关系进行排序）
		vector<int> arrAddFlag(nQltfaceCount);
		arrAddFlag[0] = 1;
		vector<int> arrMergeIndex;
		arrMergeIndex.push_back(0);
		for (int i=0; i<arrMergeIndex.size(); i++)
		{
			for (int j=0, nNeighborIndex=-1; j<m_arrQltfaceData[arrMergeIndex[i]].arrNeighborface.size(); j++)
			{
				nNeighborIndex = m_mapQltfaceIndex[m_arrQltfaceData[arrMergeIndex[i]].arrNeighborface[j]];
				if (arrAddFlag[nNeighborIndex] != 1)
				{
					arrMergeIndex.push_back(nNeighborIndex);
					arrAddFlag[nNeighborIndex] = 1;
				}
			}
		}
		vector<ProSelection> arrSelMergeQuilt;
		for (int i=0; i<(int)arrMergeIndex.size(); i++)
		{
			ProSelection selOffsetQlt = NULL;
			ProSelectionAlloc(NULL, &m_arrQltfaceData[arrMergeIndex[i]].itemOffsetQlt, &selOffsetQlt);
			arrSelMergeQuilt.push_back(selOffsetQlt);
		}
		MergeSurfs(pMdlPart, arrSelMergeQuilt);

		// 第五步：整体按最大偏离值进行加厚
		ProSelection selQuiltBase;
		ProSelectionAlloc(NULL, &m_itemQuiltBase, &selQuiltBase);
		ThickQuilt(pMdlPart, selQuiltBase, m_dMaxThick);

		// 第六步：实体化（裁剪实体）
		int nFeatID = CreateSolidify(pMdlPart, arrSelMergeQuilt[0], PRO_SOLIDIFY_SIDE_TWO);
		if (nFeatID > 0)
		{
			arrFeatID.push_back(nFeatID);
		}

		// 将其他辅助面隐藏
		for (int i=0; i<nQltfaceCount; i++)
		{
			ProModelitemHide(&m_arrQltfaceData[i].itemOffsetQlt);
		}
		CreateFeatGroup(pMdlPart, arrFeatID, L"包覆体");
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
	if (MessageBox(L"确认退出当前操作？", L"提示", MB_OKCANCEL|MB_ICONINFORMATION) == IDOK)
	{
		CDialog::OnCancel();
	}
}

void CDlgSetThick::OnBnClickedSel()
{
	if (m_itemQuiltBase.owner == NULL)
	{
		return;
	}

	this->ShowWindow(SW_HIDE);
	vector<ProSelection> arrSelSurf;
	if (SelectObject(arrSelSurf, "qltface", MAX_SELECTION))
	{
		double dThickValue;
		ShowMessageTip(L"指定厚度值，不能为0，但可以为负值：");
		if (ProMessageDoubleRead(NULL, &dThickValue) == PRO_TK_NO_ERROR)
		{
			ProQuilt quilt;
			ProGeomitemToQuilt(&m_itemQuiltBase, &quilt);

			for (int i=0; i<arrSelSurf.size(); i++)
			{
				ProModelitem itemSurf;
				ProSelectionModelitemGet(arrSelSurf[i], &itemSurf);
				ProSurface surf;
				ProGeomitemToSurface(&itemSurf, &surf);
				ProQuilt quiltOwner;
				ProSurfaceQuiltGet(ProMdlToSolid(m_itemQuiltBase.owner), surf, &quiltOwner);
				if (quiltOwner == quilt)
				{
					for (int j=0; j<m_arrQltfaceData.size(); j++)
					{
						if (surf == m_arrQltfaceData[j].qltface)
						{
							CString str;
							str.Format(L"曲面:%d (thick=%.2f)", m_arrQltfaceData[j].nSurfID, dThickValue);
							m_arrQltfaceData[j].dOffset = dThickValue;
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
	m_arrQltfaceData.clear();

	// 获取全部面
	m_pAsm = pAsm;
	ProSelectionModelitemGet(selQuilt, &m_itemQuiltBase);
	ProQuilt quilt;
	ProGeomitemToQuilt(&m_itemQuiltBase, &quilt);
	vector<ProSurface> arrQltface;
	ProQuiltSurfaceVisit(quilt, QuiltSurfacesGetAction, NULL, &arrQltface);

	for (int i=0; i<arrQltface.size(); i++)
	{
		QltfaceData data;
		data.qltface = arrQltface[i];
		data.dOffset = dThick;
		ProSurfaceIdGet(arrQltface[i], &data.nSurfID);
		m_arrQltfaceData.push_back(data);
		m_mapQltfaceIndex.insert(make_pair(data.nSurfID, i));
	}
	m_dMaxThick = dThick;
}

void CDlgSetThick::OnLbnSelchangeListSurf()
{
	ProWindowRepaint(PRO_VALUE_UNUSED);
	ProSelbufferClear();

	int nIndex = m_listSurf.GetCurSel();
	if (nIndex >= 0 && m_itemQuiltBase.owner != NULL)
	{
		ProGeomitem itemSurf;
		ProSurfaceToGeomitem(ProMdlToSolid(m_itemQuiltBase.owner), m_arrQltfaceData[nIndex].qltface, &itemSurf);
		ProSelection selSurf;
		ProSelectionAlloc(NULL, &itemSurf, &selSurf);
		//ProSelectionHighlight(selSurf, PRO_COLOR_HIGHLITE);
		ProSelectionDisplay(selSurf);
	}
}

void CDlgSetThick::OnLbnDblclkListSurf()
{
	int nIndex = m_listSurf.GetCurSel();
	if (nIndex >= 0 && m_itemQuiltBase.owner != NULL)
	{
		double dThickValue;
		ShowMessageTip(L"指定厚度值，不能为0，但可以为负值：");
		if (ProMessageDoubleRead(NULL, &dThickValue) == PRO_TK_NO_ERROR)
		{
			CString str;
			str.Format(L"曲面:%d (thick=%.2f)", m_arrQltfaceData[nIndex].nSurfID, dThickValue);
			m_arrQltfaceData[nIndex].dOffset = dThickValue;
			m_listSurf.DeleteString(nIndex);
			m_listSurf.InsertString(nIndex, str);
			if (dThickValue > m_dMaxThick)
				m_dMaxThick = dThickValue;
		}
	}
}
