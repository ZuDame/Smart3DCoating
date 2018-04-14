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
	for (int i=0; i<m_arrQltfaceData.size(); i++)
	{
		str.Format(L"����:%d (thick=%.2f)", m_arrQltfaceData[i].nSurfID, m_arrQltfaceData[i].dOffset);
		m_listSurf.AddString(str);
	}

	return TRUE;
}



// �ڶ����������ཻ
// �������������ཻ��
// ���Ĳ����ü�
// ���岽���ϲ�
// ��������ʵ�廯

void CDlgSetThick::OnOK()
{
	ShowWindow(SW_HIDE);
	ProMdl pMdlPart = m_itemQuiltBase.owner;
	if (pMdlPart != NULL)
	{
		int nQltfaceCount = (int)m_arrQltfaceData.size();
		ProQuilt quiltBase;				// ����������
		ProGeomitemToQuilt(&m_itemQuiltBase, &quiltBase);

		vector<int> arrFeatID;				// ��¼�����в�����ȫ������ID
		MapQuiltSubData mapQuiltSub;		// ƫ��ǰ����ID��ƫ������֮���ӳ���ϵ
		map<int, int>mapExtendQuiltID;		// ƫ������ID��ƫ��ǰ����ID֮���ӳ���ϵ
		
		// ��һ����Ϊ�����е��������洴��ƫ���棬����ƫ��ǰ���潨������
		for (int i=0; i<nQltfaceCount; i++)
		{
			// ��ȡ������ÿ�������������Ϣ
			GetNeighborSurfInQuilt(pMdlPart, m_arrQltfaceData[i].qltface, quiltBase, m_arrQltfaceData[i].arrNeighborface);

			// ��ÿ���������ƫ��
			ProModelitem itemQltface;
			ProSurfaceToGeomitem(ProMdlToSolid(pMdlPart), m_arrQltfaceData[i].qltface, &itemQltface);
			ProSelection selQltface = NULL;
			ProSelectionAlloc(NULL, &itemQltface, &selQltface);
			int nFeatID = OffsetSurf(pMdlPart, selQltface, m_arrQltfaceData[i].dOffset, m_arrQltfaceData[i].itemOffsetQlt);
			if (nFeatID > 0)
			{
				arrFeatID.push_back(nFeatID);

				// ��ȡƫ�������ϵ�һ����
				ProModelitem itemOffsetSurf;
				QuiltToSurf(m_arrQltfaceData[i].itemOffsetQlt, itemOffsetSurf);
				ProSurface surfOffset;
				ProGeomitemToSurface(&itemOffsetSurf, &surfOffset);
				GetPointOnSurface(pMdlPart, surfOffset, m_arrQltfaceData[i].pntOnOffset);
			}
			
			/*mapQuiltSub.insert(make_pair(m_arrQltfaceData[i].nSurfID, subData));
			mapExtendQuiltID.insert(make_pair(subData.itemExtendQuilt.id, m_arrQltfaceData[i].nSurfID));*/
		}

		// �ڶ�������ƫ���������չ�������ཻ
		for (int i=0; i<nQltfaceCount; i++)
		{
			// ƫ��������
			for (int j=0, nNeighborIndex = -1; j<m_arrQltfaceData[i].arrNeighborface.size(); j++)
			{
				// ƫ�������������
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

		// ��������ÿ�����ཻ��֮�䴴���ཻ�ߣ������вü�
		vector<vector<int>> arrIntersectFlag(nQltfaceCount, vector<int>(nQltfaceCount));
		ProFeature featTrim;
		featTrim.type = PRO_FEATURE;
		featTrim.owner = pMdlPart;
		for (int i=0; i<nQltfaceCount; i++)
		{
			// ƫ��������
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
					// ƫ�������������
					ProSelection selNeighborQlt;
					ProSelectionAlloc(NULL, &m_arrQltfaceData[nNeighborIndex].itemOffsetQlt, &selNeighborQlt);
					ProModelitem itemCurve;
					int nFeatIntersect = CreateIntersect(pMdlPart, selOffsetQlt, selNeighborQlt, itemCurve);
					if (nFeatIntersect > 0)
					{
						arrIntersectFlag[i][nNeighborIndex] = arrIntersectFlag[nNeighborIndex][i] = 1;

						// ���ཻ�߽��вü����ü�ʱ�жϻ�׼���Ƿ��ڲü�������ϣ�������ڣ�����
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

		// ���Ĳ������ü��������кϲ��������ڹ�ϵ��������
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

		// ���岽�����尴���ƫ��ֵ���мӺ�
		ProSelection selQuiltBase;
		ProSelectionAlloc(NULL, &m_itemQuiltBase, &selQuiltBase);
		ThickQuilt(pMdlPart, selQuiltBase, m_dMaxThick);

		// ��������ʵ�廯���ü�ʵ�壩
		int nFeatID = CreateSolidify(pMdlPart, arrSelMergeQuilt[0], PRO_SOLIDIFY_SIDE_TWO);
		if (nFeatID > 0)
		{
			arrFeatID.push_back(nFeatID);
		}

		// ����������������
		for (int i=0; i<nQltfaceCount; i++)
		{
			ProModelitemHide(&m_arrQltfaceData[i].itemOffsetQlt);
		}
		CreateFeatGroup(pMdlPart, arrFeatID, L"������");
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
	if (m_itemQuiltBase.owner == NULL)
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
							str.Format(L"����:%d (thick=%.2f)", m_arrQltfaceData[j].nSurfID, dThickValue);
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

	// ��ȡȫ����
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
		ShowMessageTip(L"ָ�����ֵ������Ϊ0��������Ϊ��ֵ��");
		if (ProMessageDoubleRead(NULL, &dThickValue) == PRO_TK_NO_ERROR)
		{
			CString str;
			str.Format(L"����:%d (thick=%.2f)", m_arrQltfaceData[nIndex].nSurfID, dThickValue);
			m_arrQltfaceData[nIndex].dOffset = dThickValue;
			m_listSurf.DeleteString(nIndex);
			m_listSurf.InsertString(nIndex, str);
			if (dThickValue > m_dMaxThick)
				m_dMaxThick = dThickValue;
		}
	}
}
