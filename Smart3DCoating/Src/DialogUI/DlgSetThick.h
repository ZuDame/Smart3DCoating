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

// ��Ӻ���Ϣ
typedef struct __tagSurfThickData
{
	ProSurface surf;
	int nSurfID;
	double dThick;

	__tagSurfThickData()
	{
		surf = NULL;
		nSurfID = 0;
		dThick = 0.0;
	}
	__tagSurfThickData& operator=(const __tagSurfThickData &std)
	{
		if (&std == this)
			return *this;

		surf = std.surf;
		nSurfID = std.nSurfID;
		dThick = std.dThick;
		return *this;
	}
}SurfThickData;

typedef vector<SurfThickData> SurfThickDataArray;

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

	ProGeomitem m_itemQuilt;
	SurfThickDataArray m_arrQuiltThickData;
	ProMdl m_pAsm;
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

