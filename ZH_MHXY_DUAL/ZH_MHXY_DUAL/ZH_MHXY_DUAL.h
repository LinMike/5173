#pragma once

#include "resource.h"
#include "atlimage.h"
//
#define _TEST

//等待[收到接收确认]的[次数]和[每次间隔]
#define WAIT_RECVACK_TIMES    3
#define WAIT_RECVACK_INTERVAL 100

//udp端口
UINT the_nRC2Port;
#define m_UDPPORT			6801

const CString GameName="梦幻西游";





BOOL bCloseGame=FALSE;
bool IsAnswer1=false,IsAnswer2=false;
#ifdef _TEST
#else
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif

class CGTRYLZT
{
	//构造函数与析构函数
public:
	CGTRYLZT()
	{	
		the_strOrderNo="";
	}
	~CGTRYLZT(){}

public:
	bool YLZTInit();
	//游戏开始
	BOOL GameStart();
	//读取订单明细
	BOOL ReadOrderDetail( CString strOrderData );

	//生成截图文件名
	CString GetCaptureFileName(CString strGameName);
	//发送UDP数据
	//void SendTransData(int type, const CString &strData); 
	void SendData(int type, const CString &strData,CString strIP,UINT nPort);
	void SendTransData(int type, const CString &strData,CString me_OrdeNo="",UINT nPort=the_nRC2Port);

	//接收UDP数据
	PUDP_DATA RecvTransData();
	//发送物流状态
	void SendTransGoodsStauts(int nStat);
	//发送订单状态
	void SendTransOrderStauts(int nFlag);
	//请求订单数据
	BOOL RequestOrderData();
	//发送游戏PID和HWND
	BOOL SendPidAndHwnd(DWORD dwPid, HWND hWnd);
	//发送半自动新日志状态
	BOOL SendNewLogState(LPCTSTR lpszFRCStateName, int nFRCState, LPCTSTR lpszFOrderLog = "");
	//半自动-主函数
	BOOL GameMain();


	//等待启动游戏
	int WaitStartGame();

	//等待进入游戏
	int WaitEnterGame();
	//将订单金额转换成文
	BOOL GetOrderNum();


	//选区选服
	int SelectServer();

	//输入帐号密码
	BOOL EnterAccPwd();
	//选择角色
	int SelectRole();


	int GetTextNum(CString strText,int left,int top,int right,int bottom);

	//大写锁
	bool CloseCapslockKey();
	//检查输入法,大写锁
	bool CheckIM();
	int WaitGameProess(CString strPidName,CString strPartTitle,int waitTime=30);
	int WaitWindowMiss();
	BOOL QuickStart();
	void SendPastKey();
	//读取一行颜色值
	BOOL CheckLineColor(COLORREF color0,POINT ptStart,int width);
	CString CheckCoin(HWND hWnd,int left ,int top,int right ,int bottom,CString strType);
	//自动输入字符(不安全)
	BOOL EnterString( CString strInput, DWORD dwInputTimer=100 );


	//核对服务器
	BOOL CheckServer();
	//操作二级密码
	BOOL SecondPSW(BOOL bFirst=FALSE);
	//安全时间
	int GetSafeTime();
	//int WaitSafeTime(int safeTime);
	//角色死亡
	BOOL RoleDied();
	void WriteToFile(CString log,...);
	//初始化日志
	BOOL InitLogFile( );
	BOOL PressBox();
	void CaptureJpg(CString strInfo,HWND hwnd=NULL);
	void CaptureJpgInRect(CString strPicName,int left,int top,int right,int bottom,BOOL bWaterMark=FALSE,COLORREF WaterMarkColor=0,int WaterMarkLeft=0,int WaterMarkTop=0,int WaterMarkRight=0,int WaterMarkBottom=0,
		int WaterMarkLeft2=0,int WaterMarkTop2=0,int WaterMarkRight2=0,int WaterMarkBottom2=0,
		int WaterMarkLeft3=0,int WaterMarkTop3=0,int WaterMarkRight3=0,int WaterMarkBottom3=0);
	void CaptureJpgInRect2(CString strPicName,int left,int top,int right,int bottom,BOOL bWaterMark=FALSE,COLORREF WaterMarkColor=0,int WaterMarkLeft=0,int WaterMarkTop=0,int WaterMarkRight=0,int WaterMarkBottom=0,
		int WaterMarkLeft2=0,int WaterMarkTop2=0,int WaterMarkRight2=0,int WaterMarkBottom2=0,
		int WaterMarkLeft3=0,int WaterMarkTop3=0,int WaterMarkRight3=0,int WaterMarkBottom3=0);
	void IsClosePc(int Status);
	CString RequestSafeCardInfo(int CodeType,CString ImagePath,CString Explain,int time,CString OldResult="");
	CString MailContent();




	void CloseGame();
	BOOL  PicExsit(int num);
	BOOL ComparePic(int pic1,int pic2);
	void CloseAllWindow();
	BOOL CheckAccount();







	//重启电脑
	void RestartComputerOfPower();
	//修改游戏分辨率
	void DisplaySetting();
	//插入订单请求
	BOOL InsertOrderData();

	BOOL GetMiBaoNumFromString(CString strZuoBiao);
	BOOL GetMiBaoNumFromImage(CString strZuoBiao);
	BOOL GetMiBaoZuoBiao(POINT pt);
	BOOL IsNumberS(CString strNum);
	//1开启密保卡保护,2开启任意保护,0关闭保护
	BOOL CheckSafeMode(BOOL bOpen);
	CString CheckMiBaoPic(HWND hWnd,int left ,int top,int right ,int bottom);
	void YiJiao(int nStatus);//移交订单
	//勾选密保

	BOOL CheckPicSame(CString Pic,CString Pic2);
	//生成截图时间后缀及结尾
	CString SetPicPath(CString str,CString OrderNo,CString strInfo);




	//移动到交易地点 大理仓库
	BOOL MoveToTrad(HWND hWnd);
	//发起交易 (句柄，密码，第几次发)
	//BOOL SendTrad(HWND hWnd,int theI=0);
	//输入二级密码(句柄，密码，第几次)
	int OpenLock(HWND hWnd,CString pwd);

	//选中买家
	//BOOL SelectBuyer(HWND hWnd,CString MissText);
	//从聊天框选定买家
	//BOOL SelectBuyer();
	//交易
	int Trading();
	//换元宝票 （第三参数1-5000之间）
	//BOOL ChangeYuanBao(HWND hWnd,CString pwd,int Shu);
	//交易
	int Trade();
	int Hero();
	//查看充值点数
	//int CheckChongZhi();

	//配置游戏区服
	BOOL ConfigServer();
	//循环截图
	bool CapturePictureByText(CString strName,CString strText1,CString strText2,int X,int Y,int Row,int Column,int IntervalX,int IntervalY,BOOL bAll=FALSE,int Width=0,int left=0,int top=0,int right=0,int bottom=0,BOOL bMoveMouse=FALSE);
	//循环截图
	bool CaptureGamePictureInRect(CString strName,int x,int y,int Row,int Column,int IntervalX,int IntervalY,BOOL bAll=FALSE,int Width=0,int left=0,int top=0,int right=0,int bottom=0,BOOL bMoveMouse=FALSE);
	bool CaptureGamePictureInRect2(CString strName,int x,int y,int Row,int Column,int IntervalX,int IntervalY,BOOL bAll=FALSE,int Width=0,int left=0,int top=0,int right=0,int bottom=0,BOOL bMoveMouse=FALSE);
	//生成图片
	int SaveBigPic(HWND hWnd,CString strPicName,CString strPicID="",int picWidth=0);
	int SaveBigPic(CString strPicName,CString strPicID,CString strPath);
	//截取图片
	bool PhotoToSmallPic(BYTE *&tData,POINT pt,int Sweight,int Sheight);
	//拼图
	int PinTu(CString strAllPic,CString strPicID="",int picWidth=0);
	int PinTu(CString strAllPic,CString strPicID,CString path);
	CString SetPicName(CString strPath,CString strPicID="");
	//验证码

	void GetPlanTime(int nIntervalMinute);
	BOOL SetKeybord();
	//判断是否被顶号 
	BOOL NetBreak();
	void KillProcess();
	//生成图片(竖排)
	int SaveBMPVertical(HWND hWnd,CString strPicName,CString strPicID="",CString PicPath="");
	CString FileExist(CString strName);
	void ClearAppData();
	int WebPage();
	void DragMouse(HWND hwnd,int fromX,int fromY,int toX,int toY);

	//声明方法
	void CapturePackage(HWND m_hGameWnd);
	void CapturePlayerInfo(HWND m_hGameWnd);
	void CaptureSell(HWND m_hGameWnd);
	void CaptureSafe(HWND m_hGameWnd);
	void CaptureSkill(HWND m_hGameWnd);
	void CapturePet(HWND m_hGameWnd);

public:

	//读取XML内容
	CString m_strOrderData;
	//游戏INI文件内容
	CString m_strGameIniData;
	//游戏主窗口
	HWND m_hGameWnd;
	//游戏配置文件目录
	CString m_strGameIniPath;
	//半自动程序路径
	CString m_strProgPath;
	//原图 路径
	CString m_strPicPath;


	int m_initBag;
	//需要发送的游戏币
	CString m_sendNum;
	CString m_Tax;//税
	BOOL m_bNotOpen;
	CString m_strRoleNum;
	BOOL bRoleCorrect;
	int ErrorTimes;
	BOOL bRoleNumError;
	CString m_strMiBao;
	BOOL m_bHasYXB;
	CString m_strResult;//接收验证码
	//CString m_strLastResult;
	//int m_nPort2RC2;//人工干预开关(RC2端口号)
	//BOOL m_bHuman;//服务器同意人工干预
	//服务器收到移交指令
	BOOL m_bOrderEndRet;

	//int NowOrderNo;
	//CStringArray the_arrStrOrderNo;
	//CStringArray arrReturnStatus;
	//订单号
	CString the_strOrderNo;

	CString m_strPidLst;

	int m_RoleNum;
	int m_nInitSafeTime;
	int m_nPicNum;
	CString m_strLastName;
	int m_nMIN_PIC_NUM;
	BOOL m_bClosePC;
	POINT m_ptHero;
	int m_nHero;

	BOOL bSkin;
	CString m_strTGP;
	BOOL m_bProcessMiss;
	BOOL m_bNew;
	CString m_strPcIpadd;



public:
	/*****订单具体内容*****/
	CString m_strGameName;
	//游戏进程名
	CString m_strProcessName;
	//游戏进程号
	DWORD m_dwProcessId;
	//已经使用的进程号列表
	CString m_OpenedPidLst;
	//是否打开新游戏
	BOOL m_bOpenNewGame;
	//游戏路径
	CString m_strGamePath;
	//游戏执行文件
	CString m_strGameStartFile;
	CString m_strGameStartFileYGX;
	//游戏帐号
	CString m_strAccount;
	//账号后缀
	CString m_strAccountType;
	//游戏密码
	CString m_strPassword;
	//区
	CString m_strArea;
	//服务器
	CString m_strServer;
	//角色名
	CString m_strSellerRole;

	//金额
	CString m_SellerNum;
	//交易类型
	//CString m_OfferType;

	//是否含有密保卡
	BOOL m_bHasPasspod;
	//二级密码
	CString m_strSecondPwd;
	//交易员
	CString m_strDeliverOpId;
	//截图目录
	CString m_strPictureDir;
	//拼图目录
	CString m_strCapturePath;
	//交易员部门
	CString m_strOPDN;
	//订单号
	CString m_strOrderNo;
	//交易类型 货币单位
	CString m_SellerItemId;
	CString m_strMbkImage;
	CString m_strMbkString;
	CString m_strMbkID;
	BOOL m_bHasMibao;
	CArray<CPoint,CPoint&> m_ptArrayYBP;
	CArray<CPoint,CPoint&> m_ptArraySpace;
	int m_nBagSpace;
	BOOL IsLockOn;
	int nBuyerOnline;//1名字在线,2ID在线,3名字离线;4ID离线,5角色小于10级,无法查找
	BOOL bSetBuyer;//TRUE设为私聊,2小于10级直接粘贴名字
	UINT m_nYBP;
	BOOL bAddFriend;//小号添加信任
	//CString m_strStatus;//状态
	CString m_strCangKu;
	CString m_strZhenShou;
	CString m_strXiuLian;
	CString m_strPlanTime;

	//截图字号
	int m_nFontSize1,m_nFontSize2;
	RECT rectPY;
	CString m_strOrderType;
	int m_nRoleNum;
	CString m_strRoleLevel;

public:


	static struct CoinStruct
	{
		int x;
		CString no;
	};
	struct GameInfo
	{
		CString Map;
		int Level;
		CString BagD;
		int BagP;
		int CangkuP;	
	}GAMEINFO;
	BOOL m_bBuyerError;//角色名含有0


	//人工干预返还RC2数据
	CString m_strRC2Data;
	BOOL m_bHumanFinish;
	//人工干预
	//void SendTransDataRC2(int type, const CString &strData,CString me_OrdeNo="");

};
//屏幕分辨率
const int CYSCREEN=GetSystemMetrics (SM_CYSCREEN);
const int CXSCREEN=GetSystemMetrics (SM_CXSCREEN);

//装备截图尺寸
const int Lwidth=1280;
const int Lheight=2200;
int nZHPicWidth=880;

CGTRYLZT theApp;
CMyClass myApp;

//通信
CAsyncSocket theUDPSocket;
CFile m_file;


BOOL agreeMail=FALSE;
//运行时间
int AppRunTime;
int LastUdpMsg;

BYTE bgR,bgG,bgB;


BOOL bOrderEnd=FALSE;

int nInputFail=0;
int nDiaoXian=0;
int nProcessError=0;
//请求服务器订单数据
BOOL CGTRYLZT::RequestOrderData()
{
	if (the_nRC2Port!=0)
	{	//正式订单
		Sleep(300);
		CString strSendData;
		//PUDP_DATA pRecvUDPData;
		m_strOrderData="";
		strSendData.Format( "FExeProcID=%d\r\nFRobotPort=%d\r\n", GetCurrentProcessId(), m_UDPPORT );
		SendTransData( TRANS_REQUEST_ORDER ,strSendData );
		for ( int i = 0; i < 30; i++ )
		{
			if(m_strOrderData.GetLength()>100)
				return true;
			Sleep(100);
		}
	}
	else//读本地数据
	{
		CString strOrderFile = m_strProgPath + _T("\\info.txt");
		//::MessageBox (NULL,strOrderFile,"",MB_OK);
		if ( !PathFileExists(strOrderFile) )
		{

			TRACE( _T("==========> 文件[%s]不存在 <==========\n"), strOrderFile );
			//return false;
		}
		else
		{
			m_strOrderData =myApp.ReadFile(strOrderFile);
			WriteToFile(_T(the_strOrderNo + " 读取本地文件\r\n" ));
			if (m_strOrderData != "")
				return true;
		}
	}
	WriteToFile(_T(the_strOrderNo + "请求数据失败\r\n"));
	return false;
}
//写日志
void CGTRYLZT::WriteToFile(CString log,...)
{
	if(log.Right(2)!="\r\n")
		log+="\r\n";

	CString strTime = _T("");
	CTime curTime = CTime::GetCurrentTime();
	strTime.Format("%s ", curTime.Format(_T("%Y-%m-%d %H:%M:%S")));

	CString strLog;
	va_list pArgList ;
	va_start (pArgList, log) ;
	_vsntprintf ( strLog.GetBuffer(MAX_PATH), MAX_PATH,log, pArgList);
	strLog.ReleaseBuffer();
	va_end (pArgList) ;
#ifdef _TEST
	strLog=strTime+" "+strLog;
	m_file.SeekToEnd();
	m_file.Write(strLog,strLog.GetLength());
	std::cout<<strLog;
#else
	SendTransData(18,strLog);
#endif
	Sleep(50);
}
//读取订单明细
BOOL CGTRYLZT::ReadOrderDetail( CString strOrderData )
{
	//AfxMessageBox(strOrderData);
	if ( strOrderData.IsEmpty() )
	{
		WriteToFile( _T("==========> 订单数据为空 <==========") );
		return false;
	}

	CString m_RegInfos      = myApp.FindStr( strOrderData, "<RegInfos>", "</RegInfos>" );
	CString strItem = myApp.FindStr( m_RegInfos,   "<Name>游戏账号</Name>", "</RegInfoItem>" );
	m_strAccount	= myApp.FindStr( strOrderData, "<SellerAccount>", "</SellerAccount>" );
	if(m_strAccount.IsEmpty())
		m_strAccount	= myApp.FindStr( strItem, "<Value>", "</Value>" );
	if(m_strAccount.IsEmpty())
	{
		strItem = myApp.FindStr( m_RegInfos,   "<Name>游戏帐号</Name>", "</RegInfoItem>" );
		m_strAccount	= myApp.FindStr( strItem, "<Value>", "</Value>" );
	}
	if(m_strAccount.IsEmpty())
	{
		WriteToFile( _T("帐号为空") );
		return FALSE;
	}
	strItem			= myApp.FindStr( m_RegInfos,   "<Name>游戏密码</Name>", "</RegInfoItem>" );
	m_strPassword	= myApp.FindStr( strItem, "<Value>", "</Value>" );
	strItem			= myApp.FindStr( m_RegInfos,   "<Name>游戏角色名</Name>", "</RegInfoItem>" );
	m_strSellerRole	= myApp.FindStr( strItem, "<Value>", "</Value>" );
	strItem			= myApp.FindStr( m_RegInfos,   "<Name>仓库密码</Name>", "</RegInfoItem>" );
	m_strSecondPwd	= myApp.FindStr( strItem, "<Value>", "</Value>" );
	strItem			= myApp.FindStr( m_RegInfos,   "<Name>用户类别</Name>", "</RegInfoItem>" );
	//m_strAccountType= InGame::FindStr( strItem, "<Value>", "</Value>" );
	//if(m_strAccountType.Find("@")>=0)
	//	m_strAccount+=m_strAccountType;



	m_strGameName	= myApp.FindStr( strOrderData, "<GameName>", "</GameName>" );
	m_strArea		= myApp.FindStr( strOrderData, "<GameArea>", "</GameArea>" );
	m_strServer		= myApp.FindStr( strOrderData, "<GameServer>", "</GameServer>" );
	m_strGameStartFile = myApp.FindStr( strOrderData, "<GamePath>", "</GamePath>" );
	m_strGamePath=m_strGameStartFile.Left(m_strGameStartFile.ReverseFind('\\'));


	m_strMbkID		= myApp.FindStr( strOrderData, "<Passpod_Id>", "</Passpod_Id>" );
	m_strMbkImage	= myApp.FindStr( strOrderData, "<SafeCardPath>", "</SafeCardPath>" );
	m_strMbkString	= myApp.FindStr( strOrderData, "<Passpod_Content>", "</Passpod_Content>" );
	m_strCapturePath= myApp.FindStr( strOrderData, "<CapturePath>", "</CapturePath>" );
	//WriteToFile( "拼图存放路径[%s]",m_strCapturePath);
	m_strCapturePath+="\\";
	/*if(!m_strMbkImage.IsEmpty())
	WriteToFile( "密保卡路径[%s]",m_strMbkImage);*/
	//MakeSureDirectoryPathExists(m_strCapturePath);
	//WriteToFile( "游戏名[%s]",m_strGameName);
	if(!m_strMbkString.IsEmpty())
		m_strMbkString=m_strMbkString+",";
	int num=m_strServer.Find("/");
	if(num>0)
		m_strServer=m_strServer.Left(num);

	if(!m_strMbkID.IsEmpty())
		m_bHasMibao=TRUE;
	return true;
}
//发送UDP数据
void CGTRYLZT::SendTransData(int type, const CString &strData,CString me_OrdeNo,UINT nPort)
{

	if(strData!="...")
		nProcessError=0;
	LastUdpMsg=AppRunTime;
	if (the_strOrderNo!="测试订单")
	{
		UDP_DATA *pUDPData	= new UDP_DATA;

		memset ( pUDPData, 0, sizeof( UDP_DATA ) );
		pUDPData->type = type;
		if (me_OrdeNo=="")
			strcpy_s(pUDPData->szOrderNo, the_strOrderNo);
		else
			strcpy_s(pUDPData->szOrderNo, me_OrdeNo);
		strcpy_s(pUDPData->szData,    strData);
		theUDPSocket.SendTo( pUDPData,sizeof ( UDP_DATA ),nPort, _T("127.0.0.1"), 0 );
		delete pUDPData;

	}
	return;
}
//发送UDP数据到指定IP
void CGTRYLZT::SendData(int type, const CString &strData,CString strIP,UINT nPort)
{


	UDP_DATA *pUDPData	= new UDP_DATA;

	memset ( pUDPData, 0, sizeof( UDP_DATA ) );
	pUDPData->type = type;

	strcpy_s(pUDPData->szOrderNo, the_strOrderNo);

	strcpy_s(pUDPData->szData,    strData);
	theUDPSocket.SendTo( pUDPData,sizeof ( UDP_DATA ),nPort, strIP, 0 );
	delete pUDPData;


	return;
}

//volatile BOOL m_ThreadRun[2];
//volatile BOOL m_ThreadEnd[2];
UINT Fun1Proc(LPVOID lpParameter)
{
	AppRunTime=0;
	while (1)
	{
		AppRunTime++;
		Sleep(1000);
		if (AppRunTime-LastUdpMsg>25)
		{
			theApp.SendTransData(18,"...");
			LastUdpMsg=AppRunTime;
			nProcessError++;
		}
		if(nProcessError>=5)
		{
			ExitProcess(1);
			return 1;
		}
	}
	return 1;
} //声明线程函数1
UINT Fun3Proc(LPVOID lpParameter)
{
#ifdef _TEST
	//
#else
	while(!bOrderEnd)
	{
		theApp.InsertOrderData();
		Sleep(100);
	}
#endif
	return 1;
} //声明线程函数3


//插入订单请求
BOOL CGTRYLZT::InsertOrderData()
{

	if(the_strOrderNo.IsEmpty())
		return FALSE;
	PUDP_DATA pRecvUDPData;
	pRecvUDPData = RecvTransData();
	if(pRecvUDPData->type==0)
		return FALSE;

	else if(strcmp(pRecvUDPData->szOrderNo, the_strOrderNo) != 0)
		return FALSE;
	else if(pRecvUDPData->type == TRANS_ORDER_END_RET)
		m_bOrderEndRet=TRUE;
	else if(pRecvUDPData->type == TRANS_ORDER_DATA )
		m_strOrderData = pRecvUDPData->szData;	//读取订单数据
	else if(pRecvUDPData->type == TRANS_SEND_MAIL )
		agreeMail = TRUE;
	else if (pRecvUDPData->type  == 31 /*&& m_strLastResult!=pRecvUDPData->szData*/)
		m_strResult = pRecvUDPData->szData;		//验证码
	else if(pRecvUDPData->type == 81)
		m_strRC2Data = pRecvUDPData->szData;	//人工干预返回RC2数据
	else if(pRecvUDPData->type == 82)
		m_bHumanFinish=TRUE;	//人工干预完成

	return FALSE;
}
//接收UDP数据
PUDP_DATA CGTRYLZT::RecvTransData()
{
	PUDP_DATA pUDPData;
	if (the_strOrderNo!="测试订单")
	{
		char m_recvData[UDP_DATA_LEN];
		theUDPSocket.Receive(m_recvData,UDP_DATA_LEN);
		pUDPData = ( PUDP_DATA ) m_recvData;
		m_strOrderData= pUDPData->szData ;
	}
	return pUDPData;
}
//重启电脑
void CGTRYLZT::RestartComputerOfPower()
{
	return;
	Sleep(5000);
	const int SE_SHUTDOWN_PRIVILEGE  = 0x13; 
	typedef int (*RtlAdjustPrivilege)(int,bool, bool,int*); 
	typedef int (*ZwShutdownSystem)(int); int en = 0; 
	HMODULE module = LoadLibrary("ntdll.dll"); 
	RtlAdjustPrivilege rtl = (RtlAdjustPrivilege)GetProcAddress(module, "RtlAdjustPrivilege"); 
	ZwShutdownSystem shutdown = (ZwShutdownSystem)GetProcAddress(module,"ZwShutdownSystem");    
	int nRet=rtl(SE_SHUTDOWN_PRIVILEGE,TRUE,TRUE,&en);    
	if(nRet==0x0C000007C)    
		nRet = rtl(SE_SHUTDOWN_PRIVILEGE,TRUE,FALSE,&en);         
	nRet=shutdown(1);
}
//初始化日志
BOOL CGTRYLZT::InitLogFile( )
{
#ifdef _TEST
	//////////根据日期命名日志文件///////////////
	/*SYSTEMTIME ct;
	GetLocalTime(&ct);
	CString date;
	date.Format("%02d-%02d",ct.wMonth,ct.wDay);*/
	////////////////////////////////////////////

	CString strLogFile = myApp.GetAppPath() + _T("\\RC2YLZT") + ".log";
	BOOL bLog=m_file.Open(strLogFile,CFile::modeCreate|CFile::modeReadWrite/*|CFile::modeNoTruncate*/|CFile::shareDenyNone);
	if (!bLog)
		return FALSE;
	WriteToFile("---------------------------------------------------\r\n");
#else
	////	//
#endif
	return TRUE;

}
bool CGTRYLZT::YLZTInit()
{
	m_bClosePC=FALSE;
	agreeMail=FALSE;
	ErrorTimes=0;
	bSkin=FALSE;

	m_strProgPath=myApp.GetAppPath();
	m_strPicPath = m_strProgPath+"\\"+GameName+"\\";

	//in_addr addr;
	m_strPcIpadd   =myApp.GetIPAddress(); 
	//CString m_strPcIpadd  = inet_ntoa(addr);


	WriteToFile("%s版本%s\r\n",m_strPcIpadd , myApp.GetFileVersion());


	ClearAppData();


	WinExec("rasphone -h 宽带连接",SW_SHOW);
	Sleep(1000);
	DWORD flags;
	IsNetworkAlive(&flags);

	if (flags!=3 && the_strOrderNo!="测试订单")
	{
		WriteToFile ("宽带未连接\r\n");
		WinExec("rasphone -d 宽带连接",SW_SHOW);
		for(int i=0;i<10;i++)
		{
			Sleep(400);
			if(FindWindow(NULL,"网络连接"))
			{
				PostMessage(FindWindow(NULL,"网络连接"),WM_CLOSE,0,0);
				break;
			}
		}
	}



	m_strPictureDir	= "E:\\"+GameName+"账号截图";

	if ((m_strPictureDir==""))
		return false;
	m_strGamePath+="\\";
	if(m_strPictureDir.Right(1)!="\\")
		m_strPictureDir+="\\";

	int thisNUm=0;
	char strV[256];

	GetPrivateProfileStringA("记录参数","ADSL本次做单","",strV,256,m_strProgPath + "\\adsl.ini");
	thisNUm= atoi(CString(strV)) + 1;
	CString strTemp;
	strTemp.Format ("%d",thisNUm);
	WritePrivateProfileStringA("记录参数","ADSL本次做单",strTemp,m_strProgPath + "\\adsl.ini");

	GetPrivateProfileStringA("记录参数","连续失败","",strV,256,m_strProgPath + "\\adsl.ini");



	WriteToFile ("本机第%s单,连续失败%d单\r\n",strTemp,atoi(CString(strV)));
	if(thisNUm>100 /*|| atoi(strV)>10*/)
	{
		m_bClosePC=TRUE;
		WriteToFile ("本单做完重启电脑\r\n");
	}
	//myApp.SetDisplay();


	if (!GameStart())
		return false;

	//#ifdef _TEST
	//	//
	//#else
	//	//设置截图共享目录
	//	WriteToFile( "游戏名[%s]",m_strGameName);
	//	CString strParam=m_strPictureDir+" "+m_strGameName;
	//	ShellExecute(NULL,"open", "ShareFolder.exe",strParam,m_strProgPath ,SW_SHOWNORMAL );
	//#endif
	return true;

}

void CGTRYLZT::CaptureJpg(CString strInfo,HWND hwnd)
{
	hwnd=GetDesktopWindow();
	/*if(!IsWindow(hwnd))
	hwnd=m_hGameWnd;
	if(!IsWindow(m_hGameWnd))
	hwnd=GetDesktopWindow();*/
	CString tmp=SetPicPath(m_strPictureDir,the_strOrderNo,strInfo);
	myApp.CaptureGamePicture(tmp,hwnd);
	//WriteToFile(tmp);

	CImage image;
	image.Load(tmp);
	image.Save(tmp.Left(tmp.ReverseFind('.')) + ".png",Gdiplus::ImageFormatPNG);
	//InGame::BMPToJPG(tmp,tmp.Left(tmp.ReverseFind('.')) + ".jpg");
	DeleteFile(tmp);

	return;
}
CString CGTRYLZT::SetPicPath(CString str,CString OrdNo,CString strInfo)
{
	if(str.Right(1)!="\\")
		str+="\\";
	CString strFileName;

	CTime t = CTime::GetCurrentTime();
	strFileName.Format("%s%04d_%02d_%02d\\%s\\",str,t.GetYear(), t.GetMonth(), t.GetDay(),OrdNo);
	str =strFileName;
	MakeSureDirectoryPathExists(str);
	strFileName.Format("%02d%02d%02d",t.GetHour(), t.GetMinute(), t.GetSecond() );
	/*strFileName.Format("%s%04d_%02d_%02d\\",str,t.GetYear(), t.GetMonth(), t.GetDay());
	str =strFileName;
	MakeSureDirectoryPathExists(str);

	strFileName.Format("%s_%02d%02d%02d",OrdNo,t.GetHour(), t.GetMinute(), t.GetSecond());*/
	if(!strInfo.IsEmpty())
	{
		int index=strInfo.Find("\r");
		if(index<0)
			index=strInfo.Find("\n");
		if(index>0)
			strInfo=strInfo.Left(index);
		strFileName+="_"+strInfo;
	}
	return str + strFileName+".bmp";
}
void CGTRYLZT::CaptureJpgInRect(CString strPicName,int left,int top,int right,int bottom,BOOL bWaterMark,COLORREF WaterMarkColor,int WaterMarkLeft,int WaterMarkTop,int WaterMarkRight,int WaterMarkBottom,
								int WaterMarkLeft2,int WaterMarkTop2,int WaterMarkRight2,int WaterMarkBottom2,
								int WaterMarkLeft3,int WaterMarkTop3,int WaterMarkRight3,int WaterMarkBottom3)
{

	static CString strLastName="";
	static int num=0;
	CString str=strPicName;
	if(strPicName==strLastName)
		strPicName.Format("%s%d",str,num);
	else
		num=0;
	if(strPicName.Right(4).CompareNoCase(".bmp")!=0)
		strPicName+=".bmp";
	//CTime t = CTime::GetCurrentTime();
	/*CString strFileName;
	strFileName.Format("%s%04d_%02d\\%02d\\%s\\",m_strCapturePath,t.GetYear(), t.GetMonth(), t.GetDay(),the_strOrderNo);*/
	MakeSureDirectoryPathExists(m_strCapturePath);
	//strFileName+=strPicName;
	myApp.CapturePictureInRect(m_hGameWnd,m_strCapturePath+strPicName,left,top,right,bottom,bWaterMark,WaterMarkColor,WaterMarkLeft,WaterMarkTop,WaterMarkRight,WaterMarkBottom,
		WaterMarkLeft2,WaterMarkTop2,WaterMarkRight2,WaterMarkBottom2,
		WaterMarkLeft3,WaterMarkTop3,WaterMarkRight3,WaterMarkBottom3);
	/*InGame::BMPToJPG(strFileName,strFileName.Left(strFileName.ReverseFind('.')) + ".jpg");
	DeleteFile(strFileName);*/
	strLastName=str;
	num++;
	return;
}
void CGTRYLZT::CaptureJpgInRect2(CString strPicName,int left,int top,int right,int bottom,BOOL bWaterMark,COLORREF WaterMarkColor,int WaterMarkLeft,int WaterMarkTop,int WaterMarkRight,int WaterMarkBottom,
								 int WaterMarkLeft2,int WaterMarkTop2,int WaterMarkRight2,int WaterMarkBottom2,
								 int WaterMarkLeft3,int WaterMarkTop3,int WaterMarkRight3,int WaterMarkBottom3)
{



	if(strPicName.Right(4).CompareNoCase(".bmp")!=0)
		strPicName+=".bmp";

	MakeSureDirectoryPathExists(m_strCapturePath);

	myApp.CapturePictureInRect(m_hGameWnd,m_strCapturePath+strPicName,left,top,right,bottom,bWaterMark,WaterMarkColor,WaterMarkLeft,WaterMarkTop,WaterMarkRight,WaterMarkBottom,
		WaterMarkLeft2,WaterMarkTop2,WaterMarkRight2,WaterMarkBottom2,
		WaterMarkLeft3,WaterMarkTop3,WaterMarkRight3,WaterMarkBottom3);


	return;
}
void CGTRYLZT::IsClosePc(int Status)
{
	/*if (the_strOrderNo == _T("测试订单"))
	return;*/
	int thisNUm=0,thisNUm2=0;
	char strV[256];
	CString strTemp;
	/*GetPrivateProfileStringA("记录参数","ADSL本次做单","",strV,256,"..\\adsl.ini");
	thisNUm=atoi(CString(strV));*/


	if (m_bClosePC)
	{
		WritePrivateProfileStringA("记录参数","ADSL本次做单","0",m_strProgPath + "\\adsl.ini");
		//WritePrivateProfileStringA("记录参数","连续失败","0",m_strProgPath + "\\adsl.ini");
		Sleep(500);
		WriteToFile("发送重启电脑指令\r\n");
		SendTransData (TRANS_REBOOTCOMPUT,"");
		//RestartComputerOfPower();
		return;
	}
	else
	{
		GetPrivateProfileStringA("记录参数","连续失败","",strV,256,m_strProgPath + "\\adsl.ini");
		thisNUm2=atoi(CString(strV));
		//WriteToFile("无需重启电脑\r\n");
		if (Status>1000 && Status<3000)
		{
			thisNUm2++;
			strTemp.Format ("%d",thisNUm2);

			WritePrivateProfileStringA(_T("记录参数"),"连续失败",strTemp,m_strProgPath + "\\adsl.ini");

			strTemp.Format ("%s$%s$%d$%d",m_strGameName,m_strPcIpadd,thisNUm2,Status);
			SendData( 101 ,strTemp,"192.168.60.35" ,19871);
		}
		else
		{
			WritePrivateProfileStringA("记录参数","连续失败","0",m_strProgPath + "\\adsl.ini");
			strTemp.Format ("%s$%s$0",m_strGameName,m_strPcIpadd);
			SendData( 101 ,strTemp,"192.168.60.35" ,19871);
		}
	}

}

//检查输入法,大写锁
bool CGTRYLZT::CheckIM()
{

	//关闭输入法
	if (!myApp.SelectIME())
	{
		return false;
	}
	if(!CloseCapslockKey())//判断CAPLOCK键是否打开,若打开,则弹框.
		return false;
	return true;
}
bool CGTRYLZT::CloseCapslockKey()
{
	int x = GetKeyState(VK_CAPITAL);
	if(x==1)
	{
		myApp.SendKeys(VK_CAPITAL);
		//CWinAPIKey::VKSendFunKey(VK_CAPITAL);
		Sleep(2000);

	}//判断CAPLOCK键是否打开,若打开,则关闭.

	byte keyS[256];
	::GetKeyboardState (keyS);
	//cout<<GetKeyState ( VK_CAPITAL )<<endl;
	if (/*keyS[VK_CAPITAL]*/GetKeyState ( VK_CAPITAL )!=0)
	{
		WriteToFile("请关闭大写锁\r\n");
		return false;
	}
	return true;

}
//发送验证码
CString CGTRYLZT::RequestSafeCardInfo(int CodeType,CString ImagePath,CString Explain,int time,CString OldResult)
{
	//AfxMessageBox("密保卡");
	//请求订单数据 & 接收订单数据 & 发送确认
	//	TRANS_REQ_IDCODE_RESULT  = 30,    //机器人请求GTR处理验证码               ( ROBOT -> RC2 ) 
	//TRANS_RES_IDCODE_RESULT  = 31,    //发送处理完的验证码的到机器人程序      ( RC2 -> ROBOT ) 
	//TRANS_IDCODE_INPUT_RESULT = 32,   //机器人输入验证码后的结果发送给客户端  ( ROBOT -> RC2 )
	// 
	//30 数据格式:
	//FCodeType=  答题类型(不能为空)
	//1. 文字验证码.
	//2. 密保验证码.
	//3. 坐标验证码.
	//FImageName= 验证码图片文件的全路径(不能为空)
	//FQuestion=  一些说明文本(可为空) 
	//FTimeout=   超时值(单位秒)
	if (the_strOrderNo=="测试订单")
	{
		SetForegroundWindow(GetConsoleWindow());
		std::cout << "请输入密保:";
		char a[20];
		std::cin>>a;
		Sleep(1000);
		SetForegroundWindow(m_hGameWnd);
		Sleep(1000);
		return CString(a);
	}
	SendTransData (0,"","1",m_UDPPORT);
	Sleep(1000);
	m_strResult="";
	CString strSendData;
	WriteToFile( _T("发送验证码...\r\n"));
	CString strAuthentication=m_strOrderData;
	strSendData.Format( "FCodeType=%d\r\nFImageName=%s\r\nFQuestion=%s\r\nFTimeout=%d\r\n", CodeType, ImagePath,Explain,time );
	SendTransData( 30 ,strSendData );
	Sleep(200);
	if(CodeType==1)
		IsAnswer1=true;
	else
		IsAnswer2=true;
	for (int i=0;i<time;i++)
	{
		Sleep(1000);
		if (i % 20== 15)
			WriteToFile( _T("等待验证码...\r\n"));
		if(m_strResult=="")
			continue;
		CString tmp;
		tmp.Format ("验证码返回：%s\r\n",m_strResult);
		WriteToFile( tmp);
		//m_strLastResult=m_strResult;
		//IsAnswer=true;
		return m_strResult;	
	}
	WriteToFile( _T("等待验证码超时...")) ;
	return "";
}

////发送UDP数据(RC2人工干预)
//void CGTRYLZT::SendTransDataRC2(int type, const CString &strData,CString me_OrdeNo)
//{
//	if (the_strOrderNo!="测试订单")
//	{
//		UDP_DATA *pUDPData	= new UDP_DATA;
//
//		memset ( pUDPData, 0, sizeof( UDP_DATA ) );
//		pUDPData->type = type;
//		if (me_OrdeNo=="")
//			strcpy_s(pUDPData->szOrderNo, the_strOrderNo);
//		else
//			strcpy_s(pUDPData->szOrderNo, me_OrdeNo);
//		strcpy_s(pUDPData->szData,    strData);
//		theUDPSocket.SendTo( pUDPData,sizeof ( UDP_DATA ),m_nPort2RC2, _T("127.0.0.1"), 0 );
//		delete pUDPData;
//
//	}
//	return;
//}

