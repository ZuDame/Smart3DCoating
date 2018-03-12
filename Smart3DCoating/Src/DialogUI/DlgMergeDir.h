//===================================================================================================
// Summary:
//		合并方向修改对话框
// Date:
//		2018-03-12
// Author:
//		
//===================================================================================================

#ifndef __DialogUI_DlgMergeDir_h__
#define __DialogUI_DlgMergeDir_h__

//===================================================================================================

#include "resource.h"

//===================================================================================================

class CDlgMergeDir : public CDialog
{
public:
	CDlgMergeDir(CWnd* pParent = NULL); 

	//{{AFX_DATA(CDlgMergeDir)
	enum { IDD = IDD_DLG_MERGEDIR };
	//}}AFX_DATA

protected:
	ProFeature m_feat;

public:
	void IntiFeature(ProFeature feat);

protected:
	//{{AFX_VIRTUAL(CDlgMergeDir)	
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgMergeDir)
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckFirst();
	afx_msg void OnBnClickedCheckSecond();
};

//===================================================================================================

#endif

