// EmpUDPTcpIp.h: interface for the CEmpUDPTcpIp class.
// (c) EMP.2001, by Le Ngoc Thanh. University of Economics.
// HCM.city
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EmpUDPTcpIp_H__3FBEC021_5044_11D5_AAAA_0008C7331CD4__INCLUDED_)
#define AFX_EmpUDPTcpIp_H__3FBEC021_5044_11D5_AAAA_0008C7331CD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SD_RECEIVE	0x00
#define SD_SEND		0x01

class CEmpUDPTcpIp
{
public:
	CEmpUDPTcpIp();
	virtual ~CEmpUDPTcpIp();
	// for local information
	char *GetMyName(void);
	char *GetMyIPAddress(void);
	char *GetMyAddress(void);
	char *GetMyDomainName(void);

	// for host (remote station) information
	BOOL GetHostInfo(in_addr* haddr);
	BOOL GetHostInfo(char* hname);
	void GetHostInfoStop(void);
	BOOL GetHostInfoError(WPARAM hwnd, LPARAM result);
	char *GetHostInfoIPAddress(void);
	char *GetHostInfoAddress(void);
	char *GetHostInfoAlias(void);
	char *GetHostInfoName(void);

	// network initiation
	BOOL ProtocolInit(void);
	BOOL IsOK();
	char *SystemStatus(void);
	char *TCPIPDescription(void);

	// send & receive
	BOOL CanReceive(char* buf, int buflen);
	BOOL CanSend(void);
	void WantReceive(void);
	void WantSend(void);
	BOOL SetReceiveSockAddr(u_short port, u_long addr);
	BOOL SetReceiveSockAddr(u_short port, in_addr* addr);
	void SendTo(BOOL to_all, u_short port, in_addr* addr, char* sbuf, int sbuflen);
	void SendTo(BOOL to_all, u_short port, char* IPaddr, char *sbuf, int sbuflen);
	void SetWindowProc(HWND hwnd, UINT s_mess, UINT r_mess, UINT gethot_mess);
private:
	int		InitOK;
	char	GetHostInFoBuf[MAXGETHOSTSTRUCT];
	HANDLE	hGetHost;			// handle to get host job
	UINT	idm_send, idm_receive, idm_gethost;
	int		last_r_sockaddr_size;
	SOCKET	s_sock, r_sock;
	HWND	m_hWnd;
	char	*MyDomainName, *MyName;
	in_addr MyAddress;
	WSADATA mydata;
	struct SENT_CHAINE {
		BOOL	to_all;
		u_short	port;
		in_addr	addr;
		char    *buf;
		int		buflen;
		struct	SENT_CHAINE *next;
	} *sph, *spc, *sptmp;
	SOCKADDR_IN s_sockaddr, r_sockaddr, last_r_sockaddr;
};

#endif // !defined(AFX_EmpUDPTcpIp_H__3FBEC021_5044_11D5_AAAA_0008C7331CD4__INCLUDED_)
