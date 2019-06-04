// EmpUDPTcpIp.cpp: implementation of the CEmpUDPTcpIp class.
// (c) EMP.2001, by Le Ngoc Thanh. University of Economics.
// HCM.city
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmpTcpIp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEmpUDPTcpIp::CEmpUDPTcpIp()
{
	InitOK = 0;
	MyDomainName = NULL;
	sph = NULL;	// nothing to send
}

BOOL CEmpUDPTcpIp::ProtocolInit()
{
	char	hname[1025];
	HOSTENT	*hinfo;
	unsigned long sz=1024;
	WORD	version = MAKEWORD(1,1);

	if (WSAStartup(version, &mydata)) return FALSE;

	InitOK = 1;

	if (gethostname(hname, 1024) == SOCKET_ERROR ) return FALSE;
	hinfo = gethostbyname(hname);
	if (hinfo == NULL) return FALSE;

	InitOK = 2;

	GetComputerName(hname, &sz);

	MyName = new char[sz+1];
	strcpy(MyName, hname);
	memcpy(&MyAddress, hinfo->h_addr_list[0], sizeof(in_addr));
	MyDomainName = new char[strlen(hinfo->h_name)+1];
	strcpy(MyDomainName, hinfo->h_name);

	s_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (s_sock == INVALID_SOCKET) return FALSE;
	r_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (r_sock == INVALID_SOCKET) return FALSE;

	InitOK = 3;
	return TRUE;
}

BOOL CEmpUDPTcpIp::IsOK()
{
	return (InitOK == 3);
}

CEmpUDPTcpIp::~CEmpUDPTcpIp()
{
	switch (InitOK) {
	case 3:
		// all network jobs was well done
		// To keep all pending information
//		shutdown(s_sock, SD_SEND);
//		shutdown(r_sock, SD_RECEIVE);
		closesocket(s_sock);
		closesocket(r_sock);
		// free pending send information
		while (sph != NULL) {
			sptmp = sph;
			sph = sph->next;
			delete sptmp;
		}
	case 2:
		// all names was well gotten.
		delete MyDomainName;
		delete MyName;
	case 1:
		// first step was done - winsock init OK
		WSACleanup();
	case 0:
		// winsock is not started yet
		break;
	}
}

char* CEmpUDPTcpIp::TCPIPDescription()
{
	return mydata.szDescription;
}

char* CEmpUDPTcpIp::SystemStatus()
{
	return mydata.szSystemStatus;
}

BOOL CEmpUDPTcpIp::SetReceiveSockAddr(u_short port, in_addr* addr)
{
	r_sockaddr.sin_family = AF_INET;
	r_sockaddr.sin_port   = htons(port);
	memcpy(&(r_sockaddr.sin_addr), addr, sizeof(in_addr));
	return (bind(r_sock, (sockaddr*) &r_sockaddr, 
				 sizeof(r_sockaddr)) != SOCKET_ERROR);
}

BOOL CEmpUDPTcpIp::SetReceiveSockAddr(u_short port, u_long addr)
{
	r_sockaddr.sin_family = AF_INET;
	r_sockaddr.sin_port   = htons(port);
	r_sockaddr.sin_addr.s_addr = htonl(addr);
	return (bind(r_sock, (sockaddr*) &r_sockaddr, 
				 sizeof(r_sockaddr)) != SOCKET_ERROR);
}

void CEmpUDPTcpIp::SetWindowProc(HWND hwnd, UINT s_mess, UINT r_mess, UINT gethot_mess)
{
	m_hWnd = hwnd;
	idm_send = s_mess;
	idm_receive = r_mess;
	idm_gethost = gethot_mess;
	WantSend();
	WantReceive();
}

void CEmpUDPTcpIp::SendTo(BOOL to_all, u_short port, char *IPaddr, char *sbuf, int sbuflen)
{
	in_addr tmpAddr;
	tmpAddr.S_un.S_addr = inet_addr(IPaddr);
	SendTo(to_all, port, &tmpAddr, sbuf, sbuflen);
}

void CEmpUDPTcpIp::SendTo(BOOL to_all, u_short port, in_addr* addr, char *sbuf, int sbuflen)
{
	if (sbuflen <= 0) return;
	BOOL how = (sph == NULL);
	sptmp = new (struct SENT_CHAINE);
	sptmp->next = NULL;
	memcpy(&(sptmp->addr), addr, sizeof(in_addr));
	sptmp->buf = new char[sbuflen];
	memcpy(sptmp->buf, sbuf, sbuflen);
	sptmp->buflen = sbuflen;
	sptmp->port = port;
	sptmp->to_all = to_all;
	if (how) sph = sptmp;
	else spc->next = sptmp;
	spc = sptmp;
	if (how) WantSend();
}

void CEmpUDPTcpIp::WantSend()
{
	WSAAsyncSelect(s_sock, m_hWnd, idm_send, FD_WRITE);
}

void CEmpUDPTcpIp::WantReceive()
{
	WSAAsyncSelect(r_sock, m_hWnd, idm_receive, FD_READ);
}

BOOL CEmpUDPTcpIp::CanSend()
{
	int rs;
	if (sph == NULL) return FALSE;

	if (setsockopt(s_sock, SOL_SOCKET, SO_BROADCAST, 
			(char*)&(sph->to_all),sizeof(BOOL)) != SOCKET_ERROR) {
		s_sockaddr.sin_family = AF_INET;
		s_sockaddr.sin_port = htons(sph->port);
		if (sph->to_all)
			s_sockaddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
		else
			memcpy(&(s_sockaddr.sin_addr), &(sph->addr),sizeof(in_addr));
		rs = sendto(s_sock, sph->buf, sph->buflen, 0,
					(SOCKADDR*)&s_sockaddr, sizeof(s_sockaddr));
		if (rs > 0 && rs != SOCKET_ERROR) {
			sptmp = sph;
			sph = sph->next;
			delete sptmp->buf; delete sptmp;
			if (sph != NULL) WantSend();
			return TRUE;
		}
		if (sph != NULL) WantSend();
	}
	return FALSE;
}

BOOL CEmpUDPTcpIp::CanReceive(char* buf, int buflen)
{
	int rs;
	last_r_sockaddr_size = sizeof(SOCKADDR_IN);
	rs = recvfrom(r_sock, buf, buflen, 0, 
			(sockaddr*)&last_r_sockaddr, &last_r_sockaddr_size);
	return (rs > 0 && rs != SOCKET_ERROR);
}

BOOL CEmpUDPTcpIp::GetHostInfo(char* hname)
{
	hGetHost = WSAAsyncGetHostByName(m_hWnd, idm_gethost, hname,
						GetHostInFoBuf, MAXGETHOSTSTRUCT);
	return (hGetHost != NULL);
}

BOOL CEmpUDPTcpIp::GetHostInfo(in_addr* haddr)
{
	hGetHost = WSAAsyncGetHostByAddr(m_hWnd, idm_gethost,
						(char*) haddr, sizeof(in_addr), AF_INET,
						GetHostInFoBuf, MAXGETHOSTSTRUCT);
	return (hGetHost != NULL);
}

char* CEmpUDPTcpIp::GetHostInfoAddress()
{
	PHOSTENT pht = (PHOSTENT) GetHostInFoBuf;
	return pht->h_addr_list[0];
}

char* CEmpUDPTcpIp::GetHostInfoAlias()
{
	PHOSTENT pht = (PHOSTENT) GetHostInFoBuf;
	if (*(pht->h_aliases) == NULL)
		return pht->h_name;				// no alias

	return *pht->h_aliases;
}

// To test whether GetHotInfo completted successful or not
BOOL CEmpUDPTcpIp::GetHostInfoError(WPARAM hwnd, LPARAM result)
{
	return (WSAGETASYNCERROR(result) != 0 || 
						hwnd != (WPARAM) hGetHost);
}

char* CEmpUDPTcpIp::GetHostInfoIPAddress()
{
	PHOSTENT pht = (PHOSTENT) GetHostInFoBuf;
	return inet_ntoa(*((in_addr*)pht->h_addr_list[0]));
}

char* CEmpUDPTcpIp::GetHostInfoName()
{
	PHOSTENT pht = (PHOSTENT) GetHostInFoBuf;
	return pht->h_name;
}

void CEmpUDPTcpIp::GetHostInfoStop()
{
	if (hGetHost != NULL) {
		WSACancelAsyncRequest(hGetHost);
		hGetHost = NULL;
	}
}

char* CEmpUDPTcpIp::GetMyAddress()
{
	return (char*) &(this->MyAddress);
}

char* CEmpUDPTcpIp::GetMyDomainName()
{
	return this->MyDomainName;
}

char* CEmpUDPTcpIp::GetMyIPAddress()
{
	return inet_ntoa(MyAddress);
}

char* CEmpUDPTcpIp::GetMyName()
{
	return this->MyName;
}