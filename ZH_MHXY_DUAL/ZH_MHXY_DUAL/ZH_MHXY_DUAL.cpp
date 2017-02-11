// RC-GTR-TLBBOL.cpp : 定义控制台应用程序的入口点。
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

//选择服务器，是否第二次进入游戏
BOOL SecondEnter=FALSE;


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
#ifdef _TEST
	//
#else

	//return theApp.Trading();

	//程序接收参数：OrderNo + 1 + UDPPort
	if ( argc != 4 )
	{
		TCHAR szBuf[128] = {0};
		_stprintf( szBuf, _T("使用方法: ZH_WZRY_DUAL.exe <订单号> 1 <UDP端口号>\n") );
		//_tprintf(szBuf);
		MessageBox(NULL, szBuf, "提示", MB_ICONINFORMATION);
		return 0;
	}
	theApp.the_strOrderNo = argv[1];
	the_nRC2Port = atoi(argv[3]);
#endif//_TEST
	
	system("adb kill-server");   //放程序启动最前面，不然会脚本崩溃

	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
		WSADATA wsaData;
		if(!AfxSocketInit(&wsaData))
		{
			_tprintf(_T("无法启动WinSock通讯库\n"));
			MessageBox(NULL, _T("无法启动WinSock通讯库"), "提示", MB_ICONINFORMATION);
			return -1;
		}

		
		//初始化UDPSocket
		if (!theUDPSocket.Create( m_UDPPORT, SOCK_DGRAM ) )//if (!theUDPSocket.InitSocket())
		{
			MessageBox(NULL, _T("初始化UDPSocket失败"), "提示", MB_ICONINFORMATION);
			return -1;
		}
	}

	HANDLE the_hMutex = CreateMutex(NULL, FALSE, "rc2_lol_event");
	if ( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		CloseHandle(the_hMutex);
		the_hMutex = NULL;
		MessageBox(NULL, _T("程序已经运行"), "提示", MB_ICONINFORMATION);
		return -1;
	}

	if ( argc == 4 )
	{
		theApp.the_strOrderNo=argv[1];
		the_nRC2Port = atoi(argv[3]);
	}
	else
	{
		theApp.the_strOrderNo=_T("测试订单");

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

	if(Status!=1000 && theApp.the_strOrderNo!="测试订单")
		myApp.ClearFolder(theApp.m_strCapturePath);

	theApp.IsClosePc(Status);
	theApp.YiJiao(Status);

	system("adb kill-server");
	theApp.WriteToFile("断开UDPSocket连接");
	theUDPSocket.Close();
	return 1;
}

BOOL CGTRYLZT::GameStart()
{

	//InitKeyMouseDriver();
#ifdef _TEST
	//读取订单数据
	CString strOrderFile = _T(m_strProgPath+"\\info.txt");
	if( !PathFileExists(strOrderFile) )//检查路径文件
	{
		WriteToFile( "文件[%s]不存在", strOrderFile );
		return FALSE;
	}
	//WriteToFile( _T("读取[%s]\n"), strOrderFile );
	m_strOrderData = myApp.ReadFile(strOrderFile);

#else
	//请求订单数据
	if ( !RequestOrderData() )
	{
		WriteToFile( _T("请求订单数据失败\n") );
		return FALSE;
	}

#endif
	//读取订单明细
	if ( !ReadOrderDetail(m_strOrderData) )
	{
		WriteToFile( _T("读取订单明细失败") );
		return FALSE;
	}




	return true;
}
//主函数
BOOL CGTRYLZT::GameMain()
{

	CRect rect;
	GetWindowRect(GetDesktopWindow(),&rect);
	if(rect.Width()!=1280 || rect.Height()!=1024)
	{
		WriteToFile("电脑分辨率不是1280*1024\r\n");
		return 2260;
	}
	if(the_strOrderNo.Find("-")>0  || the_strOrderNo.Find("MZH")==0|| the_strOrderNo=="测试订单")
		m_strOrderType="发布单";
	else
		m_strOrderType="交易单";
	WriteToFile ("订单类型:%s\r\n",m_strOrderType);
	int Status=0;
	myApp.DeleteLog(m_strPictureDir,3);
	/*if(!CheckAccount())
	{
	WriteToFile("账号不是纯数字");
	return 3000;
	}*/



#ifdef _TEST
	/*m_hGameWnd=myApp.GetHwndByPid(NULL,"海马玩");
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
	


	for (int i=0;i<3;i++)
	{
		Status=CheckAccount();
		if (Status>=3000)
			return Status;
		else if (Status==2120)
			continue;
		else break;
	}
	
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
	myApp.KillWindow("浏览器.exe");*/
	
	

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
//	int index=m_strArea.Find("区");
//	if( m_strArea.Find("区")<0)
//	{
//		WriteToFile ("区服错误");
//		return 2260;
//	}
//	CString strServer=m_strArea.Left(index);
//	CString strText="手Q,";
//	CString strText1="手Q_1,";
//	for(int i=0;i<strServer.GetLength();i++)
//	{
//		strText+=strServer.Mid(i,1)+",";
//		strText1+=strServer.Mid(i,1)+"_1,";
//	}
//	strText+="区";
//	strText1+="区_1";
//	PinTu(strText,"区服",m_strPicPath+"区服");
//	PinTu(strText1,"区服1",m_strPicPath+"区服");
//	POINT  pt;
//	myApp.PressMouseKey (m_hGameWnd,710,465);
//	for(int i=0;i<10;i++)
//	{
//		Sleep(1000);
//		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"区服页面",&pt)
//			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"区服页面2",&pt))
//			break;
//	}
//	if(pt.x<0)
//	{
//		WriteToFile ("等待区服页面超时");
//		return 2260;
//	}
//	CaptureJpg("区服页面");
//	for(int i=0;i<10;i++)
//	{
//		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"我的服务器",&pt)
//			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"我的服务器2",&pt)
//			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"我的服务器3",&pt))
//			break;
//		DragMouse(m_hGameWnd,60,200,60,800);
//	}
//	if(pt.x<0)
//	{
//		WriteToFile ("找不到[我的服务器]");
//		return 2260;
//	}
//	myApp.PressMouseKey (m_hGameWnd,pt.x+50,pt.y+10);
//	Sleep(1000);
//	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"区服\\区服",&pt)
//		&& !myApp.FindBmp(m_hGameWnd,m_strPicPath+"区服\\区服1",&pt))
//	{
//		WriteToFile ("找不到服务器");
//		return 2260;
//	}
//	myApp.PressMouseKey (m_hGameWnd,pt.x+100,pt.y+10);
//	Sleep(1000);
//	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"开始",&pt)
//		&& !myApp.FindBmp(m_hGameWnd,m_strPicPath+"开始2",&pt))
//	{
//		WriteToFile ("找不到[开始]");
//		return 2260;
//	}
//	CaptureJpg("区服");
//	myApp.PressMouseKey (m_hGameWnd,pt.x+10,pt.y+10);
//	Sleep(5000);
//	for(int i=0;i<20;i++)
//	{
//		Sleep(3000);
//		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"英雄",&pt))
//			break;
//		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"英雄2",&pt))
//			break;
//		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"叉叉",&pt)
//			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"叉叉2",&pt)
//			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"叉叉3",&pt)
//			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"叉叉4",&pt))
//		{
//			myApp.PressMouseKey (m_hGameWnd,pt.x+20,pt.y+20);
//			//myApp.PressMouseKey (m_hGameWnd,pt.x+20,pt.y+20);
//			Sleep(500);
//			myApp.PressMouseKey (m_hGameWnd,20,20);
//		}
//		else
//			myApp.PressMouseKey (m_hGameWnd,880,115);
//		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"赛事系统",&pt))
//			myApp.PressMouseKey (m_hGameWnd,50,70);
//
//	}
//	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"英雄",&pt)
//		&& !myApp.FindBmp(m_hGameWnd,m_strPicPath+"英雄2",&pt))
//	{
//		WriteToFile ("找不到[英雄]");
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
	myApp.KillWindow("浏览器.exe");
	WinExec(m_strProgPath+"\\浏览器.exe http://reg.163.com/", SW_SHOW);
	for(int i=0;i<10;i++)
	{
		Sleep(2000);
		m_hGameWnd=FindWindow(NULL,"浏览器");
		if(IsWindow(m_hGameWnd))
			break;
	}
	if(!IsWindow(m_hGameWnd))
	{
		WriteToFile ("打开浏览器失败");
		return 2120;
	}
//#ifdef _DEBUG
	/*m_hGameWnd=myApp.GetHwndByPid("浏览器.exe");
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
		if(/*myApp.FindBmp(m_hGameWnd,m_strPicPath+"网页登录",&pt)
		   || */myApp.FindBmp(m_hGameWnd,m_strPicPath+"普通登录",&pt,0,0,0,0,TRUE,24,FALSE))
		   break;
		myApp.SendFuncKey(VK_F5);
	}
	if(pt.x<0)
	{
		WriteToFile ("找不到普通登录");
		CaptureJpg("找不到普通登录");
		return 2120;
	}

	myApp.PressMouseKey (m_hGameWnd,pt.x+120,pt.y+90);//账号textbox
	Sleep (300);
	myApp.SendFuncKey(VK_HOME);
	Sleep(300);
	myApp.SendFuncKey(VK_DELETE,30);
	Sleep(300);
	myApp.SendKeys(m_strAccount,200);

	Sleep (300);
	myApp.PressMouseKey (m_hGameWnd,pt.x+120,pt.y+160);//密码textbox
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
		//TODO:点击验证码滑块，拖动滑块
		if (myApp.FindBmp(m_hGameWnd,m_strPicPath+"滑动验证",&pt))
		{
			myApp.MoveMouse(m_hGameWnd,pt.x+10,pt.y+5);
			myApp.CapturePictureInRect(m_hGameWnd,"E:\\1.bmp",pt.x-20,pt.y-130,pt.x+310,pt.y-30);
			myApp.BMPToJPG("E:\\1.bmp");
			//验证码类型(不能为空)
			//1. 文字验证码.
			//2. 密保验证码.
			//3. 坐标验证码.
			CString strResult=RequestSafeCardInfo(3,"E:\\1.jpg","",90);
			if (strResult.GetLength() < 1)
			{
				WriteToFile ("答题员放弃验证码答题");
				return 2120;
			}

			int tx=atoi(strResult);
			WriteToFile("%s%d","验证码距离是：",tx);
			Sleep(1000);
			myApp.DragMouse(m_hGameWnd,pt.x+10+tx+2-40,pt.y+5,pt.x+10,pt.y+5,1000);//图片偏移量
			break;
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"字符验证",&pt))
		{
			myApp.CapturePictureInRect(m_hGameWnd,"E:\\1.bmp",890,330,1010,380);
			myApp.BMPToJPG("E:\\1.bmp");
			CString strResult=RequestSafeCardInfo(1,"E:\\1.jpg","",90);
			if(strResult.GetLength()<4)
			{
				WriteToFile("答题员放弃验证码答题");
				return 2120;
			}
			WriteToFile("%s%s","验证码是：",strResult);
			Sleep(1000);
			myApp.PressMouseKey(m_hGameWnd,pt.x+10,pt.y+10);
			Sleep(1000);
			myApp.SendString(strResult);
			break;
		}
	}
	if(pt.x<0)
	{
		WriteToFile("没有找到滑动验证按钮或字符验证，刷新页面");
		yzmTimes++;
		if(yzmTimes>3)
		{
			WriteToFile ("验证码未显示超过3次");
			return 2120;
		}
		myApp.SendFuncKey(VK_F5);
		goto PWD_ERROR;
	}
	

	WriteToFile ("输入完成 账号[%s]密码[%d]位",m_strAccount,m_strPassword.GetLength());
	Sleep (2000);
	CaptureJpg("账号密码");

	/*if(!(myApp.FindBmp(m_hGameWnd,m_strPicPath+"网页登录2",&pt)
	|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"网页登录",&pt)))
	{
	WriteToFile("找不到网页登录按钮");
	return 2120;
	}*/
	pt.x=pt.y=-1;
	for (int i=0;i<5;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"网页登录2",&pt,650,370,1000,510))
		{
			break;
		}
		Sleep(1000);
	}
	if(pt.x<0) 
	{
		WriteToFile("找不到网页登录按钮");
		//return 2120;
	}
	else myApp.PressMouseKey (m_hGameWnd,pt.x+10,pt.y+10);
	Sleep (2000);
	//CaptureJpg("账号密码输完");
	static int inputTimes=0;
	for(int i=0;i<3;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"账号管理",&pt,0,0,0,0,TRUE,24,FALSE)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"登录",&pt,0,0,0,0,TRUE,24,FALSE))
			break;

		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"账号密码错",&pt))
		{
			WriteToFile ("账号密码错");
			//CaptureJpg("账号密码错");
			if(errorTimes>0)
				return 3000;
			errorTimes++;
			goto PWD_ERROR;
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"验证码错",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"验证码错2",&pt))
		{
			WriteToFile ("验证码不正确");
			CaptureJpg("验证码错");
			yzmTimes++;
			if(yzmTimes>3)
			{
				WriteToFile ("验证码错超过3次");
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
		if (myApp.FindBmp(m_hGameWnd,m_strPicPath+"登录",&pt,0,0,0,0,TRUE,24,FALSE))
		{
			myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y);//手机pt.x-120,pt.y+10

			myApp.ActiveWindow(m_hGameWnd);
			Sleep(2000);
			break;
		}
		Sleep(2000);
	}
	

	for (int i=0;i<5;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"以后",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,pt.x+5,pt.y+5);
			Sleep(2000);
			break;
		}
		Sleep(2000);
	}

	for (int i=0;i<5;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"账号管理",&pt,0,0,0,0,TRUE,24,FALSE)
			/*&& !myApp.FindBmp(m_hGameWnd,m_strPicPath+"安全中心2",&pt)*/)
		{
			WriteToFile("登录成功");
			break;
		}
		else WriteToFile("找不到账号管理");
		Sleep(2000);
	}
	if(pt.x<0)
	{
		CaptureJpg("登录失败");
		WriteToFile("登录失败");
		return 3700;
	}

	myApp.PressMouseKey (m_hGameWnd,pt.x+10,pt.y+10);
	Sleep (2000);
	for (int i=0;i<5;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"账号安全",&pt,0,0,0,0,TRUE,24,FALSE)
			/*&& !myApp.FindBmp(m_hGameWnd,m_strPicPath+"账号安全2",&pt)*/)
		{
			WriteToFile("找到账号安全");
			break;
		}
		else WriteToFile("找不到账号安全");
		Sleep(2000);
	}
	if(pt.x<0)
	{
		CaptureJpg("账号安全");
		WriteToFile("查找账号安全失败");
		return 2120;
	}

	myApp.PressMouseKey(m_hGameWnd,pt.x+5,pt.y+15);
	Sleep(5000);
	CaptureJpgInRect("账号安全", pt.x-10,pt.y-170,pt.x+890,pt.y+290);

	myApp.KillWindow("浏览器.exe");
	return 1;
}

//判断是否被顶号 
BOOL CGTRYLZT::NetBreak()
{
	WriteToFile("订单失败");
	CaptureJpg("订单失败",GetDesktopWindow());
	POINT pt;

	if(!IsWindow(m_hGameWnd))
	{
		WriteToFile ("游戏窗口消失");
		return FALSE;
	}

	//myApp.CaptureGamePicture(".\\订单失败.bmp",GetDesktopWindow());
	if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"顶号",&pt))
	{
		WriteToFile ("被顶号");
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
		WriteToFile("截图成功,共%d张\r\n",num);
	/*if(Status==1000 && num>18)
	{
	WriteToFile("不符合规范18张\r\n");
	Status=2300;
	}*/

	Sleep(100);
	log.Format ("移交状态=%d\r\n",Status);
	WriteToFile(log);
	Sleep(100);

	if(m_strOrderType=="发布单" && (!m_strPlanTime.IsEmpty()) && the_strOrderNo.Find("MZH")<0)
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
			WriteToFile("等待游戏窗口");
		Sleep(2000);
		m_hGameWnd=FindWindow("CrossFire","穿越火线");
		if(IsWindowVisible(m_hGameWnd))
			break;


	}
	if(!IsWindowVisible(m_hGameWnd))
	{
		WriteToFile("等待游戏窗口超时");
		return 2260;
	}

	for(int i=0;i<20;i++)
	{
		Sleep(2000);
		if(i%10==0)
			WriteToFile("等待公告页面");
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"公告",&pt))
		{
			Sleep(1000);
			CaptureJpg("公告页面");			
			myApp.PressMouseKey(m_hGameWnd,500,340);
			Sleep(1000);
			myApp.PressMouseKey(m_hGameWnd,950,705);
			Sleep(2000);
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"角色属性",&pt))
			break;
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"确定",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"确定2",&pt))
		{

			if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"封号",&pt1,pt.x-220,pt.y-150,pt.x+250,pt.x-40,TRUE,24,FALSE))
			{
				CaptureJpg("封号");
				WriteToFile("账号被封");
				myApp.PressMouseKey(m_hGameWnd,pt.x+5,pt.y+5);
				return 3300;
			}
			CaptureJpg("确定");
			myApp.PressMouseKey(m_hGameWnd,pt.x+5,pt.y+5);
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"取消",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"取消2",&pt))
		{
			CaptureJpg("取消");
			myApp.PressMouseKey(m_hGameWnd,pt.x+5,pt.y+5);
		}
	}
	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"角色属性",&pt))
	{
		WriteToFile("找不到角色属性");
		return 2260;
	}

	return 1;

}






//等待启动游戏
int CGTRYLZT::WaitStartGame()
{


	POINT pt,ptTemp;
	CString strLog;

	//启动游戏
	if ( !PathFileExists(m_strGameStartFile))
	{
		WriteToFile( _T("游戏文件[%s]不存在"), m_strGameStartFile );
		return 2260;
	}

	WriteToFile("开游戏");
	m_hGameWnd=myApp.GetHwndByPid("Droid4X.exe");
	if(IsWindow(m_hGameWnd))
	{
		myApp.ActiveWindow(m_hGameWnd);
		WriteToFile("模拟器已经运行");
	}
	else
	{
		WriteToFile("打开模拟器");
		//TODO:打开游戏进程
		HINSTANCE n =::ShellExecute(NULL,"open", m_strGameStartFile,"",m_strGamePath ,SW_SHOWNORMAL );
		if( n < (HINSTANCE) 31)
		{
			WriteToFile("打开游戏失败\r\n");
			return 2120;//开启异常
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
		WriteToFile("等待模拟器窗口超时");
		return 2260;
	}

	
	Sleep(1000);
	for(int i=0;i<50;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"游戏",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"游戏2",&pt))
			break;
		Sleep(2000);
	}
	if(pt.x<0)
	{
		WriteToFile("找不到图片[游戏]");
		return 2260;
	}

	//TODO:登录游戏之前清除配置文件
	//删除指定文件
	WriteToFile("开启游戏之前清除区服配置文件");
	system("adb kill-server");
	system("adb connect 127.0.0.1:26944");
	system("adb shell \"su -c \' rm /data/data/com.netease.my/shared_prefs/Cocos2dxPrefsFile.xml\'\""); 
	
	Sleep(2000);
	system("adb kill-server");

	myApp.PressMouseKey(m_hGameWnd,pt.x+30,pt.y-50,1,50);
	WriteToFile("等待游戏窗口");

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
		WriteToFile("等待游戏窗口超时");
		return 2260;
	}

	pt.x=pt.y=-1;//初始化
	for(int i=0;i<60;i++)
	{
		Sleep(5000);
		if(i%20==0)
			WriteToFile("等待登录页面");
		//用户协议弹窗
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"接受用户协议",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y);
			Sleep(2000);
		}
		if (myApp.FindBmp(m_hGameWnd,m_strPicPath+"取消",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y);
			Sleep(2000);
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"确定",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y);
			Sleep(2000);
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"其他账号",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"其他账号2",&pt))
		{
			goto CheckInOther;
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"用户中心",&pt))
			break;
	}
	if(pt.x<0)
	{
		WriteToFile("找不到图片[用户中心]或[其他账号登录]");
		return 2260;
	}
	else 
	{
		Sleep(3000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"用户中心",&pt))
		{
			WriteToFile("找到用户中心");
			myApp.PressMouseKey(m_hGameWnd,pt.x+10,pt.y+10);
		}
		
		//Sleep(2000);
		for (int i=0;i<10;i++)
		{
			if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"切换账号",&ptTemp)
				|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"切换账号2",&ptTemp))
			{
				myApp.PressMouseKey(m_hGameWnd,ptTemp.x+20,ptTemp.y+20);
				WriteToFile("切换账号");
				break;
			}
			Sleep(1000);
		}
		if(ptTemp.x<0)
		{
			WriteToFile("找不到切换账号");
			return 1;
		}
		for (int i=0;i<10;i++)
		{
			if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"其他账号",&pt)
				|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"其他账号2",&pt))
			{
CheckInOther:	myApp.PressMouseKey(m_hGameWnd,pt.x+10,pt.y+10);//点击使用其他账号登录
				WriteToFile("其他账号登录");
				break;
			}
			Sleep(1000);
		}
		if(pt.x<0)
		{
			WriteToFile("找不到其他账号");
			return 1;
		}
	}
	
	/*SetWindowPos(m_hGameWnd,NULL,0,0,0,0,SWP_NOSIZE);
	Sleep(1000);*/
	return 1;


}

//操作输入账号密码
BOOL CGTRYLZT::EnterAccPwd()
{

	//SendGoodsState("输入帐号密码",60);
	//CString strOS=CMyClass::GetOSVersion();


	POINT pt;
	
	int nCheckTimes=0;

	for (int i=0;i<10;i++)
	{
		if(!(myApp.FindBmp(m_hGameWnd,m_strPicPath+"网易账号",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"网易账号2",&pt) ))
		{
			WriteToFile("不是账号页面");
			return 2260;
		}
		else break;
		Sleep(1000);
	}
	Sleep(1000);
	
	//检查输入法,大写锁
	if (!CheckIM())
		return 2120;
	
	myApp.PressMouseKey (m_hGameWnd,pt.x,pt.y);//点击网易邮箱账号登录
	Sleep (1000);
	pt.x=pt.y=-1;
	for (int i=0;i<5;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"邮箱登录",&pt,415,370,920,440)
			/*|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"邮箱登录2",&pt,415,370,920,440)*/)
			break;
		Sleep(1000);
	}
	if(pt.x<0) 
	{
		WriteToFile("找不到邮箱登录按钮");
		return 2260;
	}

	myApp.PressMouseKey(m_hGameWnd,pt.x+180,pt.y-150);
	myApp.SendFuncKey(VK_BACK,20);

	WriteToFile("开始输入账号密码\r\n");



	myApp.SendKeys(m_strAccount,200);
	myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y-70);//点击账号框收起账号提示文本
	Sleep(1000);
	myApp.PressMouseKey (m_hGameWnd,pt.x+180,pt.y-70);//聚焦密码框

PASSWORD_ERROR:
	static int runTimes=0;
	if(runTimes>8)
	{
		WriteToFile("输账号超过8次\r\n");
		return 3700;
	}
	runTimes++;

	Sleep (300);
	//myApp.SendFuncKey(VK_HOME);
	myApp.SendFuncKey(VK_BACK,20);//如果记住了密码，则清除
	myApp.SendKeys(m_strPassword,200);
	//RC2Tool::DoubleClick (m_hGameWnd,pt_Pwd.x,pt_Pwd.y);
	CString strLog;



	WriteToFile ("输入完成 账号[%s]密码[%d]位",m_strAccount,m_strPassword.GetLength());
	Sleep (1000);
	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"邮箱登录2",&pt,0,0,0,0,TRUE,60))
	{
		WriteToFile("不是账号页面");
		return 2260;
	}

	myApp.PressMouseKey (m_hGameWnd,pt.x+5,pt.y+5);//点击登录
	
	for(int i=0;i<5;i++)
	{
		Sleep(2000);
		if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"邮箱登录2",&pt,0,0,0,0,TRUE,60))
			break;

	}
	if(pt.x>0)
	{
		WriteToFile ("登录页面没有消失");
		goto PASSWORD_ERROR;
		//return 2260;
	}
	
	
	/************************************************************************/
	/* 登录游戏部分  
		先删除游戏的区服配置文件，然后默认登录一个服务器，
		配置文件被游戏软件修改，此时拷贝出配置文件从中找到要登录的
		区服，然后修改lastLogin信息，使下一次登录时的默认区服是
		要登录的区服。
	*/
	/************************************************************************/

	SecondEnter=FALSE;
	//进行第一次登录，登录完成后退出
	
UserLogin:
	
	if(SecondEnter==TRUE)
	{
		//TODO:重新点击游戏图标打开游戏
		WriteToFile("修改区服配置文件，重新打开游戏");
		for(int i=0;i<50;i++)
		{
			if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"游戏",&pt)
				|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"游戏2",&pt))
				break;
			Sleep(1000);
		}
		if(pt.x<0)
		{
			WriteToFile("找不到图片[游戏]");
			return 2260;
		}
		myApp.PressMouseKey(m_hGameWnd,pt.x+30,pt.y-50,1,50);
		Sleep(1000);
		WriteToFile("等待游戏窗口");
	}

	if (SecondEnter==TRUE)
	{
		int ret=SelectServer();
		if (ret!=1)
		{
			WriteToFile("选择区服出现错误,错误代码="+ret);
			return ret;
		}
	}


	//本地账号已退出，请重新登录的提示框
	//等待连接服务器
	/*POINT temp;
	for (int i=0;i<5;i++)
	{
		Sleep(1000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"确定",&temp))
		{
			myApp.PressMouseKey(m_hGameWnd,temp.x,temp.y);
			Sleep(1000);
			WriteToFile("关闭重新登录的提示框");
			break;
		}
	}*/


	CheckGameDialog(m_hGameWnd,m_strPicPath);//关闭可能出现的弹窗

	//判断是否已经自动选择默认服务器
	for (int i=0;i<10;i++)
	{
		Sleep(1000);
		POINT ptemp;
		if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"未选择服务器",&ptemp,405,400,765,450))
		{
			WriteToFile("已经连接服务器");
			//通过修改游戏的配置文件设置默认区服
			myApp.PressMouseKey(m_hGameWnd,590,520);//直接点击登录游戏 pt.x-70,pt.y+110
			WriteToFile("点击登陆游戏");
			break;
		}
		else WriteToFile("还没有连接上服务器");
	}
	
/*	POINT ptemp;
	for (int i=0;i<10;i++)
	{
		Sleep(2000);
		

		for(int j=0;j<3;j++)
		{
			if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"用户中心",&ptemp))
			{
				WriteToFile("正在登录游戏");
				//break;
			}
			Sleep(1000);
		}
		
	}*/
	

	/*myApp.PressMouseKey(m_hGameWnd,pt.x+20,pt.y+20);//点击选服按钮
	Sleep(1000);
	//CaptureJpg("区服");

	//TODO:选区1~8
	myApp.PressMouseKey(m_hGameWnd,240,140);//已有角色


	TODO:选服

	myApp.PressMouseKey(m_hGameWnd,500,140);//已有角色的第一个服务器
	*/

	//TODO:取消活动公告等窗口,点击esc取消活动窗口，直到出现退出游戏的提示窗口

	POINT p;
	p.x=p.y=-1;
	Sleep(2000);
	//if(SecondEnter==TRUE) //二次登录
	//{
	for (int i=0;i<60;i++)
	{
		myApp.SendFuncKey(VK_ESCAPE);
		//WriteToFile("发送esc键");
		Sleep(1000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"退出游戏",&p,350,250,820,470))
		{
			myApp.SendFuncKey(VK_ESCAPE);
			break;
		}
		//Sleep(1000);
	}
	if (p.x<0)
	{
		WriteToFile("登录游戏超时");
		return 4120;
	}
	//}
	if(SecondEnter==FALSE)//一次登录
	{
		CloseGame();//关闭游戏后配置文件会被修改
		int ret = SelectServer();
		if(ret==3010) return ret;//该区没有角色
		SecondEnter=TRUE;
		goto UserLogin;
		//if(ret==1) return 1;//第一次登录区服正确，直接进入游戏
		//else
		//{
			
			//SelectServer();
			
		//}
	}
	
	return 1;

}



//核对服务器
BOOL CGTRYLZT::CheckServer()
{
	char chTitle[MAX_PATH];
	CString strTitle;
	CString strLog;
	::GetWindowText(m_hGameWnd,chTitle,MAX_PATH);
	strTitle.Format("%s",chTitle);
	if (strTitle.Find(m_strServer)<0)
	{
		strLog.Format("核对区服失败,窗口标题为:[%s]\r\n",strTitle);
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
		//WriteToFile("范围%d,%d,%d,%d\r\n",leftNow,top,right,bottom);
		if(myApp.FindBMPText(m_hGameWnd,"宋体",strText.Mid(i,2),14,RGB(237,190,0),&pt,leftNow,top,right,bottom,FALSE))
		{
			//WriteToFile("匹配到[%s]\r\n",strText.Mid(i,2));
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



//交易
int CGTRYLZT::Trading()
{
	//CString path=m_strCapturePath+TEXT("拼图\\");//拼图路径
	//WriteToFile("拼图路径："+m_strCapturePath+_T("拼图\\"));
	bgR=223,bgG=206,bgB=163;

	pBigPic = new BYTE[(Lwidth+3)/4*4 * Lheight * 4 ];

	WriteToFile ("开始拼图MHXY");
	PinTu("审核信息","MHXY10");
	PinTu("总览","MHXY2");
	PinTu("包裹","MHXY4");
	PinTu("包裹物品1,包裹物品2","MHXY5");
	PinTu("包裹物品3,包裹物品4","MHXY5");
	
	CString ppath,ppath2,ppath3,path4;
	for (int i=1;i<=16;i+=3)
	{
		ppath=ppath2=ppath3=path4=_T("包裹装备");
		ppath.Format("%s%d",ppath,i);
		ppath2.Format("%s%d",ppath2,i+1);
		ppath3.Format("%s%d",ppath3,i+2);

		CString str=ppath+_T(",")+ppath2+_T(",")+ppath3;
		if (FileExist(str)!="")
		{
			if (i>12)//包裹装备上传限制4张，超出部分作为MHXY3属性
			{
				PinTu(str,_T("MHXY3"));
			}
			else PinTu(str,_T("MHXY1"));
		}
		else 
		{
			WriteToFile("找不到截图:%s",str);
			break;
		}
	}
	for (int i=1;i<=12;i+=2)
	{
		ppath=ppath2=_T("仓库");
		ppath.Format("%s%d",ppath,i);
		ppath2.Format("%s%d",ppath2,i+1);
		CString str=ppath+_T(",")+ppath2;
		if(FileExist(str)!="")
			PinTu(str,_T("MHXY5"));
		else
		{
			WriteToFile("找不到%s",str);
			break;
		}
	}
	for (int i=1;i<=8;i++)
	{
		CString s=TEXT("宠物"),picName;
		s.Format("%s%d",s,i);
		picName=s;
		s.Format("%s,%s,%s",s+_T("基本属性"),s+_T("资质技能"),s+_T("宠物内丹"));
		if (FileExist(s)!="")
		{
			PinTu(s,"MHXY4");
		}
		else
		{
			WriteToFile("找不到%s",s);
			break;
		}
	}
	PinTu("人物技能,帮派技能,修炼技能","MHXY8");

	PinTu("关联手机","MHXY9");
	PinTu("安全锁","MHXY9");
	PinTu("欢乐家园","MHXY7");
	PinTu("充值返利","MHXY7");
	//TODO:财产宝海包括，银币和仙玉数量，我要出售
	PinTu("游戏币,仙玉","MHXY7");
	PinTu("我要出售","MHXY7");
	
	return 1000;
}
int CGTRYLZT::Hero()
{
	POINT pt;

	myApp.PressMouseKey(m_hGameWnd,95,600);
	for(int i=0;i<5;i++)
	{
		Sleep(2000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"已拥有",&pt))
			break;
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"已拥有2",&pt))
			break;
		myApp.PressMouseKey(m_hGameWnd,850,125);
	}
	if(pt.x<0)
	{
		WriteToFile ("找不到[已拥有]");
		return 2260;
	}
	CaptureJpg("英雄");
	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"勾勾",&pt)
		&& !myApp.FindBmp(m_hGameWnd,m_strPicPath+"勾勾2",&pt))
	{
		myApp.PressMouseKey(m_hGameWnd,715,65);
		Sleep(1000);
		CaptureJpg("已拥有");
	}
	for(int i=0;i<10;i++)
	{
		CaptureJpgInRect("英雄",140,80,990,630);

		myApp.CapturePictureInRect(m_hGameWnd,m_strProgPath+"\\英雄.bmp",170,410,280,450);
		DragMouse(m_hGameWnd,570,560,570,20);
		Sleep(1000);
		if(myApp.FindBmp(m_hGameWnd,m_strProgPath+"\\英雄.bmp",&pt,0,300))
		{
			WriteToFile ("已无英雄...");
			CaptureJpg("已无英雄");
			return 1;
		}
	}
}
//交易
int CGTRYLZT::Trade()
{
	m_hGameWnd=myApp.GetHwndByPid("Droid4X.exe");
	POINT pt,ptTemp,ptTemp2;
	SetWindowPos(m_hGameWnd,NULL,0,0,0,0,SWP_NOSIZE);
	Sleep(1000);
	//myApp.MoveMouse(m_hGameWnd,50,50);
	
	BackToGamePage(m_hGameWnd,m_strPicPath);
	WriteToFile ("开始截图");

	CaptureJpgInRect( "总览", 2,40,1170,690, TRUE, RGB( 255, 188,0 ),530,370,650,420);//进入游戏后的界面截图

	//Sleep(1000);
	static int DialogOccurTime=0;//弹窗导致的循环次数限制

ReCapturePackage:
	//TODO：对包裹截图
	CapturePackage(m_hGameWnd);
	//TODO:如果发现当前页面有弹窗出现，则上面的截图可能失败，需要重新截图
	if(CheckGameDialog(m_hGameWnd,m_strPicPath))
	{
		WriteToFile("进行包裹截图时出现弹窗");
		DialogOccurTime++;
		if(DialogOccurTime<3)
		{
			goto ReCapturePackage;
		}
		else WriteToFile("弹窗导致包裹截图失败次数超过%d次",DialogOccurTime);
	}

	DialogOccurTime=0;
ReCapturePlayerInfo:
	//TODO:对人物信息截图
	CapturePlayerInfo(m_hGameWnd);
	//TODO:如果发现当前页面有弹窗出现，则上面的截图可能失败，需要重新截图
	if(CheckGameDialog(m_hGameWnd,m_strPicPath))
	{
		WriteToFile("进行人物信息截图时出现弹窗");
		DialogOccurTime++;
		if(DialogOccurTime<3)
		{
			goto ReCapturePlayerInfo;
		}
		else WriteToFile("弹窗导致人物信息截图失败次数超过%d次",DialogOccurTime);
	}

	DialogOccurTime=0;
ReCapturePet:
	//TODO:宠物截图
	CapturePet(m_hGameWnd);
	//TODO:如果发现当前页面有弹窗出现，则上面的截图可能失败，需要重新截图
	if(CheckGameDialog(m_hGameWnd,m_strPicPath))
	{
		WriteToFile("进行宠物锁截图时出现弹窗");
		DialogOccurTime++;
		if(DialogOccurTime<3)
		{
			goto ReCapturePet;
		}
		else WriteToFile("弹窗导致宠物截图失败次数超过%d次",DialogOccurTime);
	}

	DialogOccurTime=0;
ReCaptureSell:
	////TODO:出售和充值返利
	CaptureSell(m_hGameWnd);
	//TODO:如果发现当前页面有弹窗出现，则上面的截图可能失败，需要重新截图
	if(CheckGameDialog(m_hGameWnd,m_strPicPath))
	{
		WriteToFile("进行出售和充值返利截图时出现弹窗");
		DialogOccurTime++;
		if(DialogOccurTime<3)
		{
			goto ReCaptureSell;
		}
		else WriteToFile("弹窗导致出售和充值返利截图失败次数超过%d次",DialogOccurTime);
	}

	myApp.PressMouseKey(m_hGameWnd,1130,650);//打开功能菜单
	Sleep(1000);

	DialogOccurTime=0;
ReCaptureSkill:
	////TODO：技能截图
	CaptureSkill(m_hGameWnd);
	//TODO:如果发现当前页面有弹窗出现，则上面的截图可能失败，需要重新截图
	if(CheckGameDialog(m_hGameWnd,m_strPicPath))
	{
		WriteToFile("进行人物技能截图时出现弹窗");
		DialogOccurTime++;
		if(DialogOccurTime<3)
		{
			goto ReCaptureSkill;
		}
		else WriteToFile("弹窗导致技能截图失败次数超过%d次",DialogOccurTime);
	}

	DialogOccurTime=0;
ReCaptureSafe:
	//TODO:手机关联和安全锁
	CaptureSafe(m_hGameWnd);
	//TODO:如果发现当前页面有弹窗出现，则上面的截图可能失败，需要重新截图
	if(CheckGameDialog(m_hGameWnd,m_strPicPath))
	{
		WriteToFile("进行手机关联和安全锁截图时出现弹窗");
		DialogOccurTime++;
		if(DialogOccurTime<3)
		{
			goto ReCaptureSafe;
		}
		else WriteToFile("弹窗导致手机关联和安全锁截图失败次数超过%d次",DialogOccurTime);
	}

	myApp.PressMouseKey(m_hGameWnd,1130,650);//关闭功能菜单
	Sleep(1000);

	int status=Trading();
	return status;



	/*for(int i=0;i<5;i++)
	{
	Sleep(2000);
	if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"套装下拉",&pt))
	break;
	}
	if(pt.x<0)
	{
	WriteToFile ("找不到[套装下拉]");
	return 2260;
	}
	CaptureJpgInRect( "套装",0,35,1024,610);
	myApp.PressMouseKey(m_hGameWnd,pt.x+5,pt.y+5);
	Sleep(1000);
	CaptureJpg("套装");
	CArray<CPoint,CPoint&> ptArray;
	int num=myApp.GetBmpNum(m_hGameWnd,m_strPicPath+"套装",ptArray,0,120,130,300);
	WriteToFile ("有%d个套装",num);
	for(int i=1;i<num;i++)
	{
	myApp.PressMouseKey(m_hGameWnd,ptArray.GetAt(i).x,ptArray.GetAt(i).y);
	Sleep(2000);
	CaptureJpgInRect( "套装",0,35,1024,610);
	myApp.PressMouseKey(m_hGameWnd,pt.x+5,pt.y+5);
	Sleep(1000);
	}
	myApp.PressMouseKey(m_hGameWnd,970,60);
	Sleep(3000);


	WriteToFile ("仓库截图");
	myApp.PressMouseKey(m_hGameWnd,770,550);
	Sleep(3000);
	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"武器",&pt))
	{
	WriteToFile ("找不到[武器]");
	return 2260;
	}
	CaptureJpgInRect( "装备",0,35,1024,610);
	myApp.PressMouseKey(m_hGameWnd,100,250);
	Sleep(1000);
	CaptureJpgInRect( "碎片",0,35,1024,610);
	myApp.PressMouseKey(m_hGameWnd,100,340);
	Sleep(1000);
	CaptureJpgInRect( "材料",0,35,1024,610);
	myApp.PressMouseKey(m_hGameWnd,100,420);
	Sleep(1000);
	CaptureJpgInRect( "礼物",0,35,1024,610);
	myApp.PressMouseKey(m_hGameWnd,100,500);
	Sleep(1000);
	CaptureJpgInRect( "消耗品",0,35,1024,610);
	myApp.PressMouseKey(m_hGameWnd,970,60);
	Sleep(3000);


	myApp.PressMouseKey(m_hGameWnd,20,205);
	Sleep(2000);
	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"咖啡馆未解锁",&pt))
	{
	myApp.PressMouseKey(m_hGameWnd,20,205);
	Sleep(3000);
	CaptureJpg("咖啡馆");
	CaptureJpgInRect("咖啡馆",0,35,1024,610);
	}

	else
	WriteToFile ("咖啡馆未解锁");

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
	Pic.Format("%s皮肤%d.bmp",m_strCapturePath,pic1);
	Pic2.Format("%s皮肤%d.bmp",m_strCapturePath,pic2);
	if(!PathFileExists(Pic2))
		return FALSE;
	BYTE r, g, b, now_r, now_g, now_b;
	//对比图片装入数组
	::HBITMAP PicH=  (HBITMAP) LoadImage(NULL,Pic,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	BITMAP bm,bm1; 
	::GetObject(PicH,sizeof(BITMAP),&bm); 

	HBITMAP PicH2=  (HBITMAP) LoadImage(NULL,Pic2,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	::GetObject(PicH2,sizeof(BITMAP),&bm1);
	if(bm.bmWidth!=bm1.bmWidth  || bm.bmHeight!=bm1.bmHeight)
	{
		cout<<"两张图片大小不一致"<<endl<<bm.bmWidth<<","<<bm.bmHeight<<endl<<bm1.bmWidth<<","<<bm1.bmHeight<<endl;
		return FALSE;
	}
	BYTE *tData;
	BYTE *pData;
	//取到的数组，如果宽为奇数，每行尾自动加1字节补足，需要取余忽略
	int aaa=(bm.bmWidth * (bm.bmBitsPixel/8) + 3)/4*4* bm.bmHeight;
	aaa=bm.bmWidth * bm.bmHeight * 4;
	tData=new BYTE[ (bm.bmWidth * (bm.bmBitsPixel/8) + 3)/4*4* bm.bmHeight];
	pData=new BYTE[ (bm1.bmWidth * (bm1.bmBitsPixel/8) + 3)/4*4* bm1.bmHeight];
	long Ret1=GetBitmapBits (PicH,(bm.bmWidth * (bm.bmBitsPixel/8) + 3)/4*4* bm.bmHeight,&tData[0]);
	Ret1=GetBitmapBits (PicH2,(bm1.bmWidth * (bm1.bmBitsPixel/8) + 3)/4*4* bm1.bmHeight,&pData[0]);
	BOOL fFalse;

	//循环比较
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
			//取到的数组，如果宽为奇数，每行尾自动加1字节补足，需要取余忽略
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
	//TODO:关闭mhxy游戏
	POINT pt;

	for (int i=0;i<20;i++)
	{
		myApp.SendFuncKey(VK_ESCAPE);
		Sleep(1000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"退出游戏",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,pt.x+120,pt.y+90);//确定按钮
			break;
		}
		else WriteToFile("等待退出游戏");
		//myApp.SendFuncKey(VK_ESCAPE);
		//Sleep(1000);
	}
	Sleep(1000);
	for (int i=0;i<10;i++)
	{
		if (myApp.FindBmp(m_hGameWnd,m_strPicPath+"游戏",&pt)
			||myApp.FindBmp(m_hGameWnd,m_strPicPath+"游戏2",&pt))
		{
			WriteToFile("关闭游戏成功");
			break;
		}
		else
		{
			WriteToFile("关闭游戏失败");
		}
		Sleep(1000);
	}


	/*POINT pt;
	if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"账号提示框",&pt))
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
	arrWhiteList.Add("账号GTR");
	arrWhiteList.Add("开始");
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
				WriteToFile ("关闭窗口[%s]",strTitle);
				CaptureJpg("关闭窗口",GetDesktopWindow());
				PostMessage(hWnd,WM_CLOSE,0,0);
				Sleep(1000);
			}

		}

		hWnd = GetWindow( hWnd, GW_HWNDNEXT );
	}
	hWnd=FindWindow(NULL,"账号GTR");
	if(IsWindowVisible(hWnd))
		ShowWindow(hWnd,SW_MINIMIZE);

	return;
}








//循环截图
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
			//找不到左上角,且没有配置需要全部执行完,退出本函数
			if(!myApp.FindBMPText(m_hGameWnd,"宋体",strText1,m_nFontSize1,RGB(239,22,48),&pt,left,top,right,bottom))
			{
				myApp.MoveMouse(m_hGameWnd,X+j*IntervalX+2,Y+i*IntervalY+2);
				Sleep(600);
				if(!myApp.FindBMPText(m_hGameWnd,"宋体",strText1,m_nFontSize1,RGB(239,22,48),&pt,left,top,right,bottom))
				{
					WriteToFile("[%s%d]不存在\r\n",strName,m_nPicNum);
					//WriteToFile("找不到文字[%s]\r\n",strText1);
					if (bAll)//需要全部执行完,继续循环
						continue;
					else
						goto THE_END;
				}

				/*pt.x+=189;
				pt.y-=49;*/
			}

			if(!myApp.FindBMPText(m_hGameWnd,"宋体",strText2,m_nFontSize2,RGB(171,165,151),&pt2,pt.x-Width,pt.y+10,pt.x,bottom))
			{
				height=rect.Height();
				WriteToFile("找不到文字[%s]\r\n",strText2);
				wide = Width;
			}
			else
			{
				wide = pt2.x+rectPY.right-pt.x+rectPY.left;
				height=pt2.y+rectPY.bottom-pt.y+rectPY.top;
			}

			//CString strFileName = RC2Tool::GetCaptureFileName( m_strPictureDir, m_strOPDN, m_strGameName, the_strOrderNo, m_strDeliverOpId );
			//CString strLog;
			//strLog.Format("截取小图,宽=%d,高=%d\r\n",wide,height);
			//WriteToFile(strLog);
			if(wide>Lwidth)
				wide=Lwidth;
			if(height>Lheight)
				height=Lheight;
			if(wide<0 || height<0 /*|| wide<(Width-10) || pt2.x+rectPY.right<pt.x || pt2.y+rectPY.bottom<pt.y*/)
			{
				WriteToFile("小图失败,宽=%d,高=%d,pt.x=%d,pt2.x=%d,pt2.y=%d\r\n",wide,height,pt.x,pt2.x,pt2.y);
				return false;
			}
			CString strPicName;
			strPicName.Format("%s%d.bmp",strName,m_nPicNum);
			if(pt2.x<0)
			{
				//WriteToFile("小图宽[%d]\r\n",pt.x-rectPY.left+wide);
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
//生成图片
int CGTRYLZT::SaveBigPic(HWND hWnd,CString strPicName,CString strPicID,int picWidth)
{



	if(strPicName=="生成图片")
	{

		goto NEXT_STEP;
	}
	if (strPicName=="换行")
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
		WriteToFile("图片[%s]不存在\r\n",strPicName);
		return FALSE;
	}
	//WriteToFile("小图[%s]\r\n",strPicName);
	//WriteToFile("将小图拼到大图坐标%d,%d\r\n",ptBigPic.x,ptBigPic.y);

	//小图装入数组
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


	//WriteToFile("保存[%s],宽=%d,高=%d\r\n",strPicName,Swidth,Sheight);



	if (bPicFull)
	{

		/*pBigPic = new BYTE[Lwidth * Lheight * 4 ];*/
		if (pBigPic==NULL)
		{
			WriteToFile("申请内存失败\r\n");
			return 2120;
		}

		//背景色
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
		//WriteToFile("宽度不足,换行\r\n");
		ptMAX.x=max(ptBigPic.x,ptMAX.x);
		ptBigPic.x = 0;
		ptBigPic.y+=ptMAX.y+1;
		ptMAX.y=0;
		//WriteToFile("ptBigPic.y=%d\r\n",ptBigPic.y);
	}

	if ((Lheight-ptBigPic.y)<Sheight)
	{
		WriteToFile("Lheight-ptBigPic.y=%d-%d",Lheight,ptBigPic.y);
		WriteToFile("高度不足\r\n");
		return 2120;
		//bPicFull = TRUE;
		////nRetCode=2;
		//goto NEXT_STEP;
	}

	BYTE *tData;
	tData = new BYTE[(Swidth*4+4-Swidth%4) * Sheight];
	if (tData==NULL)
	{
		WriteToFile("tData申请内存失败\r\n");
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
	::AfxMessageBox("全是0");*/
	//strLog.Format("将小图拼到大图坐标%d,%d\r\n",ptBigPic.x,ptBigPic.y);
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
	//	WriteToFile("上面一行是黑色\r\n");
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
	//		WriteToFile("左边是黑色\r\n");
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

	//没满不生成图片
	if (!bPicFull)
		return 1;

NEXT_STEP:
	if(ptMAX.x==0 && ptMAX.y==0)
	{
		//WriteToFile("大图为空\r\n");
		return 2120;
	}







	////////////////缩放///////////////////////////////
	if(ptBigPic.y>0)//多排
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
	//修改背景颜色
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

	if(the_strOrderNo.Find("MZH")==0 /*|| the_strOrderNo=="测试订单"*/)
	{
		/////////加水印/////////////////////////////////////////////////////////
		if(!PathFileExists(m_strPicPath+"\\水印.bmp"))
		{
			WriteToFile("水印图片不存在\r\n");
			goto WATERMARK_FAIL;
		}
		HBITMAP PicH1=  (HBITMAP) LoadImage(NULL,m_strPicPath+"\\水印.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
		BITMAP bm1; 
		::GetObject(PicH1,sizeof(BITMAP),&bm1); 
		int nBitsPixel=bm1.bmBitsPixel;
		//取到的数组，如果宽为奇数，每行尾自动加1字节补足，需要取余忽略,
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



	LPBITMAPINFO lpbmih = new BITMAPINFO;//图形格式参数
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = nZHPicWidth;//
	lpbmih->bmiHeader.biHeight = ptMAX.y;
	lpbmih->bmiHeader.biPlanes = 1;//目标设备位面数，必须为1
	lpbmih->bmiHeader.biBitCount = 32;//颜色深度
	lpbmih->bmiHeader.biCompression = BI_RGB;//位图压缩类型
	lpbmih->bmiHeader.biSizeImage = 0;//大小，字节单位
	lpbmih->bmiHeader.biXPelsPerMeter = 0;//水平分辨率
	lpbmih->bmiHeader.biYPelsPerMeter = 0;//垂直分辨率
	lpbmih->bmiHeader.biClrUsed = 0;//位图实际使用颜色表中颜色数
	lpbmih->bmiHeader.biClrImportant = 0;//位图显示中比较重要的颜色数

	BITMAPFILEHEADER bmfh;
	*((char *)&bmfh.bfType) = 'B';
	*(((char *)&bmfh.bfType) + 1) = 'M';//表明是BMP文件
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//偏移位置
	//int iBMPBytes = (Lwidth*Lheight) * 32 / 8;//数据文件大小
	//bmfh.bfSize = bmfh.bfOffBits + (Lwidth*Lheight) * 32 / 8;//文件大小
	int iBMPBytes = (nZHPicWidth * 4+3)/4*4*ptMAX.y;//数据文件大小
	bmfh.bfSize = bmfh.bfOffBits + iBMPBytes;//文件大小
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
	ShellExecuteA(NULL,"open",m_strProgPath+"\\缩放图片.exe",PicName,NULL,SW_SHOW);
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

//生成图片
int CGTRYLZT::SaveBigPic(CString strPicName,CString strPicID,CString strPath)
{



	if(strPicName=="生成图片")
	{

		goto NEXT_STEP;
	}
	if (strPicName=="换行")
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
		//WriteToFile("图片[%s]不存在\r\n",strPicName);
		return FALSE;
	}
	//WriteToFile("小图[%s]\r\n",strPicName);
	//WriteToFile("将小图拼到大图坐标%d,%d\r\n",ptBigPic.x,ptBigPic.y);

	//小图装入数组
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
			WriteToFile("申请内存失败\r\n");
			return 2120;
		}
		//背景色
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
		//WriteToFile("宽度不足,换行\r\n");
		ptMAX.x=max(ptBigPic.x,ptMAX.x);
		ptBigPic.x = 0;
		ptBigPic.y+=ptMAX.y+1;
		ptMAX.y=0;
		//WriteToFile("ptBigPic.y=%d\r\n",ptBigPic.y);
	}

	if ((Lheight-ptBigPic.y)<Sheight)
	{
		WriteToFile("Lheight-ptBigPic.y=%d-%d",Lheight,ptBigPic.y);
		WriteToFile("高度不足\r\n");
		return 2120;
		//bPicFull = TRUE;
		////nRetCode=2;
		//goto NEXT_STEP;
	}

	BYTE *tData;
	tData = new BYTE[(Swidth*4+4-Swidth%4) * Sheight];
	if (tData==NULL)
	{
		WriteToFile("tData申请内存失败\r\n");
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

	//没满不生成图片
	if (!bPicFull)
		return 1;

NEXT_STEP:
	if(ptMAX.x==0 && ptMAX.y==0)
	{
		//WriteToFile("大图为空\r\n");
		return 2120;
	}







	////////////////缩放///////////////////////////////
	if(ptBigPic.y>0)//多排
	{
		ptMAX.x=max(ptBigPic.x,ptMAX.x);
		ptMAX.y+=ptBigPic.y;
	}
	else
		ptMAX.x+=ptBigPic.x;




	BYTE *pBMPData=new BYTE [(ptBigPic.x+3)/4*4*ptMAX.y*4];
	//修改背景颜色
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





	LPBITMAPINFO lpbmih = new BITMAPINFO;//图形格式参数
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = ptBigPic.x;//
	lpbmih->bmiHeader.biHeight = ptMAX.y;
	lpbmih->bmiHeader.biPlanes = 1;//目标设备位面数，必须为1
	lpbmih->bmiHeader.biBitCount = 32;//颜色深度
	lpbmih->bmiHeader.biCompression = BI_RGB;//位图压缩类型
	lpbmih->bmiHeader.biSizeImage = 0;//大小，字节单位
	lpbmih->bmiHeader.biXPelsPerMeter = 0;//水平分辨率
	lpbmih->bmiHeader.biYPelsPerMeter = 0;//垂直分辨率
	lpbmih->bmiHeader.biClrUsed = 0;//位图实际使用颜色表中颜色数
	lpbmih->bmiHeader.biClrImportant = 0;//位图显示中比较重要的颜色数

	BITMAPFILEHEADER bmfh;
	*((char *)&bmfh.bfType) = 'B';
	*(((char *)&bmfh.bfType) + 1) = 'M';//表明是BMP文件
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//偏移位置
	//int iBMPBytes = (Lwidth*Lheight) * 32 / 8;//数据文件大小
	//bmfh.bfSize = bmfh.bfOffBits + (Lwidth*Lheight) * 32 / 8;//文件大小
	int iBMPBytes = (ptBigPic.x * 4+3)/4*4*ptMAX.y;//数据文件大小
	bmfh.bfSize = bmfh.bfOffBits + iBMPBytes;//文件大小
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
//截取小图片
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

	CString PicName=m_strProgPath+"\\小图.bmp";

	//PicName.Format("F:\\%d.bmp",0);

	//RC2Tool::CaptureGamePictureEx(NULL,PicName,left,top,Sweight,Sheight);
	return true;

}
//拼图(需要拼在一起的图片名,用逗号分隔)
int CGTRYLZT::PinTu(CString strAllPic,CString strPicID,int picWidth)
{
	CStringArray arrPicName;
	myApp.SplitString(strAllPic,",",arrPicName);

	int status=0;
	for(int i=0;i<arrPicName.GetSize();i++)
	{
		if(arrPicName.GetAt(i)=="换行")
		{
			SaveBigPic(m_hGameWnd,"换行");
			continue;
		}

		status=SaveBigPic(m_hGameWnd,m_strCapturePath+arrPicName.GetAt(i),"",picWidth);
		if(status>1000)
			return status;
	}


	SaveBigPic(m_hGameWnd,"生成图片",strPicID);
	arrPicName.RemoveAll();
	return 1;
}
//拼图(需要拼在一起的图片名,用逗号分隔)
int CGTRYLZT::PinTu(CString strAllPic,CString strPicID,CString path)
{
	CStringArray arrPicName;
	myApp.SplitString(strAllPic,",",arrPicName);

	int status=0;
	for(int i=0;i<arrPicName.GetSize();i++)
	{
		if(arrPicName.GetAt(i)=="换行")
		{
			SaveBigPic("换行",strPicID,path);
			continue;
		}

		status=SaveBigPic(path+"\\"+arrPicName.GetAt(i),strPicID,path);
		if(status>1000)
			return status;
	}

	SaveBigPic("生成图片",strPicID,path);
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
	if (the_strOrderNo == _T("测试订单"))
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

	//WinExec(m_strProgPath+"\\1.bat",SW_SHOW);//清理缓存WorldOfWarships.exe


	/*for(int i=0;i<20;i++)
	{
		myApp.MoveMouse(GetDesktopWindow(),900+i*10,1005);
		Sleep(50);
	}*/
	//myApp.MoveMouse(GetDesktopWindow(),50,50);

}
//生成图片
int CGTRYLZT::SaveBMPVertical(HWND hWnd,CString strPicName,CString strPicID,CString PicPath)
{


	if(strPicName=="生成图片")
	{

		goto NEXT_STEP;
	}
	if (strPicName=="换行")
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
		//WriteToFile("图片[%s]不存在\r\n",strPicName);
		return FALSE;
	}
	//WriteToFile("小图[%s]\r\n",strPicName);
	//WriteToFile("将小图拼到大图坐标%d,%d\r\n",ptBigPic.x,ptBigPic.y);

	//小图装入数组
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


	//WriteToFile("保存[%s],宽=%d,高=%d\r\n",strPicName,Swidth,Sheight);


	//Sleep(200);
	if (bPicFull)
	{

		/*pBigPic = new BYTE[Lwidth * Lheight * 4 ];*/
		if (pBigPic==NULL)
		{
			WriteToFile("申请内存失败\r\n");
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
		//WriteToFile("宽度不足,换行\r\n");
		ptMAX.x=max(ptBigPic.x,ptMAX.x);
		ptBigPic.x = 0;
		ptBigPic.y+=ptMAX.y+1;
		ptMAX.y=0;
		//WriteToFile("ptBigPic.y=%d\r\n",ptBigPic.y);
	}

	if ((Lheight-ptBigPic.y)<Sheight)
	{
		WriteToFile("Lheight-ptBigPic.y=%d-%d",Lheight,ptBigPic.y);
		WriteToFile("高度不足\r\n");
		return 2120;
		//bPicFull = TRUE;
		////nRetCode=2;
		//goto NEXT_STEP;
	}

	BYTE *tData;
	tData = new BYTE[(Swidth*4+4-Swidth%4) * Sheight];
	if (tData==NULL)
	{
		WriteToFile("tData申请内存失败\r\n");
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

	//没满不生成图片
	if (!bPicFull)
		return 1;

NEXT_STEP:
	if(ptMAX.x==0 && ptMAX.y==0)
	{
		WriteToFile("大图为空\r\n");
		return 2120;
	}







	////////////////缩放///////////////////////////////
	if(ptBigPic.y>0)//多排
	{
		ptMAX.x=max(ptBigPic.x,ptMAX.x);
		ptMAX.y+=ptBigPic.y;
	}
	else
		ptMAX.x+=ptBigPic.x;

	BYTE *pBMPData=new BYTE [(ptMAX.x+3)/4*4*ptMAX.y*4];
	//修改背景颜色
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


	LPBITMAPINFO lpbmih = new BITMAPINFO;//图形格式参数
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = ptMAX.x;//
	lpbmih->bmiHeader.biHeight = ptMAX.y;
	lpbmih->bmiHeader.biPlanes = 1;//目标设备位面数，必须为1
	lpbmih->bmiHeader.biBitCount = 32;//颜色深度
	lpbmih->bmiHeader.biCompression = BI_RGB;//位图压缩类型
	lpbmih->bmiHeader.biSizeImage = 0;//大小，字节单位
	lpbmih->bmiHeader.biXPelsPerMeter = 0;//水平分辨率
	lpbmih->bmiHeader.biYPelsPerMeter = 0;//垂直分辨率
	lpbmih->bmiHeader.biClrUsed = 0;//位图实际使用颜色表中颜色数
	lpbmih->bmiHeader.biClrImportant = 0;//位图显示中比较重要的颜色数

	BITMAPFILEHEADER bmfh;
	*((char *)&bmfh.bfType) = 'B';
	*(((char *)&bmfh.bfType) + 1) = 'M';//表明是BMP文件
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//偏移位置
	//int iBMPBytes = (Lwidth*Lheight) * 32 / 8;//数据文件大小
	//bmfh.bfSize = bmfh.bfOffBits + (Lwidth*Lheight) * 32 / 8;//文件大小
	int iBMPBytes = (ptMAX.x * 4+3)/4*4*ptMAX.y;//数据文件大小
	bmfh.bfSize = bmfh.bfOffBits + iBMPBytes;//文件大小
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
//循环查看文件是否存在,返回存在的文件名
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
/* 调用Wangyi.exe程序执行网页登录和账号绑定信息截图的功能               */
/************************************************************************/
BOOL CGTRYLZT::CheckAccount()
{
	CString stry;
	CString orderdata;
	HWND hwnd1;
	orderdata.Format("游戏名称：阴阳师；游戏大区：%s；游戏服务器：%s；游戏帐号：%s；游戏密码：%s；",m_strArea,m_strServer,m_strAccount,m_strPassword);
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
			WriteToFile("等待账号验证结果...");
		}
		hwnd1=::FindWindowA(NULL,"WangYi");
		if (hwnd1)
		{
			Sleep(1000);
		}
		else
		{
			char strBuilder[520];
			::GetPrivateProfileString("账号信息", "执行状态", "", strBuilder, 520,m_strProgPath+"\\roleInfo.ini");
			CString s;
			s.Format("%s",strBuilder);
			if (s=="1000")
			{
				WriteToFile("账号密码正确");
			
				return 1000;
			}
			else if (s=="2000")
			{
				WriteToFile("账号密码错误");
				
				return 3000;
			}
			else
			{
				WriteToFile("账号密码验证失败");
				
				return 2120;
			}
		}
	}
	WriteToFile("等待账号验证结果超时");
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
TODO:装备属性截图
对人物套装的每一个装备属性截图，头盔，配饰等  
TODO:对装备仓库截图
对仓库中的物品截图，不包含我的包裹
TODO:包裹物品截图,只包括我的包裹
960550->960150 拖动范围
590,120->1000,590 截图范围
TODO:包裹法宝截图
1060,450 法宝按钮
150,120->560,650 截图范围
*/
/************************************************************************/
void CGTRYLZT::CapturePackage(HWND m_hGameWnd)
{
	BackToGamePage(m_hGameWnd,m_strPicPath);

	WriteToFile("包裹装备截图");
	POINT pt;
	Sleep(2000);
	for (int i=0;i<5;i++)
	{
		//TODO:查找包裹的图片
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"包裹菜单",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,pt.x+10,pt.y+10);//点击包裹[1130,560]
			break;
		}
		else WriteToFile("找不到包裹菜单%d次",i);
		Sleep(1000);
	}
	if (pt.x<0)
	{
		WriteToFile("找不到包裹菜单");
		return;
	}
	
	Sleep(5000);

	CaptureJpgInRect("包裹",150,110,560,650);
	CaptureJpgInRect("游戏币",130,620,570,660);
	
	BOOL flag=FALSE;//判断现在显示的是不是第一个套装
	if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"装备切换",&pt))
	{
		flag=TRUE;
	}
	//TODO：装备属性截图
	for(int t=1;t<=16;t++)
	{
		if(t==9)
		{
			//TODO：切换套装时判断是否有其他套装可以切换
			myApp.PressMouseKey(m_hGameWnd,340,140);//切换套装
			Sleep(2000);
			if(flag==TRUE && myApp.FindBmp(m_hGameWnd,m_strPicPath+"装备切换",&pt))
			{
				WriteToFile("玩家等级未满60级");
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
			if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"装备卸下",&pt,0,0,0,0,TRUE,60)
				|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"装备卸下2",&pt,0,0,0,0,TRUE,60)
				/*||myApp.FindBmp(m_hGameWnd,m_strPicPath+"查看装备",&pt,0,0,0,0,TRUE,60,TRUE)*/)
			{
				CString s;
				s.Format("%s%d","包裹装备",t);
				CaptureJpgInRect(s,600,50,930,680);//装备属性截图
				break;
			}
		}
	}
	WriteToFile("完成装备属性截图");

	//TODO:对装备仓库截图
	myApp.PressMouseKey(m_hGameWnd,1060,330);//点击仓库
	Sleep(5000);
	CString s;
	int f=0;
	for(int i=0;i<12;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"仓库1",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"仓库2",&pt))
		{
			f++;
			WriteToFile("找到第一个仓库%d次",f);
		}
		if(f>1) break;
		s.Format("%s%d","仓库",i+1);
		CaptureJpgInRect(s,150,110,550,590);//仓库物品截图
		WriteToFile("完成对仓库%d截图",i);
		//myApp.PressMouseKey(m_hGameWnd,340,150);//点击选择仓库
		myApp.PressMouseKey(m_hGameWnd,280,640);//点击下一个仓库按钮
		Sleep(2000);
	}

	//TODO:包裹物品截图
	for (int i=0;i<4;i++)
	{
		WriteToFile("包裹物品截图%d次",i);
		s.Format("%s%d","包裹物品",i+1);
		CaptureJpgInRect(s,590,120,1000,590);
		myApp.DragMouse(m_hGameWnd,960,150,960,550);//拖动列表下拉
		WriteToFile("拖动物品列表下拉");
		Sleep(1000);
	}
	//TODO:包裹法宝截图
	//1060,450 法宝按钮
	//150,120->560,650 截图范围
	WriteToFile("包裹法宝截图");
	Sleep(2000);
	if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"法宝",&pt))
	{
		myApp.PressMouseKey(m_hGameWnd,1060,450);
		Sleep(1000);
		myApp.PressMouseKey(m_hGameWnd,250,400);
		Sleep(1000);
		CaptureJpgInRect("主动法宝",150,120,560,650);
		myApp.PressMouseKey(m_hGameWnd,450,400);
		Sleep(1000);
		CaptureJpgInRect("被动法宝",150,120,560,650);
	}
	else WriteToFile("包裹中没有法宝");
	
	myApp.PressMouseKey(m_hGameWnd,1020,80);//关闭包裹页面
	Sleep(1000);
}
/************************************************************************/
/* TODO:对人物信息截图
分别是属性、信息、加点的截图
*/
/************************************************************************/
void CGTRYLZT::CapturePlayerInfo(HWND m_hGameWnd)
{

	BackToGamePage(m_hGameWnd,m_strPicPath);

	Sleep(2000);
	WriteToFile ("人物资料截图");
	myApp.PressMouseKey(m_hGameWnd,1130,80);//点击人物属性,角色头像
	Sleep(2000);
	CaptureJpgInRect("人物属性",120,60,1020,660,TRUE, RGB( 255, 188,0 ),260,110,400,150);//角色名加水印
	Sleep(1000);



	myApp.PressMouseKey(m_hGameWnd,1060,330);//点击人物信息
	Sleep(2000);
	CaptureJpgInRect("人物信息",120,60,1020,660);
	Sleep(1000);

	//TODO:人物加点信息有多个，逐个选中截图。。。
	myApp.PressMouseKey(m_hGameWnd,1060,450);//点击人物加点
	Sleep(2000);
	//CaptureJpgInRect("人物加点1",120,60,1020,660);
	//Sleep(1000);
	myApp.PressMouseKey(m_hGameWnd,250,140);//切换加点方案
	Sleep(1000);
	myApp.PressMouseKey(m_hGameWnd,250,220);//加点方案1
	Sleep(1000);
	CaptureJpgInRect("加点方案1",140,110,1000,650);

	myApp.PressMouseKey(m_hGameWnd,250,140);//切换加点方案
	Sleep(1000);
	myApp.PressMouseKey(m_hGameWnd,250,300);//加点方案2
	Sleep(1000);
	CaptureJpgInRect("加点方案2",140,110,1000,650);

	myApp.PressMouseKey(m_hGameWnd,1020,80);//关闭人物资料叉叉
}
/************************************************************************/
/* TODO：对宠物信息截图
包括宠物基础属性，资质技能，宠物内丹
*/
/************************************************************************/
void CGTRYLZT::CapturePet(HWND m_hGameWnd)
{

	WriteToFile("开始对宠物信息截图");
	CString s;
	POINT pt;
	for (int n=0;n<5;n++)
	{
		//返回游戏页面
		BackToGamePage(m_hGameWnd,m_strPicPath);

		myApp.PressMouseKey(m_hGameWnd,980,80);//宠物头像
		Sleep(2000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"宠物",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"宠物属性",&pt))
		{
			for (int i=0;i<8;i++)
			{
				myApp.PressMouseKey(m_hGameWnd,200+(i%4)*100,520+100*(i/4));//选中宠物
				Sleep(2000);
				//TODO:没有该宠物
				if (!myApp.FindBmp(m_hGameWnd,m_strPicPath+"宠物属性",&pt)
					|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"购买宠物",&pt)
					|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"购买宠物2",&pt))
				{
					myApp.PressMouseKey(m_hGameWnd,1060,80);//关闭购买宠物窗口
					Sleep(1000);
					break;
				}
				myApp.PressMouseKey(m_hGameWnd,650,140);//点击宠物的基础属性
				Sleep(1000);
				s.Format("%s%d","宠物",i+1);
				CaptureJpgInRect(s+"基本属性",140,120,1010,670);
				myApp.PressMouseKey(m_hGameWnd,800,140);//资质技能
				Sleep(1000);
				CaptureJpgInRect(s+"资质技能",580,120,1010,660);
				myApp.PressMouseKey(m_hGameWnd,940,140);
				Sleep(1000);
				CaptureJpgInRect(s+"宠物内丹",580,120,1010,660);
			}	
			myApp.PressMouseKey(m_hGameWnd,1020,80);//关闭宠物界面
			WriteToFile("宠物信息截图完成");
			break;
		}
		else WriteToFile("找不到宠物信息%d次",n);
	}
	
	
	
	//TODO:退回到游戏页面
	BackToGamePage(m_hGameWnd,m_strPicPath);
}
/************************************************************************/
/* TODO:对人物技能截图
包括人物技能，帮派技能和修炼技能
*/
/************************************************************************/

void CGTRYLZT::CaptureSkill(HWND m_hGameWnd)
{
	BackToGamePage(m_hGameWnd,m_strPicPath);

	WriteToFile("人物技能截图");
	POINT pt;
	for (int i=0;i<5;i++)
	{
		if (!myApp.FindBmp(m_hGameWnd,m_strPicPath+"技能",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,1130,650);//功能菜单
			Sleep(1000);
		}
		else break;
		Sleep(1000);
	}
	
	if (myApp.FindBmp(m_hGameWnd,m_strPicPath+"技能",&pt))
	{
		//Sleep(1000);
		myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y);//人物[950,650]
		Sleep(2000);
		CaptureJpgInRect("人物技能",150,140,600,620);

		myApp.PressMouseKey(m_hGameWnd,1060,330);//帮派
		Sleep(2000);
		CaptureJpgInRect("帮派技能",150,100,340,650);


		myApp.PressMouseKey(m_hGameWnd,1060,450);//修炼
		Sleep(2000);
		CaptureJpgInRect("修炼技能",150,130,370,620);

		myApp.PressMouseKey(m_hGameWnd,1020,80);
		Sleep(1000);
	}
	else WriteToFile("找不到人物技能");
	
	if (!myApp.FindBmp(m_hGameWnd,m_strPicPath+"家园",&pt))
	{
		myApp.PressMouseKey(m_hGameWnd,1130,650);//功能菜单
		Sleep(1000);
	}
	if (myApp.FindBmp(m_hGameWnd,m_strPicPath+"家园",&pt))
	{
		myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y);//家园[870,650]
		Sleep(1000);
		CaptureJpgInRect("欢乐家园",240,90,940,640);
		myApp.PressMouseKey(m_hGameWnd,920,120);//关闭家园
		Sleep(1000);
	}
	else WriteToFile("玩家没有家园");
}
/************************************************************************/
/* TODO:对手机关联和安全锁截图                                                                     */
/************************************************************************/
void CGTRYLZT::CaptureSafe(HWND m_hGameWnd)
{
	BackToGamePage(m_hGameWnd,m_strPicPath);

	WriteToFile("开始对手机关联和安全锁截图");
	POINT pt;
	for (int i=0;i<4;i++)
	{
		Sleep(1000);
		if (!myApp.FindBmp(m_hGameWnd,m_strPicPath+"系统",&pt,230,580,1170,700))
		{
			myApp.PressMouseKey(m_hGameWnd,1130,650);//功能菜单
			Sleep(1000);
		}
		if (myApp.FindBmp(m_hGameWnd,m_strPicPath+"系统",&pt,230,580,1170,700))
		{
			WriteToFile("正在对手机关联和安全锁截图");
			myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y);//系统设置
			Sleep(1000);

			myApp.PressMouseKey(m_hGameWnd,830,580);//手机关联
			Sleep(1000);
			CaptureJpgInRect("关联手机",330,200,850,540);
			myApp.PressMouseKey(m_hGameWnd,830,220);
			Sleep(1000);

			myApp.PressMouseKey(m_hGameWnd,1000,380);//安全锁
			Sleep(1000);
			CaptureJpgInRect("安全锁",180,130,970,620);
			myApp.PressMouseKey(m_hGameWnd,950,140);//关闭系统设置
			Sleep(1000);
			break;
		}
		else WriteToFile("找不到系统菜单按钮");
	}
	
}
/************************************************************************/
/* TODO:出售和充值返利                                                                     */
/************************************************************************/
void CGTRYLZT::CaptureSell(HWND m_hGameWnd)
{
	BackToGamePage(m_hGameWnd,m_strPicPath);
	
	POINT pt;
	WriteToFile("准备对出售和充值返利截图");
	for (int i=0;i<5;i++)
	{
		Sleep(1000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"商城",&pt))
		{
			WriteToFile("正在进行出售和充值返利截图");
			myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y);//商城
			Sleep(1000);
			myApp.PressMouseKey(m_hGameWnd,430,140);//出售
			Sleep(1000);
			CaptureJpgInRect("我要出售",140,110,1010,660);
			myApp.PressMouseKey(m_hGameWnd,1060,570);//充值
			Sleep(2000);
			//TODO:拥有的仙玉截图
			CaptureJpgInRect("仙玉",770,130,1020,170);//仙玉数量
			myApp.PressMouseKey(m_hGameWnd,430,130);//充值返利
			Sleep(1000);
			myApp.DragMouse(m_hGameWnd,300,220,300,600);
			Sleep(1000);
			CaptureJpgInRect("充值返利",140,110,1010,660);
			myApp.PressMouseKey(m_hGameWnd,1020,80);
			WriteToFile("完成对出售和充值返利截图");
			break;
		}
	}
	if(pt.x<0)
	{
		WriteToFile("找不到商城");
	}
}

/************************************************************************/
/* 选择游戏区服                                                         */
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
			WriteToFile("区服正确");
			return 1;
		}
		else
		{
			if (SecondEnter)
			{
				WriteToFile("区服配置有误");
				return 2260;
			}
			WriteToFile("默认区服错误,修改区服配置");
			//修改完成区服后需要重新登录，配置文件才能生效
			//CloseGame();//关闭游戏
			
			//CloseWind();
			//SecondEnter=TRUE;
			CString m_Hostnum,m_UserId,NowLoginInfo,m_LoginInfo;
			int l=stmp.Find(":"+m_strServer);
			if (stmp.Find(":"+m_strServer)<0)
			{
				WriteToFile("账号不存在该区服角色");
				return 3010;
			}
			m_UserId=myApp.FindStr(stmp,":"+m_strServer+":",":");
			m_Hostnum=stmp.Mid(stmp.Find(":"+m_strServer)-4,4);
			NowLoginInfo=myApp.FindStr(stmp,"\<string name=\"XyqPocket_LoginInfo_"+m_Hostnum+"_"+m_UserId+"\"","/string\>");
			m_LoginInfo=m_strArea+":"+m_Hostnum+":"+m_strServer+":"+m_UserId+":9999999999:1::0";
			WriteToFile("登陆服务器的信息："+m_LoginInfo);
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
		WriteToFile("读取配置游戏文件出错");
		return 2260;
	}
}

