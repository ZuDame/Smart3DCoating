//===================================================================================================
// Summary:
//		合并方向修改对话框
// Date:
//		2018-03-12
// Author:
//		
//===================================================================================================

#include "stdafx.h"
#include "DlgMergeDir.h"
#include "PreGlobal.h"

//===================================================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//===================================================================================================

CDlgMergeDir::CDlgMergeDir(CWnd* pParent /*=NULL*/) : CDialog(CDlgMergeDir::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMergeDir)
	//}}AFX_DATA_INIT
}

void CDlgMergeDir::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMergeDir)
	//}}AFX_DATA_MAP	
}

BEGIN_MESSAGE_MAP(CDlgMergeDir, CDialog)
	//{{AFX_MSG_MAP(CDlgMergeDir)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_CHECK_FIRST, OnBnClickedCheckFirst)
	ON_BN_CLICKED(IDC_CHECK_SECOND, OnBnClickedCheckSecond)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgMergeDir::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
}

BOOL CDlgMergeDir::OnInitDialog() 
{
	CDialog::OnInitDialog();
	return TRUE;
}

void CDlgMergeDir::IntiFeature(ProFeature feat)
{
	 m_feat.id = feat.id;
	 m_feat.type = feat.type;
	 m_feat.owner = feat.owner;
}

void CDlgMergeDir::OnBnClickedCheckFirst()
{
	ChangeMergeDir(m_feat, 0);
	InvalidateDrawing();
}

void CDlgMergeDir::OnBnClickedCheckSecond()
{
	ChangeMergeDir(m_feat, 1);
	InvalidateDrawing();
}
