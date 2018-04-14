//===================================================================================================
// Summary:
//		�������ȷ�϶Ի���
// Date:
//		2017-04-11
// Author:
//		
//===================================================================================================

#ifndef __DialogUI_DlgSetThick_h__
#define __DialogUI_DlgSetThick_h__

//===================================================================================================

#include "resource.h"

//===================================================================================================

// ������������Ϣ
typedef struct __tagQltfaceData
{
	int nSurfID;
	ProSurface qltface;
	vector<int> arrNeighborface;	// ���������ID

	double dOffset;					// ƫ�ƾ���
	ProModelitem itemOffsetQlt;		// ƫ������(����)
	Pro3dPnt pntOnOffset;				// ƫ�����ϵĵ㣬���ڲü�ʱ�жϲü�����

	__tagQltfaceData()
	{
		nSurfID = -1;
		qltface = NULL;
		dOffset = 0.0;
	}
	__tagQltfaceData& operator=(const __tagQltfaceData &qfd)
	{
		if (&qfd == this)
			return *this;

		nSurfID = qfd.nSurfID;
		qltface = qfd.qltface;
		dOffset = qfd.dOffset;
		arrNeighborface = qfd.arrNeighborface;
		itemOffsetQlt.id = qfd.itemOffsetQlt.id;
		itemOffsetQlt.type = qfd.itemOffsetQlt.type;
		itemOffsetQlt.owner = qfd.itemOffsetQlt.owner;
		for (int i=0; i<3; i++)
			pntOnOffset[i] = qfd.pntOnOffset[i];
		return *this;
	}
}QltfaceData;

typedef vector<QltfaceData> QltfaceDataArray;

typedef map<int, int> QltfaceDataIndexMap;



// �������赥��ƫ���������Ϣ
typedef struct __tagQuiltSubData
{
	ProSurface subSurf;				// ����ID
	vector<int> arrNeighborID;		// �����ID

	ProModelitem itemExtendQuilt;	// ƫ������
	BOOL bSolidity;					// ƫ����Ƿ����ʵ�廯

	__tagQuiltSubData()
	{
		subSurf = NULL;
		bSolidity = FALSE;
	}
	__tagQuiltSubData& operator=(const __tagQuiltSubData &qsd)
	{
		if (&qsd == this)
			return *this;

		subSurf = qsd.subSurf;
		itemExtendQuilt.id = itemExtendQuilt.id;
		itemExtendQuilt.type = itemExtendQuilt.type;
		itemExtendQuilt.owner = itemExtendQuilt.owner;
		arrNeighborID = qsd.arrNeighborID;
		bSolidity = qsd.bSolidity;
		return *this;
	}
}QuiltSubData;

typedef map<int, QuiltSubData> MapQuiltSubData;


//===================================================================================================

class CDlgSetThick : public CDialog
{
public:
	CDlgSetThick(CWnd* pParent = NULL); 

	//{{AFX_DATA(CDlgSetThick)
	enum { IDD = IDD_DLG_SETTHICK };
	//}}AFX_DATA

	CListBox m_listSurf;

	ProMdl m_pAsm;
	ProGeomitem m_itemQuiltBase;	// ������

	QltfaceDataArray m_arrQltfaceData;
	QltfaceDataIndexMap m_mapQltfaceIndex;
	double m_dMaxThick;

protected:
	//{{AFX_VIRTUAL(CDlgSetThick)	
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

public:
	void InitSurfData(ProMdl pAsm, ProSelection selQuilt, double dThick);

protected:
	//{{AFX_MSG(CDlgSetThick)
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedSel();
	afx_msg void OnLbnSelchangeListSurf();
	afx_msg void OnLbnDblclkListSurf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//===================================================================================================

#endif
