#pragma once

//���ݳ���
#define UDP_DATA_LEN	8192
#include "afxsock.h"
typedef enum TRANSTYPE
{
	TRANS_NONE					= 0,				//��ʼ״̬
	TRANS_REQUEST_ORDER			= 1,				//���������󶩵�����					( ROBOT -> RC2 )
	TRANS_ORDER_DATA			= 2,				//RC2���������ݴ����������				( RC2 -> ROBOT )
	TRANS_ORDER_DATA_RET		= 3,				//�����˷����յ��������ݵ�ȷ����Ϣ		( ROBOT -> RC2 )

	TRANS_GAME_DATA				= 10,				//�����˷��ض�������Ϸ����				( ROBOT -> RC2 ) 
	TRANS_GAME_DATA_RET			= 11,				//RC2ȷ���յ������˵���Ϸ����			( RC2 -> ROBOT )

	TRANS_GOODS_STAUTS			= 12,				//�����˷���ִ�е�����״̬				( ROBOT -> RC2 )
	TRANS_GOODS_STATUS_RET		= 13,				//���߻������յ� TRANS_GOODS_STAUTS ��Ϣ( RC2 -> ROBOT )

	TRANS_ORDER_END				= 50,				//����������ɣ������ƽ���				( ROBOT -> RC2 )
	TRANS_ORDER_END_RET			= 51,				//���߻������յ� TRANS_ORDER_END ��Ϣ	( RC2 -> ROBOT )

	TRANS_ORDER_CANCEL			= 52,				//���볷��								( ROBOT -> RC2 )
	TRANS_ORDER_CANCEL_RET		= 53,				//���߻������յ� TRANS_ORDER_CANCEL ��Ϣ( RC2 -> ROBOT )

	TRANS_ORDER_ABOLISH			= 54,				//ȡ������								( ROBOT -> RC2 )
	TRANS_ORDER_ABOLISH_RET		= 55,				//���߻������յ� TRANS_ORDER_ABOLISH ��Ϣ( RC2 -> ROBOT )

	TRANS_ORDER_OP				= 56,				//ת�˹�								( ROBOT -> RC2 )
	TRANS_ORDER_OP_RET			= 57,				//���߻������յ� TRANS_ORDER_OP ��Ϣ	( RC2 -> ROBOT )

	TRANS_SEND_MAIL             = 36,				//������ͬ���ʼ�
	TRANS_INSERT_ORDER          = 40,				//���붩��								( RC2 -> ROBOT )
	TRANS_REQ_ALLOC_ORDER_RET	= 41,				//�յ����붩��							( ROBOT -> RC2 )

	TRANS_REBOOTCOMPUT			= 60				//������������������					( ROBOT -> RC2 )
};

//���ݽṹ
typedef struct 
{
	int  type;				//��������	(  0: ����  10:״̬  20:�쳣  )
	char szOrderNo[50];		//�������  
	char szData[4096];		//��ϸ���� 
}UDP_DATA, *PUDP_DATA;


/////////////////////////////////////////////////////////////////////////
//
//   UDPͨѶ��
//

class CUDPSocket : public CSocket
{
public:
	CUDPSocket(void);
	~CUDPSocket(void);

	//�������ݰ�
	virtual void OnReceive(int nErrorCode);

public:
	//��ʼ��SOCKET
	BOOL InitSocket();
	//�ر�SOCKET
	BOOL CloseSocket();

	//�������ݰ�
	BOOL SendData( UINT nUDPPort, PUDP_DATA pUDPData );

public:
	//��ʼ��״̬
	BOOL	m_bInit;
	//�򿪵Ķ˿�
	UINT	m_nUDPPort;
	//�������ݻ�����
	char	m_sendData[UDP_DATA_LEN];
	//�������ݻ�����
	char	m_recvData[UDP_DATA_LEN];	
};
