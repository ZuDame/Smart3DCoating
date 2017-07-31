//===================================================================================================
// Summary:
//		操作结果确认对话框
// Date:
//		2017-04-11
// Author:
//		
//===================================================================================================

#ifndef __DialogUI_DlgConfirm_h__
#define __DialogUI_DlgConfirm_h__

//===================================================================================================

#include "resource.h"

//===================================================================================================

class CDlgConfirm : public CDialog
{
public:
	CDlgConfirm(CWnd* pParent = NULL); 

	//{{AFX_DATA(CDlgConfirm)
	enum { IDD = IDD_DLG_CONFIRM };
	//}}AFX_DATA

protected:
	//{{AFX_VIRTUAL(CDlgConfirm)	
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgConfirm)
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedRetry();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//===================================================================================================

#endif

