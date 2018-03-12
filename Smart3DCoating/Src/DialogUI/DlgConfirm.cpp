//===================================================================================================
// Summary:
//		操作结果确认对话框
// Date:
//		2017-04-11
// Author:
//		
//===================================================================================================

#include "stdafx.h"
#include "DlgConfirm.h"

//===================================================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//===================================================================================================

CDlgConfirm::CDlgConfirm(CWnd* pParent /*=NULL*/) : CDialog(CDlgConfirm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgConfirm)
	//}}AFX_DATA_INIT
}

void CDlgConfirm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConfirm)
	//}}AFX_DATA_MAP	
}

BEGIN_MESSAGE_MAP(CDlgConfirm, CDialog)
	//{{AFX_MSG_MAP(CDlgConfirm)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDRETRY, OnBnClickedRetry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgConfirm::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
}

BOOL CDlgConfirm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	return TRUE;
}

void CDlgConfirm::OnBnClickedRetry()
{
	CDialog::EndDialog(IDRETRY);
}
