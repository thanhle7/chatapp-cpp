// tucDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tuc.h"
#include "tucDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg(CWnd* parent);

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg(CWnd* parent) : CDialog(CAboutDlg::IDD, parent)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTucDlg dialog

CTucDlg::CTucDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTucDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTucDlg)
	m_net = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	anet[0] = AfxGetApp()->LoadIcon(IDC_NET0);
	anet[1] = AfxGetApp()->LoadIcon(IDC_NET1);
	anet[2] = AfxGetApp()->LoadIcon(IDC_NET2);
	anet[3] = AfxGetApp()->LoadIcon(IDC_NET3);
}

CTucDlg::~CTucDlg()
{
	
}

void CTucDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTucDlg)
	DDX_Control(pDX, IDC_MYIPADDR, m_ip);
	DDX_Text(pDX, IDC_NET, m_net);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTucDlg, CDialog)
	//{{AFX_MSG_MAP(CTucDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PEER, OnPeer)
	ON_BN_CLICKED(IDC_ALL, OnAll)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_GETHOT, OnGethot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTucDlg message handlers

BOOL CTucDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	pnet = 0; dpnet = 1;		// Icon animation

	CheckDlgButton(IDC_ALL, BST_CHECKED);

	if (!myNet.ProtocolInit()) EndDialog(IDCANCEL);
	myNet.SetWindowProc(m_hWnd, IDM_SEND, IDM_RECEV, IDM_HOTNAME);

	SetDlgItemText(IDC_HOSTADDR, inet_ntoa(*((in_addr*)myNet.GetMyAddress() )));
	wsprintf(sbuf,"%s on %s", myNet.GetMyName(), myNet.GetMyDomainName());
	SetDlgItemText(IDC_HOSTNAME, sbuf);
	SetDlgItemText(IDC_SYS1, myNet.TCPIPDescription());
	SetDlgItemText(IDC_SYS2, myNet.SystemStatus());

	myNet.SetReceiveSockAddr(port, INADDR_ANY);
	myNet.WantReceive();

	this->SetTimer(999, 400, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTucDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTucDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

LRESULT CTucDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message) {
	case IDM_SEND:
		if (LOWORD(lParam) == FD_WRITE) {
			myNet.CanSend();
		}
		break;
	case IDM_RECEV:
		if (LOWORD(lParam) == FD_READ) {
			myNet.CanReceive(rbuf, 500); 
			SetDlgItemText(IDC_RECINFO, rbuf);
			myNet.WantReceive();
		}
		break;
	case IDM_HOTNAME:
		if (myNet.GetHostInfoError(wParam, lParam))
			MessageBox("Host can't be found !");
		else {
			wsprintf(sbuf, "Name: %s \n\nAlias: %s\n\nAddr: %s\n"
							"\nClick OK to place address in send-to,"
							"\nClick No to copy address to clipboard",
							myNet.GetHostInfoName(),
							myNet.GetHostInfoAlias(),
							inet_ntoa(*((in_addr*)myNet.GetHostInfoAddress())));
			switch (MessageBox(sbuf, "Info", MB_YESNOCANCEL)) {
				case IDYES:
					m_ip.SetWindowText(inet_ntoa(*((in_addr*)myNet.GetHostInfoAddress())));
					break;
				case IDNO:
				{
					strcpy(sbuf, inet_ntoa(*((in_addr*)myNet.GetHostInfoAddress())));
					HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, strlen(sbuf)+1); 
					if (hglbCopy != NULL) {
						char* lptstrCopy = (char*)GlobalLock(hglbCopy); 
						strcpy(lptstrCopy, sbuf); 
						GlobalUnlock(hglbCopy); 

						if (::OpenClipboard(this->GetSafeHwnd())) {
							::EmptyClipboard();
							SetClipboardData(CF_TEXT, hglbCopy);
							CloseClipboard();
						}
						else GlobalFree(hglbCopy);
						break;
					}
				}
			}
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CTucDlg::OnOK()
{
	sall = (IsDlgButtonChecked(IDC_ALL) != 0);
	if (!sall) {
		m_ip.GetWindowText(sbuf, 1024);
		adr.S_un.S_addr = inet_addr(sbuf);
	}
	else adr.S_un.S_addr = htonl(INADDR_BROADCAST);

	GetDlgItemText(IDC_SENDINFO, sbuf, 1024);
	myNet.SendTo(sall, port, &adr, sbuf, strlen(sbuf)+1);
}

void CTucDlg::OnPeer() 
{
	m_ip.EnableWindow(1);	
}

void CTucDlg::OnAll() 
{
	m_ip.EnableWindow(0);
}

void CTucDlg::OnTimer(UINT nIDEvent) 
{
	((CStatic*)GetDlgItem(IDC_NET))->SetIcon(anet[pnet]);
	pnet += dpnet;
	if (pnet==0 || pnet == 3) dpnet = -dpnet;
	CDialog::OnTimer(nIDEvent);
}

void CTucDlg::OnAbout()
{
	CAboutDlg tuc(this);
	tuc.DoModal();
}

void CTucDlg::OnGethot() 
{
	if (GetDlgItemText(IDC_SENDINFO, sbuf, 500)) {
		myNet.GetHostInfo(sbuf);
	}
	else {
		m_ip.GetWindowText(sbuf, 1024);
		if (strcmp(sbuf, "0.0.0.0")) {
			adr.S_un.S_addr = inet_addr(sbuf);
			myNet.GetHostInfo(&adr);
		}
		else
			MessageBox( "Enter the domain name in edit box\n"
						"or the IP address in IP edit box\n"
						"to see such host information",
						"warning", MB_OK|MB_ICONEXCLAMATION);
	}
}
