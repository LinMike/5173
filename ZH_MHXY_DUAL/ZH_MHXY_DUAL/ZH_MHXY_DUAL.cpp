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



int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
#ifdef _TEST
	//
#else

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
	/*if(rect.Width()!=1280 || rect.Height()!=1024)
	{
		WriteToFile("电脑分辨率不是1280*1024\r\n");
		return 2260;
	}*/
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
		/*KillProcess();

		Status=WaitStartGame();
		if (Status==2120)
			continue;
		if (Status > 1000)
			return Status;
		Status=EnterAccPwd();
		if (Status==2120)
			continue;
		if (Status > 1000)
			return Status;*/


		Status=Trade();
		if (Status==2120)
			continue;
		return Status;
	}



	return Status;


}
int CGTRYLZT::SelectServer()
{
	int index=m_strArea.Find("区");
	if( m_strArea.Find("区")<0)
	{
		WriteToFile ("区服错误");
		return 2260;
	}
	CString strServer=m_strArea.Left(index);
	CString strText="手Q,";
	CString strText1="手Q_1,";
	for(int i=0;i<strServer.GetLength();i++)
	{
		strText+=strServer.Mid(i,1)+",";
		strText1+=strServer.Mid(i,1)+"_1,";
	}
	strText+="区";
	strText1+="区_1";
	PinTu(strText,"区服",m_strPicPath+"区服");
	PinTu(strText1,"区服1",m_strPicPath+"区服");
	POINT  pt;
	myApp.PressMouseKey (m_hGameWnd,710,465);
	for(int i=0;i<10;i++)
	{
		Sleep(1000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"区服页面",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"区服页面2",&pt))
			break;
	}
	if(pt.x<0)
	{
		WriteToFile ("等待区服页面超时");
		return 2260;
	}
	CaptureJpg("区服页面");
	for(int i=0;i<10;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"我的服务器",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"我的服务器2",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"我的服务器3",&pt))
			break;
		DragMouse(m_hGameWnd,60,200,60,800);
	}
	if(pt.x<0)
	{
		WriteToFile ("找不到[我的服务器]");
		return 2260;
	}
	myApp.PressMouseKey (m_hGameWnd,pt.x+50,pt.y+10);
	Sleep(1000);
	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"区服\\区服",&pt)
		&& !myApp.FindBmp(m_hGameWnd,m_strPicPath+"区服\\区服1",&pt))
	{
		WriteToFile ("找不到服务器");
		return 2260;
	}
	myApp.PressMouseKey (m_hGameWnd,pt.x+100,pt.y+10);
	Sleep(1000);
	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"开始",&pt)
		&& !myApp.FindBmp(m_hGameWnd,m_strPicPath+"开始2",&pt))
	{
		WriteToFile ("找不到[开始]");
		return 2260;
	}
	CaptureJpg("区服");
	myApp.PressMouseKey (m_hGameWnd,pt.x+10,pt.y+10);
	Sleep(5000);
	for(int i=0;i<20;i++)
	{
		Sleep(3000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"英雄",&pt))
			break;
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"英雄2",&pt))
			break;
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"叉叉",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"叉叉2",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"叉叉3",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"叉叉4",&pt))
		{
			myApp.PressMouseKey (m_hGameWnd,pt.x+20,pt.y+20);
			//myApp.PressMouseKey (m_hGameWnd,pt.x+20,pt.y+20);
			Sleep(500);
			myApp.PressMouseKey (m_hGameWnd,20,20);
		}
		else
			myApp.PressMouseKey (m_hGameWnd,880,115);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"赛事系统",&pt))
			myApp.PressMouseKey (m_hGameWnd,50,70);

	}
	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"英雄",&pt)
		&& !myApp.FindBmp(m_hGameWnd,m_strPicPath+"英雄2",&pt))
	{
		WriteToFile ("找不到[英雄]");
		return 2260;
	}
	return 1;
}
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
	WinExec(m_strProgPath+"\\浏览器.exe https://my.xoyo.com",SW_SHOW);
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
		return 2260;
	}
	SetWindowPos(m_hGameWnd,NULL,0,0,0,0,SWP_NOSIZE);






	static int errorTimes=0;
	static int yzmTimes=0;

PWD_ERROR:

	for(int i=0;i<10;i++)
	{
		Sleep(3000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"网页登录",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"网页登录2",&pt,0,0,0,0,TRUE,24,FALSE))
			break;
		myApp.SendFuncKey(VK_F5);
	}
	if(pt.x<0)
	{
		WriteToFile ("找不到网页登录");
		CaptureJpg("找不到网页登录");
		return 2120;
	}

	for(int i=0;i<10;i++)
	{	
		cout<<"00"<<endl;
		if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"验证码空",&ptTemp,800,310,940,370,TRUE,24,FALSE))
			break;
		cout<<"11"<<endl;
		myApp.SendFuncKey(VK_F5);
		Sleep(5000);
	}
	if(ptTemp.x>0)
	{
		WriteToFile ("验证码刷新失败");
		return 2260;
	}

	myApp.PressMouseKey (m_hGameWnd,pt.x+100,pt.y-188);
	Sleep (300);
	myApp.SendKeys(m_strAccount,200);


	Sleep (300);
	myApp.PressMouseKey (m_hGameWnd,pt.x+100,pt.y-145);

	Sleep (300);
	myApp.SendKeys(m_strPassword,200);

	myApp.CapturePictureInRect(m_hGameWnd,"E:\\1.bmp",pt.x+38,pt.y-120,pt.x+175,pt.y-60);
	myApp.BMPToJPG("E:\\1.bmp");

	CString strResult=RequestSafeCardInfo(1,"E:\\1.jpg","",90);
	if (strResult.GetLength() < 4)
	{
		WriteToFile ("答题员放弃验证码答题");
		return 2230;
	}

	myApp.PressMouseKey(m_hGameWnd,pt.x+10,pt.y-90);
	Sleep(1000);

	myApp.SendKeys(strResult,200);


	WriteToFile ("输入完成 账号[%s]密码[%d]位",m_strAccount,m_strPassword.GetLength());
	Sleep (500);
	CaptureJpg("账号密码");

	myApp.PressMouseKey (m_hGameWnd,pt.x+10,pt.y+10);
	Sleep (2000);
	CaptureJpg("账号密码输完");
	static int inputTimes=0;
	for(int i=0;i<3;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"安全中心",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"安全中心2",&pt))
			break;

		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"网页密码错",&pt))
		{
			WriteToFile ("账号密码错");
			CaptureJpg("账号密码错");
			if(errorTimes>0)
				return 3000;
			errorTimes++;
			goto PWD_ERROR;
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"验证码错",&pt))
		{
			WriteToFile ("验证码不正确");
			CaptureJpg("验证码错");
			yzmTimes++;
			if(yzmTimes>3)
			{
				WriteToFile ("验证码错超过3次");
			}
			return 2120;
		}
		myApp.SendFuncKey(VK_F5);
		Sleep (5000);
	}
	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"安全中心",&pt)
		&& !myApp.FindBmp(m_hGameWnd,m_strPicPath+"安全中心2",&pt))
	{
		CaptureJpg("登录失败");
		WriteToFile("登录失败");
		return 2260;
	}
	myApp.PressMouseKey (m_hGameWnd,pt.x+10,pt.y+10);
	Sleep (2000);
	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"账号安全",&pt)
		&& !myApp.FindBmp(m_hGameWnd,m_strPicPath+"账号安全2",&pt))
	{
		CaptureJpg("账号安全");
		WriteToFile("找不到账号安全");
		return 2260;
	}

	CaptureJpgInRect("账号安全", pt.x-45,pt.y-15,pt.x+650,pt.y+375);

	myApp.KillWindow("浏览器.exe");
	return 1;
}

//判断是否被顶号 
BOOL CGTRYLZT::NetBreak()
{
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
	//TODO:搜索后台进程查找模拟器
	/*m_hGameWnd=FindWindow(NULL,"海马玩模拟器 0.10.5 Beta");//myApp.GetHwndByPid("海马玩模拟器 0.10.5 Beta");// 
	if(IsWindow(m_hGameWnd))
	{
		WriteToFile("获取模拟器窗口");
		ShowWindow(m_hGameWnd,-1);
		
		WriteToFile("显示模拟器窗口");
	}
	else
	{*/
		//TODO:打开游戏进程
		//HINSTANCE n =::ShellExecute(NULL,"open", m_strGameStartFile,"",m_strGamePath ,SW_SHOWNORMAL );
		//if( n < (HINSTANCE) 31)
		//{
		//	WriteToFile("打开游戏失败\r\n");
		//	return 2120;//开启异常
		//}

		//for(int i=0;i<30;i++)
		//{
		//	//m_hGameWnd=FindWindow(NULL,"海马玩模拟器 0.10.5 Beta");
		//	m_hGameWnd=myApp.GetHwndByPid("VBoxHeadless.exe");
		//	if(IsWindowVisible(m_hGameWnd))
		//		break;
		//	Sleep(3000);
		//}
	//}
	for(int i=0;i<30;i++)
	{
		m_hGameWnd=myApp.GetHwndByPid("Droid4X.exe");
		if(IsWindowVisible(m_hGameWnd))
			break;
		else ShowWindow(m_hGameWnd,SW_SHOW);
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
		//myApp.PressMouseKey(m_hGameWnd,740,120);//点击home
		//WriteToFile("点击home");
		//Sleep(1000);
		//myApp.PressMouseKey(m_hGameWnd,690,90);//点击我的桌面
		//WriteToFile("点击我的桌面");

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
	myApp.PressMouseKey(m_hGameWnd,pt.x+30,pt.y-50,1,50);
	WriteToFile("等待游戏窗口");
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
CheckInOther:
	pt.x=pt.y=-1;//初始化
	for(int i=0;i<10;i++)
	{
		Sleep(1000);
		if(i%20==0)
			WriteToFile("等待登录页面");
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"其他账号",&pt))
			break;
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"切换账号",&ptTemp))
			myApp.PressMouseKey(m_hGameWnd,ptTemp.x+20,ptTemp.y+20);
	}
	if(pt.x<0)
	{
		WriteToFile("找不到图片[其他账号登录]");
		for(int i=0;i<10;i++)
		{
			if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"用户中心",&pt))
				break;
			Sleep(1000);
		}
		if(pt.x<0)
			return 2260;
		else {
			WriteToFile("找到用户中心");
			myApp.PressMouseKey(m_hGameWnd,pt.x+20,pt.y+20);
			goto CheckInOther;
		}
	}
	Sleep(2000);
	myApp.PressMouseKey(m_hGameWnd,pt.x+10,pt.y+10);//点击使用其他账号登录
	Sleep(1000);
	SetWindowPos(m_hGameWnd,NULL,0,0,0,0,SWP_NOSIZE);
	Sleep(1000);
	return 1;


}

//操作输入账号密码
BOOL CGTRYLZT::EnterAccPwd()
{

	//SendGoodsState("输入帐号密码",60);
	//CString strOS=CMyClass::GetOSVersion();
	POINT pt;
	//CaptureJpg("账号页面");
	int nCheckTimes=0;
	//WriteToFile("等待安全检查\r\n");
PASSWORD_ERROR:
	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"网易账号",&pt))
	{
		WriteToFile("不是账号页面");
		return 2260;
	}
	//检查输入法,大写锁
	if (!CheckIM())
		return 2120;
	//myApp.MoveMouse(m_hGameWnd,50,50);
	static int runTimes=0;
	if(runTimes>8)
	{
		WriteToFile("输账号超过8次\r\n");
		return 2260;
	}
	runTimes++;

	/*for(int i=0;i<5;i++)
	{
	myApp.PressMouseKey (m_hGameWnd,670,465,2);
	Sleep(1000);
	myApp.SendKeys(VK_BACK,5);
	Sleep(1000);
	}*/
	myApp.PressMouseKey (m_hGameWnd,pt.x,pt.y);//点击网易邮箱账号登录
	Sleep (1000);
	pt.x=pt.y=-1;
	for (int i=0;i<3;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"邮箱登录",&pt,0,0,0,0,TRUE,60)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"邮箱登录2",&pt,0,0,0,0,TRUE,60))
			break;
		Sleep(1000);
	}
	if(pt.x<0) 
	{
		WriteToFile("找不到邮箱登录按钮");
		return 2260;
	}
	//RC2Tool::DoubleClick (m_hGameWnd,pt_Id.x,pt_Id.y);
	//Sleep(1000);
	//myApp.Send2Keys(VK_CONTROL,'A');
	//Sleep(1000);
	myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y-150);
	myApp.SendFuncKey(VK_BACK,20);
		
	WriteToFile("开始输入账号密码\r\n");



	myApp.SendKeys(m_strAccount,200);
	myApp.PressMouseKey(m_hGameWnd,pt.x,pt.y-70);//点击账号框收起账号提示文本
	Sleep(1000);
	myApp.PressMouseKey (m_hGameWnd,pt.x+10,pt.y-70);//聚焦密码框
	//Sleep (1000);
	/*if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"登录",&pt,0,0,0,0,TRUE,60))
	{
		WriteToFile("不是账号页面");
		return 2260;
	}*/


	//myApp.PressMouseKey (m_hGameWnd,pt.x+60,pt.y-70);
	//CWinAPIKey::VKSendTabKey();//发送TAB换行
	Sleep (300);
	myApp.SendFuncKey(VK_BACK,20);//如果记住了密码，则清除
	myApp.SendKeys(m_strPassword,200);
	//RC2Tool::DoubleClick (m_hGameWnd,pt_Pwd.x,pt_Pwd.y);
	CString strLog;



	WriteToFile ("输入完成 账号[%s]密码[%d]位",m_strAccount,m_strPassword.GetLength());
	Sleep (1000);
	//CaptureJpg("账号密码");
	if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"邮箱登录2",&pt,0,0,0,0,TRUE,60))
	{
		WriteToFile("不是账号页面");
		return 2260;
	}

	myApp.PressMouseKey (m_hGameWnd,pt.x+5,pt.y+5);
	Sleep(2000);
	//CaptureJpg("登录");
	for(int i=0;i<5;i++)
	{
		Sleep(2000);
		if(!myApp.FindBmp(m_hGameWnd,m_strPicPath+"邮箱登录2",&pt,0,0,0,0,TRUE,60))
			break;

	}
	if(pt.x>0)
	{
		WriteToFile ("登录页面没有消失");
		return 2260;
	}






	for(int i=0;i<30;i++)
	{
		if(i%10==0)
			WriteToFile ("等待进入游戏");
		//Sleep(2000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"点击选服",&pt))
			break;
		Sleep(1000);
				

		/*if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"公告叉叉",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"公告叉叉2",&pt))
		{
			myApp.PressMouseKey (m_hGameWnd,pt.x+20,pt.y+20);
			Sleep(1000);
		}

		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"查看",&pt)
			|| myApp.FindBmp(m_hGameWnd,m_strPicPath+"查看2",&pt))
		{
			break;
		}*/
	}
	if(pt.x<0)
	{
		WriteToFile ("找不到[点击选服]");
		return 2260;
	}
	myApp.PressMouseKey(m_hGameWnd,pt.x+20,pt.y+20);//点击选服按钮
	Sleep(1000);
	//CaptureJpg("区服");
	
	//TODO:选区1~8
	myApp.PressMouseKey(m_hGameWnd,240,140);//已有角色
	
	/*if(m_strArea.Find("八区")==0)
		myApp.PressMouseKey (m_hGameWnd,240,280);
	else if(m_strArea.Find("七区")==0)
		myApp.PressMouseKey (m_hGameWnd,240,360);
	else if (m_strArea.Find("六区")==0)
		myApp.PressMouseKey(m_hGameWnd,240,430);
	else if (m_strArea.Find("五区")==0)
		myApp.PressMouseKey(m_hGameWnd,240,500);*/
	
	//TODO:选服

	myApp.PressMouseKey(m_hGameWnd,500,140);//已有角色的第一个服务器
	
	/*if(m_strServer.Find("繁华笙歌")==0)
		myApp.PressMouseKey(m_hGameWnd,500,140);
	else if(m_strServer.Find("欢天喜地")==0)
		myApp.PressMouseKey(m_hGameWnd,850,140);*/

	Sleep(1000);
	//CaptureJpg("区服");
	//myApp.PressMouseKey (m_hGameWnd,510,530);
	
	/*for(int i=0;i<20;i++)
	{
		Sleep(3000);
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"启动",&pt,0,0,0,0,TRUE,80))
		{
			myApp.PressMouseKey (m_hGameWnd,pt.x+10,pt.y+10);
			Sleep(3000);
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"移动",&pt))
			break;
	}
	if(pt.x<0)
	{
		WriteToFile ("进入游戏超时");
		return 2120;
	}*/
	
	//TODO:取消活动公告等窗口
	POINT p;
	//p.x=p.y=-1;
	for (int i=0;i<5;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"取消",&p))
		{
			myApp.PressMouseKey(m_hGameWnd,p.x+5,p.y+5);
			break;
		}
	}
	for (int i=0;i<5;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"叉叉",&p))
		{
			myApp.PressMouseKey(m_hGameWnd,p.x+5,p.y+5);
			break;
		}
	}

	//CaptureJpg("登录成功");
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
	/*bgR=20,bgG=20,bgB=20;

	pBigPic = new BYTE[(Lwidth+3)/4*4 * Lheight * 4 ];

	WriteToFile ("开始拼图MHXY");
	PinTu("账号安全","MHXY1");
	PinTu("总览","MHXY2");
	PinTu("店长信息","MHXY3");
	PinTu("宅力评定","MHXY3");
	PinTu("套装","MHXY4");
	PinTu("套装1","MHXY4");
	PinTu("套装2","MHXY4");
	PinTu("套装3","MHXY4");
	PinTu("套装4","MHXY4");

	PinTu("装备","MHXY5");
	PinTu("碎片","MHXY5");
	PinTu("材料","MHXY5");
	PinTu("礼物","MHXY5");
	PinTu("消耗品","MHXY5");
	PinTu("咖啡馆","MHXY6");*/



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


	WriteToFile ("开始截图");
	//CaptureJpg("开始截图");

	/*if(m_strOrderType=="发布单")
		CaptureJpgInRect( "总览", 0,37,1024,610, TRUE, RGB( 255, 188,0 ),45,43,125,63 );
	else
		CaptureJpgInRect( "总览", 0,37,1024,610 );*/


	//TODO:关闭进入游戏后的通知公告等。。。
	for(int i=0;i<10;i++)
	{
		if (myApp.FindBmp(m_hGameWnd,m_strPicPath+"取消",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,pt.x+10,pt.y+10);
		}
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"叉叉",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,pt.x+5,pt.y+5);
		}
		Sleep(1000);
	}

	CaptureJpgInRect( "总览", 2,40,1170,690, TRUE, RGB( 255, 188,0 ),1080,660,1170,690);//进入游戏后的界面截图

	Sleep(1000);
	
	//TODO：对包裹截图
	//CapturePackage(m_hGameWnd);


	//TODO:对人物信息截图
	//CapturePlayerInfo(m_hGameWnd);

	//TODO:宠物截图
	//CapturePet(m_hGameWnd);
	
	//TODO:出售和充值返利
	//CaptureSell(m_hGameWnd);

	myApp.PressMouseKey(m_hGameWnd,1130,650);//打开功能菜单
	//TODO：技能截图
	//CaptureSkill(m_hGameWnd);
	
	//TODO:手机关联和安全锁
	CaptureSafe(m_hGameWnd);

	myApp.PressMouseKey(m_hGameWnd,1130,650);//关闭功能菜单

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
	POINT pt;
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

	return;
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

	//CloseGame();


	myApp.KillWindow("AndroidEmulator.exe");
	Sleep(100);
	myApp.KillWindow("AppMarket.exe");
	Sleep(100);
	myApp.KillWindow("kpzsPay.exe");
	Sleep(100);
	myApp.KillWindow("TianTianPlayer.exe");
	Sleep(100);
	myApp.KillWindow("TTVMSVC.exe");
	Sleep(100);
	myApp.KillWindow("VBoxHeadless.exe");
	Sleep(100);

	//WinExec(m_strProgPath+"\\1.bat",SW_SHOW);//清理缓存WorldOfWarships.exe


	for(int i=0;i<20;i++)
	{
		myApp.MoveMouse(GetDesktopWindow(),900+i*10,1005);
		Sleep(50);
	}
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

BOOL CGTRYLZT::CheckAccount()
{
	for(int i=0;i<m_strAccount.GetLength();i++)
	{
		int n=m_strAccount.GetAt(i);
		if(n<48 || n>57)
			return FALSE;
	}

	return TRUE;
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
	POINT pt;
	myApp.PressMouseKey(m_hGameWnd,1130,560);//点击包裹
	Sleep(2000);

	CaptureJpgInRect("包裹",150,110,560,650);
	//TODO：装备属性截图
	for(int t=1;t<=12;t++)
	{
		if(t==7)
		{
			myApp.PressMouseKey(m_hGameWnd,340,140);//切换套装
		}
		for (int i=0;i<3;i++)
		{
			int temp=t;
			if(temp>6) temp-=6;
			if(t%2!=0) myApp.PressMouseKey(m_hGameWnd,200,210+temp/2*100);
			else myApp.PressMouseKey(m_hGameWnd,490,210+(temp-1)/2*100);
			Sleep(2000);
			if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"装备卸下",&pt))
			{
				CString s;
				s.Format("%s%d","包裹装备",t);
				CaptureJpgInRect(s,600,110,980,720);//装备属性截图
				break;
			}
		}
	}
	//TODO:对装备仓库截图
	myApp.PressMouseKey(m_hGameWnd,1060,330);//点击仓库
	Sleep(1000);
	CString s;
	int f=0;
	for(int i=0;i<12;i++)
	{
		if(myApp.FindBmp(m_hGameWnd,m_strPicPath+"仓库1",&pt))
		{
			f++;
		}
		if(f>1) break;
		s.Format("%s%d","仓库",i+1);
		CaptureJpgInRect(s,150,110,550,590);//仓库物品截图

		//myApp.PressMouseKey(m_hGameWnd,340,150);//点击选择仓库
		myApp.PressMouseKey(m_hGameWnd,280,640);//点击下一个仓库按钮
		Sleep(1000);
	}

	//TODO:包裹物品截图
	for (int i=0;i<4;i++)
	{
		s.Format("%s%d","包裹物品",i+1);
		CaptureJpgInRect(s,590,120,1000,590);
		myApp.DragMouse(m_hGameWnd,960,150,960,550);//拖动列表下拉
		Sleep(1000);
	}
	//TODO:包裹法宝截图
	//1060,450 法宝按钮
	//150,120->560,650 截图范围
	myApp.PressMouseKey(m_hGameWnd,1060,450);
	Sleep(1000);
	myApp.PressMouseKey(m_hGameWnd,250,400);
	Sleep(1000);
	CaptureJpgInRect("主动法宝",150,120,560,650);
	myApp.PressMouseKey(m_hGameWnd,450,400);
	Sleep(1000);
	CaptureJpgInRect("被动法宝",150,120,560,650);

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
	WriteToFile ("人物资料截图");
	myApp.PressMouseKey(m_hGameWnd,1130,80);//点击人物属性,角色头像
	Sleep(1000);
	CaptureJpgInRect("人物属性",120,60,1020,660);
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
	CString s;
	POINT pt;
	myApp.PressMouseKey(m_hGameWnd,980,80);//宠物头像
	Sleep(1000);
	for (int i=0;i<8;i++)
	{
		myApp.PressMouseKey(m_hGameWnd,200+(i%4)*100,520+100*(i/4));//选中宠物
		Sleep(1000);
		//TODO:没有该宠物
		if (myApp.FindBmp(m_hGameWnd,m_strPicPath+"购买宠物",&pt))
		{
			myApp.PressMouseKey(m_hGameWnd,1060,80);//关闭购买宠物窗口
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
	myApp.PressMouseKey(m_hGameWnd,1020,80);
}
/************************************************************************/
/* TODO:对人物技能截图
包括人物技能，帮派技能和修炼技能
*/
/************************************************************************/

void CGTRYLZT::CaptureSkill(HWND m_hGameWnd)
{
	//myApp.PressMouseKey(m_hGameWnd,1130,650);//功能菜单
	Sleep(1000);
	myApp.PressMouseKey(m_hGameWnd,950,650);//人物
	Sleep(1000);
	CaptureJpgInRect("人物技能",150,140,600,620);

	myApp.PressMouseKey(m_hGameWnd,1060,330);//帮派
	Sleep(1000);
	CaptureJpgInRect("帮派技能",150,100,340,650);


	myApp.PressMouseKey(m_hGameWnd,1060,450);//修炼
	Sleep(1000);
	CaptureJpgInRect("修炼技能",150,130,370,620);

	myApp.PressMouseKey(m_hGameWnd,1020,80);


	//myApp.PressMouseKey(m_hGameWnd,1130,650);//功能菜单
	Sleep(1000);
	myApp.PressMouseKey(m_hGameWnd,870,650);//家园
	CaptureJpgInRect("欢乐家园",240,90,940,640);

	myApp.PressMouseKey(m_hGameWnd,920,120);//关闭家园
	Sleep(1000);
	//myApp.PressMouseKey(m_hGameWnd,1130,650);//功能菜单
	Sleep(1000);
}
/************************************************************************/
/* TODO:对手机关联和安全锁截图                                                                     */
/************************************************************************/
void CGTRYLZT::CaptureSafe(HWND m_hGameWnd)
{
	myApp.PressMouseKey(m_hGameWnd,700,650);//系统设置
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
}
/************************************************************************/
/* TODO:出售和充值返利                                                                     */
/************************************************************************/
void CGTRYLZT::CaptureSell(HWND m_hGameWnd)
{
	myApp.PressMouseKey(m_hGameWnd,40,250);//商城
	Sleep(1000);
	myApp.PressMouseKey(m_hGameWnd,430,140);//出售
	Sleep(1000);
	CaptureJpgInRect("我要出售",140,110,1010,660);
	myApp.PressMouseKey(m_hGameWnd,1060,570);//充值
	Sleep(1000);
	myApp.PressMouseKey(m_hGameWnd,430,130);//充值返利
	Sleep(1000);
	myApp.DragMouse(m_hGameWnd,300,220,300,600);
	Sleep(1000);
	CaptureJpgInRect("充值返利",140,110,1010,660);
	myApp.PressMouseKey(m_hGameWnd,1020,80);
}