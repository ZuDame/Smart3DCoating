/////////////////////////////////////////////////////////////////////////////
//  File			:   DlgAbout.h
//  Author			:   Yang Xuefeng
//  Date			:	2006-6-12
//  Version			:	
//  Description		:   
/////////////////////////////////////////////////////////////////////////////

#ifndef	__DLGABOUT_H_INCLUDE__
#define __DLGABOUT_H_INCLUDE__

/////////////////////////////////////////////////////////////////////////////

#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	
protected:
	CBitmap m_bitmap;
	BOOL m_bFirst;

	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	virtual BOOL OnInitDialog();
	afx_msg void OnNcPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined __DLGABOUT_H_INCLUDE__
