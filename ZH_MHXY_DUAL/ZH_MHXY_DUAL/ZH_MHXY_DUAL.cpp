// RC-GTR-TLBBOL.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ZH_MHXY_DUAL.h"
#include<math.h>


//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif



using namespace std;





BOOL bPicFull=TRUE;
BYTE *pBigPic;
POINT ptBigPic;
POINT ptMAX;

//ѡ����������Ƿ�ڶ��ν�����Ϸ
BOOL SecondEnter=FALSE;


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
#ifdef _TEST
	//

#else

	/*CString strTemp,m_strAccount;
	strTemp="15811761092��163.com";
	m_strAccount=strTemp;
	toDBS(m_strAccount,strTemp);
	m_strAccount=strTemp;
	if (m_strAccount.Find("@")>0)
	{
		cout<<1<<endl;
	}else cout<<0<<endl;*/

	/*theApp.m_strCapturePath=_T("E:\\�λ�����ƴͼ\\");
	return theApp.Trading();*/

	//������ղ�����OrderNo + 1 + UDPPort
	if ( argc != 4 )
	{
		TCHAR szBuf[128] = {0};
		_stprintf( szBuf, _T("ʹ�÷���: ZH_WZRY_DUAL.exe <������> 1 <UDP�˿ں�>\n") );
		//_tprintf(szBuf);
		MessageBox(NULL, szBuf, "��ʾ", MB_ICONINFORMATION);
		return 0;
	}
	theApp.the_strOrderNo = argv[1];
	the_nRC2Port = atoi(argv[3]);
#endif//_TEST
	
	system("adb kill-server");   //�ų���������ǰ�棬��Ȼ��ű�����

	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
		WSADATA wsaData;
		if(!AfxSocketInit(&wsaData))
		{
			_tprintf(_T("�޷�����WinSockͨѶ��\n"));
			MessageBox(NULL, _T("�޷�����WinSockͨѶ��"), "��ʾ", MB_ICONINFORMATION);
			return -1;
		}

		
		//��ʼ��UDPSocket
		if (!theUDPSocket.Create( m_UDPPORT, SOCK_DGRAM ) )//if (!theUDPSocket.InitSocket())
		{
			MessageBox(NULL, _T("��ʼ��UDPSocketʧ��"), "��ʾ", MB_ICONINFORMATION);
			return -1;
		}
	}

	HANDLE the_hMutex = CreateMutex(NULL, FALSE, "rc2_lol_event");
	if ( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		CloseHandle(the_hMutex);
		the_hMutex = NULL;
		MessageBox(NULL, _T("�����Ѿ�����"), "��ʾ", MB_ICONINFORMATION);
		return -1;
	}

	if ( argc == 4 )
	{
		theApp.the_strOrderNo=argv[1];
		the_nRC2Port = atoi(argv[3]);
	}
	else
	{
		theApp.the_strOrderNo=_T("���Զ���");

	}
	theApp.InitLogFile();


#ifdef _TEST
	//
#else
	AfxBeginThread(Fun1Proc, NULL);
	AfxBeginThread(Fun3Proc, NULL);

#endif


	CString log;
	int Status;


	if (theApp.YLZTInit())
		Status=theApp.GameMain();
	else
		Status=2260;


	if (Status>1000)
	{
		if(theApp.NetBreak())
			Status=2050;
	}
	//theApp.CloseGame();

	theApp.KillProcess();
#ifdef _TEST
	//
#else
	theApp.CloseAllWindow();
#endif	

	if(Status!=1000 && theApp.the_strOrderNo!="���Զ���")
		myApp.ClearFolder(theApp.m_strCapturePath);

	theApp.IsClosePc(Status);
	theApp.YiJiao(Status);

	system("adb kill-server");
	theApp.WriteToFile("�Ͽ�UDPSocket����");
	theUDPSocket.Close();
	return 1;
}

BOOL CGTRYLZT::GameStart()
{

	//InitKeyMouseDriver();
#ifdef _TEST
	//��ȡ��������
	CString strOrderFile = _T(m_strProgPath+"\\info.txt");
	if( !PathFileExists(strOrderFile) )//���·���ļ�
	{
		WriteToFile( "�ļ�[%s]������", strOrderFile );
		return FALSE;
	}
	//WriteToFile( _T("��ȡ[%s]\n"), strOrderFile );
	m_strOrderData = myApp.ReadFile(strOrderFile);

#else
	//���󶩵�����
	if ( !RequestOrderData() )
	{
		WriteToFile( _T("���󶩵�����ʧ��\n") );
		return FALSE;
	}

#endif
	//��ȡ������ϸ
	if ( !ReadOrderDetail(m_strOrderData) )
	{
		WriteToFile( _T("��ȡ������ϸʧ��") );
		return FALSE;
	}




	return true;
}
//������
BOOL CGTRYLZT::GameMain()
{

	CRect rect;
	GetWindowRect(GetDesktopWindow(),&rect);
	if(rect.Width()!=1280 || rect.Height()!=1024)
	{
		WriteToFile("���Էֱ��ʲ���1280*1024\r\n");
		return 2260;
	}
	if(the_strOrderNo.Find("-")>0  || the_strOrderNo.Find("MZH")==0|| the_strOrderNo=="���Զ���")
		m_strOrderType="������";
	else
		m_strOrderType="���׵�";
	WriteToFile ("��������:%s\r\n",m_strOrderType);
	int Status=0;
	myApp.DeleteLog(m_strPictureDir,3);
	/*if(!CheckAccount())
	{
	WriteToFile("�˺Ų��Ǵ�����");
	return 3000;
	}*/



#ifdef _TEST
	/*m_hGameWnd=myApp.GetHwndByPid(NULL,"������");
	if(m_hGameWnd)
	{
	myApp.ActiveWindow(m_hGameWnd);
	for (int i=0;i<3;i++)
	{
	myApp.DragMouse(m_hGameWnd,960,200,960,550);
	}
	return 1;
	}*/
	
	
#else
	//
#endif
	
	//if (m_strAccount.Find("��")>0)//ȫ��@
	//{
	//	m_strAccount.Replace("��","@");
	//}
	//�ַ���ȫ��ת��Ϊ���
	m_strAccount=toDBS(m_strAccount);
	m_strPassword=toDBS(m_strPassword);
	if (m_strAccount.Find("@")<0)
	{
		m_strAccount.Append("@163.com");
	}
	WriteToFile("��¼�˺�Ϊ:%s",m_strAccount);
	
	for (int i=0;i<3;i++)
	{
		Status=CheckAccount();
		if (Status>=3000)
		{
			WriteToFile("�˺Ż��������Status=%d",Status);
			myApp.KillWindow("WangYi.exe");
			return Status;
		}
		else if (Status==2120)
			continue;
		else break;
	}
	if(Status!=1000)
	{
		WriteToFile("�˺�������֤ʧ�ܣ�Status=%d",Status);
		myApp.KillWindow("WangYi.exe");
		return Status;
	}
	myApp.KillWindow("WangYi.exe");
	//Sleep(1000*50);
	/*for(int i=0;i<3;i++)
	{
		Status = WebPage();
		if(Status>=3000)
			return Status;
		else if(Status==2120)
			continue;
		else
			break;
	}
	myApp.KillWindow("�����.exe");*/
	
	

	for(int i=0;i<3;i++)
	{
		KillProcess();

		Status=WaitStartGame();
		if (Status==2120)
		continue;
		if (Status > 1000)
		return Status;
		//m_hGameWnd=myApp.GetHwndByPid("Droid4X.exe");
		Sleep(2000);
		Status=EnterAccPwd();
		if (Status==2120)
		continue;
		if (Status > 1000)
		return Status;


		Status=Trade();
		if (Status==2120)
			continue;
		return Status;
	}



	return Status;


}
//int CGTRYLZT::SelectServer()
//{
//	int index=m_strArea.Find("��");
//	if( m_strArea.Find("��")<0)
//	{
//		WriteToFile ("��������");
//		return 2260;
//	}
//	CString strServer=m_strArea.Left(index);
//	CString strText="��Q,";
//	CString strText1="��Q_1,";
//	for(int i=0;i<strServer.GetLength();i++)
//	{
//		strText+=strServer.Mid(i,1)+",";
//		strText1+=strServer.Mid(i,1)+"_1,";
//	}
//	strText+="��";
//	strText1+="��_1";
//	PinTu(strText,"����",m_strPicPath+"����");
//	PinTu(strText1,"����1",m_strPicPath+"����");
//	POINT  pt;
//	myApp.PressMouseKey (m_hGameWnd,710,465);
//	for(int i=0;i<10;i++)
//	{
//		Sleep(1000);
//		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"����ҳ��",&pt)
//			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"����ҳ��2",&pt))
//			break;
//	}
//	if(pt.x<0)
//	{
//		WriteToFile ("�ȴ�����ҳ�泬ʱ");
//		return 2260;
//	}
//	CaptureJpg("����ҳ��");
//	for(int i=0;i<10;i++)
//	{
//		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�ҵķ�����",&pt)
//			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"�ҵķ�����2",&pt)
//			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"�ҵķ�����3",&pt))
//			break;
//		DragMouse(m_hGameWnd,60,200,60,800);
//	}
//	if(pt.x<0)
//	{
//		WriteToFile ("�Ҳ���[�ҵķ�����]");
//		return 2260;
//	}
//	myApp.PressMouseKey (m_hGameWnd,pt.x+50,pt.y+10);
//	Sleep(1000);
//	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"����\\����",&pt)
//		&& !myApp.FindBmp(m_hGameWnd,m_strPicPath+"����\\����1",&pt))
//	{
//		WriteToFile ("�Ҳ���������");
//		return 2260;
//	}
//	myApp.PressMouseKey (m_hGameWnd,pt.x+100,pt.y+10);
//	Sleep(1000);
//	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"��ʼ",&pt)
//		&& !myApp.FindBmp(m_hGameWnd,m_strPicPath+"��ʼ2",&pt))
//	{
//		WriteToFile ("�Ҳ���[��ʼ]");
//		return 2260;
//	}
//	CaptureJpg("����");
//	myApp.PressMouseKey (m_hGameWnd,pt.x+10,pt.y+10);
//	Sleep(5000);
//	for(int i=0;i<20;i++)
//	{
//		Sleep(3000);
//		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"Ӣ��",&pt))
//			break;
//		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"Ӣ��2",&pt))
//			break;
//		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"���",&pt)
//			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"���2",&pt)
//			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"���3",&pt)
//			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"���4",&pt))
//		{
//			myApp.PressMouseKey (m_hGameWnd,pt.x+20,pt.y+20);
//			//myApp.PressMouseKey (m_hGameWnd,pt.x+20,pt.y+20);
//			Sleep(500);
//			myApp.PressMouseKey (m_hGameWnd,20,20);
//		}
//		else
//			myApp.PressMouseKey (m_hGameWnd,880,115);
//		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"����ϵͳ",&pt))
//			myApp.PressMouseKey (m_hGameWnd,50,70);
//
//	}
//	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"Ӣ��",&pt)
//		&& !myApp.FindBmp(m_hGameWnd,m_strPicPath+"Ӣ��2",&pt))
//	{
//		WriteToFile ("�Ҳ���[Ӣ��]");
//		return 2260;
//	}
//	return 1;
//}
void CGTRYLZT::DragMouse(HWND hwnd,int fromX,int fromY,int toX,int toY)
{
	myApp.MoveMouse(hwnd,fromX,fromY);
	Sleep(500);
	myApp.PressMouseKey(1);
	Sleep(500);
	myApp.MoveMouse(hwnd,toX,toY);
	Sleep(500);
	myApp.PressMouseKey(2);
	Sleep(500);
}
int CGTRYLZT::WebPage()
{

	POINT  pt,ptTemp;
	myApp.KillWindow("�����.exe");
	WinExec(m_strProgPath+"\\�����.exe http://reg.163.com/", SW_SHOW);
	for(int i=0;i<10;i++)
	{
		Sleep(2000);
		m_hGameWnd=FindWindow(NULL,"�����");
		if(IsWindow(m_hGameWnd))
			break;
	}
	if(!IsWindow(m_hGameWnd))
	{
		WriteToFile ("�������ʧ��");
		return 2120;
	}
//#ifdef _DEBUG
	/*m_hGameWnd=myApp.GetHwndByPid("�����.exe");
	myApp.ActiveWindow(m_hGameWnd);
	int fl=0;
	cin>>fl;
	myApp.ActiveWindow(m_hGameWnd);*/

//#endif
	SetWindowPos(m_hGameWnd,NULL,0,0,0,0,SWP_NOSIZE);






	static int errorTimes=0;
	static int yzmTimes=0;

PWD_ERROR:

	for(int i=0;i<10;i++)
	{
		Sleep(5000);
		if(/*myApp.FindBmp(m_hGameWnd,m_strPicPath+"��ҳ��¼",&pt)
		   || */myApp.FindBmp(m_hGameWnd,m_strPicPath+"��ͨ��¼",&pt,0,0,0,0,TRUE,24,FALSE))
		   break;
		myApp.SendFuncKey(VK_F5);
	}
	if(pt.x<0)
	{
		WriteToFile ("�Ҳ�����ͨ��¼");
		CaptureJpg("�Ҳ�����ͨ��¼");
		return 2120;
	}

	myApp.PressMouseKey (m_hGameWnd,pt.x+120,pt.y+90);//�˺�textbox
	Sleep (300);
	myApp.SendFuncKey(VK_HOME);
	Sleep(300);
	myApp.SendFuncKey(VK_DELETE,30);
	Sleep(300);
	myApp.SendKeys(m_strAccount,200);

	Sleep (300);
	myApp.PressMouseKey (m_hGameWnd,pt.x+120,pt.y+160);//����textbox
	Sleep(300);
	myApp.SendFuncKey(VK_HOME);
	Sleep(300);
	myApp.SendFuncKey(VK_DELETE,20);
	Sleep (300);
	myApp.SendKeys(m_strPassword,200);

SafeCode:
	for (int i=0;i<5;i++)
	{
		Sleep(1000);
		//TODO:�����֤�뻬�飬�϶�����
		if (myApp.FindBmp(m_hGameWnd,m_strPicPath+"������֤",&pt))
		{
			myApp.MoveMouse(m_hGameWnd,pt.x+10,pt.y+5);
			myApp.CapturePictureInRect(m_hGameWnd,"E:\\1.bmp",pt.x-20,pt.y-130,pt.x+310,pt.y-30);
			myApp.BMPToJPG("E:\\1.bmp");
			//��֤������(����Ϊ��)
			//1. ������֤��.
			//2. �ܱ���֤��.
			//3. ������֤��.
			CString strResult=RequestSafeCardInfo(3,"E:\\1.jpg","",90);
			if (strResult.GetLength() < 1)
			{
				WriteToFile ("����Ա������֤�����");
				return 2120;
			}

			int tx=atoi(strResult);
			WriteToFile("%s%d","��֤������ǣ�",tx);
			Sleep(1000);
			myApp.DragMouse(m_hGameWnd,pt.x+10+tx+2-40,pt.y+5,pt.x+10,pt.y+5,1000);//ͼƬƫ����
			break;
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�ַ���֤",&pt))
		{
			myApp.CapturePictureInRect(m_hGameWnd,"E:\\1.bmp",890,330,1010,380);
			myApp.BMPToJPG("E:\\1.bmp");
			CString strResult=RequestSafeCardInfo(1,"E:\\1.jpg","",90);
			if(strResult.GetLength()<4)
			{
				WriteToFile("����Ա������֤�����");
				return 2120;
			}
			WriteToFile("%s%s","��֤���ǣ�",strResult);
			Sleep(1000);
			myApp.PressMouseKey(m_hGameWnd,pt.x+10,pt.y+10);
			Sleep(1000);
			myApp.SendString(strResult);
			break;
		}
	}
	if(pt.x<0)
	{
		WriteToFile("û���ҵ�������֤��ť���ַ���֤��ˢ��ҳ��");
		yzmTimes++;
		if(yzmTimes>3)
		{
			WriteToFile ("��֤��δ��ʾ����3��");
			return 2120;
		}
		myApp.SendFuncKey(VK_F5);
		goto PWD_ERROR;
	}
	

	WriteToFile ("������� �˺�[%s]����[%d]λ",m_strAccount,m_strPassword.GetLength());
	Sleep (2000);
	CaptureJpg("�˺�����");

	/*if(!(myApp.FindBmp(m_hGameWnd,m_strPicPath+"��ҳ��¼2",&pt)
	|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"��ҳ��¼",&pt)))
	{
	WriteToFile("�Ҳ�����ҳ��¼��ť");
	return 2120;
	}*/
	pt.x=pt.y=-1;
	for (int i=0;i<5;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"��ҳ��¼2",&pt,650,370,1000,510))
		{
			break;
		}
		Sleep(1000);
	}
	if(pt.x<0) 
	{
		WriteToFile("�Ҳ�����ҳ��¼��ť");
		//return 2120;
	}
	else myApp.PressMouseKey (m_hGameWnd,pt.x+10,pt.y+10);
	Sleep (2000);
	//CaptureJpg("�˺���������");
	static int inputTimes=0;
	for(int i=0;i<3;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�˺Ź���",&pt,0,0,0,0,TRUE,24,FALSE)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"��¼",&pt,0,0,0,0,TRUE,24,FALSE))
			break;

		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�˺������",&pt))
		{
			WriteToFile ("�˺������");
			//CaptureJpg("�˺������");
			if(errorTimes>0)
				return 3000;
			errorTimes++;
			goto PWD_ERROR;
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"��֤���",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"��֤���2",&pt))
		{
			WriteToFile ("��֤�벻��ȷ");
			CaptureJpg("��֤���");
			yzmTimes++;
			if(yzmTimes>3)
			{
				WriteToFile ("��֤�����3��");
				return 4330;
			}
			goto SafeCode;
		}
		//myApp.SendFuncKey(VK_F5);
		Sleep (5000);
	}


	Sleep(2000);
	for (int i=0;i<3;i++)
	{
		if (myApp.FindBmp(m_hGameWnd,m_strPicPath+"��¼",&pt,0,0,0,0,TRUE,24,FALSE))
		{
			myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y);//�ֻ�pt.x-120,pt.y+10

			myApp.ActiveWindow(m_hGameWnd);
			Sleep(2000);
			break;
		}
		Sleep(2000);
	}
	

	for (int i=0;i<5;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�Ժ�",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,pt.x+5,pt.y+5);
			Sleep(2000);
			break;
		}
		Sleep(2000);
	}

	for (int i=0;i<5;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�˺Ź���",&pt,0,0,0,0,TRUE,24,FALSE)
			/*&& !myApp.FindBmp(m_hGameWnd,m_strPicPath+"��ȫ����2",&pt)*/)
		{
			WriteToFile("��¼�ɹ�");
			break;
		}
		else WriteToFile("�Ҳ����˺Ź���");
		Sleep(2000);
	}
	if(pt.x<0)
	{
		CaptureJpg("��¼ʧ��");
		WriteToFile("��¼ʧ��");
		return 3700;
	}

	myApp.PressMouseKey (m_hGameWnd,pt.x+10,pt.y+10);
	Sleep (2000);
	for (int i=0;i<5;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�˺Ű�ȫ",&pt,0,0,0,0,TRUE,24,FALSE)
			/*&& !myApp.FindBmp(m_hGameWnd,m_strPicPath+"�˺Ű�ȫ2",&pt)*/)
		{
			WriteToFile("�ҵ��˺Ű�ȫ");
			break;
		}
		else WriteToFile("�Ҳ����˺Ű�ȫ");
		Sleep(2000);
	}
	if(pt.x<0)
	{
		CaptureJpg("�˺Ű�ȫ");
		WriteToFile("�����˺Ű�ȫʧ��");
		return 2120;
	}

	myApp.PressMouseKey(m_hGameWnd,pt.x+5,pt.y+15);
	Sleep(5000);
	CaptureJpgInRect("�˺Ű�ȫ", pt.x-10,pt.y-170,pt.x+890,pt.y+290);

	myApp.KillWindow("�����.exe");
	return 1;
}

//�ж��Ƿ񱻶��� 
BOOL CGTRYLZT::NetBreak()
{
	WriteToFile("����ʧ��");
	CaptureJpg("����ʧ��",GetDesktopWindow());
	POINT pt;

	if(!IsWindow(m_hGameWnd))
	{
		WriteToFile ("��Ϸ������ʧ");
		return FALSE;
	}

	//myApp.CaptureGamePicture(".\\����ʧ��.bmp",GetDesktopWindow());
	if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"����",&pt))
	{
		WriteToFile ("������");
		return TRUE;
	}
	return FALSE;
}


void CGTRYLZT::YiJiao(int Status)
{
	if(the_strOrderNo.Find("M")==0 && Status!=1000)
	{
		if(Status>2000 && Status<3000)
			Status+=2000;
	}
	PUDP_DATA pRecvUDPData;
	CString log;

	int num=myApp.GetFileNum(m_strCapturePath);
	if(Status==1000)
		WriteToFile("��ͼ�ɹ�,��%d��\r\n",num);
	/*if(Status==1000 && num>18)
	{
	WriteToFile("�����Ϲ淶18��\r\n");
	Status=2300;
	}*/

	Sleep(100);
	log.Format ("�ƽ�״̬=%d\r\n",Status);
	WriteToFile(log);
	Sleep(100);

	if(m_strOrderType=="������" && (!m_strPlanTime.IsEmpty()) && the_strOrderNo.Find("MZH")<0)
	{
		log.Format ("FStatus=%d\r\n%s",0,m_strPlanTime);
		WriteToFile("%s",m_strPlanTime);
	}
	else
		log.Format ("FStatus=%d\r\n",Status);
	SendTransData(50,log);

	/*if(m_bProcessMiss)
	{
	Sleep(1000);
	myApp.KillWindow("ZH_GTR_Client.exe");
	}*/


	return ;
}
void CGTRYLZT::GetPlanTime(int nInterval)
{
	if(nInterval>60)
		return;
	CTime t = CTime::GetCurrentTime();
	int year=t.GetYear(),month=t.GetMonth(),day=t.GetDay(),hour=t.GetHour(),minute=t.GetMinute(),second=t.GetSecond();


	if(minute>=(60-nInterval))
	{
		if(hour<23)
		{
			hour++;
			minute=minute+nInterval-60;
		}
		else
		{
			hour=0;
			if(month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12)
			{
				if(day==31)
				{
					day=1;
					if(month==12)
					{
						month=1;
						year++;
					}
					else
						month++;
				}
				else
					day++;
			}
			else if(month==2)
			{
				if(year%4==0)
				{
					if(day==29)
					{
						day=1;
						month++;
					}
					else
						day++;
				}
				else
				{
					if(day==28)
					{
						day=1;
						month++;
					}
					else
						day++;
				}
			}
			else
			{
				if(day==30)
				{
					day=1;
					month++;
				}
				else
					day++;
			}
		}
	}
	else
	{
		minute+=nInterval;
	}

	m_strPlanTime.Format("FPlanTime=%04d-%02d-%02d %02d:%02d:%02d\r\n",year,month,day,hour,minute,second);
	return;

}
BOOL CGTRYLZT::GetOrderNum()
{
	for(int i=0;i<m_strAccount.GetLength();i++)
	{
		int n=m_strAccount.GetAt(i);
		if(n<48 || n>57)
			return FALSE;
	}

	return TRUE;
}
int CGTRYLZT::WaitEnterGame()
{
	POINT pt,pt1;

	for(int i=0;i<60;i++)
	{
		if(i%10==0)
			WriteToFile("�ȴ���Ϸ����");
		Sleep(2000);
		m_hGameWnd=FindWindow("CrossFire","��Խ����");
		if(IsWindowVisible(m_hGameWnd))
			break;


	}
	if(!IsWindowVisible(m_hGameWnd))
	{
		WriteToFile("�ȴ���Ϸ���ڳ�ʱ");
		return 2260;
	}

	for(int i=0;i<20;i++)
	{
		Sleep(2000);
		if(i%10==0)
			WriteToFile("�ȴ�����ҳ��");
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"����",&pt))
		{
			Sleep(1000);
			CaptureJpg("����ҳ��");			
			myApp.PressMouseKey(m_hGameWnd,500,340);
			Sleep(1000);
			myApp.PressMouseKey(m_hGameWnd,950,705);
			Sleep(2000);
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"��ɫ����",&pt))
			break;
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"ȷ��",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"ȷ��2",&pt))
		{

			if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"���",&pt1,pt.x-220,pt.y-150,pt.x+250,pt.x-40,TRUE,24,FALSE))
			{
				CaptureJpg("���");
				WriteToFile("�˺ű���");
				myApp.PressMouseKey(m_hGameWnd,pt.x+5,pt.y+5);
				return 3300;
			}
			CaptureJpg("ȷ��");
			myApp.PressMouseKey(m_hGameWnd,pt.x+5,pt.y+5);
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"ȡ��",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"ȡ��2",&pt))
		{
			CaptureJpg("ȡ��");
			myApp.PressMouseKey(m_hGameWnd,pt.x+5,pt.y+5);
		}
	}
	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"��ɫ����",&pt))
	{
		WriteToFile("�Ҳ�����ɫ����");
		return 2260;
	}

	return 1;

}






//�ȴ�������Ϸ
int CGTRYLZT::WaitStartGame()
{


	POINT pt,ptTemp;
	CString strLog;

	//������Ϸ
	if ( !PathFileExists(m_strGameStartFile))
	{
		WriteToFile( _T("��Ϸ�ļ�[%s]������"), m_strGameStartFile );
		return 2260;
	}

	WriteToFile("����Ϸ");
	m_hGameWnd=myApp.GetHwndByPid("Droid4X.exe");
	if(IsWindow(m_hGameWnd))
	{
		myApp.ActiveWindow(m_hGameWnd);
		WriteToFile("ģ�����Ѿ�����");
	}
	else
	{
		WriteToFile("��ģ����");
		//TODO:����Ϸ����
		HINSTANCE n =::ShellExecute(NULL,"open", m_strGameStartFile,"",m_strGamePath ,SW_SHOWNORMAL );
		if( n < (HINSTANCE) 31)
		{
			WriteToFile("����Ϸʧ��\r\n");
			return 2120;//�����쳣
		}
	}

	for(int i=0;i<30;i++)
	{
		m_hGameWnd=myApp.GetHwndByPid("Droid4X.exe");
		if(IsWindowVisible(m_hGameWnd))
			break;
		else myApp.ActiveWindow(m_hGameWnd);
		Sleep(3000);
	}

	if(!IsWindowVisible(m_hGameWnd))
	{
		WriteToFile("�ȴ�ģ�������ڳ�ʱ");
		return 2260;
	}

	
	Sleep(1000);
	for(int i=0;i<50;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"��Ϸ",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"��Ϸ2",&pt))
			break;
		Sleep(2000);
	}
	if(pt.x<0)
	{
		WriteToFile("�Ҳ���ͼƬ[��Ϸ]");
		return 2260;
	}

	//TODO:��¼��Ϸ֮ǰ��������ļ�
	//ɾ��ָ���ļ�
	WriteToFile("������Ϸ֮ǰ������������ļ�");
	system("adb kill-server");
	system("adb connect 127.0.0.1:26944");
	system("adb shell \"su -c \' rm /data/data/com.netease.my/shared_prefs/Cocos2dxPrefsFile.xml\'\""); 
	
	Sleep(2000);
	system("adb kill-server");

	myApp.PressMouseKey(m_hGameWnd,pt.x+30,pt.y-50,1,50);
	WriteToFile("�ȴ���Ϸ����");

	SetWindowPos(m_hGameWnd,NULL,0,0,0,0,SWP_NOSIZE);
	Sleep(1000);

	for(int i=0;i<10;i++)
	{
		m_hGameWnd=myApp.GetHwndByPid("Droid4X.exe");
		if(IsWindowVisible(m_hGameWnd))
			break;
		Sleep(3000);
	}
	if(!IsWindowVisible(m_hGameWnd))
	{
		WriteToFile("�ȴ���Ϸ���ڳ�ʱ");
		return 2260;
	}

	pt.x=pt.y=-1;//��ʼ��
	for(int i=0;i<60;i++)
	{
		Sleep(5000);
		if(i%20==0)
			WriteToFile("�ȴ���¼ҳ��");
		//�û�Э�鵯��
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�����û�Э��",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"�����û�Э��2",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y);
			Sleep(2000);
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"���µ�¼",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y);
			Sleep(2000);
		}
		if (myApp.FindBmp(m_hGameWnd,m_strPicPath+"ȡ��",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y);
			Sleep(2000);
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"ȷ��",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"ȷ��2",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y);
			Sleep(2000);
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�����˺�",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"�����˺�2",&pt))
		{
			goto CheckInOther;
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�û�����",&pt))
			break;
	}
	if(pt.x<0)
	{
		WriteToFile("�Ҳ���ͼƬ[�û�����]��[�����˺ŵ�¼]");
		return 2260;
	}
	else 
	{
		Sleep(3000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�û�����",&pt))
		{
			WriteToFile("�ҵ��û�����");
			myApp.PressMouseKey(m_hGameWnd,pt.x+10,pt.y+10);
		}
		
		//Sleep(2000);
		for (int i=0;i<5;i++)
		{
			if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�л��˺�",&ptTemp)
				|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"�л��˺�2",&ptTemp))
			{
				myApp.PressMouseKey(m_hGameWnd,ptTemp.x+20,ptTemp.y+20);
				WriteToFile("�л��˺�");
				break;
			}
			Sleep(1000);
		}
		if(ptTemp.x<0)
		{
			WriteToFile("�Ҳ����л��˺�");
			//return 1;
		}
		for (int i=0;i<5;i++)
		{
			if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�����˺�",&pt)
				|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"�����˺�2",&pt))
			{
CheckInOther:	myApp.PressMouseKey(m_hGameWnd,pt.x+10,pt.y+10);//���ʹ�������˺ŵ�¼
				WriteToFile("�����˺ŵ�¼");
				break;
			}
			Sleep(1000);
		}
		if(pt.x<0)
		{
			WriteToFile("�Ҳ��������˺�");
			//return 1;
		}
	}
	
	/*SetWindowPos(m_hGameWnd,NULL,0,0,0,0,SWP_NOSIZE);
	Sleep(1000);*/
	return 1;


}

//���������˺�����
BOOL CGTRYLZT::EnterAccPwd()
{

	//SendGoodsState("�����ʺ�����",60);
	//CString strOS=CMyClass::GetOSVersion();


	POINT pt;
	
	int nCheckTimes=0;

	for (int i=0;i<10;i++)
	{
		if(!(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�����˺�",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"�����˺�2",&pt) ))
		{
			WriteToFile("�����˺�ҳ��");
			return 2260;
		}
		else break;
		Sleep(1000);
	}
	Sleep(1000);
	
	//������뷨,��д��
	if (!CheckIM())
		return 2120;
	
	myApp.PressMouseKey (m_hGameWnd,pt.x,pt.y);//������������˺ŵ�¼
	Sleep (1000);
	pt.x=pt.y=-1;
	for (int i=0;i<5;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�����¼",&pt,415,370,920,440)
			/*|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"�����¼2",&pt,415,370,920,440)*/)
			break;
		Sleep(1000);
	}
	if(pt.x<0) 
	{
		WriteToFile("�Ҳ��������¼��ť");
		return 2260;
	}

	myApp.PressMouseKey(m_hGameWnd,pt.x+180,pt.y-150);
	myApp.SendFuncKey(VK_BACK,20);

	WriteToFile("��ʼ�����˺�����\r\n");



	myApp.SendKeys(m_strAccount,200);
	myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y-70);//����˺ſ������˺���ʾ�ı�
	Sleep(1000);
	myApp.PressMouseKey (m_hGameWnd,pt.x+180,pt.y-70);//�۽������

PASSWORD_ERROR:
	static int runTimes=0;
	if(runTimes>8)
	{
		WriteToFile("���˺ų���8��\r\n");
		return 3700;
	}
	runTimes++;

	Sleep (300);
	//myApp.SendFuncKey(VK_HOME);
	myApp.SendFuncKey(VK_BACK,20);//�����ס�����룬�����
	myApp.SendKeys(m_strPassword,200);
	//RC2Tool::DoubleClick (m_hGameWnd,pt_Pwd.x,pt_Pwd.y);
	CString strLog;



	WriteToFile ("������� �˺�[%s]����[%d]λ",m_strAccount,m_strPassword.GetLength());
	Sleep (1000);
	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"�����¼2",&pt,0,0,0,0,TRUE,60))
	{
		WriteToFile("�����˺�ҳ��");
		return 2260;
	}

	myApp.PressMouseKey (m_hGameWnd,pt.x+5,pt.y+5);//�����¼
	
	for(int i=0;i<5;i++)
	{
		Sleep(2000);
		if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"�����¼2",&pt,0,0,0,0,TRUE,60))
			break;

	}
	if(pt.x>0)
	{
		WriteToFile ("��¼ҳ��û����ʧ");
		goto PASSWORD_ERROR;
		//return 2260;
	}
	
	
	/************************************************************************/
	/* ��¼��Ϸ����  
		��ɾ����Ϸ�����������ļ���Ȼ��Ĭ�ϵ�¼һ����������
		�����ļ�����Ϸ����޸ģ���ʱ�����������ļ������ҵ�Ҫ��¼��
		������Ȼ���޸�lastLogin��Ϣ��ʹ��һ�ε�¼ʱ��Ĭ��������
		Ҫ��¼��������
	*/
	/************************************************************************/

	SecondEnter=FALSE;
	//���е�һ�ε�¼����¼��ɺ��˳�
	
UserLogin:
	
	if(SecondEnter==TRUE)
	{
		//TODO:���µ����Ϸͼ�����Ϸ
		WriteToFile("�޸����������ļ������´���Ϸ");
		for(int i=0;i<50;i++)
		{
			if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"��Ϸ",&pt)
				|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"��Ϸ2",&pt))
				break;
			Sleep(1000);
		}
		if(pt.x<0)
		{
			WriteToFile("�Ҳ���ͼƬ[��Ϸ]");
			return 2260;
		}
		myApp.PressMouseKey(m_hGameWnd,pt.x+30,pt.y-50,1,50);
		Sleep(1000);
		WriteToFile("�ȴ���Ϸ����");
	}

	if (SecondEnter==TRUE)
	{
		int ret=SelectServer();
		if (ret!=1)
		{
			WriteToFile("ѡ���������ִ���,�������="+ret);
			return ret;
		}
	}


	//�����˺����˳��������µ�¼����ʾ��

	CheckGameDialog(m_hGameWnd,m_strPicPath);//�رտ��ܳ��ֵĵ���

	//�ж��Ƿ��Ѿ��Զ�ѡ��Ĭ�Ϸ�����
	for (int i=0;i<10;i++)
	{
		Sleep(1000);
		POINT ptemp;
		if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"δѡ�������",&ptemp,405,400,765,450))
		{
			WriteToFile("�Ѿ����ӷ�����");
			
			myApp.PressMouseKey(m_hGameWnd,590,520);//ֱ�ӵ����¼��Ϸ pt.x-70,pt.y+110
			WriteToFile("�����½��Ϸ");
			break;
		}
		else WriteToFile("��û�������Ϸ�����");
	}
	
/*	POINT ptemp;
	for (int i=0;i<10;i++)
	{
		Sleep(2000);
		

		for(int j=0;j<3;j++)
		{
			if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"�û�����",&ptemp))
			{
				WriteToFile("���ڵ�¼��Ϸ");
				//break;
			}
			Sleep(1000);
		}
		
	}*/
	

	/*myApp.PressMouseKey(m_hGameWnd,pt.x+20,pt.y+20);//���ѡ����ť
	Sleep(1000);
	//CaptureJpg("����");

	//TODO:ѡ��1~8
	myApp.PressMouseKey(m_hGameWnd,240,140);//���н�ɫ


	TODO:ѡ��

	myApp.PressMouseKey(m_hGameWnd,500,140);//���н�ɫ�ĵ�һ��������
	*/

	//TODO:ȡ�������ȴ���,���escȡ������ڣ�ֱ�������˳���Ϸ����ʾ����

	CheckGameDialog(m_hGameWnd,m_strPicPath);//�����¼��Ϸ���ж��Ƿ��е�������

	POINT p;
	p.x=p.y=-1;
	Sleep(2000);
	//if(SecondEnter==TRUE) //���ε�¼
	//{
	for (int i=0;i<60*3;i++)
	{
		myApp.SendFuncKey(VK_ESCAPE);
		//WriteToFile("����esc��");
		Sleep(1000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�˳���Ϸ",&p,350,250,820,470))
		{
			myApp.SendFuncKey(VK_ESCAPE);
			//ͨ���޸���Ϸ�������ļ�����Ĭ������
			Sleep(2000);
			if (SecondEnter==FALSE && CheckGameServer(m_strServer)==1)//������Ϸ���ȡ�����ļ��жϵ�ǰ�Ƿ�����ȷ������
			{
				WriteToFile("Ĭ�Ϸ�������ȷ������Ҫ�޸������ļ�");
				SecondEnter=TRUE;
			}

			break;
		}
		//Sleep(1000);
	}
	if (p.x<0)
	{
		WriteToFile("��¼��Ϸ��ʱ");
		return 4120;
	}
	//}
	if(SecondEnter==FALSE)//һ�ε�¼
	{
		CloseGame();//�ر���Ϸ�������ļ��ᱻ�޸�
		int ret = SelectServer();
		if(ret==3010) return ret;//����û�н�ɫ
		SecondEnter=TRUE;
		goto UserLogin;
		//if(ret==1) return 1;//��һ�ε�¼������ȷ��ֱ�ӽ�����Ϸ
		//else
		//{
			
			//SelectServer();
			
		//}
	}
	
	return 1;

}



//�˶Է�����
BOOL CGTRYLZT::CheckServer()
{
	char chTitle[MAX_PATH];
	CString strTitle;
	CString strLog;
	::GetWindowText(m_hGameWnd,chTitle,MAX_PATH);
	strTitle.Format("%s",chTitle);
	if (strTitle.Find(m_strServer)<0)
	{
		strLog.Format("�˶�����ʧ��,���ڱ���Ϊ:[%s]\r\n",strTitle);
		WriteToFile(strLog);
		return FALSE;
	}
	return TRUE;
}


int CGTRYLZT::GetTextNum(CString strText,int left,int top,int right,int bottom)
{
	int num=0;
	POINT pt;

	int leftNow=left;
	for(int i=0;i<strText.GetLength();i+=2)
	{
		//WriteToFile("��Χ%d,%d,%d,%d\r\n",leftNow,top,right,bottom);
		if(myApp.FindBMPText(m_hGameWnd,"����",strText.Mid(i,2),14,RGB(237,190,0),&pt,leftNow,top,right,bottom,FALSE))
		{
			//WriteToFile("ƥ�䵽[%s]\r\n",strText.Mid(i,2));
			//leftNow=pt.x;
			num++;
		}
	}

	return num;
}


CString CGTRYLZT::CheckCoin(HWND hWnd,int left ,int top,int right ,int bottom,CString strType)
{
	CString a[10] ={"8","0","1","2","3","4","5","6","7","9"} ;
	CoinStruct CoinArr[30];
	CoinStruct CoinTemp;
	int size=12;
	CString strNum = "";
	int FENDUAN=0;
	int num=0;
	POINT pt;
	BOOL key;
	myApp.GetDCData(m_hGameWnd);
	for(int i= 0;i<10;i++)
	{
		int nleft  = left;

		while(nleft <right)
		{
			key = TRUE;
			myApp.FindBmp(hWnd,m_strPicPath+strType+"\\"+a[i],&pt,nleft,top,right,bottom,FALSE);
			if(pt.x>0)
			{
				for(int k=0;k<num;k++)
				{
					if(abs(CoinArr[k].x - pt.x)<(size/2-1))
					{
						key = FALSE;
						k   = num;
					}
				}
				if(key)
				{
					CoinArr[num].x  = pt.x;
					CoinArr[num].no = a[i];
					num++;
				}
				nleft = pt.x + 1;
			}
			else
				nleft  +=right;
		}	
	}
	for(int i=0;i<num;i++)
	{
		for(int k=i;k<num;k++)
		{
			if(CoinArr[i].x>CoinArr[k].x)
			{
				CoinTemp.x    = CoinArr[i].x;
				CoinTemp.no   = CoinArr[i].no;
				CoinArr[i].x  = CoinArr[k].x;
				CoinArr[i].no = CoinArr[k].no;
				CoinArr[k].x  = CoinTemp.x;
				CoinArr[k].no = CoinTemp.no;
			}			
		}
	}
	int k=0;
	while(k<num)
	{
		strNum += (CoinArr[k].no);
		k++;
		if((k<num)&&(CoinArr[k].x-CoinArr[k-1].x>12))
			break;
	}
	return strNum;
}



//����
int CGTRYLZT::Trading()
{
	//CString path=m_strCapturePath+TEXT("ƴͼ\\");//ƴͼ·��
	//WriteToFile("ƴͼ·����"+m_strCapturePath+_T("ƴͼ\\"));
	bgR=223,bgG=206,bgB=163;

	pBigPic = new BYTE[(Lwidth+3)/4*4 * Lheight * 4 ];

	WriteToFile ("��ʼƴͼMHXY");
	PinTu("�����Ϣ","MHXY10");
	PinTu("����","MHXY2");
	PinTu("����","MHXY4");
	PinTu("������Ʒ1,������Ʒ2","MHXY5");
	PinTu("������Ʒ3,������Ʒ4","MHXY5");
	PinTu("��������,��������","MHXY4");//MHXY4ʮ��
	PinTu("��������","MHXY3");
	PinTu("������Ϣ","MHXY3");
	PinTu("�ӵ㷽��1","MHXY9");
	PinTu("�ӵ㷽��2","MHXY8");

	CString ppath,ppath2,ppath3,path4;
	for (int i=1;i<=16;i+=3)
	{
		ppath=ppath2=ppath3=path4=_T("����װ��");
		ppath.Format("%s%d",ppath,i);
		ppath2.Format("%s%d",ppath2,i+1);
		ppath3.Format("%s%d",ppath3,i+2);
		//path4.Format("%s%d",path4,i+3);
		//PinTu(ppath+_T(",")+ppath2+_T(",")+ppath3+_T(",")+path4,_T("MHXY1"));
		//CString str=ppath+_T(",")+ppath2+_T(",")+_T("����")+_T(",")+ppath3+_T(",")+path4;
		CString str=ppath+_T(",")+ppath2+_T(",")+ppath3;
		if (FileExist(str)!="")
		{
			if (i>12)//����װ���ϴ�����4�ţ�����������ΪMHXY3����
			{
				PinTu(str,_T("MHXY3"));
			}
			else PinTu(str,_T("MHXY1"));
		}
		else 
		{
			WriteToFile("�Ҳ�����ͼ:%s",str);
			//break;
		}
	}
	for (int i=1;i<=12;i+=2)
	{
		ppath=ppath2=_T("�ֿ�");
		ppath.Format("%s%d",ppath,i);
		ppath2.Format("%s%d",ppath2,i+1);
		CString str=ppath+_T(",")+ppath2;
		if(FileExist(str)!="")
			PinTu(str,_T("MHXY5"));
		else
		{
			WriteToFile("�Ҳ�����ͼ��%s",str);
			break;
		}
	}
	for (int i=1;i<=8;i++)
	{
		CString s=TEXT("����"),picName;
		s.Format("%s%d",s,i);
		picName=s;
		s.Format("%s,%s,%s",s+_T("��������"),s+_T("���ʼ���"),s+_T("�����ڵ�"));
		if (FileExist(s)!="")
		{
			PinTu(s,"MHXY4");
		}
		else
		{
			WriteToFile("�Ҳ�����ͼ:%s",s);
			break;
		}
	}
	PinTu("���＼��,���ɼ���,��������","MHXY8");

	PinTu("�����ֻ�","MHXY9");
	PinTu("��ȫ��","MHXY9");
	PinTu("���ּ�԰","MHXY7");
	PinTu("��ֵ����","MHXY7");
	//TODO:�Ʋ��������������Һ�������������Ҫ����
	PinTu("��Ϸ��,����","MHXY7");
	PinTu("��Ҫ����","MHXY7");
	
	return 1000;
}
int CGTRYLZT::Hero()
{
	POINT pt;

	myApp.PressMouseKey(m_hGameWnd,95,600);
	for(int i=0;i<5;i++)
	{
		Sleep(2000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"��ӵ��",&pt))
			break;
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"��ӵ��2",&pt))
			break;
		myApp.PressMouseKey(m_hGameWnd,850,125);
	}
	if(pt.x<0)
	{
		WriteToFile ("�Ҳ���[��ӵ��]");
		return 2260;
	}
	CaptureJpg("Ӣ��");
	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"����",&pt)
		&& !myApp.FindBmp(m_hGameWnd,m_strPicPath+"����2",&pt))
	{
		myApp.PressMouseKey(m_hGameWnd,715,65);
		Sleep(1000);
		CaptureJpg("��ӵ��");
	}
	for(int i=0;i<10;i++)
	{
		CaptureJpgInRect("Ӣ��",140,80,990,630);

		myApp.CapturePictureInRect(m_hGameWnd,m_strProgPath+"\\Ӣ��.bmp",170,410,280,450);
		DragMouse(m_hGameWnd,570,560,570,20);
		Sleep(1000);
		if(myApp.FindBmp(m_hGameWnd,m_strProgPath+"\\Ӣ��.bmp",&pt,0,300))
		{
			WriteToFile ("����Ӣ��...");
			CaptureJpg("����Ӣ��");
			return 1;
		}
	}
}
//����
int CGTRYLZT::Trade()
{
	m_hGameWnd=myApp.GetHwndByPid("Droid4X.exe");
	POINT pt,ptTemp,ptTemp2;
	SetWindowPos(m_hGameWnd,NULL,0,0,0,0,SWP_NOSIZE);
	Sleep(1000);
	//myApp.MoveMouse(m_hGameWnd,50,50);
	
	BackToGamePage(m_hGameWnd,m_strPicPath);
	WriteToFile ("��ʼ��ͼ");

	CaptureJpgInRect( "����", 2,40,1170,690, TRUE, RGB( 255, 188,0 ),530,370,650,420);//������Ϸ��Ľ����ͼ

	//Sleep(1000);
	static int DialogOccurTime=0;//�������µ�ѭ����������

ReCapturePackage:
	//TODO���԰�����ͼ
	CapturePackage(m_hGameWnd);
	//TODO:������ֵ�ǰҳ���е������֣�������Ľ�ͼ����ʧ�ܣ���Ҫ���½�ͼ
	if(CheckGameDialog(m_hGameWnd,m_strPicPath))
	{
		WriteToFile("���а�����ͼʱ���ֵ���");
		DialogOccurTime++;
		if(DialogOccurTime<3)
		{
			goto ReCapturePackage;
		}
		else WriteToFile("�������°�����ͼʧ�ܴ�������%d��",DialogOccurTime);
	}

	DialogOccurTime=0;
ReCapturePlayerInfo:
	//TODO:��������Ϣ��ͼ
	CapturePlayerInfo(m_hGameWnd);
	//TODO:������ֵ�ǰҳ���е������֣�������Ľ�ͼ����ʧ�ܣ���Ҫ���½�ͼ
	if(CheckGameDialog(m_hGameWnd,m_strPicPath))
	{
		WriteToFile("����������Ϣ��ͼʱ���ֵ���");
		DialogOccurTime++;
		if(DialogOccurTime<3)
		{
			goto ReCapturePlayerInfo;
		}
		else WriteToFile("��������������Ϣ��ͼʧ�ܴ�������%d��",DialogOccurTime);
	}

	DialogOccurTime=0;
ReCapturePet:
	//TODO:�����ͼ
	CapturePet(m_hGameWnd);
	//TODO:������ֵ�ǰҳ���е������֣�������Ľ�ͼ����ʧ�ܣ���Ҫ���½�ͼ
	if(CheckGameDialog(m_hGameWnd,m_strPicPath))
	{
		WriteToFile("���г�������ͼʱ���ֵ���");
		DialogOccurTime++;
		if(DialogOccurTime<3)
		{
			goto ReCapturePet;
		}
		else WriteToFile("�������³����ͼʧ�ܴ�������%d��",DialogOccurTime);
	}

	DialogOccurTime=0;
ReCaptureSell:
	////TODO:���ۺͳ�ֵ����
	CaptureSell(m_hGameWnd);
	//TODO:������ֵ�ǰҳ���е������֣�������Ľ�ͼ����ʧ�ܣ���Ҫ���½�ͼ
	if(CheckGameDialog(m_hGameWnd,m_strPicPath))
	{
		WriteToFile("���г��ۺͳ�ֵ������ͼʱ���ֵ���");
		DialogOccurTime++;
		if(DialogOccurTime<3)
		{
			goto ReCaptureSell;
		}
		else WriteToFile("�������³��ۺͳ�ֵ������ͼʧ�ܴ�������%d��",DialogOccurTime);
	}

	myApp.PressMouseKey(m_hGameWnd,1130,650);//�򿪹��ܲ˵�
	Sleep(1000);

	DialogOccurTime=0;
ReCaptureSkill:
	////TODO�����ܽ�ͼ
	CaptureSkill(m_hGameWnd);
	//TODO:������ֵ�ǰҳ���е������֣�������Ľ�ͼ����ʧ�ܣ���Ҫ���½�ͼ
	if(CheckGameDialog(m_hGameWnd,m_strPicPath))
	{
		WriteToFile("�������＼�ܽ�ͼʱ���ֵ���");
		DialogOccurTime++;
		if(DialogOccurTime<3)
		{
			goto ReCaptureSkill;
		}
		else WriteToFile("�������¼��ܽ�ͼʧ�ܴ�������%d��",DialogOccurTime);
	}

	DialogOccurTime=0;
ReCaptureSafe:
	//TODO:�ֻ������Ͱ�ȫ��
	CaptureSafe(m_hGameWnd);
	//TODO:������ֵ�ǰҳ���е������֣�������Ľ�ͼ����ʧ�ܣ���Ҫ���½�ͼ
	if(CheckGameDialog(m_hGameWnd,m_strPicPath))
	{
		WriteToFile("�����ֻ������Ͱ�ȫ����ͼʱ���ֵ���");
		DialogOccurTime++;
		if(DialogOccurTime<3)
		{
			goto ReCaptureSafe;
		}
		else WriteToFile("���������ֻ������Ͱ�ȫ����ͼʧ�ܴ�������%d��",DialogOccurTime);
	}

	myApp.PressMouseKey(m_hGameWnd,1130,650);//�رչ��ܲ˵�
	Sleep(1000);

	int status=Trading();
	return status;



	/*for(int i=0;i<5;i++)
	{
	Sleep(2000);
	if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"��װ����",&pt))
	break;
	}
	if(pt.x<0)
	{
	WriteToFile ("�Ҳ���[��װ����]");
	return 2260;
	}
	CaptureJpgInRect( "��װ",0,35,1024,610);
	myApp.PressMouseKey(m_hGameWnd,pt.x+5,pt.y+5);
	Sleep(1000);
	CaptureJpg("��װ");
	CArray<CPoint,CPoint&> ptArray;
	int num=myApp.GetBmpNum(m_hGameWnd,m_strPicPath+"��װ",ptArray,0,120,130,300);
	WriteToFile ("��%d����װ",num);
	for(int i=1;i<num;i++)
	{
	myApp.PressMouseKey(m_hGameWnd,ptArray.GetAt(i).x,ptArray.GetAt(i).y);
	Sleep(2000);
	CaptureJpgInRect( "��װ",0,35,1024,610);
	myApp.PressMouseKey(m_hGameWnd,pt.x+5,pt.y+5);
	Sleep(1000);
	}
	myApp.PressMouseKey(m_hGameWnd,970,60);
	Sleep(3000);


	WriteToFile ("�ֿ��ͼ");
	myApp.PressMouseKey(m_hGameWnd,770,550);
	Sleep(3000);
	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"����",&pt))
	{
	WriteToFile ("�Ҳ���[����]");
	return 2260;
	}
	CaptureJpgInRect( "װ��",0,35,1024,610);
	myApp.PressMouseKey(m_hGameWnd,100,250);
	Sleep(1000);
	CaptureJpgInRect( "��Ƭ",0,35,1024,610);
	myApp.PressMouseKey(m_hGameWnd,100,340);
	Sleep(1000);
	CaptureJpgInRect( "����",0,35,1024,610);
	myApp.PressMouseKey(m_hGameWnd,100,420);
	Sleep(1000);
	CaptureJpgInRect( "����",0,35,1024,610);
	myApp.PressMouseKey(m_hGameWnd,100,500);
	Sleep(1000);
	CaptureJpgInRect( "����Ʒ",0,35,1024,610);
	myApp.PressMouseKey(m_hGameWnd,970,60);
	Sleep(3000);


	myApp.PressMouseKey(m_hGameWnd,20,205);
	Sleep(2000);
	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"���ȹ�δ����",&pt))
	{
	myApp.PressMouseKey(m_hGameWnd,20,205);
	Sleep(3000);
	CaptureJpg("���ȹ�");
	CaptureJpgInRect("���ȹ�",0,35,1024,610);
	}

	else
	WriteToFile ("���ȹ�δ����");

	myApp.PressMouseKey(m_hGameWnd,1000,17);
	Sleep(1000);
	myApp.PressMouseKey(m_hGameWnd,575,345);

	*/

}
BOOL CGTRYLZT:: PicExsit(int num)
{
	for(int i=1;i<num;i++)
	{
		if(ComparePic(i,num))
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CGTRYLZT::ComparePic(int pic1,int pic2)
{
	int xiangshi=64;
	CString Pic,Pic2;
	Pic.Format("%sƤ��%d.bmp",m_strCapturePath,pic1);
	Pic2.Format("%sƤ��%d.bmp",m_strCapturePath,pic2);
	if(!PathFileExists(Pic2))
		return FALSE;
	BYTE r, g, b, now_r, now_g, now_b;
	//�Ա�ͼƬװ������
	::HBITMAP PicH=  (HBITMAP) LoadImage(NULL,Pic,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	BITMAP bm,bm1; 
	::GetObject(PicH,sizeof(BITMAP),&bm); 

	HBITMAP PicH2=  (HBITMAP) LoadImage(NULL,Pic2,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	::GetObject(PicH2,sizeof(BITMAP),&bm1);
	if(bm.bmWidth!=bm1.bmWidth  || bm.bmHeight!=bm1.bmHeight)
	{
		cout<<"����ͼƬ��С��һ��"<<endl<<bm.bmWidth<<","<<bm.bmHeight<<endl<<bm1.bmWidth<<","<<bm1.bmHeight<<endl;
		return FALSE;
	}
	BYTE *tData;
	BYTE *pData;
	//ȡ�������飬�����Ϊ������ÿ��β�Զ���1�ֽڲ��㣬��Ҫȡ�����
	int aaa=(bm.bmWidth * (bm.bmBitsPixel/8) + 3)/4*4* bm.bmHeight;
	aaa=bm.bmWidth * bm.bmHeight * 4;
	tData=new BYTE[ (bm.bmWidth * (bm.bmBitsPixel/8) + 3)/4*4* bm.bmHeight];
	pData=new BYTE[ (bm1.bmWidth * (bm1.bmBitsPixel/8) + 3)/4*4* bm1.bmHeight];
	long Ret1=GetBitmapBits (PicH,(bm.bmWidth * (bm.bmBitsPixel/8) + 3)/4*4* bm.bmHeight,&tData[0]);
	Ret1=GetBitmapBits (PicH2,(bm1.bmWidth * (bm1.bmBitsPixel/8) + 3)/4*4* bm1.bmHeight,&pData[0]);
	BOOL fFalse;

	//ѭ���Ƚ�
	for (int i=0;i<bm.bmHeight ;i++)
	{
		/*if (!fFalse)
		break;*/
		for (int j=0;j<bm.bmWidth;j++)
		{
			/*if(i==5 && j==5)
			b=0;*/
			/*if (!fFalse)
			break;*/
			//ȡ�������飬�����Ϊ������ÿ��β�Զ���1�ֽڲ��㣬��Ҫȡ�����
			/*b=tData[(i*bm.bmWidth +j)*3];
			g=tData[(i*bm.bmWidth +j)*3+1];
			r=tData[(i*bm.bmWidth +j)*3+2];*/
			/*if(i==bm.bmHeight-1)
			cout<<"";*/
			b=tData[ i* ((bm.bmWidth *(bm.bmBitsPixel/8)+1)/2*2)+ j*(bm.bmBitsPixel/8)+0];
			g=tData[ i* ((bm.bmWidth *(bm.bmBitsPixel/8)+1)/2*2)+ j*(bm.bmBitsPixel/8)+1];
			r=tData[ i* ((bm.bmWidth *(bm.bmBitsPixel/8)+1)/2*2)+ j*(bm.bmBitsPixel/8)+2];

			now_r = pData[i* ((bm1.bmWidth *(bm1.bmBitsPixel/8)+1)/2*2)+ j*(bm1.bmBitsPixel/8)+2];
			now_g = pData[i* ((bm1.bmWidth *(bm1.bmBitsPixel/8)+1)/2*2)+ j*(bm1.bmBitsPixel/8)+1];
			now_b = pData[i* ((bm1.bmWidth *(bm1.bmBitsPixel/8)+1)/2*2)+ j*(bm1.bmBitsPixel/8)+0];



			if ((abs(now_r - r)<xiangshi)&&(abs(now_g - g)<xiangshi)&&(abs(now_b -b)<xiangshi)   )
				fFalse=TRUE;
			else
			{
				return FALSE;
			}
		}
	}	

	delete []tData;
	delete []pData;
	::DeleteObject (PicH);
	return TRUE;
}
void CGTRYLZT::CloseGame()
{
	//TODO:�ر�mhxy��Ϸ
	POINT pt;

	for (int i=0;i<20;i++)
	{
		myApp.SendFuncKey(VK_ESCAPE);
		Sleep(1000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�˳���Ϸ",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,pt.x+120,pt.y+90);//ȷ����ť
			break;
		}
		else WriteToFile("�ȴ��˳���Ϸ");
		//myApp.SendFuncKey(VK_ESCAPE);
		//Sleep(1000);
	}
	Sleep(1000);
	for (int i=0;i<10;i++)
	{
		if (myApp.FindBmp(m_hGameWnd,m_strPicPath+"��Ϸ",&pt)
			||myApp.FindBmp(m_hGameWnd,m_strPicPath+"��Ϸ2",&pt))
		{
			WriteToFile("�ر���Ϸ�ɹ�");
			break;
		}
		else
		{
			WriteToFile("�ر���Ϸʧ��");
		}
		Sleep(1000);
	}


	/*POINT pt;
	if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�˺���ʾ��",&pt))
	{
		myApp.PressMouseKey(m_hGameWnd,pt.x+10,pt.y+10);
		Sleep(3000);
	}
	if(IsWindowVisible(m_hGameWnd))
	{
		myApp.PressMouseKey(m_hGameWnd,1267,12);
		Sleep(2000);
		myApp.PressMouseKey(m_hGameWnd,590,435);
		Sleep(3000);
	}

	return;*/
}
void CGTRYLZT::CloseAllWindow()
{
	if(IsWindowVisible(m_hGameWnd))
	{
		myApp.PressMouseKey(m_hGameWnd,1266,12);
		Sleep(2000);
		myApp.PressMouseKey(m_hGameWnd,595,435);
		Sleep(5000);
	}
	CStringArray arrWhiteList;
	arrWhiteList.Add("�˺�GTR");
	arrWhiteList.Add("��ʼ");
	arrWhiteList.Add("Program Manager");
	HWND hWnd = NULL;
	DWORD dwProcessID = 0;
	CString strProcessID = _T("");
	char szTitle[MAX_PATH] = {0};
	CString strTitle = "";

	hWnd = GetTopWindow(NULL);
	if (!hWnd)
		return ;

	hWnd = GetWindow( hWnd, GW_HWNDFIRST );
	while (hWnd)
	{
		if ( IsWindowVisible(hWnd) && !IsIconic(hWnd))
		{
			GetWindowTextA(hWnd, szTitle, MAX_PATH);
			strTitle.Format(_T("%s"), szTitle);
			strTitle.MakeUpper();
			BOOL bWhite=FALSE;
			for(int i=0;i<arrWhiteList.GetCount();i++)
			{
				CString strWhite=arrWhiteList.GetAt(i);
				if(strTitle==strWhite.MakeUpper())
				{
					bWhite=TRUE;
					break;
				}
			}
			if(!bWhite && strTitle!="")
			{
				WriteToFile ("�رմ���[%s]",strTitle);
				CaptureJpg("�رմ���",GetDesktopWindow());
				PostMessage(hWnd,WM_CLOSE,0,0);
				Sleep(1000);
			}

		}

		hWnd = GetWindow( hWnd, GW_HWNDNEXT );
	}
	hWnd=FindWindow(NULL,"�˺�GTR");
	if(IsWindowVisible(hWnd))
		ShowWindow(hWnd,SW_MINIMIZE);

	return;
}








//ѭ����ͼ
bool CGTRYLZT::CapturePictureByText(CString strName,CString strText1,CString strText2,int X,int Y,int Row,int Column,int IntervalX,int IntervalY,BOOL bAll,int Width,int left,int top,int right,int bottom,BOOL bMoveMouse)
{
	if(strName!=m_strLastName)
		m_nPicNum=0;
	m_strLastName=strName;



	POINT pt,pt2;
	//pt2 = ptInGame;
	CString strLog;
	int wide,height;
	CRect rect;
	::GetWindowRect(m_hGameWnd,&rect);
	for (int i=0;i<Row;i++)
	{
		for (int j=0;j<Column;j++)
		{
			if(!IsWindow(m_hGameWnd))
				return false;
			m_nPicNum++;
			SetForegroundWindow(m_hGameWnd);
			if(bMoveMouse)
			{
				myApp.MoveMouse(m_hGameWnd,50,50);
				Sleep(600);
			}

			myApp.MoveMouse(m_hGameWnd,X+j*IntervalX,Y+i*IntervalY);

			Sleep(600);
			//�Ҳ������Ͻ�,��û��������Ҫȫ��ִ����,�˳�������
			if(!myApp.FindBMPText(m_hGameWnd,"����",strText1,m_nFontSize1,RGB(239,22,48),&pt,left,top,right,bottom))
			{
				myApp.MoveMouse(m_hGameWnd,X+j*IntervalX+2,Y+i*IntervalY+2);
				Sleep(600);
				if(!myApp.FindBMPText(m_hGameWnd,"����",strText1,m_nFontSize1,RGB(239,22,48),&pt,left,top,right,bottom))
				{
					WriteToFile("[%s%d]������\r\n",strName,m_nPicNum);
					//WriteToFile("�Ҳ�������[%s]\r\n",strText1);
					if (bAll)//��Ҫȫ��ִ����,����ѭ��
						continue;
					else
						goto THE_END;
				}

				/*pt.x+=189;
				pt.y-=49;*/
			}

			if(!myApp.FindBMPText(m_hGameWnd,"����",strText2,m_nFontSize2,RGB(171,165,151),&pt2,pt.x-Width,pt.y+10,pt.x,bottom))
			{
				height=rect.Height();
				WriteToFile("�Ҳ�������[%s]\r\n",strText2);
				wide = Width;
			}
			else
			{
				wide = pt2.x+rectPY.right-pt.x+rectPY.left;
				height=pt2.y+rectPY.bottom-pt.y+rectPY.top;
			}

			//CString strFileName = RC2Tool::GetCaptureFileName( m_strPictureDir, m_strOPDN, m_strGameName, the_strOrderNo, m_strDeliverOpId );
			//CString strLog;
			//strLog.Format("��ȡСͼ,��=%d,��=%d\r\n",wide,height);
			//WriteToFile(strLog);
			if(wide>Lwidth)
				wide=Lwidth;
			if(height>Lheight)
				height=Lheight;
			if(wide<0 || height<0 /*|| wide<(Width-10) || pt2.x+rectPY.right<pt.x || pt2.y+rectPY.bottom<pt.y*/)
			{
				WriteToFile("Сͼʧ��,��=%d,��=%d,pt.x=%d,pt2.x=%d,pt2.y=%d\r\n",wide,height,pt.x,pt2.x,pt2.y);
				return false;
			}
			CString strPicName;
			strPicName.Format("%s%d.bmp",strName,m_nPicNum);
			if(pt2.x<0)
			{
				//WriteToFile("Сͼ��[%d]\r\n",pt.x-rectPY.left+wide);
				CaptureJpgInRect(strPicName,pt.x-rectPY.left,pt.y-rectPY.top,pt.x-rectPY.left+wide,height);
			}
			else
				CaptureJpgInRect(strPicName,pt.x-rectPY.left,pt.y-rectPY.top,pt2.x+rectPY.right,pt2.y+rectPY.bottom);
		}
	}
THE_END:

	myApp.MoveMouse(m_hGameWnd,50,50);
	Sleep(600);
	return true;
}
//����ͼƬ
int CGTRYLZT::SaveBigPic(HWND hWnd,CString strPicName,CString strPicID,int picWidth)
{



	if(strPicName=="����ͼƬ")
	{

		goto NEXT_STEP;
	}
	if (strPicName=="����")
	{
		ptMAX.x=max(ptBigPic.x,ptMAX.x);
		ptBigPic.x = 0;
		ptBigPic.y+=ptMAX.y;
		ptMAX.y=0;
		return 1;
	}
	if(strPicName.Find(".bmp")<0)
		strPicName+=".bmp";
	if(!PathFileExists(strPicName))
	{
		WriteToFile("ͼƬ[%s]������\r\n",strPicName);
		return FALSE;
	}
	//WriteToFile("Сͼ[%s]\r\n",strPicName);
	//WriteToFile("��Сͼƴ����ͼ����%d,%d\r\n",ptBigPic.x,ptBigPic.y);

	//Сͼװ������
	::HBITMAP PicH=  (HBITMAP) LoadImage(NULL,strPicName,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	BITMAP bm; 
	::GetObject(PicH,sizeof(BITMAP),&bm); 
	int Swidth =bm.bmWidth;
	int Sheight=bm.bmHeight;
#ifdef _TEST
	//
#else
	DeleteFile(strPicName);
#endif


	//WriteToFile("����[%s],��=%d,��=%d\r\n",strPicName,Swidth,Sheight);



	if (bPicFull)
	{

		/*pBigPic = new BYTE[Lwidth * Lheight * 4 ];*/
		if (pBigPic==NULL)
		{
			WriteToFile("�����ڴ�ʧ��\r\n");
			return 2120;
		}

		//����ɫ
		for(int y = 0; y < Lheight;y++)
		{
			for(int x = 0;x < Lwidth;x++)
			{
				pBigPic[ (Lwidth*y+x)*4 + 2] = bgR;
				pBigPic[ (Lwidth*y+x)*4 + 1] = bgG;
				pBigPic[ (Lwidth*y+x)*4 + 0] = bgB;
			}
		}
		//::memset(pBigPic,0,(Lwidth+3)/4*4 * Lheight * 4);
		bPicFull = FALSE;
	}




	if ((Lwidth-ptBigPic.x)<(Swidth/*+8*/) )
	{
		//WriteToFile("��Ȳ���,����\r\n");
		ptMAX.x=max(ptBigPic.x,ptMAX.x);
		ptBigPic.x = 0;
		ptBigPic.y+=ptMAX.y+1;
		ptMAX.y=0;
		//WriteToFile("ptBigPic.y=%d\r\n",ptBigPic.y);
	}

	if ((Lheight-ptBigPic.y)<Sheight)
	{
		WriteToFile("Lheight-ptBigPic.y=%d-%d",Lheight,ptBigPic.y);
		WriteToFile("�߶Ȳ���\r\n");
		return 2120;
		//bPicFull = TRUE;
		////nRetCode=2;
		//goto NEXT_STEP;
	}

	BYTE *tData;
	tData = new BYTE[(Swidth*4+4-Swidth%4) * Sheight];
	if (tData==NULL)
	{
		WriteToFile("tData�����ڴ�ʧ��\r\n");
		return 2120;
	}
	//memset(tData,255,(Swidth*4+4-Swidth%4) * Sheight);
	//PhotoToSmallPic(tData,pt,Swidth,Sheight);
	/*int n=0;
	for(int i=0;i<(Swidth * Sheight * 5);i++)
	{
	if(tData[i]==0)
	n++;
	}
	if (n>((Swidth * Sheight * 5)-10))
	::AfxMessageBox("ȫ��0");*/
	//strLog.Format("��Сͼƴ����ͼ����%d,%d\r\n",ptBigPic.x,ptBigPic.y);
	//WriteToFile(strLog);


	POINT ptLPic;
	ptLPic.y = ptBigPic.y;
	ptLPic.x = ptBigPic.x;


	//BOOL bbb=FALSE;
	//for(int y = 1; y <ptBigPic.y;y++)
	//{
	//	for(int x = 0;x < Swidth;x++)
	//	{
	//		r=pBigPic[ (Lwidth*(Lheight-(ptBigPic.y-y))+ x +ptBigPic.x)*4 + 2];
	//		g=pBigPic[ (Lwidth*(Lheight-(ptBigPic.y-y))+ x +ptBigPic.x)*4 + 1];
	//		b=pBigPic[ (Lwidth*(Lheight-(ptBigPic.y-y))+ x +ptBigPic.x)*4 + 0];
	//		if (!((r==0) && (g==0 )&&( b==0)))
	//		{
	//			bbb=TRUE;
	//			ptLPic.y = ptBigPic.y-y+1;
	//			break;
	//		}
	//	}
	//	if (bbb)
	//		break;
	//	WriteToFile("����һ���Ǻ�ɫ\r\n");
	//}
	//if (!bbb && ptBigPic.x>0)
	//{
	//	for(int x=1;x<ptBigPic.x;x++)
	//	{

	//		r=pBigPic[ (Lwidth*(Lheight-ptLPic.y- x) +ptBigPic.x-5)*4 + 2];
	//		g=pBigPic[ (Lwidth*(Lheight-ptLPic.y- x) +ptBigPic.x-5)*4 + 1];
	//		b=pBigPic[ (Lwidth*(Lheight-ptLPic.y- x) +ptBigPic.x-5)*4 + 0];
	//		if (!((r==0) && (g==0 )&&( b==0)) || (x==ptBigPic.x-1))
	//		{
	//			ptBigPic.x = ptBigPic.x-x;
	//			break;
	//		}
	//		WriteToFile("����Ǻ�ɫ\r\n");
	//	}
	//}


	long Ret1=GetBitmapBits (PicH,bm.bmWidth * bm.bmHeight * 4,&tData[0]);






	for(int y = 0; y < Sheight;y++)
	{
		for(int x = 0;x < Swidth;x++)
		{
			pBigPic[ (Lwidth*(Lheight-(ptLPic.y+y)-1)+ x +ptBigPic.x)*4 + 2] = tData[ (Swidth*(y) + x )*4 + 2];
			pBigPic[ (Lwidth*(Lheight-(ptLPic.y+y)-1)+ x +ptBigPic.x)*4 + 1] = tData[ (Swidth*(y) + x )*4 + 1];
			pBigPic[ (Lwidth*(Lheight-(ptLPic.y+y)-1)+ x +ptBigPic.x)*4 + 0] = tData[ (Swidth*(y) + x )*4 + 0];
		}
	}

	ptMAX.y = max(ptMAX.y,Sheight);/*ptMAX.y<Sheight?Sheight:ptMAX.y;*/
	//WriteToFile("ptMAX.y=%d\r\n",ptMAX.y);

	ptBigPic.x+=Swidth;
	delete []tData;
	tData = NULL;
	DeleteObject (PicH);

	//û��������ͼƬ
	if (!bPicFull)
		return 1;

NEXT_STEP:
	if(ptMAX.x==0 && ptMAX.y==0)
	{
		//WriteToFile("��ͼΪ��\r\n");
		return 2120;
	}







	////////////////����///////////////////////////////
	if(ptBigPic.y>0)//����
	{
		ptMAX.x=max(ptBigPic.x,ptMAX.x);
		ptMAX.y+=ptBigPic.y;
	}
	else
		ptMAX.x+=ptBigPic.x;
	if(the_strOrderNo.Find("MZH")==0)
		nZHPicWidth=ptMAX.x;


	//cout<<"ptMAX.x="<<ptMAX.x<<endl;
	/*BYTE *pBMPData=new BYTE [(nZHPicWidth*4+3)/4*4*ptMAX.y];
	ZeroMemory(pBMPData,(nZHPicWidth*4+3)/4*4*ptMAX.y);*/
	BYTE *pBMPData=new BYTE [(nZHPicWidth+3)/4*4*ptMAX.y*4];
	//�޸ı�����ɫ
	for(int y = 0; y < ptMAX.y;y++)
	{
		for(int x = 0;x < nZHPicWidth;x++)
		{
			pBMPData[ (nZHPicWidth*y+x)*4 + 2] = bgR;
			pBMPData[ (nZHPicWidth*y+x)*4 + 1] = bgG;
			pBMPData[ (nZHPicWidth*y+x)*4 + 0] = bgB;
		}
	}
	POINT ptSuoXiao;
	int y1=Lheight-ptMAX.y;
	int number1=0;
	int nInterval;
	if(ptMAX.x==nZHPicWidth)
		nInterval=1;
	else
		nInterval=nZHPicWidth/(ptMAX.x-nZHPicWidth);
	if(nInterval==1)
		nInterval=2;
	for(int y=0;y<ptMAX.y;y++)
	{
		number1=0;
		for(int x = 0;x < nZHPicWidth;x++)
		{
			if(x==0)
				ptSuoXiao.x=0;

			if((x%nInterval)==0 && ptMAX.x>nZHPicWidth && number1<=(ptMAX.x-nZHPicWidth))
			{
				//cout<<x<<","<<number1<<endl;
				pBMPData[(nZHPicWidth*y+x)*4 + 2]=(pBigPic[(Lwidth*y1+ptSuoXiao.x)*4 + 2]+pBigPic[(Lwidth*y1+ptSuoXiao.x+1)*4 + 2])/2;
				pBMPData[(nZHPicWidth*y+x)*4 + 1]=(pBigPic[(Lwidth*y1+ptSuoXiao.x)*4 + 1]+pBigPic[(Lwidth*y1+ptSuoXiao.x+1)*4 + 1])/2;
				pBMPData[(nZHPicWidth*y+x)*4 + 0]=(pBigPic[(Lwidth*y1+ptSuoXiao.x)*4 + 0]+pBigPic[(Lwidth*y1+ptSuoXiao.x+1)*4 + 0])/2;
				ptSuoXiao.x+=2;
				number1++;
				continue;
			}

			if(ptSuoXiao.x>=ptMAX.x)
				break;
			pBMPData[(nZHPicWidth*y+x)*4 + 2]=pBigPic[(Lwidth*y1+ptSuoXiao.x)*4 + 2];
			pBMPData[(nZHPicWidth*y+x)*4 + 1]=pBigPic[(Lwidth*y1+ptSuoXiao.x)*4 + 1];
			pBMPData[(nZHPicWidth*y+x)*4 + 0]=pBigPic[(Lwidth*y1+ptSuoXiao.x)*4 + 0];
			ptSuoXiao.x++;
		}
		y1++;
	}

	if(the_strOrderNo.Find("MZH")==0 /*|| the_strOrderNo=="���Զ���"*/)
	{
		/////////��ˮӡ/////////////////////////////////////////////////////////
		if(!PathFileExists(m_strPicPath+"\\ˮӡ.bmp"))
		{
			WriteToFile("ˮӡͼƬ������\r\n");
			goto WATERMARK_FAIL;
		}
		HBITMAP PicH1=  (HBITMAP) LoadImage(NULL,m_strPicPath+"\\ˮӡ.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
		BITMAP bm1; 
		::GetObject(PicH1,sizeof(BITMAP),&bm1); 
		int nBitsPixel=bm1.bmBitsPixel;
		//ȡ�������飬�����Ϊ������ÿ��β�Զ���1�ֽڲ��㣬��Ҫȡ�����,
		BYTE *tData1=new BYTE[(bm1.bmWidth * (nBitsPixel/8) + 3)/4*4* bm1.bmHeight];
		Ret1=GetBitmapBits (PicH1,(bm1.bmWidth * (nBitsPixel/8) + 3)/4*4* bm1.bmHeight,tData1);

		int num=0,dataNum=0;
		for(int i=0;i<bm1.bmHeight;i++)
		{

			num=bm1.bmWidth * (nBitsPixel/8)*i;
			for(int j=0;j<bm1.bmWidth;j++)
			{

				dataNum=nZHPicWidth*4*(bm1.bmHeight-i )+(nZHPicWidth-bm1.bmWidth+j)*4;

				pBMPData[dataNum+2]=tData1[num+2];
				pBMPData[dataNum+1]=tData1[num+1];
				pBMPData[dataNum+0]=tData1[num+0];


				num+=(nBitsPixel/8);
			}
		}

		delete []tData1;
		DeleteObject (PicH1);
	}
WATERMARK_FAIL:



	LPBITMAPINFO lpbmih = new BITMAPINFO;//ͼ�θ�ʽ����
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = nZHPicWidth;//
	lpbmih->bmiHeader.biHeight = ptMAX.y;
	lpbmih->bmiHeader.biPlanes = 1;//Ŀ���豸λ����������Ϊ1
	lpbmih->bmiHeader.biBitCount = 32;//��ɫ���
	lpbmih->bmiHeader.biCompression = BI_RGB;//λͼѹ������
	lpbmih->bmiHeader.biSizeImage = 0;//��С���ֽڵ�λ
	lpbmih->bmiHeader.biXPelsPerMeter = 0;//ˮƽ�ֱ���
	lpbmih->bmiHeader.biYPelsPerMeter = 0;//��ֱ�ֱ���
	lpbmih->bmiHeader.biClrUsed = 0;//λͼʵ��ʹ����ɫ������ɫ��
	lpbmih->bmiHeader.biClrImportant = 0;//λͼ��ʾ�бȽ���Ҫ����ɫ��

	BITMAPFILEHEADER bmfh;
	*((char *)&bmfh.bfType) = 'B';
	*(((char *)&bmfh.bfType) + 1) = 'M';//������BMP�ļ�
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//ƫ��λ��
	//int iBMPBytes = (Lwidth*Lheight) * 32 / 8;//�����ļ���С
	//bmfh.bfSize = bmfh.bfOffBits + (Lwidth*Lheight) * 32 / 8;//�ļ���С
	int iBMPBytes = (nZHPicWidth * 4+3)/4*4*ptMAX.y;//�����ļ���С
	bmfh.bfSize = bmfh.bfOffBits + iBMPBytes;//�ļ���С
	CFile file;
	CString PicName = SetPicName(m_strCapturePath,strPicID);
	if(file.Open(PicName,CFile::modeWrite | CFile::modeCreate))
	{
		file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
		file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
		file.Write(pBMPData,iBMPBytes);
		file.Close();
	}
	/*if(the_strOrderNo.Find("MZH")<0 && nZHPicWidth>880)
	{
	ShellExecuteA(NULL,"open",m_strProgPath+"\\����ͼƬ.exe",PicName,NULL,SW_SHOW);
	Sleep(300);
	}*/

	myApp.BMPToJPG(PicName);


	delete []pBMPData;
	bPicFull = TRUE;
	ptBigPic.x=0;
	ptBigPic.y=0;
	ptMAX.y=0;
	ptMAX.x=0;
	return 1;
}

//����ͼƬ
int CGTRYLZT::SaveBigPic(CString strPicName,CString strPicID,CString strPath)
{



	if(strPicName=="����ͼƬ")
	{

		goto NEXT_STEP;
	}
	if (strPicName=="����")
	{
		ptMAX.x=max(ptBigPic.x,ptMAX.x);
		ptBigPic.x = 0;
		ptBigPic.y+=ptMAX.y;
		ptMAX.y=0;
		return 1;
	}
	if(strPicName.Find(".bmp")<0)
		strPicName+=".bmp";
	if(!PathFileExists(strPicName))
	{
		//WriteToFile("ͼƬ[%s]������\r\n",strPicName);
		return FALSE;
	}
	//WriteToFile("Сͼ[%s]\r\n",strPicName);
	//WriteToFile("��Сͼƴ����ͼ����%d,%d\r\n",ptBigPic.x,ptBigPic.y);

	//Сͼװ������
	::HBITMAP PicH=  (HBITMAP) LoadImage(NULL,strPicName,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	BITMAP bm; 
	::GetObject(PicH,sizeof(BITMAP),&bm); 
	int Swidth =bm.bmWidth;
	int Sheight=bm.bmHeight;







	if (bPicFull)
	{

		pBigPic = new BYTE[Lwidth * Lheight * 4 ];
		if (pBigPic==NULL)
		{
			WriteToFile("�����ڴ�ʧ��\r\n");
			return 2120;
		}
		//����ɫ
		for(int y = 0; y < Lheight;y++)
		{
			for(int x = 0;x < Lwidth;x++)
			{
				pBigPic[ (Lwidth*y+x)*4 + 2] = 255;
				pBigPic[ (Lwidth*y+x)*4 + 1] = 0;
				pBigPic[ (Lwidth*y+x)*4 + 0] = 0;
			}
		}

		bPicFull = FALSE;
	}




	if ((Lwidth-ptBigPic.x)<(Swidth/*+8*/) )
	{
		//WriteToFile("��Ȳ���,����\r\n");
		ptMAX.x=max(ptBigPic.x,ptMAX.x);
		ptBigPic.x = 0;
		ptBigPic.y+=ptMAX.y+1;
		ptMAX.y=0;
		//WriteToFile("ptBigPic.y=%d\r\n",ptBigPic.y);
	}

	if ((Lheight-ptBigPic.y)<Sheight)
	{
		WriteToFile("Lheight-ptBigPic.y=%d-%d",Lheight,ptBigPic.y);
		WriteToFile("�߶Ȳ���\r\n");
		return 2120;
		//bPicFull = TRUE;
		////nRetCode=2;
		//goto NEXT_STEP;
	}

	BYTE *tData;
	tData = new BYTE[(Swidth*4+4-Swidth%4) * Sheight];
	if (tData==NULL)
	{
		WriteToFile("tData�����ڴ�ʧ��\r\n");
		return 2120;
	}



	POINT ptLPic;
	ptLPic.y = ptBigPic.y;
	ptLPic.x = ptBigPic.x;





	long Ret1=GetBitmapBits (PicH,bm.bmWidth * bm.bmHeight * 4,&tData[0]);






	for(int y = 0; y < Sheight;y++)
	{
		for(int x = 0;x < Swidth;x++)
		{
			if(bm.bmBitsPixel==32)
			{
				pBigPic[ (Lwidth*(Lheight-(ptLPic.y+y)-1)+ x +ptBigPic.x)*4 + 2] = tData[ (Swidth*(y) + x )*4 + 2];
				pBigPic[ (Lwidth*(Lheight-(ptLPic.y+y)-1)+ x +ptBigPic.x)*4 + 1] = tData[ (Swidth*(y) + x )*4 + 1];
				pBigPic[ (Lwidth*(Lheight-(ptLPic.y+y)-1)+ x +ptBigPic.x)*4 + 0] = tData[ (Swidth*(y) + x )*4 + 0];
			}
			else
			{
				pBigPic[ (Lwidth*(Lheight-(ptLPic.y+y)-1)+ x +ptBigPic.x)*4 + 2] = tData[ y* (bm.bmWidth *3+(bm.bmWidth % 2))+ x*3 + 2];
				pBigPic[ (Lwidth*(Lheight-(ptLPic.y+y)-1)+ x +ptBigPic.x)*4 + 1] = tData[ y* (bm.bmWidth *3+(bm.bmWidth % 2))+ x*3 + 1];
				pBigPic[ (Lwidth*(Lheight-(ptLPic.y+y)-1)+ x +ptBigPic.x)*4 + 0] = tData[ y* (bm.bmWidth *3+(bm.bmWidth % 2))+ x*3 + 0];

				/*b=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3];
				g=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+1];
				r=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+2];*/
			}


		}
	}

	ptMAX.y = max(ptMAX.y,Sheight);/*ptMAX.y<Sheight?Sheight:ptMAX.y;*/
	//WriteToFile("ptMAX.y=%d\r\n",ptMAX.y);

	ptBigPic.x+=Swidth;
	delete []tData;
	tData = NULL;
	DeleteObject (PicH);

	//û��������ͼƬ
	if (!bPicFull)
		return 1;

NEXT_STEP:
	if(ptMAX.x==0 && ptMAX.y==0)
	{
		//WriteToFile("��ͼΪ��\r\n");
		return 2120;
	}







	////////////////����///////////////////////////////
	if(ptBigPic.y>0)//����
	{
		ptMAX.x=max(ptBigPic.x,ptMAX.x);
		ptMAX.y+=ptBigPic.y;
	}
	else
		ptMAX.x+=ptBigPic.x;




	BYTE *pBMPData=new BYTE [(ptBigPic.x+3)/4*4*ptMAX.y*4];
	//�޸ı�����ɫ
	for(int y = 0; y < ptMAX.y;y++)
	{
		for(int x = 0;x < ptBigPic.x;x++)
		{
			pBMPData[ (ptBigPic.x*y+x)*4 + 2] = 255;
			pBMPData[ (ptBigPic.x*y+x)*4 + 1] = 255;
			pBMPData[ (ptBigPic.x*y+x)*4 + 0] = 255;
		}
	}

	int y1=Lheight-ptMAX.y;
	for(int y=0;y<ptMAX.y;y++)
	{

		for(int x = 0;x < ptBigPic.x;x++)
		{
			pBMPData[(ptBigPic.x*y+x)*4 + 2]=pBigPic[(Lwidth*y1+x)*4 + 2];
			pBMPData[(ptBigPic.x*y+x)*4 + 1]=pBigPic[(Lwidth*y1+x)*4 + 1];
			pBMPData[(ptBigPic.x*y+x)*4 + 0]=pBigPic[(Lwidth*y1+x)*4 + 0];
		}
		y1++;
	}





	LPBITMAPINFO lpbmih = new BITMAPINFO;//ͼ�θ�ʽ����
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = ptBigPic.x;//
	lpbmih->bmiHeader.biHeight = ptMAX.y;
	lpbmih->bmiHeader.biPlanes = 1;//Ŀ���豸λ����������Ϊ1
	lpbmih->bmiHeader.biBitCount = 32;//��ɫ���
	lpbmih->bmiHeader.biCompression = BI_RGB;//λͼѹ������
	lpbmih->bmiHeader.biSizeImage = 0;//��С���ֽڵ�λ
	lpbmih->bmiHeader.biXPelsPerMeter = 0;//ˮƽ�ֱ���
	lpbmih->bmiHeader.biYPelsPerMeter = 0;//��ֱ�ֱ���
	lpbmih->bmiHeader.biClrUsed = 0;//λͼʵ��ʹ����ɫ������ɫ��
	lpbmih->bmiHeader.biClrImportant = 0;//λͼ��ʾ�бȽ���Ҫ����ɫ��

	BITMAPFILEHEADER bmfh;
	*((char *)&bmfh.bfType) = 'B';
	*(((char *)&bmfh.bfType) + 1) = 'M';//������BMP�ļ�
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//ƫ��λ��
	//int iBMPBytes = (Lwidth*Lheight) * 32 / 8;//�����ļ���С
	//bmfh.bfSize = bmfh.bfOffBits + (Lwidth*Lheight) * 32 / 8;//�ļ���С
	int iBMPBytes = (ptBigPic.x * 4+3)/4*4*ptMAX.y;//�����ļ���С
	bmfh.bfSize = bmfh.bfOffBits + iBMPBytes;//�ļ���С
	CFile file;
	CString PicName = strPath+"\\"+strPicID+".bmp";
	if(file.Open(PicName,CFile::modeWrite | CFile::modeCreate))
	{
		file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
		file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
		file.Write(pBMPData,iBMPBytes);
		file.Close();
	}



	delete []pBMPData;
	bPicFull = TRUE;
	ptBigPic.x=0;
	ptBigPic.y=0;
	ptMAX.y=0;
	ptMAX.x=0;
	return 1;
}
//��ȡСͼƬ
bool CGTRYLZT::PhotoToSmallPic(BYTE *&tData,POINT pt,int Sweight,int Sheight)
{
	/*CRect rect;
	::GetWindowRect( m_hGameWnd, &rect );*/

	int Height=GetSystemMetrics (SM_CYSCREEN);
	int Width=GetSystemMetrics( SM_CXSCREEN);
	int left=pt.x/*-rect.left*/;
	int top=pt.y/*-rect.top*/;
	int right= left + Sweight;
	int bottom= top + Sheight;

	CString PicName=m_strProgPath+"\\Сͼ.bmp";

	//PicName.Format("F:\\%d.bmp",0);

	//RC2Tool::CaptureGamePictureEx(NULL,PicName,left,top,Sweight,Sheight);
	return true;

}
//ƴͼ(��Ҫƴ��һ���ͼƬ��,�ö��ŷָ�)
int CGTRYLZT::PinTu(CString strAllPic,CString strPicID,int picWidth)
{
	CStringArray arrPicName;
	myApp.SplitString(strAllPic,",",arrPicName);

	int status=0;
	for(int i=0;i<arrPicName.GetSize();i++)
	{
		if(arrPicName.GetAt(i)=="����")
		{
			SaveBigPic(m_hGameWnd,"����");
			continue;
		}

		status=SaveBigPic(m_hGameWnd,m_strCapturePath+arrPicName.GetAt(i),"",picWidth);
		if(status>1000)
			return status;
	}


	SaveBigPic(m_hGameWnd,"����ͼƬ",strPicID);
	arrPicName.RemoveAll();
	return 1;
}
//ƴͼ(��Ҫƴ��һ���ͼƬ��,�ö��ŷָ�)
int CGTRYLZT::PinTu(CString strAllPic,CString strPicID,CString path)
{
	CStringArray arrPicName;
	myApp.SplitString(strAllPic,",",arrPicName);

	int status=0;
	for(int i=0;i<arrPicName.GetSize();i++)
	{
		if(arrPicName.GetAt(i)=="����")
		{
			SaveBigPic("����",strPicID,path);
			continue;
		}

		status=SaveBigPic(path+"\\"+arrPicName.GetAt(i),strPicID,path);
		if(status>1000)
			return status;
	}

	SaveBigPic("����ͼƬ",strPicID,path);
	arrPicName.RemoveAll();
	return 1;
}

CString CGTRYLZT::SetPicName(CString str,CString strPicID)
{
	static int PicNum=1;
	static CString strLastPicID="";
	if(strLastPicID!=strPicID)
		PicNum=1;
	if(str.Right(1)!="\\")
		str+="\\";
	CString strFileName;
	if(strPicID.IsEmpty())
		strFileName.Format("%sR_%02d.bmp",str,PicNum++);
	else
		strFileName.Format("%s%s_%02d.bmp",str,strPicID,PicNum++);
	strLastPicID=strPicID;
	return strFileName;
}

void CGTRYLZT::KillProcess()
{
	if (the_strOrderNo == _T("���Զ���"))
		return;

//	CloseGame();

	
	myApp.KillWindow("Droid4X.exe");
	Sleep(100);
	myApp.KillWindow("Droid4XService.exe");
	Sleep(100);
	myApp.KillWindow("kpzsPay.exe");
	Sleep(100);
	myApp.KillWindow("VBoxSVC.exe");
	Sleep(100);
	myApp.KillWindow("VBoxHeadless.exe");
	Sleep(100);

	//WinExec(m_strProgPath+"\\1.bat",SW_SHOW);//������WorldOfWarships.exe


	/*for(int i=0;i<20;i++)
	{
		myApp.MoveMouse(GetDesktopWindow(),900+i*10,1005);
		Sleep(50);
	}*/
	//myApp.MoveMouse(GetDesktopWindow(),50,50);

}
//����ͼƬ
int CGTRYLZT::SaveBMPVertical(HWND hWnd,CString strPicName,CString strPicID,CString PicPath)
{


	if(strPicName=="����ͼƬ")
	{

		goto NEXT_STEP;
	}
	if (strPicName=="����")
	{
		ptMAX.x=max(ptBigPic.x,ptMAX.x);
		ptBigPic.x = 0;
		ptBigPic.y+=ptMAX.y+1;
		ptMAX.y=0;
		return 1;
	}
	if(strPicName.Find(".bmp")<0)
		strPicName+=".bmp";
	if(!PathFileExists(strPicName))
	{
		//WriteToFile("ͼƬ[%s]������\r\n",strPicName);
		return FALSE;
	}
	//WriteToFile("Сͼ[%s]\r\n",strPicName);
	//WriteToFile("��Сͼƴ����ͼ����%d,%d\r\n",ptBigPic.x,ptBigPic.y);

	//Сͼװ������
	::HBITMAP PicH=  (HBITMAP) LoadImage(NULL,strPicName,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	BITMAP bm; 
	::GetObject(PicH,sizeof(BITMAP),&bm);
	int Swidth =bm.bmWidth;
	int Sheight=bm.bmHeight;
#ifdef _TEST
	//
#else
	DeleteFile(strPicName);
#endif


	//WriteToFile("����[%s],��=%d,��=%d\r\n",strPicName,Swidth,Sheight);


	//Sleep(200);
	if (bPicFull)
	{

		/*pBigPic = new BYTE[Lwidth * Lheight * 4 ];*/
		if (pBigPic==NULL)
		{
			WriteToFile("�����ڴ�ʧ��\r\n");
			return 2120;
		}

		for(int y = 0; y < Lheight;y++)
		{
			for(int x = 0;x < Lwidth;x++)
			{
				pBigPic[ (Lwidth*y+x)*4 + 2] = 24;
				pBigPic[ (Lwidth*y+x)*4 + 1] = 16;
				pBigPic[ (Lwidth*y+x)*4 + 0] = 8;
			}
		}
		//::memset(pBigPic,0,(Lwidth+3)/4*4 * Lheight * 4);
		bPicFull = FALSE;
	}




	if ((Lwidth-ptBigPic.x)<(Swidth/*+8*/) )
	{
		//WriteToFile("��Ȳ���,����\r\n");
		ptMAX.x=max(ptBigPic.x,ptMAX.x);
		ptBigPic.x = 0;
		ptBigPic.y+=ptMAX.y+1;
		ptMAX.y=0;
		//WriteToFile("ptBigPic.y=%d\r\n",ptBigPic.y);
	}

	if ((Lheight-ptBigPic.y)<Sheight)
	{
		WriteToFile("Lheight-ptBigPic.y=%d-%d",Lheight,ptBigPic.y);
		WriteToFile("�߶Ȳ���\r\n");
		return 2120;
		//bPicFull = TRUE;
		////nRetCode=2;
		//goto NEXT_STEP;
	}

	BYTE *tData;
	tData = new BYTE[(Swidth*4+4-Swidth%4) * Sheight];
	if (tData==NULL)
	{
		WriteToFile("tData�����ڴ�ʧ��\r\n");
		return 2120;
	}



	POINT ptLPic;
	ptLPic.y = ptBigPic.y;
	ptLPic.x = ptBigPic.x;


	long Ret1=GetBitmapBits (PicH,bm.bmWidth * bm.bmHeight * 4,&tData[0]);



WATERMARK_FAIL:

	for(int y = 0; y < Sheight;y++)
	{
		for(int x = 0;x < Swidth;x++)
		{
			pBigPic[ (Lwidth*(Lheight-(ptLPic.y+y)-1)+ x +ptBigPic.x)*4 + 2] = tData[ (Swidth*(y) + x )*4 + 2];
			pBigPic[ (Lwidth*(Lheight-(ptLPic.y+y)-1)+ x +ptBigPic.x)*4 + 1] = tData[ (Swidth*(y) + x )*4 + 1];
			pBigPic[ (Lwidth*(Lheight-(ptLPic.y+y)-1)+ x +ptBigPic.x)*4 + 0] = tData[ (Swidth*(y) + x )*4 + 0];
		}
	}

	ptMAX.y = max(ptMAX.y,Sheight);/*ptMAX.y<Sheight?Sheight:ptMAX.y;*/
	//WriteToFile("ptMAX.y=%d\r\n",ptMAX.y);

	ptBigPic.x+=Swidth;
	delete []tData;
	tData = NULL;
	DeleteObject (PicH);

	//û��������ͼƬ
	if (!bPicFull)
		return 1;

NEXT_STEP:
	if(ptMAX.x==0 && ptMAX.y==0)
	{
		WriteToFile("��ͼΪ��\r\n");
		return 2120;
	}







	////////////////����///////////////////////////////
	if(ptBigPic.y>0)//����
	{
		ptMAX.x=max(ptBigPic.x,ptMAX.x);
		ptMAX.y+=ptBigPic.y;
	}
	else
		ptMAX.x+=ptBigPic.x;

	BYTE *pBMPData=new BYTE [(ptMAX.x+3)/4*4*ptMAX.y*4];
	//�޸ı�����ɫ
	for(int y = 0; y < ptMAX.y;y++)
	{
		for(int x = 0;x < ptMAX.x;x++)
		{
			pBMPData[ (ptMAX.x*y+x)*4 + 2] = 24;
			pBMPData[ (ptMAX.x*y+x)*4 + 1] = 16;
			pBMPData[ (ptMAX.x*y+x)*4 + 0] = 8;
		}
	}
	POINT ptSuoXiao;
	int y1=Lheight-ptMAX.y;


	for(int y=0;y<ptMAX.y;y++)
	{
		for(int x = 0;x < ptMAX.x;x++)
		{
			if(x==0)
				ptSuoXiao.x=0;

			pBMPData[(ptMAX.x*y+x)*4 + 2]=pBigPic[(Lwidth*y1+ptSuoXiao.x)*4 + 2];
			pBMPData[(ptMAX.x*y+x)*4 + 1]=pBigPic[(Lwidth*y1+ptSuoXiao.x)*4 + 1];
			pBMPData[(ptMAX.x*y+x)*4 + 0]=pBigPic[(Lwidth*y1+ptSuoXiao.x)*4 + 0];
			ptSuoXiao.x++;
		}
		y1++;
	}


	LPBITMAPINFO lpbmih = new BITMAPINFO;//ͼ�θ�ʽ����
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = ptMAX.x;//
	lpbmih->bmiHeader.biHeight = ptMAX.y;
	lpbmih->bmiHeader.biPlanes = 1;//Ŀ���豸λ����������Ϊ1
	lpbmih->bmiHeader.biBitCount = 32;//��ɫ���
	lpbmih->bmiHeader.biCompression = BI_RGB;//λͼѹ������
	lpbmih->bmiHeader.biSizeImage = 0;//��С���ֽڵ�λ
	lpbmih->bmiHeader.biXPelsPerMeter = 0;//ˮƽ�ֱ���
	lpbmih->bmiHeader.biYPelsPerMeter = 0;//��ֱ�ֱ���
	lpbmih->bmiHeader.biClrUsed = 0;//λͼʵ��ʹ����ɫ������ɫ��
	lpbmih->bmiHeader.biClrImportant = 0;//λͼ��ʾ�бȽ���Ҫ����ɫ��

	BITMAPFILEHEADER bmfh;
	*((char *)&bmfh.bfType) = 'B';
	*(((char *)&bmfh.bfType) + 1) = 'M';//������BMP�ļ�
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//ƫ��λ��
	//int iBMPBytes = (Lwidth*Lheight) * 32 / 8;//�����ļ���С
	//bmfh.bfSize = bmfh.bfOffBits + (Lwidth*Lheight) * 32 / 8;//�ļ���С
	int iBMPBytes = (ptMAX.x * 4+3)/4*4*ptMAX.y;//�����ļ���С
	bmfh.bfSize = bmfh.bfOffBits + iBMPBytes;//�ļ���С
	CFile file;
	CString PicName;
	if(PicPath.IsEmpty())
		PicName = m_strCapturePath+strPicID+".bmp";
	else
		PicName = PicPath+strPicID+".bmp";
	//CString PicName = m_strCapturePath+strPicID+".bmp";
	if(file.Open(PicName,CFile::modeWrite | CFile::modeCreate))
	{
		file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
		file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
		file.Write(pBMPData,iBMPBytes);
		file.Close();
	}

	delete []pBMPData;
	bPicFull = TRUE;
	ptBigPic.x=0;
	ptBigPic.y=0;
	ptMAX.y=0;
	ptMAX.x=0;
	return 1;
}
//ѭ���鿴�ļ��Ƿ����,���ش��ڵ��ļ���
CString CGTRYLZT::FileExist(CString strName)
{
	CString str="";
	while(strName.Find(",")>0)
	{
		str=strName.Left(strName.Find(","));
		strName=strName.Mid(strName.Find(",")+1);
		if(PathFileExists(m_strCapturePath+str+".bmp"))
			return str;
	}
	return "";
}
void CGTRYLZT::ClearAppData()
{

	CString strUser=myApp.GetWinUser();
	CString strFolder;
	strFolder.Format("C:\\Users\\%s\\AppData\\Roaming\\Tencent\\TGP\\Users",strUser);
	strFolder.Replace("jhlcitadmin","administrator");
	myApp.DeleteFolder(strFolder);
	Sleep(1000);
}
/************************************************************************/
/* ����Wangyi.exe����ִ����ҳ��¼���˺Ű���Ϣ��ͼ�Ĺ���               */
/************************************************************************/
BOOL CGTRYLZT::CheckAccount()
{
	CString stry;
	CString orderdata;
	HWND hwnd1;

	orderdata.Format("��Ϸ���ƣ�����ʦ����Ϸ������%s����Ϸ��������%s����Ϸ�ʺţ�%s����Ϸ���룺%s��",m_strArea,m_strServer,m_strAccount,m_strPassword);
	stry.Format("%s %s %d %d %s %s",m_strCapturePath,the_strOrderNo,0,0,orderdata,"0");
	if(::ShellExecuteA (NULL,"open", m_strProgPath+"\\WangYi.exe",stry,m_strProgPath ,1)< (HINSTANCE) 31)
		return 2260;
	
	Sleep(5000);
	for (int i=0;i<5;i++)
	{
		hwnd1=::FindWindowA(NULL,"WangYi");
		if (hwnd1)
		{
			break;
		}
		else
		{
			if(::ShellExecuteA (NULL,"open", m_strProgPath+"\\WangYi.exe",stry,m_strProgPath ,1)< (HINSTANCE) 31)
				return 2260;
			Sleep(1000*10);
		}
	}
	for (int i=0;i<3*60;i++)
	{
		if (i%20==19)
		{
			WriteToFile("�ȴ��˺���֤���...");
		}
		hwnd1=::FindWindowA(NULL,"WangYi");
		if (hwnd1)
		{
			Sleep(1000);
		}
		else
		{
			char strBuilder[520];
			::GetPrivateProfileString("�˺���Ϣ", "ִ��״̬", "", strBuilder, 520,m_strProgPath+"\\roleInfo.ini");
			CString s;
			s.Format("%s",strBuilder);
			if (s=="1000")
			{
				WriteToFile("�˺�������ȷ");
			
				return 1000;
			}
			else if (s=="2000")
			{
				WriteToFile("�˺��������");
				
				return 3000;
			}
			else
			{
				WriteToFile("�˺�������֤ʧ��");
				
				return 2120;
			}
		}
	}
	WriteToFile("�ȴ��˺���֤�����ʱ");
	return 2120;


	/*for(int i=0;i<m_strAccount.GetLength();i++)
	{
		int n=m_strAccount.GetAt(i);
		if(n<48 || n>57)
			return FALSE;
	}

	return TRUE;*/
}


/************************************************************************/
/* 
TODO:װ�����Խ�ͼ
��������װ��ÿһ��װ�����Խ�ͼ��ͷ�������ε�  
TODO:��װ���ֿ��ͼ
�Բֿ��е���Ʒ��ͼ���������ҵİ���
TODO:������Ʒ��ͼ,ֻ�����ҵİ���
960550->960150 �϶���Χ
590,120->1000,590 ��ͼ��Χ
TODO:����������ͼ
1060,450 ������ť
150,120->560,650 ��ͼ��Χ
*/
/************************************************************************/
void CGTRYLZT::CapturePackage(HWND m_hGameWnd)
{
	BackToGamePage(m_hGameWnd,m_strPicPath);

	WriteToFile("����װ����ͼ");
	POINT pt;
	Sleep(2000);
	for (int i=0;i<5;i++)
	{
		//TODO:���Ұ�����ͼƬ
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�����˵�",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,pt.x+10,pt.y+10);//�������[1130,560]
			break;
		}
		else WriteToFile("�Ҳ��������˵�%d��",i);
		Sleep(1000);
	}
	if (pt.x<0)
	{
		WriteToFile("�Ҳ��������˵�");
		return;
	}
	
	Sleep(5000);

	CaptureJpgInRect("����",150,110,560,650);
	CaptureJpgInRect("��Ϸ��",130,620,570,700);
	
	BOOL flag=FALSE;//�ж�������ʾ���ǲ��ǵ�һ����װ
	if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"װ���л�",&pt))
	{
		flag=TRUE;
	}
	//TODO��װ�����Խ�ͼ
	for(int t=1;t<=16;t++)
	{
		if(t==9)
		{
			//TODO���л���װʱ�ж��Ƿ���������װ�����л�
			myApp.PressMouseKey(m_hGameWnd,340,140);//�л���װ
			Sleep(2000);
			if(flag==TRUE && myApp.FindBmp(m_hGameWnd,m_strPicPath+"װ���л�",&pt))
			{
				WriteToFile("��ҵȼ�δ��60��");
				break;
			}
		}
		for (int i=0;i<2;i++)
		{
			int temp=t;
			if(temp>8) temp-=8;
			if(t%2!=0) myApp.PressMouseKey(m_hGameWnd,200,210+temp/2*100);
			else myApp.PressMouseKey(m_hGameWnd,490,210+(temp-1)/2*100);
			Sleep(2000);
			if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"װ��ж��",&pt,0,0,0,0,TRUE,60)
				|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"װ��ж��2",&pt,0,0,0,0,TRUE,60)
				/*||myApp.FindBmp(m_hGameWnd,m_strPicPath+"�鿴װ��",&pt,0,0,0,0,TRUE,60,TRUE)*/)
			{
				CString s;
				s.Format("%s%d","����װ��",t);
				CaptureJpgInRect(s,600,50,930,680);//װ�����Խ�ͼ
				break;
			}
		}
	}
	WriteToFile("���װ�����Խ�ͼ");

	//TODO:��װ���ֿ��ͼ
	myApp.PressMouseKey(m_hGameWnd,1060,330);//����ֿ�
	Sleep(5000);
	CString s;
	int f=0;
	for(int i=0;i<12;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�ֿ�1",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"�ֿ�2",&pt))
		{
			f++;
			WriteToFile("�ҵ���һ���ֿ�%d��",f);
		}
		if(f>1) break;
		s.Format("%s%d","�ֿ�",i+1);
		CaptureJpgInRect(s,150,110,550,590);//�ֿ���Ʒ��ͼ
		WriteToFile("��ɶԲֿ�%d��ͼ",i);
		//myApp.PressMouseKey(m_hGameWnd,340,150);//���ѡ��ֿ�
		myApp.PressMouseKey(m_hGameWnd,280,640);//�����һ���ֿⰴť
		Sleep(2000);
	}

	//TODO:������Ʒ��ͼ
	for (int i=0;i<4;i++)
	{
		WriteToFile("������Ʒ��ͼ%d��",i);
		s.Format("%s%d","������Ʒ",i+1);
		CaptureJpgInRect(s,590,120,1000,590);
		myApp.DragMouse(m_hGameWnd,960,150,960,550);//�϶��б�����
		WriteToFile("�϶���Ʒ�б�����");
		Sleep(1000);
	}
	//TODO:����������ͼ
	//1060,450 ������ť
	//150,120->560,650 ��ͼ��Χ
	WriteToFile("����������ͼ");
	Sleep(2000);
	if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"����",&pt))
	{
		myApp.PressMouseKey(m_hGameWnd,1060,450);
		Sleep(1000);
		myApp.PressMouseKey(m_hGameWnd,250,400);
		Sleep(1000);
		CaptureJpgInRect("��������",150,120,560,650);
		myApp.PressMouseKey(m_hGameWnd,450,400);
		Sleep(1000);
		CaptureJpgInRect("��������",150,120,560,650);
	}
	else WriteToFile("������û�з���");
	
	myApp.PressMouseKey(m_hGameWnd,1020,80);//�رհ���ҳ��
	Sleep(1000);
}
/************************************************************************/
/* TODO:��������Ϣ��ͼ
�ֱ������ԡ���Ϣ���ӵ�Ľ�ͼ
*/
/************************************************************************/
void CGTRYLZT::CapturePlayerInfo(HWND m_hGameWnd)
{

	BackToGamePage(m_hGameWnd,m_strPicPath);

	Sleep(2000);
	WriteToFile ("�������Ͻ�ͼ");
	myApp.PressMouseKey(m_hGameWnd,1130,80);//�����������,��ɫͷ��
	Sleep(2000);
	CaptureJpgInRect("��������",120,60,1020,660,TRUE, RGB( 255, 188,0 ),260,110,400,150);//��ɫ����ˮӡ
	Sleep(1000);



	myApp.PressMouseKey(m_hGameWnd,1060,330);//���������Ϣ
	Sleep(2000);
	CaptureJpgInRect("������Ϣ",120,60,1020,660);
	Sleep(1000);

	//TODO:����ӵ���Ϣ�ж�������ѡ�н�ͼ������
	myApp.PressMouseKey(m_hGameWnd,1060,450);//�������ӵ�
	Sleep(2000);
	//CaptureJpgInRect("����ӵ�1",120,60,1020,660);
	//Sleep(1000);
	myApp.PressMouseKey(m_hGameWnd,250,140);//�л��ӵ㷽��
	Sleep(1000);
	myApp.PressMouseKey(m_hGameWnd,250,220);//�ӵ㷽��1
	Sleep(1000);
	CaptureJpgInRect("�ӵ㷽��1",140,110,1000,650);

	myApp.PressMouseKey(m_hGameWnd,250,140);//�л��ӵ㷽��
	Sleep(1000);
	myApp.PressMouseKey(m_hGameWnd,250,300);//�ӵ㷽��2
	Sleep(1000);
	CaptureJpgInRect("�ӵ㷽��2",140,110,1000,650);

	myApp.PressMouseKey(m_hGameWnd,1020,80);//�ر��������ϲ��
}
/************************************************************************/
/* TODO���Գ�����Ϣ��ͼ
��������������ԣ����ʼ��ܣ������ڵ�
*/
/************************************************************************/
void CGTRYLZT::CapturePet(HWND m_hGameWnd)
{

	WriteToFile("��ʼ�Գ�����Ϣ��ͼ");
	CString s;
	POINT pt;
	for (int n=0;n<5;n++)
	{
		//������Ϸҳ��
		BackToGamePage(m_hGameWnd,m_strPicPath);

		myApp.PressMouseKey(m_hGameWnd,980,80);//����ͷ��
		Sleep(2000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"����",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"��������",&pt))
		{
			for (int i=0;i<8;i++)
			{
				myApp.PressMouseKey(m_hGameWnd,200+(i%4)*100,520+100*(i/4));//ѡ�г���
				Sleep(2000);
				//TODO:û�иó���
				if (!myApp.FindBmp(m_hGameWnd,m_strPicPath+"��������",&pt)
					|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"�������",&pt)
					|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"�������2",&pt))
				{
					myApp.PressMouseKey(m_hGameWnd,1060,80);//�رչ�����ﴰ��
					Sleep(1000);
					break;
				}
				myApp.PressMouseKey(m_hGameWnd,650,140);//�������Ļ�������
				Sleep(1000);
				s.Format("%s%d","����",i+1);
				CaptureJpgInRect(s+"��������",140,120,1010,670);
				myApp.PressMouseKey(m_hGameWnd,800,140);//���ʼ���
				Sleep(1000);
				CaptureJpgInRect(s+"���ʼ���",580,120,1010,660);
				myApp.PressMouseKey(m_hGameWnd,940,140);
				Sleep(1000);
				CaptureJpgInRect(s+"�����ڵ�",580,120,1010,660);
			}	
			myApp.PressMouseKey(m_hGameWnd,1020,80);//�رճ������
			WriteToFile("������Ϣ��ͼ���");
			break;
		}
		else WriteToFile("�Ҳ���������Ϣ%d��",n);
	}
	
	
	
	//TODO:�˻ص���Ϸҳ��
	BackToGamePage(m_hGameWnd,m_strPicPath);
}
/************************************************************************/
/* TODO:�����＼�ܽ�ͼ
�������＼�ܣ����ɼ��ܺ���������
*/
/************************************************************************/

void CGTRYLZT::CaptureSkill(HWND m_hGameWnd)
{
	BackToGamePage(m_hGameWnd,m_strPicPath);

	WriteToFile("���＼�ܽ�ͼ");
	POINT pt;
	for (int i=0;i<5;i++)
	{
		if (!myApp.FindBmp(m_hGameWnd,m_strPicPath+"����",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,1130,650);//���ܲ˵�
			Sleep(1000);
		}
		else break;
		Sleep(1000);
	}
	
	if (myApp.FindBmp(m_hGameWnd,m_strPicPath+"����",&pt))
	{
		//Sleep(1000);
		myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y);//����[950,650]
		Sleep(2000);
		CaptureJpgInRect("���＼��",150,140,600,620);

		myApp.PressMouseKey(m_hGameWnd,1060,330);//����
		Sleep(2000);
		CaptureJpgInRect("���ɼ���",150,100,340,650);


		myApp.PressMouseKey(m_hGameWnd,1060,450);//����
		Sleep(2000);
		CaptureJpgInRect("��������",150,130,370,620);

		myApp.PressMouseKey(m_hGameWnd,1020,80);
		Sleep(1000);
	}
	else WriteToFile("�Ҳ������＼��");
	
	if (!myApp.FindBmp(m_hGameWnd,m_strPicPath+"��԰",&pt))
	{
		myApp.PressMouseKey(m_hGameWnd,1130,650);//���ܲ˵�
		Sleep(1000);
	}
	if (myApp.FindBmp(m_hGameWnd,m_strPicPath+"��԰",&pt))
	{
		myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y);//��԰[870,650]
		Sleep(1000);
		CaptureJpgInRect("���ּ�԰",240,90,940,640);
		myApp.PressMouseKey(m_hGameWnd,920,120);//�رռ�԰
		Sleep(1000);
	}
	else WriteToFile("���û�м�԰");
}
/************************************************************************/
/* TODO:���ֻ������Ͱ�ȫ����ͼ                                                                     */
/************************************************************************/
void CGTRYLZT::CaptureSafe(HWND m_hGameWnd)
{
	BackToGamePage(m_hGameWnd,m_strPicPath);

	WriteToFile("��ʼ���ֻ������Ͱ�ȫ����ͼ");
	POINT pt;
	for (int i=0;i<4;i++)
	{
		Sleep(1000);
		if (!myApp.FindBmp(m_hGameWnd,m_strPicPath+"ϵͳ",&pt,230,580,1170,700))
		{
			myApp.PressMouseKey(m_hGameWnd,1130,650);//���ܲ˵�
			Sleep(1000);
		}
		if (myApp.FindBmp(m_hGameWnd,m_strPicPath+"ϵͳ",&pt,230,580,1170,700))
		{
			WriteToFile("���ڶ��ֻ������Ͱ�ȫ����ͼ");
			myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y);//ϵͳ����
			Sleep(1000);

			myApp.PressMouseKey(m_hGameWnd,830,580);//�ֻ�����
			Sleep(1000);
			CaptureJpgInRect("�����ֻ�",330,200,850,540);
			myApp.PressMouseKey(m_hGameWnd,830,220);
			Sleep(1000);

			myApp.PressMouseKey(m_hGameWnd,1000,380);//��ȫ��
			Sleep(1000);
			CaptureJpgInRect("��ȫ��",180,130,970,620);
			myApp.PressMouseKey(m_hGameWnd,950,140);//�ر�ϵͳ����
			Sleep(1000);
			break;
		}
		else WriteToFile("�Ҳ���ϵͳ�˵���ť");
	}
	
}
/************************************************************************/
/* TODO:���ۺͳ�ֵ����                                                                     */
/************************************************************************/
void CGTRYLZT::CaptureSell(HWND m_hGameWnd)
{
	BackToGamePage(m_hGameWnd,m_strPicPath);
	
	POINT pt;
	WriteToFile("׼���Գ��ۺͳ�ֵ������ͼ");
	for (int i=0;i<5;i++)
	{
		Sleep(1000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"�̳�",&pt))
		{
			WriteToFile("���ڽ��г��ۺͳ�ֵ������ͼ");
			myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y);//�̳�
			Sleep(1000);
			myApp.PressMouseKey(m_hGameWnd,430,140);//����
			Sleep(1000);
			CaptureJpgInRect("��Ҫ����",140,110,1010,660);
			myApp.PressMouseKey(m_hGameWnd,1060,570);//��ֵ
			Sleep(2000);
			//TODO:ӵ�е������ͼ
			CaptureJpgInRect("����",770,130,1020,210);//��������
			myApp.PressMouseKey(m_hGameWnd,430,130);//��ֵ����
			Sleep(1000);
			myApp.DragMouse(m_hGameWnd,300,220,300,600);
			Sleep(1000);
			CaptureJpgInRect("��ֵ����",140,110,1010,660);
			myApp.PressMouseKey(m_hGameWnd,1020,80);
			WriteToFile("��ɶԳ��ۺͳ�ֵ������ͼ");
			break;
		}
	}
	if(pt.x<0)
	{
		WriteToFile("�Ҳ����̳�");
	}
}

/************************************************************************/
/* ѡ����Ϸ����                                                         */
/************************************************************************/
int CGTRYLZT::SelectServer()
{
	DeleteFile("E:\\Cocos2dxPrefsFile.xml");
	system("adb kill-server");
	system("adb connect 127.0.0.1:26944");
	system("adb pull /data/data/com.netease.my/shared_prefs/Cocos2dxPrefsFile.xml E:\\");

	Sleep(2000);
	system("adb kill-server");

	CString stmp=myApp.ReadFile("E:\\Cocos2dxPrefsFile.xml");
	stmp=U2G(stmp);
	if (stmp)
	{
		CString NowServer=myApp.FindStr(stmp,"\<string name=\"LastLoginServerName\"","/string\>");
		CString NowArea=myApp.FindStr(stmp,"\<string name=\"LastLoginDistrictName\"","/string\>");
		CString NowHostnum=myApp.FindStr(stmp,"\<string name=\"LastLoginHostnum\"","/string\>");
		CString NowUserId=myApp.FindStr(stmp,"\<string name=\"LastLoginUserId\"","/string\>");
		if (NowServer.Find(m_strServer)>=0)
		{
			WriteToFile("������ȷ");
			return 1;
		}
		else
		{
			if (SecondEnter)
			{
				WriteToFile("������������");
				return 2260;
			}
			WriteToFile("Ĭ����������,�޸���������");
			//�޸������������Ҫ���µ�¼�������ļ�������Ч
			//CloseGame();//�ر���Ϸ
			
			//CloseWind();
			//SecondEnter=TRUE;
			CString m_Hostnum,m_UserId,NowLoginInfo,m_LoginInfo;
			int l=stmp.Find(":"+m_strServer);
			if (stmp.Find(":"+m_strServer)<0)
			{
				WriteToFile("�˺Ų����ڸ�������ɫ");
				return 3010;
			}
			m_UserId=myApp.FindStr(stmp,":"+m_strServer+":",":");
			m_Hostnum=stmp.Mid(stmp.Find(":"+m_strServer)-4,4);
			NowLoginInfo=myApp.FindStr(stmp,"\<string name=\"XyqPocket_LoginInfo_"+m_Hostnum+"_"+m_UserId+"\"","/string\>");
			m_LoginInfo=m_strArea+":"+m_Hostnum+":"+m_strServer+":"+m_UserId+":9999999999:1::0";
			WriteToFile("��½����������Ϣ��"+m_LoginInfo);
			stmp.Replace(NowServer,"\>"+m_strServer+"\<");
			stmp.Replace(NowArea,"\>"+m_strArea+"\<");
			stmp.Replace(NowHostnum,"\>"+m_Hostnum+"\<");
			stmp.Replace(NowUserId,"\>"+m_UserId+"\<");
			stmp.Replace(NowLoginInfo,"\>"+m_LoginInfo+"\<");

			stmp=CString_To_UTF8(stmp);
			FILE*  fp=0;
			fopen_s(&fp,"E:\\Cocos2dxPrefsFile.xml","wb");
			int len = stmp.GetLength();
			fwrite(stmp.GetBuffer(),1,len,fp);
			stmp.ReleaseBuffer();
			fclose(fp);
			Sleep(2000);
			system("adb kill-server");
			system("adb connect 127.0.0.1:26944");
			system("adb push E:\\Cocos2dxPrefsFile.xml /data/data/com.netease.my/shared_prefs");

			Sleep(2000);
			system("adb kill-server");
			return 2;
		}
	}
	else
	{	
		WriteToFile("��ȡ������Ϸ�ļ�����");
		return 2260;
	}
}

