#include "StdAfx.h"
#include "UDPSocket.h"

CUDPSocket::CUDPSocket(void)
{
	m_bInit = FALSE;
	m_nUDPPort = 0;

	memset ( m_sendData, 0, sizeof( m_sendData ) );
	memset ( m_recvData, 0, sizeof( m_recvData ) );
}

CUDPSocket::~CUDPSocket(void)
{
}

//初始化SOCKET
BOOL CUDPSocket::InitSocket()
{
	if ( m_bInit ) 
		return FALSE;
	if ( !Create( m_nUDPPort, SOCK_DGRAM ) ) 		
		return FALSE;
	m_bInit = TRUE;
	CString strTmpAddr;
	GetSockName(strTmpAddr, m_nUDPPort);
	return TRUE;
}

//关闭SOCKET
BOOL CUDPSocket::CloseSocket()
{
	if ( !m_bInit ) 
		return FALSE;
	CAsyncSocket::Close();
	m_bInit = FALSE;
	m_nUDPPort = 0;
	return TRUE;
}

//接收数据包
void CUDPSocket::OnReceive(int nErrorCode)
{
	int nLen = 0;
	nLen = sizeof ( m_recvData );
	memset ( m_recvData, 0, nLen );
	nLen = Receive ( m_recvData, nLen );
	if ( nLen == -1 ) 
		return;
	//PUDP_DATA pData = ( PUDP_DATA ) m_recvData;
}

//发送数据包
BOOL CUDPSocket::SendData( UINT nUDPPort, PUDP_DATA pUDPData )
{
	SendTo( pUDPData, sizeof ( UDP_DATA ), nUDPPort, _T("127.0.0.1"), 0 );
	return TRUE;
}
