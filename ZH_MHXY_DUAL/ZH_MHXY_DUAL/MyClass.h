#pragma once


class CMyClass
{

public:

	// 获取本程序路径
	static CString GetAppPath(void);
public:
	//美式键盘输入法ID
	static CString meishiID;
public:
	//搜狗拼音输入法ID
	static CString sougouID;


public:
	// 获取windows登录用户名
	static CString GetWinUser(void);
	// 获取计算机名
	static CString GetComputerName(void);
public:
	// 创建子目录
	static BOOL CheckSubDir(CString szDir);
public:
	// 删除指定的文件
	static void DeleteFile(CString strPathName);
public:
	static void DeleteFile(CString strPath,CString strFileName);
public:
	// 获取文件大小
	static int GetFileSize(CString strPathName);

public:
	// 获取剪贴板内容
	static CString GetClipboardText(HWND hwnd);
public:
	// 将文本内容拷贝到剪切版
	static void SetClipBoardText(HWND hwnd, CString strText);
	//日志
	static void WriteToFile(CString log,...);
	//初始化日志,默认在程序所在文件夹
	static BOOL InitLogFile( LPCTSTR lpszFolder=NULL );
	static BOOL m_bGetData;

public:
	// 获取指定窗口设备环境(截图)
	static BOOL GetDCData(HWND hWnd);
	// 获取指定窗口设备环境(截图)32位
	static BOOL GetDCData32(HWND hWnd);
	static POINT FindBmp (HWND hWnd,CString Pic,int left=0 ,int top=0 ,int right=0 ,int bottom=0,int xiangshi=24);
	static BOOL FindBmp (HWND hWnd,CString Pic,LPPOINT pt,int left=0 ,int top=0 ,int right=0 ,int bottom=0,BOOL bGetData=TRUE,int xiangshi=24,BOOL bFirst=TRUE);
	static BOOL FindBmp (HWND hWnd,CString Pic,LPPOINT PT,COLORREF color,int left=0 ,int top=0 ,int right=0 ,int bottom=0,BOOL bGetData=TRUE,int xiangshi=24);
	//寻找文字（句柄，文字，字体号，字间距，寻找范围左，上，右，下，忽略相同RGB值）
	static POINT FindBMPText(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,int left=0 ,int top=0,int right=0 ,int bottom=0,int Interval=0,int JiaCu=0,int xiangshi=24,int width=0,int IntervalE=0);
	//寻找文字个数
	static int GetTextNum(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,CArray<CPoint,CPoint&> &ptArray,int left=0 ,int top=0,int right=0 ,int bottom=0,int Interval=0,int JiaCu=0,int xiangshi=24,int width=0,int IntervalE=0);
	//寻找文字个数
	static int GetRoleNum(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,CArray<CPoint,CPoint&> &ptArray,int left=0 ,int top=0,int right=0 ,int bottom=0,int Interval=0,int JiaCu=0,int xiangshi=24,int width=0,int IntervalE=0);

	//黑白找字
	static POINT FindTextBlackWhite(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,int left=0 ,int top=0,int right=0 ,int bottom=0,BOOL bGetData=TRUE,int Interval=0,int JiaCu=0,int xiangshi=24,int width=0,int IntervalE=0);
	static POINT FindTextRole(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,int left=0 ,int top=0,int right=0 ,int bottom=0,int Interval=0,int JiaCu=0,int xiangshi=24,int IntervalE=0);
	static BOOL FindBMPText(HWND hWnd,CString ZiTi,CString Outtext,UINT size,COLORREF color,LPPOINT pt,UINT left=0 ,UINT top=0,UINT right=0 ,UINT bottom=0,BOOL bGetData=TRUE,int Interval=0,int JiaCu=0,int xiangshi=24);
	//前后不加空格
	static POINT FindBMPNumber(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,int left=0 ,int top=0,int right=0 ,int bottom=0,int Interval=0,int JiaCu=0,int xiangshi=24);
	//选角色(逐个字符查找)
	static bool SelectRoleEx(HWND m_hGameWnd,CString strScr);
	//找角色 参数（游戏句柄，字，字号，颜色，字间距，左，上，右，下，忽略值,字宽）
	static POINT FTextRole(HWND hWnd,CString Outtext,int size,COLORREF color,int Interval,int left=0 ,int top=0 ,int right=0 ,int bottom=0,int JiaCu=0,int xiangshi=8,CString ZiTi=_T("宋体"));
	//查看字符前面或者后面有没有相同的色素
	static bool CheckRoleClor(HWND m_hGameWnd,POINT pt,COLORREF colorRole,BOOL bFirst,int nLenth=1);
	//BOOL CheakPoint(HWND hWnd,pPointColor pPTC,int Num,int xiangshi =8,bool C_out=false);



	//根据pid和部分标题获取句柄
	static HWND GetHwndByPid(DWORD Pid,CString PartTitle="");
	//根据进程名和部分标题获取句柄
	static HWND GetHwndByPid(CString ProcessName);
	//根据进程名获取pid(不区分大小写)
	static DWORD GetProcessIdByName(CString in_processName,CString strOpenedGamePidLst=NULL);
	//根据部分标题,获取进程列表
	static BOOL GetOpenedPIDLst(CString strPartTitle, CString &strOpenedPidLst);
	//根据进程号获取进程名
	static CString GetProcessNameById(DWORD dwProcessId);
	//打印所有进程名
	static CString PrintAllProcess();
	//获取操作系统版本号
	static CString GetOSVersion(void);
	//获取内网IP地址
	static CString GetIPAddress(void);
	//获取外网IP地址
	static CString GetInternetIP(void);
	//获取IP所在地
	static CString GetIPLocation(void);
	//获取搜狗输入法路径
	static CString GetSougouPath(void);
	//获取鼠标所在位置的句柄
	static HWND WindowFromCursor();
	//根据句柄或进程名关闭窗口
	static BOOL KillWindow(HWND hwnd=NULL);
	static BOOL KillWindow(CString strProcessName);
	static BOOL IsProcessID(DWORD dwPid);
	// 生成指定文字的图片
	static void CreateBMP( CString strPic,CString Outtext);

public:
	// 禁止在任务管理器中结束进程,默认为本程序,不区分大小写,后缀可省略
	static void RefuseKillProcess(CString ProcessName=NULL);
	//读取订单明细
	//static bool ReadOrderDetail( CString strOrderData );
	//static CString m_strGameName;
	//static CString m_strAccount;
	//static CString m_strPassword;
	//static CString m_strArea;
	//static CString m_strServer;
	//static CString m_strGamePath;
	//static CString m_strGameStartFile;
	//static CString m_strPictureDir;
	//static CString m_strOPDN;
	//static CString m_strDeliverOpId;
	//static CString m_strRole;
	//static CString m_strAfter;
	//提取指定的两个字符串之间的内容
	static CString FindStr(CString str,CString str1,CString str2="\r\n",CString keyName1="",CString keyName2="");
	//查找第index个字符(strS),取direction(0左,1右)部分
	CString FindStr2(CString Str,CString StrS,int index,int direction=0);
	//游戏页面截图
	static void CaptureGamePicture(CString strPictureName,HWND hwnd=0);
	//区域截图
	static void CapturePictureInRect(HWND hwnd,CString strPictureName,int left,int top,int right,int bottom,BOOL bWaterMark=FALSE,COLORREF WaterMarkColor=0,int WaterMarkLeft=0,int WaterMarkTop=0,int WaterMarkRight=0,int WaterMarkBottom=0,
		int WaterMarkLeft2=0,int WaterMarkTop2=0,int WaterMarkRight2=0,int WaterMarkBottom2=0,
		int WaterMarkLeft3=0,int WaterMarkTop3=0,int WaterMarkRight3=0,int WaterMarkBottom3=0);
	//生成截图文件名
	static CString GetCaptureFileName();
	static void DeleteLog(LPCTSTR strPath, int nDays=30 );
	static void PressMouseKey(HWND hwnd,int x,int y,int nTimes=1,int nWaitTime=100);
	static void PressMouse( HWND hWnd , int x, int y,int nTimes=1 ,int nWaitTime=100);
	static void PressMouseKey( int type);
	//单击鼠标右键
	void PressRightKey( HWND hWnd , int x, int y, int nTimes=1);
	//发送字符串(限半角)
	static void SendKeys(CString input,int waitTime=100);
	//发送功能键,回车等,默认按一下
	static void SendFuncKey(char key,int n=1);
	//发送字符按键,默认按一下
	static void SendKeys(char ch,int n=1);
	void KMSendKey(char chrInput);
	//发送组合键,1ctrl,2alt
	static void Send2Keys(char FuncKey,char ch);
	//发送字符串(支持全角,需要获取子句柄)
	static void SendString(CString str);
	static void ActiveWindow(HWND hActiveWnd);
	//获取指定程序版本号,默认为本程序
	static CString GetProductVersion(CString name="");
	//分割字符串
	static int SplitString(CString strScr,CString strFG,CStringArray &strArray);
	//函数暂停,等待鼠标移动
	static void WaitMouseMove();
	// 给图片加水印
	static void WaterMark( CString strMark);
	// 生成指定文字的图片
	static int GetColorNumFormText( COLORREF color,CString Outtext);
	static BYTE *pData;
	//读取指定文件的内容
	static CString ReadFile ( CString strPathName );
	//修改INI文件的内容(参数个数)
	static BOOL WriteINIFile (int num, LPCTSTR fileName, ... );
	//创建文本文件
	static BOOL CreateTextFile (CString FileName,CString FileContent);
	//修改图片背景(参数为保留区域)
	static void ChangeBgColor(CString strPic,BYTE pixelS,BYTE pixelE);
	//获取网络内容,直接读取,不下载
	static CString GetURLContent(CString url);
	//可以格式化的messagebox
	static void ShowMessage (CString strFormat, ...);
	//读库存金币,安全时间等
	static CString GetNumberInBMP(COLORREF color,int left,int top,int right,int bottom,CString ziti="宋体",int size=12,int interval=0);
	//根据进程名或窗口标题获取路径
	static CString GetOtherProgPath(CString strTitle,CString strProcessName);
	//将字符串中的pt.x,pt.y替换成最近一次的找字或找图的坐标
	static BOOL PickXY(CString &strScr);
	//关闭输入法
	static BOOL SelectIME(LPCTSTR strID="00000804");
	//检查电脑中安装了哪些输入法
	static void CheckIME();
	//获取本程序版本号
	static CString GetFileVersion();
	//生成截图时间后缀及结尾
	static CString SetPicPath(CString str,CString OrderNo);
	static int getcolor(HWND hwnd,COLORREF color,int left,int top,int right,int bottom,int xiangshi=16);
	static POINT FindBmpBlackWhite (HWND hWnd,CString Pic,int left=0 ,int top=0 ,int right=0 ,int bottom=0,int xiangshi=1);

	static int GetBmpNum (HWND hWnd,CString Pic,CArray<CPoint,CPoint&> &ptArray,int left=0 ,int top=0 ,int right =0,int bottom=0,int xiangshi=24,BOOL bColor=FALSE,COLORREF color=0);
	static int GetBmpNum (HWND hWnd,CString Pic,int left=0 ,int top=0 ,int right =0,int bottom=0,int xiangshi=24);
	static void DragMouse(HWND hwnd,int ToX,int ToY,int FromX=-1,int FromY=-2,int nWaitTime=10);

	//选角色(逐个字符查找)
	static POINT FindTextBySingle(HWND hWnd,CString ZiTi,CString Outtext,UINT size,COLORREF color,UINT left=0 ,UINT top=0,UINT right=0 ,UINT bottom=0);
	//替换文件中的指定内容
	static void ReplaceFileText (LPCTSTR fileName,CString strReplace,CString strS,CString strE="\r\n");
	static void ClearFolder(CString strFolder);
	static int GetFileNum(CString strFolder);


	static BOOL DeleteFolder(LPCTSTR lpszPath);
	static void MoveMouse(HWND hwnd,int x,int y,int absx=3);
	static void SetDisplay();
	static LPCTSTR GetCoin(HWND hWnd,CString PicPath,int left ,int top ,int right ,int bottom,COLORREF color);
	//bmp图像转jpg
	static void BMPToJPG(CString strBMPFile,BOOL bDelete=TRUE);
};

