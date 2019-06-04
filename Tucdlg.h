// tucDlg.h : header file
//

#if !defined(AFX_TUCDLG_H__F608A687_AD6B_11D3_BACF_0008C7331CD4__INCLUDED_)
#define AFX_TUCDLG_H__F608A687_AD6B_11D3_BACF_0008C7331CD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "winsock.h"
#include "EmpTcpIp.h"	// Added by ClassView

const port = 4000;//2050;

#define IDM_SEND                        WM_USER+1
#define IDM_RECEV                       WM_USER+2
#define IDM_HOTNAME                     WM_USER+3

/////////////////////////////////////////////////////////////////////////////
// CTucDlg dialog

class CTucDlg : public CDialog
{
// Construction
public:
	CTucDlg(CWnd* pParent = NULL);	// standard constructor
	~CTucDlg();	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTucDlg)
	enum { IDD = IDD_TUC_DIALOG };
	CIPAddressCtrl	m_ip;
	CString	m_net;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTucDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	int			sall;
	char		sbuf[1024], rbuf[1024];
	HICON		m_hIcon;
	int			pnet, dpnet;
	HICON		anet[4];
	in_addr		adr;

	// Generated message map functions
	//{{AFX_MSG(CTucDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnPeer();
	afx_msg void OnAll();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAbout();
	afx_msg void OnGethot();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CEmpUDPTcpIp myNet;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TUCDLG_H__F608A687_AD6B_11D3_BACF_0008C7331CD4__INCLUDED_)
