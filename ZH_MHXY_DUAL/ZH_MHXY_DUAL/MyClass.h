#pragma once


class CMyClass
{

public:

	// ��ȡ������·��
	static CString GetAppPath(void);
public:
	//��ʽ�������뷨ID
	static CString meishiID;
public:
	//�ѹ�ƴ�����뷨ID
	static CString sougouID;


public:
	// ��ȡwindows��¼�û���
	static CString GetWinUser(void);
	// ��ȡ�������
	static CString GetComputerName(void);
public:
	// ������Ŀ¼
	static BOOL CheckSubDir(CString szDir);
public:
	// ɾ��ָ�����ļ�
	static void DeleteFile(CString strPathName);
public:
	static void DeleteFile(CString strPath,CString strFileName);
public:
	// ��ȡ�ļ���С
	static int GetFileSize(CString strPathName);

public:
	// ��ȡ����������
	static CString GetClipboardText(HWND hwnd);
public:
	// ���ı����ݿ��������а�
	static void SetClipBoardText(HWND hwnd, CString strText);
	//��־
	static void WriteToFile(CString log,...);
	//��ʼ����־,Ĭ���ڳ��������ļ���
	static BOOL InitLogFile( LPCTSTR lpszFolder=NULL );
	static BOOL m_bGetData;

public:
	// ��ȡָ�������豸����(��ͼ)
	static BOOL GetDCData(HWND hWnd);
	// ��ȡָ�������豸����(��ͼ)32λ
	static BOOL GetDCData32(HWND hWnd);
	static POINT FindBmp (HWND hWnd,CString Pic,int left=0 ,int top=0 ,int right=0 ,int bottom=0,int xiangshi=24);
	static BOOL FindBmp (HWND hWnd,CString Pic,LPPOINT pt,int left=0 ,int top=0 ,int right=0 ,int bottom=0,BOOL bGetData=TRUE,int xiangshi=24,BOOL bFirst=TRUE);
	static BOOL FindBmp (HWND hWnd,CString Pic,LPPOINT PT,COLORREF color,int left=0 ,int top=0 ,int right=0 ,int bottom=0,BOOL bGetData=TRUE,int xiangshi=24);
	//Ѱ�����֣���������֣�����ţ��ּ�࣬Ѱ�ҷ�Χ���ϣ��ң��£�������ͬRGBֵ��
	static POINT FindBMPText(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,int left=0 ,int top=0,int right=0 ,int bottom=0,int Interval=0,int JiaCu=0,int xiangshi=24,int width=0,int IntervalE=0);
	//Ѱ�����ָ���
	static int GetTextNum(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,CArray<CPoint,CPoint&> &ptArray,int left=0 ,int top=0,int right=0 ,int bottom=0,int Interval=0,int JiaCu=0,int xiangshi=24,int width=0,int IntervalE=0);
	//Ѱ�����ָ���
	static int GetRoleNum(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,CArray<CPoint,CPoint&> &ptArray,int left=0 ,int top=0,int right=0 ,int bottom=0,int Interval=0,int JiaCu=0,int xiangshi=24,int width=0,int IntervalE=0);

	//�ڰ�����
	static POINT FindTextBlackWhite(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,int left=0 ,int top=0,int right=0 ,int bottom=0,BOOL bGetData=TRUE,int Interval=0,int JiaCu=0,int xiangshi=24,int width=0,int IntervalE=0);
	static POINT FindTextRole(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,int left=0 ,int top=0,int right=0 ,int bottom=0,int Interval=0,int JiaCu=0,int xiangshi=24,int IntervalE=0);
	static BOOL FindBMPText(HWND hWnd,CString ZiTi,CString Outtext,UINT size,COLORREF color,LPPOINT pt,UINT left=0 ,UINT top=0,UINT right=0 ,UINT bottom=0,BOOL bGetData=TRUE,int Interval=0,int JiaCu=0,int xiangshi=24);
	//ǰ�󲻼ӿո�
	static POINT FindBMPNumber(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,int left=0 ,int top=0,int right=0 ,int bottom=0,int Interval=0,int JiaCu=0,int xiangshi=24);
	//ѡ��ɫ(����ַ�����)
	static bool SelectRoleEx(HWND m_hGameWnd,CString strScr);
	//�ҽ�ɫ ��������Ϸ������֣��ֺţ���ɫ���ּ�࣬���ϣ��ң��£�����ֵ,�ֿ�
	static POINT FTextRole(HWND hWnd,CString Outtext,int size,COLORREF color,int Interval,int left=0 ,int top=0 ,int right=0 ,int bottom=0,int JiaCu=0,int xiangshi=8,CString ZiTi=_T("����"));
	//�鿴�ַ�ǰ����ߺ�����û����ͬ��ɫ��
	static bool CheckRoleClor(HWND m_hGameWnd,POINT pt,COLORREF colorRole,BOOL bFirst,int nLenth=1);
	//BOOL CheakPoint(HWND hWnd,pPointColor pPTC,int Num,int xiangshi =8,bool C_out=false);



	//����pid�Ͳ��ֱ����ȡ���
	static HWND GetHwndByPid(DWORD Pid,CString PartTitle="");
	//���ݽ������Ͳ��ֱ����ȡ���
	static HWND GetHwndByPid(CString ProcessName);
	//���ݽ�������ȡpid(�����ִ�Сд)
	static DWORD GetProcessIdByName(CString in_processName,CString strOpenedGamePidLst=NULL);
	//���ݲ��ֱ���,��ȡ�����б�
	static BOOL GetOpenedPIDLst(CString strPartTitle, CString &strOpenedPidLst);
	//���ݽ��̺Ż�ȡ������
	static CString GetProcessNameById(DWORD dwProcessId);
	//��ӡ���н�����
	static CString PrintAllProcess();
	//��ȡ����ϵͳ�汾��
	static CString GetOSVersion(void);
	//��ȡ����IP��ַ
	static CString GetIPAddress(void);
	//��ȡ����IP��ַ
	static CString GetInternetIP(void);
	//��ȡIP���ڵ�
	static CString GetIPLocation(void);
	//��ȡ�ѹ����뷨·��
	static CString GetSougouPath(void);
	//��ȡ�������λ�õľ��
	static HWND WindowFromCursor();
	//���ݾ����������رմ���
	static BOOL KillWindow(HWND hwnd=NULL);
	static BOOL KillWindow(CString strProcessName);
	static BOOL IsProcessID(DWORD dwPid);
	// ����ָ�����ֵ�ͼƬ
	static void CreateBMP( CString strPic,CString Outtext);

public:
	// ��ֹ������������н�������,Ĭ��Ϊ������,�����ִ�Сд,��׺��ʡ��
	static void RefuseKillProcess(CString ProcessName=NULL);
	//��ȡ������ϸ
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
	//��ȡָ���������ַ���֮�������
	static CString FindStr(CString str,CString str1,CString str2="\r\n",CString keyName1="",CString keyName2="");
	//���ҵ�index���ַ�(strS),ȡdirection(0��,1��)����
	CString FindStr2(CString Str,CString StrS,int index,int direction=0);
	//��Ϸҳ���ͼ
	static void CaptureGamePicture(CString strPictureName,HWND hwnd=0);
	//�����ͼ
	static void CapturePictureInRect(HWND hwnd,CString strPictureName,int left,int top,int right,int bottom,BOOL bWaterMark=FALSE,COLORREF WaterMarkColor=0,int WaterMarkLeft=0,int WaterMarkTop=0,int WaterMarkRight=0,int WaterMarkBottom=0,
		int WaterMarkLeft2=0,int WaterMarkTop2=0,int WaterMarkRight2=0,int WaterMarkBottom2=0,
		int WaterMarkLeft3=0,int WaterMarkTop3=0,int WaterMarkRight3=0,int WaterMarkBottom3=0);
	//���ɽ�ͼ�ļ���
	static CString GetCaptureFileName();
	static void DeleteLog(LPCTSTR strPath, int nDays=30 );
	static void PressMouseKey(HWND hwnd,int x,int y,int nTimes=1,int nWaitTime=100);
	static void PressMouse( HWND hWnd , int x, int y,int nTimes=1 ,int nWaitTime=100);
	static void PressMouseKey( int type);
	//��������Ҽ�
	void PressRightKey( HWND hWnd , int x, int y, int nTimes=1);
	//�����ַ���(�ް��)
	static void SendKeys(CString input,int waitTime=100);
	//���͹��ܼ�,�س���,Ĭ�ϰ�һ��
	static void SendFuncKey(char key,int n=1);
	//�����ַ�����,Ĭ�ϰ�һ��
	static void SendKeys(char ch,int n=1);
	void KMSendKey(char chrInput);
	//������ϼ�,1ctrl,2alt
	static void Send2Keys(char FuncKey,char ch);
	//�����ַ���(֧��ȫ��,��Ҫ��ȡ�Ӿ��)
	static void SendString(CString str);
	static void ActiveWindow(HWND hActiveWnd);
	//��ȡָ������汾��,Ĭ��Ϊ������
	static CString GetProductVersion(CString name="");
	//�ָ��ַ���
	static int SplitString(CString strScr,CString strFG,CStringArray &strArray);
	//������ͣ,�ȴ�����ƶ�
	static void WaitMouseMove();
	// ��ͼƬ��ˮӡ
	static void WaterMark( CString strMark);
	// ����ָ�����ֵ�ͼƬ
	static int GetColorNumFormText( COLORREF color,CString Outtext);
	static BYTE *pData;
	//��ȡָ���ļ�������
	static CString ReadFile ( CString strPathName );
	//�޸�INI�ļ�������(��������)
	static BOOL WriteINIFile (int num, LPCTSTR fileName, ... );
	//�����ı��ļ�
	static BOOL CreateTextFile (CString FileName,CString FileContent);
	//�޸�ͼƬ����(����Ϊ��������)
	static void ChangeBgColor(CString strPic,BYTE pixelS,BYTE pixelE);
	//��ȡ��������,ֱ�Ӷ�ȡ,������
	static CString GetURLContent(CString url);
	//���Ը�ʽ����messagebox
	static void ShowMessage (CString strFormat, ...);
	//�������,��ȫʱ���
	static CString GetNumberInBMP(COLORREF color,int left,int top,int right,int bottom,CString ziti="����",int size=12,int interval=0);
	//���ݽ������򴰿ڱ����ȡ·��
	static CString GetOtherProgPath(CString strTitle,CString strProcessName);
	//���ַ����е�pt.x,pt.y�滻�����һ�ε����ֻ���ͼ������
	static BOOL PickXY(CString &strScr);
	//�ر����뷨
	static BOOL SelectIME(LPCTSTR strID="00000804");
	//�������а�װ����Щ���뷨
	static void CheckIME();
	//��ȡ������汾��
	static CString GetFileVersion();
	//���ɽ�ͼʱ���׺����β
	static CString SetPicPath(CString str,CString OrderNo);
	static int getcolor(HWND hwnd,COLORREF color,int left,int top,int right,int bottom,int xiangshi=16);
	static POINT FindBmpBlackWhite (HWND hWnd,CString Pic,int left=0 ,int top=0 ,int right=0 ,int bottom=0,int xiangshi=1);

	static int GetBmpNum (HWND hWnd,CString Pic,CArray<CPoint,CPoint&> &ptArray,int left=0 ,int top=0 ,int right =0,int bottom=0,int xiangshi=24,BOOL bColor=FALSE,COLORREF color=0);
	static int GetBmpNum (HWND hWnd,CString Pic,int left=0 ,int top=0 ,int right =0,int bottom=0,int xiangshi=24);
	static void DragMouse(HWND hwnd,int ToX,int ToY,int FromX=-1,int FromY=-2,int nWaitTime=10);

	//ѡ��ɫ(����ַ�����)
	static POINT FindTextBySingle(HWND hWnd,CString ZiTi,CString Outtext,UINT size,COLORREF color,UINT left=0 ,UINT top=0,UINT right=0 ,UINT bottom=0);
	//�滻�ļ��е�ָ������
	static void ReplaceFileText (LPCTSTR fileName,CString strReplace,CString strS,CString strE="\r\n");
	static void ClearFolder(CString strFolder);
	static int GetFileNum(CString strFolder);


	static BOOL DeleteFolder(LPCTSTR lpszPath);
	static void MoveMouse(HWND hwnd,int x,int y,int absx=3);
	static void SetDisplay();
	static LPCTSTR GetCoin(HWND hWnd,CString PicPath,int left ,int top ,int right ,int bottom,COLORREF color);
	//bmpͼ��תjpg
	static void BMPToJPG(CString strBMPFile,BOOL bDelete=TRUE);
};

