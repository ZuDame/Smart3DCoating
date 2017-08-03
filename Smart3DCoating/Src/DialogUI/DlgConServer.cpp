//===================================================================================================
#include "stdafx.h"
// #include "BpRegKey.h"
#include "DlgConServer.h"

//===================================================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//===================================================================================================

CDlgConServer::CDlgConServer(CWnd* pParent)	: CDialog(CDlgConServer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgConServer)

	//}}AFX_DATA_INIT
	m_bAutoConServer = FALSE;
}

void CDlgConServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConServer)
	DDX_Control(pDX, IDC_IPA_SERVERIP, m_ipaServer);
	DDX_Control(pDX, IDC_IPA_SERVERIP2, m_ipaServer2);
	DDX_Text(pDX, IDC_SERVER_PORT, m_nServerPort);
	DDX_Text(pDX, IDC_CLIENT_PORT, m_nClientPort);
	DDV_MinMaxInt(pDX, m_nServerPort, 0, 65535);
	DDV_MinMaxInt(pDX, m_nClientPort, 0, 65535);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_AUTO, m_bAutoConServer);
}

void CDlgConServer::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	ProWindowRefresh(PRO_VALUE_UNUSED);	
}

BEGIN_MESSAGE_MAP(CDlgConServer, CDialog)
	ON_WM_MOVE()
	//{{AFX_MSG_MAP(CDlgConServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConServer message handlers

BOOL CDlgConServer::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CBpRegKey rk;
	CString strPath = APABI_USERINFO_KEYROOT;
	strPath += REGISTRY_KEY_OPTION;
	if (rk.Open(HKEY_CURRENT_USER, strPath) == ERROR_SUCCESS)
	{
		rk.Read(REGISTRY_KEY_CONSERVER, m_bAutoConServer);
		rk.Close();
	}

	// 载入上次连接的服务器信息
	LoadServerData(m_clientInfo);

	m_nServerPort = (htons)(m_clientInfo.serverport);
	m_nClientPort = (htons)(m_clientInfo.clientport);
	m_nServerIp = (htonl)(m_clientInfo.serverip);
	m_nServerIp2 = (htonl)(m_clientInfo.serverip2);

	m_ipaServer.SetAddress(m_nServerIp);
	m_ipaServer2.SetAddress(m_nServerIp2);

	UpdateData(FALSE);

	return TRUE;  	              
}

void CDlgConServer::OnOK() 
{
	UpdateData(TRUE);

	// 保存服务器的信息
	m_ipaServer.GetAddress(m_nServerIp);
	m_clientInfo.serverip = htonl(m_nServerIp);

	m_ipaServer2.GetAddress(m_nServerIp2);
	m_clientInfo.serverip2 = htonl(m_nServerIp2);

	m_clientInfo.serverport = (htons)((unsigned short)m_nServerPort);
	m_clientInfo.clientport = (htons)((unsigned short)m_nClientPort);

	g_bAutoConServer = m_bAutoConServer;
	WriteOptionInfo();

	CDialog::OnOK();
}