//===================================================================================================

#ifndef __DlgConServer_H__
#define __DlgConServer_H__

//===================================================================================================

#include "Resource.h"

//===================================================================================================


//===================================================================================================

class CDlgConServer : public CDialog
{
public:
	CDlgConServer(CWnd* pParent = NULL);   

	//{{AFX_DATA(CDlgConServer)
	enum { IDD = IDD_DLG_SERVER };
	CIPAddressCtrl	m_ipaServer;
	CIPAddressCtrl	m_ipaServer2;
	//}}AFX_DATA

public:
	SClientInfo m_clientInfo;

protected:
	unsigned long m_nServerIp;
	unsigned long m_nServerIp2;

	int m_nClientPort;
	int m_nServerPort;

	BOOL m_bAutoConServer;

protected:
	//{{AFX_VIRTUAL(CDlgConServer)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgConServer)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMove(int x, int y);
};

//===================================================================================================

#endif 
