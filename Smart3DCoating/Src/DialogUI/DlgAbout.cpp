/////////////////////////////////////////////////////////////////////////////
//  File			:   DlgAbout.cpp
//  Author			:   Yang Xuefeng
//  Date			:	2006-6-12
//  Version			:	
//  Description		:   
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PreGlobal.h"
#include "DlgAbout.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
	m_bFirst = TRUE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CAPTURECHANGED()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CAboutDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_bitmap.LoadBitmap(IDB_ABOUT))
		return FALSE;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);
	
	CRect rcFrame;
	INT	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	INT	nScreenHeight =	GetSystemMetrics(SM_CYSCREEN);
	rcFrame.left = (nScreenWidth - bm.bmWidth) / 2;
	rcFrame.top = (nScreenHeight - bm.bmHeight) / 2;
	rcFrame.bottom = rcFrame.top + bm.bmHeight;
	rcFrame.right = rcFrame.left + bm.bmWidth;
	::MoveWindow(GetSafeHwnd(), rcFrame.left , rcFrame.top, rcFrame.Width(), rcFrame.Height(), TRUE);

	return 0;
}

void CAboutDlg::OnPaint() 
{
	CPaintDC dc(this); 
	
	CDC dcImage;
	if (!dcImage.CreateCompatibleDC(&dc))
		return;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	CBitmap* pOldBitmap = dcImage.SelectObject(&m_bitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	dcImage.SelectObject(pOldBitmap);

	CPoint ptPos(15, 332);
	DisplayBuildVersionText(&dc, ptPos);
}

BOOL CAboutDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN ||
	    pMsg->message == WM_SYSKEYDOWN ||
	    pMsg->message == WM_LBUTTONDOWN ||
	    pMsg->message == WM_RBUTTONDOWN ||
	    pMsg->message == WM_MBUTTONDOWN ||
	    pMsg->message == WM_NCLBUTTONDOWN ||
	    pMsg->message == WM_NCRBUTTONDOWN ||
	    pMsg->message == WM_NCMBUTTONDOWN)
	{
		ActivateCurrentPreWindow();
		EndDialog(IDOK);
		return TRUE;
	}
	return FALSE;
}

void CAboutDlg::OnCaptureChanged(CWnd *pWnd) 
{
	PostMessage(WM_CLOSE);
	
	CDialog::OnCaptureChanged(pWnd);
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetCapture();	
	return TRUE; 
}

void CAboutDlg::OnNcPaint() 
{
	CDC*	pDC = GetWindowDC();
	CRect	rc, rcCut,rcDraw;
	GetWindowRect(&rc);
	rcDraw.top		=  0;
	rcDraw.left		=  0;
	rcDraw.right	= rc.Width() ;
	rcDraw.bottom	= rc.Height();

	CRect rcClient;
	GetClientRect(&rcClient);
	ClientToScreen(&rcClient);
	rcCut.left = rcClient.left - rc.left;
	rcCut.right = rcClient.right - rc.left;
	rcCut.top = rcClient.top - rc.top;
	rcCut.bottom = rcClient.bottom - rc.top;

	CRgn rgnCut,rgnResult;	  
	rgnCut.CreateRectRgnIndirect(&rcCut);
	rgnResult.CreateRectRgnIndirect(&rcDraw);
	rgnResult.CombineRgn(&rgnResult,&rgnCut, RGN_XOR);	
	pDC->SelectClipRgn(&rgnResult);

	pDC->FillSolidRect(rcDraw, RGB(0, 119, 158));
	ReleaseDC(pDC);
}

