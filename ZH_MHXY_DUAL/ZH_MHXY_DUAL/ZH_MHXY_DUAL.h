#pragma once

#include "resource.h"
#include "atlimage.h"
//
#define _TEST

//�ȴ�[�յ�����ȷ��]��[����]��[ÿ�μ��]
#define WAIT_RECVACK_TIMES    3
#define WAIT_RECVACK_INTERVAL 100

//udp�˿�
UINT the_nRC2Port;
#define m_UDPPORT			6801

const CString GameName="�λ�����";





BOOL bCloseGame=FALSE;
bool IsAnswer1=false,IsAnswer2=false;
#ifdef _TEST
#else
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif

class CGTRYLZT
{
	//���캯������������
public:
	CGTRYLZT()
	{	
		the_strOrderNo="";
	}
	~CGTRYLZT(){}

public:
	bool YLZTInit();
	//��Ϸ��ʼ
	BOOL GameStart();
	//��ȡ������ϸ
	BOOL ReadOrderDetail( CString strOrderData );

	//���ɽ�ͼ�ļ���
	CString GetCaptureFileName(CString strGameName);
	//����UDP����
	//void SendTransData(int type, const CString &strData); 
	void SendData(int type, const CString &strData,CString strIP,UINT nPort);
	void SendTransData(int type, const CString &strData,CString me_OrdeNo="",UINT nPort=the_nRC2Port);

	//����UDP����
	PUDP_DATA RecvTransData();
	//��������״̬
	void SendTransGoodsStauts(int nStat);
	//���Ͷ���״̬
	void SendTransOrderStauts(int nFlag);
	//���󶩵�����
	BOOL RequestOrderData();
	//������ϷPID��HWND
	BOOL SendPidAndHwnd(DWORD dwPid, HWND hWnd);
	//���Ͱ��Զ�����־״̬
	BOOL SendNewLogState(LPCTSTR lpszFRCStateName, int nFRCState, LPCTSTR lpszFOrderLog = "");
	//���Զ�-������
	BOOL GameMain();


	//�ȴ�������Ϸ
	int WaitStartGame();

	//�ȴ�������Ϸ
	int WaitEnterGame();
	//���������ת������
	BOOL GetOrderNum();


	//ѡ��ѡ��
	int SelectServer();

	//�����ʺ�����
	BOOL EnterAccPwd();
	//ѡ���ɫ
	int SelectRole();


	int GetTextNum(CString strText,int left,int top,int right,int bottom);

	//��д��
	bool CloseCapslockKey();
	//������뷨,��д��
	bool CheckIM();
	int WaitGameProess(CString strPidName,CString strPartTitle,int waitTime=30);
	int WaitWindowMiss();
	BOOL QuickStart();
	void SendPastKey();
	//��ȡһ����ɫֵ
	BOOL CheckLineColor(COLORREF color0,POINT ptStart,int width);
	CString CheckCoin(HWND hWnd,int left ,int top,int right ,int bottom,CString strType);
	//�Զ������ַ�(����ȫ)
	BOOL EnterString( CString strInput, DWORD dwInputTimer=100 );


	//�˶Է�����
	BOOL CheckServer();
	//������������
	BOOL SecondPSW(BOOL bFirst=FALSE);
	//��ȫʱ��
	int GetSafeTime();
	//int WaitSafeTime(int safeTime);
	//��ɫ����
	BOOL RoleDied();
	void WriteToFile(CString log,...);
	//��ʼ����־
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







	//��������
	void RestartComputerOfPower();
	//�޸���Ϸ�ֱ���
	void DisplaySetting();
	//���붩������
	BOOL InsertOrderData();

	BOOL GetMiBaoNumFromString(CString strZuoBiao);
	BOOL GetMiBaoNumFromImage(CString strZuoBiao);
	BOOL GetMiBaoZuoBiao(POINT pt);
	BOOL IsNumberS(CString strNum);
	//1�����ܱ�������,2�������Ᵽ��,0�رձ���
	BOOL CheckSafeMode(BOOL bOpen);
	CString CheckMiBaoPic(HWND hWnd,int left ,int top,int right ,int bottom);
	void YiJiao(int nStatus);//�ƽ�����
	//��ѡ�ܱ�

	BOOL CheckPicSame(CString Pic,CString Pic2);
	//���ɽ�ͼʱ���׺����β
	CString SetPicPath(CString str,CString OrderNo,CString strInfo);




	//�ƶ������׵ص� ����ֿ�
	BOOL MoveToTrad(HWND hWnd);
	//������ (��������룬�ڼ��η�)
	//BOOL SendTrad(HWND hWnd,int theI=0);
	//�����������(��������룬�ڼ���)
	int OpenLock(HWND hWnd,CString pwd);

	//ѡ�����
	//BOOL SelectBuyer(HWND hWnd,CString MissText);
	//�������ѡ�����
	//BOOL SelectBuyer();
	//����
	int Trading();
	//��Ԫ��Ʊ ����������1-5000֮�䣩
	//BOOL ChangeYuanBao(HWND hWnd,CString pwd,int Shu);
	//����
	int Trade();
	int Hero();
	//�鿴��ֵ����
	//int CheckChongZhi();

	//������Ϸ����
	BOOL ConfigServer();
	//ѭ����ͼ
	bool CapturePictureByText(CString strName,CString strText1,CString strText2,int X,int Y,int Row,int Column,int IntervalX,int IntervalY,BOOL bAll=FALSE,int Width=0,int left=0,int top=0,int right=0,int bottom=0,BOOL bMoveMouse=FALSE);
	//ѭ����ͼ
	bool CaptureGamePictureInRect(CString strName,int x,int y,int Row,int Column,int IntervalX,int IntervalY,BOOL bAll=FALSE,int Width=0,int left=0,int top=0,int right=0,int bottom=0,BOOL bMoveMouse=FALSE);
	bool CaptureGamePictureInRect2(CString strName,int x,int y,int Row,int Column,int IntervalX,int IntervalY,BOOL bAll=FALSE,int Width=0,int left=0,int top=0,int right=0,int bottom=0,BOOL bMoveMouse=FALSE);
	//����ͼƬ
	int SaveBigPic(HWND hWnd,CString strPicName,CString strPicID="",int picWidth=0);
	int SaveBigPic(CString strPicName,CString strPicID,CString strPath);
	//��ȡͼƬ
	bool PhotoToSmallPic(BYTE *&tData,POINT pt,int Sweight,int Sheight);
	//ƴͼ
	int PinTu(CString strAllPic,CString strPicID="",int picWidth=0);
	int PinTu(CString strAllPic,CString strPicID,CString path);
	CString SetPicName(CString strPath,CString strPicID="");
	//��֤��

	void GetPlanTime(int nIntervalMinute);
	BOOL SetKeybord();
	//�ж��Ƿ񱻶��� 
	BOOL NetBreak();
	void KillProcess();
	//����ͼƬ(����)
	int SaveBMPVertical(HWND hWnd,CString strPicName,CString strPicID="",CString PicPath="");
	CString FileExist(CString strName);
	void ClearAppData();
	int WebPage();
	void DragMouse(HWND hwnd,int fromX,int fromY,int toX,int toY);

	//��������
	void CapturePackage(HWND m_hGameWnd);
	void CapturePlayerInfo(HWND m_hGameWnd);
	void CaptureSell(HWND m_hGameWnd);
	void CaptureSafe(HWND m_hGameWnd);
	void CaptureSkill(HWND m_hGameWnd);
	void CapturePet(HWND m_hGameWnd);

public:

	//��ȡXML����
	CString m_strOrderData;
	//��ϷINI�ļ�����
	CString m_strGameIniData;
	//��Ϸ������
	HWND m_hGameWnd;
	//��Ϸ�����ļ�Ŀ¼
	CString m_strGameIniPath;
	//���Զ�����·��
	CString m_strProgPath;
	//ԭͼ ·��
	CString m_strPicPath;


	int m_initBag;
	//��Ҫ���͵���Ϸ��
	CString m_sendNum;
	CString m_Tax;//˰
	BOOL m_bNotOpen;
	CString m_strRoleNum;
	BOOL bRoleCorrect;
	int ErrorTimes;
	BOOL bRoleNumError;
	CString m_strMiBao;
	BOOL m_bHasYXB;
	CString m_strResult;//������֤��
	//CString m_strLastResult;
	//int m_nPort2RC2;//�˹���Ԥ����(RC2�˿ں�)
	//BOOL m_bHuman;//������ͬ���˹���Ԥ
	//�������յ��ƽ�ָ��
	BOOL m_bOrderEndRet;

	//int NowOrderNo;
	//CStringArray the_arrStrOrderNo;
	//CStringArray arrReturnStatus;
	//������
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
	/*****������������*****/
	CString m_strGameName;
	//��Ϸ������
	CString m_strProcessName;
	//��Ϸ���̺�
	DWORD m_dwProcessId;
	//�Ѿ�ʹ�õĽ��̺��б�
	CString m_OpenedPidLst;
	//�Ƿ������Ϸ
	BOOL m_bOpenNewGame;
	//��Ϸ·��
	CString m_strGamePath;
	//��Ϸִ���ļ�
	CString m_strGameStartFile;
	CString m_strGameStartFileYGX;
	//��Ϸ�ʺ�
	CString m_strAccount;
	//�˺ź�׺
	CString m_strAccountType;
	//��Ϸ����
	CString m_strPassword;
	//��
	CString m_strArea;
	//������
	CString m_strServer;
	//��ɫ��
	CString m_strSellerRole;

	//���
	CString m_SellerNum;
	//��������
	//CString m_OfferType;

	//�Ƿ����ܱ���
	BOOL m_bHasPasspod;
	//��������
	CString m_strSecondPwd;
	//����Ա
	CString m_strDeliverOpId;
	//��ͼĿ¼
	CString m_strPictureDir;
	//ƴͼĿ¼
	CString m_strCapturePath;
	//����Ա����
	CString m_strOPDN;
	//������
	CString m_strOrderNo;
	//�������� ���ҵ�λ
	CString m_SellerItemId;
	CString m_strMbkImage;
	CString m_strMbkString;
	CString m_strMbkID;
	BOOL m_bHasMibao;
	CArray<CPoint,CPoint&> m_ptArrayYBP;
	CArray<CPoint,CPoint&> m_ptArraySpace;
	int m_nBagSpace;
	BOOL IsLockOn;
	int nBuyerOnline;//1��������,2ID����,3��������;4ID����,5��ɫС��10��,�޷�����
	BOOL bSetBuyer;//TRUE��Ϊ˽��,2С��10��ֱ��ճ������
	UINT m_nYBP;
	BOOL bAddFriend;//С���������
	//CString m_strStatus;//״̬
	CString m_strCangKu;
	CString m_strZhenShou;
	CString m_strXiuLian;
	CString m_strPlanTime;

	//��ͼ�ֺ�
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
	BOOL m_bBuyerError;//��ɫ������0


	//�˹���Ԥ����RC2����
	CString m_strRC2Data;
	BOOL m_bHumanFinish;
	//�˹���Ԥ
	//void SendTransDataRC2(int type, const CString &strData,CString me_OrdeNo="");

};
//��Ļ�ֱ���
const int CYSCREEN=GetSystemMetrics (SM_CYSCREEN);
const int CXSCREEN=GetSystemMetrics (SM_CXSCREEN);

//װ����ͼ�ߴ�
const int Lwidth=1280;
const int Lheight=2200;
int nZHPicWidth=880;

CGTRYLZT theApp;
CMyClass myApp;

//ͨ��
CAsyncSocket theUDPSocket;
CFile m_file;


BOOL agreeMail=FALSE;
//����ʱ��
int AppRunTime;
int LastUdpMsg;

BYTE bgR,bgG,bgB;


BOOL bOrderEnd=FALSE;

int nInputFail=0;
int nDiaoXian=0;
int nProcessError=0;
//�����������������
BOOL CGTRYLZT::RequestOrderData()
{
	if (the_nRC2Port!=0)
	{	//��ʽ����
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
	else//����������
	{
		CString strOrderFile = m_strProgPath + _T("\\info.txt");
		//::MessageBox (NULL,strOrderFile,"",MB_OK);
		if ( !PathFileExists(strOrderFile) )
		{

			TRACE( _T("==========> �ļ�[%s]������ <==========\n"), strOrderFile );
			//return false;
		}
		else
		{
			m_strOrderData =myApp.ReadFile(strOrderFile);
			WriteToFile(_T(the_strOrderNo + " ��ȡ�����ļ�\r\n" ));
			if (m_strOrderData != "")
				return true;
		}
	}
	WriteToFile(_T(the_strOrderNo + "��������ʧ��\r\n"));
	return false;
}
//д��־
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
//��ȡ������ϸ
BOOL CGTRYLZT::ReadOrderDetail( CString strOrderData )
{
	//AfxMessageBox(strOrderData);
	if ( strOrderData.IsEmpty() )
	{
		WriteToFile( _T("==========> ��������Ϊ�� <==========") );
		return false;
	}

	CString m_RegInfos      = myApp.FindStr( strOrderData, "<RegInfos>", "</RegInfos>" );
	CString strItem = myApp.FindStr( m_RegInfos,   "<Name>��Ϸ�˺�</Name>", "</RegInfoItem>" );
	m_strAccount	= myApp.FindStr( strOrderData, "<SellerAccount>", "</SellerAccount>" );
	if(m_strAccount.IsEmpty())
		m_strAccount	= myApp.FindStr( strItem, "<Value>", "</Value>" );
	if(m_strAccount.IsEmpty())
	{
		strItem = myApp.FindStr( m_RegInfos,   "<Name>��Ϸ�ʺ�</Name>", "</RegInfoItem>" );
		m_strAccount	= myApp.FindStr( strItem, "<Value>", "</Value>" );
	}
	if(m_strAccount.IsEmpty())
	{
		WriteToFile( _T("�ʺ�Ϊ��") );
		return FALSE;
	}
	strItem			= myApp.FindStr( m_RegInfos,   "<Name>��Ϸ����</Name>", "</RegInfoItem>" );
	m_strPassword	= myApp.FindStr( strItem, "<Value>", "</Value>" );
	strItem			= myApp.FindStr( m_RegInfos,   "<Name>��Ϸ��ɫ��</Name>", "</RegInfoItem>" );
	m_strSellerRole	= myApp.FindStr( strItem, "<Value>", "</Value>" );
	strItem			= myApp.FindStr( m_RegInfos,   "<Name>�ֿ�����</Name>", "</RegInfoItem>" );
	m_strSecondPwd	= myApp.FindStr( strItem, "<Value>", "</Value>" );
	strItem			= myApp.FindStr( m_RegInfos,   "<Name>�û����</Name>", "</RegInfoItem>" );
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
	//WriteToFile( "ƴͼ���·��[%s]",m_strCapturePath);
	m_strCapturePath+="\\";
	/*if(!m_strMbkImage.IsEmpty())
	WriteToFile( "�ܱ���·��[%s]",m_strMbkImage);*/
	//MakeSureDirectoryPathExists(m_strCapturePath);
	//WriteToFile( "��Ϸ��[%s]",m_strGameName);
	if(!m_strMbkString.IsEmpty())
		m_strMbkString=m_strMbkString+",";
	int num=m_strServer.Find("/");
	if(num>0)
		m_strServer=m_strServer.Left(num);

	if(!m_strMbkID.IsEmpty())
		m_bHasMibao=TRUE;
	return true;
}
//����UDP����
void CGTRYLZT::SendTransData(int type, const CString &strData,CString me_OrdeNo,UINT nPort)
{

	if(strData!="...")
		nProcessError=0;
	LastUdpMsg=AppRunTime;
	if (the_strOrderNo!="���Զ���")
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
//����UDP���ݵ�ָ��IP
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
} //�����̺߳���1
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
} //�����̺߳���3


//���붩������
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
		m_strOrderData = pRecvUDPData->szData;	//��ȡ��������
	else if(pRecvUDPData->type == TRANS_SEND_MAIL )
		agreeMail = TRUE;
	else if (pRecvUDPData->type  == 31 /*&& m_strLastResult!=pRecvUDPData->szData*/)
		m_strResult = pRecvUDPData->szData;		//��֤��
	else if(pRecvUDPData->type == 81)
		m_strRC2Data = pRecvUDPData->szData;	//�˹���Ԥ����RC2����
	else if(pRecvUDPData->type == 82)
		m_bHumanFinish=TRUE;	//�˹���Ԥ���

	return FALSE;
}
//����UDP����
PUDP_DATA CGTRYLZT::RecvTransData()
{
	PUDP_DATA pUDPData;
	if (the_strOrderNo!="���Զ���")
	{
		char m_recvData[UDP_DATA_LEN];
		theUDPSocket.Receive(m_recvData,UDP_DATA_LEN);
		pUDPData = ( PUDP_DATA ) m_recvData;
		m_strOrderData= pUDPData->szData ;
	}
	return pUDPData;
}
//��������
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
//��ʼ����־
BOOL CGTRYLZT::InitLogFile( )
{
#ifdef _TEST
	//////////��������������־�ļ�///////////////
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


	WriteToFile("%s�汾%s\r\n",m_strPcIpadd , myApp.GetFileVersion());


	ClearAppData();


	WinExec("rasphone -h �������",SW_SHOW);
	Sleep(1000);
	DWORD flags;
	IsNetworkAlive(&flags);

	if (flags!=3 && the_strOrderNo!="���Զ���")
	{
		WriteToFile ("���δ����\r\n");
		WinExec("rasphone -d �������",SW_SHOW);
		for(int i=0;i<10;i++)
		{
			Sleep(400);
			if(FindWindow(NULL,"��������"))
			{
				PostMessage(FindWindow(NULL,"��������"),WM_CLOSE,0,0);
				break;
			}
		}
	}



	m_strPictureDir	= "E:\\"+GameName+"�˺Ž�ͼ";

	if ((m_strPictureDir==""))
		return false;
	m_strGamePath+="\\";
	if(m_strPictureDir.Right(1)!="\\")
		m_strPictureDir+="\\";

	int thisNUm=0;
	char strV[256];

	GetPrivateProfileStringA("��¼����","ADSL��������","",strV,256,m_strProgPath + "\\adsl.ini");
	thisNUm= atoi(CString(strV)) + 1;
	CString strTemp;
	strTemp.Format ("%d",thisNUm);
	WritePrivateProfileStringA("��¼����","ADSL��������",strTemp,m_strProgPath + "\\adsl.ini");

	GetPrivateProfileStringA("��¼����","����ʧ��","",strV,256,m_strProgPath + "\\adsl.ini");



	WriteToFile ("������%s��,����ʧ��%d��\r\n",strTemp,atoi(CString(strV)));
	if(thisNUm>100 /*|| atoi(strV)>10*/)
	{
		m_bClosePC=TRUE;
		WriteToFile ("����������������\r\n");
	}
	//myApp.SetDisplay();


	if (!GameStart())
		return false;

	//#ifdef _TEST
	//	//
	//#else
	//	//���ý�ͼ����Ŀ¼
	//	WriteToFile( "��Ϸ��[%s]",m_strGameName);
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
	/*if (the_strOrderNo == _T("���Զ���"))
	return;*/
	int thisNUm=0,thisNUm2=0;
	char strV[256];
	CString strTemp;
	/*GetPrivateProfileStringA("��¼����","ADSL��������","",strV,256,"..\\adsl.ini");
	thisNUm=atoi(CString(strV));*/


	if (m_bClosePC)
	{
		WritePrivateProfileStringA("��¼����","ADSL��������","0",m_strProgPath + "\\adsl.ini");
		//WritePrivateProfileStringA("��¼����","����ʧ��","0",m_strProgPath + "\\adsl.ini");
		Sleep(500);
		WriteToFile("������������ָ��\r\n");
		SendTransData (TRANS_REBOOTCOMPUT,"");
		//RestartComputerOfPower();
		return;
	}
	else
	{
		GetPrivateProfileStringA("��¼����","����ʧ��","",strV,256,m_strProgPath + "\\adsl.ini");
		thisNUm2=atoi(CString(strV));
		//WriteToFile("������������\r\n");
		if (Status>1000 && Status<3000)
		{
			thisNUm2++;
			strTemp.Format ("%d",thisNUm2);

			WritePrivateProfileStringA(_T("��¼����"),"����ʧ��",strTemp,m_strProgPath + "\\adsl.ini");

			strTemp.Format ("%s$%s$%d$%d",m_strGameName,m_strPcIpadd,thisNUm2,Status);
			SendData( 101 ,strTemp,"192.168.60.35" ,19871);
		}
		else
		{
			WritePrivateProfileStringA("��¼����","����ʧ��","0",m_strProgPath + "\\adsl.ini");
			strTemp.Format ("%s$%s$0",m_strGameName,m_strPcIpadd);
			SendData( 101 ,strTemp,"192.168.60.35" ,19871);
		}
	}

}

//������뷨,��д��
bool CGTRYLZT::CheckIM()
{

	//�ر����뷨
	if (!myApp.SelectIME())
	{
		return false;
	}
	if(!CloseCapslockKey())//�ж�CAPLOCK���Ƿ��,����,�򵯿�.
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

	}//�ж�CAPLOCK���Ƿ��,����,��ر�.

	byte keyS[256];
	::GetKeyboardState (keyS);
	//cout<<GetKeyState ( VK_CAPITAL )<<endl;
	if (/*keyS[VK_CAPITAL]*/GetKeyState ( VK_CAPITAL )!=0)
	{
		WriteToFile("��رմ�д��\r\n");
		return false;
	}
	return true;

}
//������֤��
CString CGTRYLZT::RequestSafeCardInfo(int CodeType,CString ImagePath,CString Explain,int time,CString OldResult)
{
	//AfxMessageBox("�ܱ���");
	//���󶩵����� & ���ն������� & ����ȷ��
	//	TRANS_REQ_IDCODE_RESULT  = 30,    //����������GTR������֤��               ( ROBOT -> RC2 ) 
	//TRANS_RES_IDCODE_RESULT  = 31,    //���ʹ��������֤��ĵ������˳���      ( RC2 -> ROBOT ) 
	//TRANS_IDCODE_INPUT_RESULT = 32,   //������������֤���Ľ�����͸��ͻ���  ( ROBOT -> RC2 )
	// 
	//30 ���ݸ�ʽ:
	//FCodeType=  ��������(����Ϊ��)
	//1. ������֤��.
	//2. �ܱ���֤��.
	//3. ������֤��.
	//FImageName= ��֤��ͼƬ�ļ���ȫ·��(����Ϊ��)
	//FQuestion=  һЩ˵���ı�(��Ϊ��) 
	//FTimeout=   ��ʱֵ(��λ��)
	if (the_strOrderNo=="���Զ���")
	{
		SetForegroundWindow(GetConsoleWindow());
		std::cout << "�������ܱ�:";
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
	WriteToFile( _T("������֤��...\r\n"));
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
			WriteToFile( _T("�ȴ���֤��...\r\n"));
		if(m_strResult=="")
			continue;
		CString tmp;
		tmp.Format ("��֤�뷵�أ�%s\r\n",m_strResult);
		WriteToFile( tmp);
		//m_strLastResult=m_strResult;
		//IsAnswer=true;
		return m_strResult;	
	}
	WriteToFile( _T("�ȴ���֤�볬ʱ...")) ;
	return "";
}

////����UDP����(RC2�˹���Ԥ)
//void CGTRYLZT::SendTransDataRC2(int type, const CString &strData,CString me_OrdeNo)
//{
//	if (the_strOrderNo!="���Զ���")
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

