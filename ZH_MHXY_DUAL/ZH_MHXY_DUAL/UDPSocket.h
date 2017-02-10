#pragma once

//数据长度
#define UDP_DATA_LEN	8192
#include "afxsock.h"
typedef enum TRANSTYPE
{
	TRANS_NONE					= 0,				//初始状态
	TRANS_REQUEST_ORDER			= 1,				//机器人请求订单数据					( ROBOT -> RC2 )
	TRANS_ORDER_DATA			= 2,				//RC2将订单数据传输给机器人				( RC2 -> ROBOT )
	TRANS_ORDER_DATA_RET		= 3,				//机器人返回收到订单数据的确认信息		( ROBOT -> RC2 )

	TRANS_GAME_DATA				= 10,				//机器人返回订单的游戏数据				( ROBOT -> RC2 ) 
	TRANS_GAME_DATA_RET			= 11,				//RC2确认收到机器人的游戏数据			( RC2 -> ROBOT )

	TRANS_GOODS_STAUTS			= 12,				//机器人返回执行的物流状态				( ROBOT -> RC2 )
	TRANS_GOODS_STATUS_RET		= 13,				//告诉机器人收到 TRANS_GOODS_STAUTS 消息( RC2 -> ROBOT )

	TRANS_ORDER_END				= 50,				//订单处理完成，正常移交。				( ROBOT -> RC2 )
	TRANS_ORDER_END_RET			= 51,				//告诉机器人收到 TRANS_ORDER_END 消息	( RC2 -> ROBOT )

	TRANS_ORDER_CANCEL			= 52,				//申请撤单								( ROBOT -> RC2 )
	TRANS_ORDER_CANCEL_RET		= 53,				//告诉机器人收到 TRANS_ORDER_CANCEL 消息( RC2 -> ROBOT )

	TRANS_ORDER_ABOLISH			= 54,				//取消订单								( ROBOT -> RC2 )
	TRANS_ORDER_ABOLISH_RET		= 55,				//告诉机器人收到 TRANS_ORDER_ABOLISH 消息( RC2 -> ROBOT )

	TRANS_ORDER_OP				= 56,				//转人工								( ROBOT -> RC2 )
	TRANS_ORDER_OP_RET			= 57,				//告诉机器人收到 TRANS_ORDER_OP 消息	( RC2 -> ROBOT )

	TRANS_SEND_MAIL             = 36,				//服务器同意邮寄
	TRANS_INSERT_ORDER          = 40,				//插入订单								( RC2 -> ROBOT )
	TRANS_REQ_ALLOC_ORDER_RET	= 41,				//收到插入订单							( ROBOT -> RC2 )

	TRANS_REBOOTCOMPUT			= 60				//机器人请求重启电脑					( ROBOT -> RC2 )
};

//数据结构
typedef struct 
{
	int  type;				//数据类型	(  0: 数据  10:状态  20:异常  )
	char szOrderNo[50];		//订单编号  
	char szData[4096];		//详细数据 
}UDP_DATA, *PUDP_DATA;


/////////////////////////////////////////////////////////////////////////
//
//   UDP通讯类
//

class CUDPSocket : public CSocket
{
public:
	CUDPSocket(void);
	~CUDPSocket(void);

	//接收数据包
	virtual void OnReceive(int nErrorCode);

public:
	//初始化SOCKET
	BOOL InitSocket();
	//关闭SOCKET
	BOOL CloseSocket();

	//发送数据包
	BOOL SendData( UINT nUDPPort, PUDP_DATA pUDPData );

public:
	//初始化状态
	BOOL	m_bInit;
	//打开的端口
	UINT	m_nUDPPort;
	//发送数据缓冲区
	char	m_sendData[UDP_DATA_LEN];
	//接收数据缓冲区
	char	m_recvData[UDP_DATA_LEN];	
};
