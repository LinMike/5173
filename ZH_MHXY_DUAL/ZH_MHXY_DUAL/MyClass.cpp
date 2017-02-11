#include "StdAfx.h"
#include "MyClass.h"
#include "io.h"
#pragma comment(lib, "version.lib")
#define  ULONG_PTR  void*//BMPTOJPG
#include<gdiplus.h>   //BMPTOJPG
#include<GdiPlusEnums.h> //BMPTOJPG
#pragma   comment(lib, "imagehlp.lib")
#include <imagehlp.h>
#pragma comment(lib,"GdiPlus.lib")
#include "afxinet.h"
#include "atlimage.h"

#include <tlhelp32.h>
#include "Psapi.h"  //GetModuleFileNameEx
#pragma comment(lib,"Psapi.lib")

static FILE* m_fpLog;

using namespace Gdiplus; //ʹ��GDI+�������ռ�
using namespace std;


////��̬��Ա������ʼ��
////CString CMyClass::m_strOrderNo ="";
//CString CMyClass::m_strGameName ="";
//CString CMyClass::m_strAccount ="";
//CString CMyClass::m_strPassword ="";
//CString CMyClass::m_strArea ="";
//CString CMyClass::m_strServer ="";
//CString CMyClass::m_strGamePath ="";
//CString CMyClass::m_strGameStartFile ="";
//CString CMyClass::m_strPictureDir ="";
//CString CMyClass::m_strOPDN ="";
//CString CMyClass::m_strDeliverOpId ="";
//CString CMyClass::m_strRole ="";
//CString CMyClass::m_strAfter ="";
BOOL CMyClass::m_bGetData=TRUE;
CString CMyClass::meishiID="";
CString CMyClass::sougouID="";

//��Ļ�ֱ���
const int CYSCREEN=GetSystemMetrics (SM_CYSCREEN);
const int CXSCREEN=GetSystemMetrics( SM_CXSCREEN);
BYTE *CMyClass::pData = new BYTE[CXSCREEN * CYSCREEN * 4];


class CStringIterator
{
public:
	CStringIterator(const char *szBuf) : str(szBuf) { pstr = str.GetBuffer((int)strlen(szBuf)); }
	~CStringIterator() { str.ReleaseBuffer(); }

	BOOL HasNext()
	{
		return *pstr != '\0';
	}

	wchar_t Next()
	{
		if ( (unsigned char)(*pstr) > 0x80 )
		{
			char cs[3] = {0};
			sprintf(cs, "%c%c", *(pstr+1), *pstr);
			pstr += 2;
			return *((wchar_t*)&cs);
		}
		else
		{
			return *pstr++;
		}
	}

private:
	CString str;
	const char *pstr;
};

// ��ȡ����·��
CString CMyClass::GetAppPath(void)
{
	TCHAR appPath[MAX_PATH];
	GetModuleFileName(NULL,appPath,MAX_PATH);//��ȡһ����װ��ģ�������·������

	CString strProgPath;
	strProgPath.Format("%s",appPath);
	int index = strProgPath.ReverseFind('\\'); 

	strProgPath = strProgPath.Mid(0,index);
	return strProgPath;

	//m_strFileName = m_strProgPath + "\\RC2.dat";
	//m_strProgVer	= GetVersion();

}

// ��ȡwindows��¼�û���
CString CMyClass::GetWinUser(void)
{
	CString strUserName;
	DWORD	dwSize=MAX_PATH;

	if ( !::GetUserName( strUserName.GetBuffer( MAX_PATH ), &dwSize ) )
	{
		strUserName.ReleaseBuffer();
		GetUserName( strUserName.GetBuffer( MAX_PATH ), &dwSize );
	}
	strUserName.ReleaseBuffer();
	//strUserName.MakeUpper();
	return strUserName;
}
// ��ȡ�������
CString CMyClass::GetComputerName(void)
{
	CString strComputerName;
	DWORD	dwSize=MAX_PATH;

	if ( !::GetComputerName( strComputerName.GetBuffer( MAX_PATH ), &dwSize ) )
	{
		strComputerName.ReleaseBuffer();
		::GetComputerName( strComputerName.GetBuffer( MAX_PATH ), &dwSize );
	}
	strComputerName.ReleaseBuffer();
	//strUserName.MakeUpper();
	return strComputerName;
}

// ������Ŀ¼
BOOL CMyClass::CheckSubDir(CString szDir)
{
	if (GetAppPath()==szDir)
		return TRUE;
	//SetCurrentDirectory(GetAppPath());
	CStringArray temp;
	int num=SplitString(szDir,"\\",temp);
	char szDirName[MAX_PATH];
	memset(szDirName,0,MAX_PATH);
	sprintf_s(szDirName,"%s\\*.*",szDir);


	if(temp.GetAt(0).IsEmpty()||temp.GetAt(1).IsEmpty())
		return FALSE;
	CString path=temp.GetAt(0);
	CFileFind fileFind;
	BOOL bResult;
	for(int i=1;i<num;i++)
	{
		path+="\\"+temp.GetAt(i);
		bResult = fileFind.FindFile(path);
		if(!bResult)
			CreateDirectory(path,NULL);
	}
	bResult = fileFind.FindFile(szDir);
	if(!bResult)
		return FALSE;

	return TRUE;
}

// ɾ��ָ�����ļ�
void CMyClass::DeleteFile(CString strPathName)
{
	int index = strPathName.ReverseFind('\\');
	if(index==-1)
		return;
	CString strFileName;
	strFileName = strPathName.Mid(index+1);
	DeleteFile(strPathName.Mid(0,index),strFileName);
}

void CMyClass::DeleteFile(CString strPath,CString strFileName)
{
	SetCurrentDirectory(strPath);
	BOOL bRet = ::DeleteFile((LPCTSTR)strFileName);	
}

// ��ȡ�ļ���С
int CMyClass::GetFileSize(CString strPathName)
{
	CFile file;
	BOOL bSuccess = file.Open(strPathName, CFile::modeRead | CFile::shareExclusive,NULL);
	if(!bSuccess)
		return -1;
	int fileSize = file.GetLength();
	file.Close(); 
	return fileSize;
}



// ��ȡ����������
CString CMyClass::GetClipboardText(HWND hwnd)
{
	CString strClipboard;
	if(OpenClipboard( hwnd ))
	{
		HANDLE hData = GetClipboardData( CF_TEXT );
		char *buffer = (char *)GlobalLock(hData);
		strClipboard = buffer;
		GlobalUnlock(hData);
		CloseClipboard();
		return strClipboard;
	}
	return "";
}

// ���ı����ݿ��������а�
void CMyClass::SetClipBoardText(HWND hwnd, CString strText)
{
	//BOOL bRet = FALSE;
	//bRet = OpenClipboard ( hwnd );
	//if( bRet )
	//{		
	//	HGLOBAL clipbuffer;
	//	char *buffer;
	//	bRet = EmptyClipboard();
	//	clipbuffer = GlobalAlloc(GMEM_DDESHARE,strText.GetLength()+1);
	//	buffer = (char *)GlobalLock(clipbuffer);
	//	strcpy_s(buffer,strText.GetLength(),LPCSTR(strText));
	//	GlobalUnlock(clipbuffer);
	//	HANDLE handle = SetClipboardData( CF_TEXT,clipbuffer );
	//	CloseClipboard();
	//}
	BOOL bRet = FALSE;
	bRet = OpenClipboard ( hwnd );
	if( bRet )
	{		
		HGLOBAL clipbuffer;
		char *buffer;
		bRet = EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE,strText.GetLength()+1);
		buffer = (char *)GlobalLock(clipbuffer);
		strcpy(buffer,LPCSTR(strText));
		GlobalUnlock(clipbuffer);
		HANDLE handle = SetClipboardData( CF_TEXT,clipbuffer );
		CloseClipboard();
	}
}
//��ʼ����־(����Ϊ���ļ���)
BOOL CMyClass::InitLogFile( LPCTSTR lpszFolder )
{
	CString strFolder;
	if(lpszFolder!="" && lpszFolder!=NULL)
	{
		CString strFileName = GetAppPath() + _T("\\") + lpszFolder;
		strFolder = strFileName.Left(strFileName.ReverseFind('\\'));
		if (!CheckSubDir(strFolder.GetBuffer()))
			return FALSE;
		strFileName.ReleaseBuffer();
	}
	else
		strFolder=GetAppPath();

	//////////��������������־�ļ�///////////////
	SYSTEMTIME ct;
	GetLocalTime(&ct);
	CString date;
	date.Format("%02d-%02d",ct.wMonth,ct.wDay);
	////////////////////////////////////////////

	CString strLogFile = strFolder + _T("\\RC2YLZT-") + date + ".log";

	if (fopen_s(&m_fpLog,strLogFile,"a+")!=0)
		return FALSE;
	WriteToFile("---------------------------------------------------\r\n");
	return TRUE;
}

//д��־
void CMyClass::WriteToFile(CString log,...)
{
	//CFile m_fpLog;
	//if(  m_fpLog == NULL)
	//{
	//	//////////��������������־�ļ�///////////////
	//	SYSTEMTIME ct;
	//	GetLocalTime(&ct);
	//	CString date;
	//	date.Format("%02d-%02d",ct.wMonth,ct.wDay);
	//	////////////////////////////////////////////

	//	CString strLogFile = GetAppPath() + _T("\\") + date + ".log";
	//	//m_fpLog = fopen(strLogFilePath, "a+");
	//	fopen_s(&m_fpLog,strLogFile,"a+");
	//	//m_fpLog.Open(strLogFile,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite);
	//}
	if(  m_fpLog != NULL)
	{
		CString strLog = _T("");
		CTime curTime = CTime::GetCurrentTime();
		strLog.Format("%s ", curTime.Format(_T("%Y-%m-%d %H:%M:%S")));


		CString strLogFormat,strArgValue;
		strLogFormat.Format( _T("%s %s"), strLog, log );

		va_list vl;
		va_start(vl, log);
		vfprintf( m_fpLog ,strLogFormat, vl );
		va_end(vl);

		//fprintf(m_fpLog, strLog);
		fflush(m_fpLog);
	}

}
// ��ȡָ�������豸����24λ(��ͼ)
BOOL CMyClass::GetDCData(HWND hWnd)
{
	//����ͼƬ�Ǵ����Ͻǿ�ʼ
	//����Ļ��ͼ�Ǵ����½ǿ�ʼ
	if(!IsWindowVisible(hWnd))
		return FALSE;
	ZeroMemory(pData,CXSCREEN*CYSCREEN*4);//���
	CRect rect;
	::GetWindowRect( hWnd, &rect );


	int w = rect.Width();
	int h = rect.Height();

	if(w<=0 || h<=0)
		return FALSE;

	HDC hdc			= ::GetWindowDC(hWnd);
	HDC hdcCompatible=CreateCompatibleDC(hdc);//����һ����ָ���豸���ݵ��ڴ��豸�����Ļ���
	HBITMAP hbmp = CreateCompatibleBitmap(hdc,w,h);//������ָ�����豸������ص��豸���ݵ�λͼ
	SelectObject(hdcCompatible,hbmp);//��λͼѡ���ڴ�


	// ��GetDIBits�õĽṹ�� 
	BITMAPINFO infoHeader;
	memset(&infoHeader.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	infoHeader.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	infoHeader.bmiHeader.biWidth = w;
	infoHeader.bmiHeader.biHeight = h;
	infoHeader.bmiHeader.biPlanes = 1;
	infoHeader.bmiHeader.biBitCount = 24; // ���ｨ����32�������������ҵĻ����ϱ�24�ٶȿ�Ƚ϶�
	infoHeader.bmiHeader.biCompression = BI_RGB;


	// �Ѵ���(hdc)�ϵ����ݱ��ݵ�hdcCompatible�� ����HBITMAP	hbmp��hdcCompatible����
	// �൱��hbmp��浽�����ݣ�����ֻҪ��hbm��Ҫ������,�����ʹ�� CAPTUREBLT ��դ������������������͸����<255�Ĵ���
	::BitBlt(hdcCompatible, 0, 0, w, h, hdc, 0, 0, SRCCOPY | CAPTUREBLT);
	// ��hbmpҪ�����ݣ�����pData��
	::GetDIBits(hdcCompatible, hbmp, 0, h, pData, &infoHeader, DIB_RGB_COLORS);

	::DeleteObject(hbmp);	
	::DeleteDC(hdcCompatible);
	::ReleaseDC(hWnd, hdc);

	return TRUE;
}
// ��ȡָ�������豸����32λ(��ͼ)
BOOL CMyClass::GetDCData32(HWND hWnd)
{
	ZeroMemory(pData,CXSCREEN*CYSCREEN*4);//���
	CRect rect;
	::GetWindowRect( hWnd, &rect );

	int w = rect.Width();
	int h = rect.Height();

	HDC hdc			= ::GetWindowDC(hWnd);
	HDC hdcCompatible=CreateCompatibleDC(hdc);//����һ����ָ���豸���ݵ��ڴ��豸�����Ļ���
	HBITMAP hbmp = CreateCompatibleBitmap(hdc,w,h);//������ָ�����豸������ص��豸���ݵ�λͼ
	SelectObject(hdcCompatible,hbmp);//��λͼѡ���ڴ�


	// ��GetDIBits�õĽṹ�� 
	BITMAPINFO infoHeader;
	memset(&infoHeader.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	infoHeader.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	infoHeader.bmiHeader.biWidth = w;
	infoHeader.bmiHeader.biHeight = h;
	infoHeader.bmiHeader.biPlanes = 1;
	infoHeader.bmiHeader.biBitCount = 32; // ���ｨ����32�������������ҵĻ����ϱ�24�ٶȿ�Ƚ϶�
	infoHeader.bmiHeader.biCompression = BI_RGB;


	// �Ѵ���(hdc)�ϵ����ݱ��ݵ�hdcCompatible�� ����HBITMAP	hbmp��hdcCompatible����
	// �൱��hbmp��浽�����ݣ�����ֻҪ��hbm��Ҫ������,�����ʹ�� CAPTUREBLT ��դ������������������͸����<255�Ĵ���
	::BitBlt(hdcCompatible, 0, 0, w, h, hdc, 0, 0, SRCCOPY | CAPTUREBLT);
	// ��hbmpҪ�����ݣ�����pData��
	::GetDIBits(hdcCompatible, hbmp, 0, h, pData, &infoHeader, DIB_RGB_COLORS);

	::DeleteObject(hbmp);	
	::DeleteDC(hdcCompatible);
	::ReleaseDC(hWnd, hdc);

	return TRUE;
}
void CMyClass::CreateBMP( CString strPic,CString Outtext)
{

	int size=100,Interval=0,width=0,JiaCu=0,IntervalE=0;
	COLORREF color=RGB(255,0,0);
	POINT PT;
	PT.x=-1;
	PT.y=-2;
	if (Outtext=="")
	{
		return ;
	}

	//����λͼ����
	HDC hdc,hdcMem;//�豸
	HFONT ftUser;//�������;��
	HBITMAP hBitMap = NULL;//λͼҪ���豸��������Ҫת���ɴ�����
	CBitmap *pBitMap = NULL;//λͼ
	CDC *pMemDC = NULL; /*HDC����ı���ָ��һ���ڴ棬���   
						�ڴ���������һ���豸����ص����ݣ�����Ҳ����   
						��ΪHDC�������һ��ָ�룻��CDC�ඨ��һ������   
						�������ӵ��HDC�����һ���豸������ͬʱҲ��   
						����HDC��صĲ����ĺ����� */  


	BYTE *tData;
	//����ͼ�δ�С
	//if (Interval == 0)
	//	Interval=size;
	int iWidth = (Outtext.GetLength() * ( size/2+Interval )+3) / 4  * 4;
	int iHeight =size;
	int iPixel = 32;
	//ͼ�θ�ʽ����
	LPBITMAPINFO lpbmih = new BITMAPINFO;//ͼ�θ�ʽ����
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = iWidth;//
	lpbmih->bmiHeader.biHeight = iHeight;
	lpbmih->bmiHeader.biPlanes = 1;//Ŀ���豸λ����������Ϊ1
	lpbmih->bmiHeader.biBitCount = iPixel;//��ɫ���
	lpbmih->bmiHeader.biCompression = BI_RGB;//λͼѹ������
	lpbmih->bmiHeader.biSizeImage = 0;//��С���ֽڵ�λ
	lpbmih->bmiHeader.biXPelsPerMeter = 0;//ˮƽ�ֱ���
	lpbmih->bmiHeader.biYPelsPerMeter = 0;//��ֱ�ֱ���
	lpbmih->bmiHeader.biClrUsed = 0;//λͼʵ��ʹ����ɫ������ɫ��
	lpbmih->bmiHeader.biClrImportant = 0;//λͼ��ʾ�бȽ���Ҫ����ɫ��


	hdc = CreateIC(TEXT("DISPLAY"),NULL,NULL,NULL);//���԰����ķ���ֵ���� HDC, ��������HDC�� ��ָ���Զ������в�ѯ������
	hdcMem = CreateCompatibleDC(hdc);//�ڴ�dc��
	hBitMap = CreateDIBSection(hdcMem,lpbmih,DIB_PAL_COLORS,(void **)&tData,NULL,0);//����һ��DIB
	if (color==0)
		memset(tData,255,iHeight * iWidth * iPixel/8);
	pBitMap = new CBitmap;
	pBitMap->Attach(hBitMap);//��λͼ����
	pMemDC = new CDC;
	pMemDC->Attach(hdcMem);//��DC�ڴ����
	pMemDC->SelectObject(pBitMap);//ѡ���ڴ��ַ

	pMemDC->SetBkMode(TRANSPARENT);//����ģʽ
	pMemDC->SetTextColor( color);//������ɫ
	ftUser = CreateFont(size,//�����С
		width,//�ֿ�
		0,//����ָ��ת��ʸ����escapement vector������ʾ�����x��ļнǣ���0.1Ϊ��λ��
		00,//����ָ���ַ����ߺ�x��ļнǣ���0.1Ϊ��λ����
		//FW_DONTCARE,//������
		JiaCu,
		FALSE, //����6��������bItalic����ָ�������Ƿ�Ϊб��Ĳ���ֵ
		FALSE,//��7��������bUnderline����ָ�������Ƿ�����»��ߵĲ���ֵ
		FALSE,//��8��������cStrickout����ָ���ַ��Ƿ����ɾ���ߵĲ���ֵ
		GB2312_CHARSET,//ָ����������弯
		OUT_DEFAULT_PRECIS,//����ָ������������ȷ��
		CLIP_DEFAULT_PRECIS,//������ȷ��
		DEFAULT_QUALITY,//�������������
		DEFAULT_PITCH|FF_MODERN,//�ַ������ַ�
		"����"//�������������
		); 
	(HFONT) SelectObject(hdcMem, ftUser);//ѡ����ʽ

	//��ͼƬ���������
	CRect rc;//�ͻ���
	rc.right=iWidth;
	rc.bottom=iHeight;
	rc.left =0;
	rc.top  =0;
	CString s;
	char* ss= Outtext.GetBuffer(Outtext.GetLength());
	int t=0;
	while ( t < Outtext.GetLength())
	{
		if(ss[t] <0)     //�Ǻ��� 
		{
			pMemDC->DrawText( Outtext.Mid(t,2), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			if (width== 0)
				rc.left=rc.left+size+Interval; 
			else
				rc.left=rc.left + width *2 + Interval;
			t++; 
		} 
		else 
		{
			pMemDC->DrawText( Outtext.Mid(t,1), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			if (width== 0)
				rc.left=rc.left+ size / 2 +IntervalE; 
			else
				rc.left=rc.left + width *2 + IntervalE;
		} 

		t++;

	}
	//pMemDC->DrawText( Outtext, &rc, DT_SINGLELINE|DT_LEFT );//����ı� 

	pMemDC->SelectObject(pBitMap);

	//AfxMessageBox("ͼƬ���ɳɹ���");
	/////////////////////////////////////�����ַ�ͼƬ
	BITMAPFILEHEADER bmfh;
	*((char *)&bmfh.bfType) = 'B';
	*(((char *)&bmfh.bfType) + 1) = 'M';//������BMP�ļ�
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//ƫ��λ��
	bmfh.bfSize = bmfh.bfOffBits + (iWidth * iHeight) * iPixel / 8;//�ļ���С
	//TCHAR szBMPFileName[128];//�ļ�����
	int iBMPBytes = iWidth * iHeight * iPixel / 8;//�����ļ���С
	CFile file;
	if(file.Open(strPic,CFile::modeWrite | CFile::modeCreate))
	{
		file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
		file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
		file.Write(tData,iBMPBytes);
		file.Close();
	}
	else
	{
		//AfxMessageBox("�޷����ɣ���ȷ������·���Ƿ���ȷ");
		return ;
	}
	delete pMemDC; pMemDC = NULL;
	delete pBitMap; pBitMap = NULL;
}
POINT CMyClass::FindBmp (HWND hWnd,CString Pic,int left ,int top ,int right ,int bottom,int xiangshi)
{
	POINT PT;
	PT.x=-1;
	PT.y=-2;
	if(Pic.Right(4).CompareNoCase(".bmp")!=0)
		Pic+=".bmp";
	if (_access(Pic,0)==-1 || !IsWindow(hWnd))
		return PT;
	if (!IsWindowVisible(hWnd) || ::IsIconic(hWnd))
		return PT;



	BYTE r, g, b, now_r, now_g, now_b;

	CRect rect;
	::GetWindowRect( hWnd, &rect );

	if (left<=0)
		left=0;
	if (top<=0)
		top=0;
	if ((right<=0) || (right>rect.Width()))
		right=rect.Width();

	if ((bottom<=0) ||(bottom>rect.Height()))
		bottom=rect.Height();

	if (!GetDCData(hWnd))
		return PT;

	//COLORREF color;
	//�Ա�ͼƬװ������
	::HBITMAP PicH=  (HBITMAP) LoadImage(NULL,Pic,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	BITMAP bm; 
	::GetObject(PicH,sizeof(BITMAP),&bm); 
	BYTE *tData;
	//ȡ�������飬�����Ϊ������ÿ��β�Զ���1�ֽڲ��㣬��Ҫȡ�����
	tData=new BYTE[ (bm.bmWidth * (bm.bmBitsPixel/8) + 3)/4*4* bm.bmHeight];
	long Ret1=GetBitmapBits (PicH,bm.bmWidth * bm.bmHeight * (bm.bmBitsPixel/8),&tData[0]);
	BOOL fFalse;
	//ѭ���Ƚ�
	for (int y=top; y<bottom-bm.bmHeight-1; y++)
	{
		for (int x=left; x<right-bm.bmWidth-1; x++)
		{

			fFalse=TRUE;
			for (int i=1;i<bm.bmHeight-1 ;i++)
			{
				if (!fFalse)
					break;
				for (int j=1;j<bm.bmWidth-1;j++)
				{
					if (!fFalse)
						break;
					//ȡ�������飬�����Ϊ������ÿ��β�Զ���1�ֽڲ��㣬��Ҫȡ�����
					/*b=tData[(i*bm.bmWidth +j)*3];
					g=tData[(i*bm.bmWidth +j)*3+1];
					r=tData[(i*bm.bmWidth +j)*3+2];*/
					b=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3];
					g=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+1];
					r=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+2];

					now_r = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +2];
					now_g = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +1];
					now_b = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +0];


					if (((abs(now_r - r)<xiangshi)&&(abs(now_g - g)<xiangshi)&&(abs(now_b -b)<xiangshi)) /* ||  ((b==tData[0]) && (g==tData[1])&&( r==tData[2]))*/  )
						fFalse=TRUE;
					else
						fFalse=FALSE;
				}

			}
			if (fFalse==TRUE) 
			{

				PT.x = x;
				PT.y = y;

				delete []tData;
				::DeleteObject (PicH);
				return PT;
			}
		}
	}

	delete []tData;
	::DeleteObject (PicH);
	return PT;
}

BOOL CMyClass::FindBmp (HWND hWnd,CString Pic,LPPOINT PT,int left ,int top ,int right ,int bottom,BOOL bGetData,int xiangshi,BOOL bFirst)
{

	PT->x=-1;
	PT->y=-2;
	if(Pic.Right(4).CompareNoCase(".bmp")!=0)
		Pic+=".bmp";
	if (_access(Pic,0)==-1 || !IsWindow(hWnd))
		return FALSE;
	if (!IsWindowVisible(hWnd) || ::IsIconic(hWnd))
		return FALSE;



	BYTE r, g, b, now_r, now_g, now_b;

	CRect rect;
	::GetWindowRect( hWnd, &rect );

	if (left<=0)
		left=0;
	if (top<=0)
		top=0;
	if ((right<=0) || (right>rect.Width()))
		right=rect.Width();

	if ((bottom<=0) ||(bottom>rect.Height()))
		bottom=rect.Height();

	if(bGetData)
	{
		if (!GetDCData(hWnd))
			return FALSE;
	}

	//COLORREF color;
	//�Ա�ͼƬװ������
	::HBITMAP PicH=  (HBITMAP) LoadImage(NULL,Pic,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	BITMAP bm; 
	::GetObject(PicH,sizeof(BITMAP),&bm); 
	BYTE *tData;
	//ȡ�������飬�����Ϊ������ÿ��β�Զ���1�ֽڲ��㣬��Ҫȡ�����
	tData=new BYTE[ (bm.bmWidth * (bm.bmBitsPixel/8) + 3)/4*4* bm.bmHeight];
	long Ret1=GetBitmapBits (PicH,bm.bmWidth * bm.bmHeight * (bm.bmBitsPixel/8),&tData[0]);
	BOOL fFalse;
	//ѭ���Ƚ�
	for (int y=top; y<bottom-bm.bmHeight-1; y++)
	{
		for (int x=left; x<right-bm.bmWidth; x++)
		{

			fFalse=TRUE;
			for (int i=1;i<bm.bmHeight-1 ;i++)
			{
				if (!fFalse)
					break;
				for (int j=1;j<bm.bmWidth-1;j++)
				{
					if (!fFalse)
						break;
					//ȡ�������飬�����Ϊ������ÿ��β�Զ���1�ֽڲ��㣬��Ҫȡ�����
					/*b=tData[(i*bm.bmWidth +j)*3];
					g=tData[(i*bm.bmWidth +j)*3+1];
					r=tData[(i*bm.bmWidth +j)*3+2];*/
					/*b=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3];
					g=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+1];
					r=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+2];*/
					//int nn=i* ((bm.bmWidth *(bm.bmBitsPixel/8)/*+3*/)/4*4)+ j*(bm.bmBitsPixel/8);
					//int mm=i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3;

					if(bm.bmBitsPixel==32)
					{
						b=tData[ i* ((bm.bmWidth *(bm.bmBitsPixel/8)+3)/4*4)+ j*(bm.bmBitsPixel/8)];
						g=tData[ i* ((bm.bmWidth *(bm.bmBitsPixel/8)+3)/4*4)+ j*(bm.bmBitsPixel/8)+1];
						r=tData[ i* ((bm.bmWidth *(bm.bmBitsPixel/8)+3)/4*4)+ j*(bm.bmBitsPixel/8)+2];
					}
					else
					{
						b=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3];
						g=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+1];
						r=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+2];
					}

					now_r = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +2];
					now_g = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +1];
					now_b = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +0];

					if(bFirst)
					{
						if (((abs(now_r - r)<xiangshi)&&(abs(now_g - g)<xiangshi)&&(abs(now_b -b)<xiangshi))  ||  ((b==tData[0]) && (g==tData[1])&&( r==tData[2]))  )
							fFalse=TRUE;
						else
							fFalse=FALSE;
					}
					else
					{
						if (((abs(now_r - r)<xiangshi)&&(abs(now_g - g)<xiangshi)&&(abs(now_b -b)<xiangshi)) )
							fFalse=TRUE;
						else
							fFalse=FALSE;
					}
				}

			}
			if (fFalse==TRUE) 
			{

				PT->x = x;
				PT->y = y;

				delete []tData;
				::DeleteObject (PicH);
				return TRUE;
			}
		}
	}

	PT->x=-1;
	PT->y=-2;

	delete []tData;
	::DeleteObject (PicH);
	return FALSE;
}
BOOL CMyClass::FindBmp (HWND hWnd,CString Pic,LPPOINT PT,COLORREF color,int left ,int top ,int right ,int bottom,BOOL bGetData,int xiangshi)
{

	PT->x=-1;
	PT->y=-2;
	if(Pic.Right(4).CompareNoCase(".bmp")!=0)
		Pic+=".bmp";
	if (_access(Pic,0)==-1 || !IsWindow(hWnd))
		return FALSE;
	if (!IsWindowVisible(hWnd) || ::IsIconic(hWnd))
		return FALSE;



	BYTE r, g, b, now_r, now_g, now_b;

	CRect rect;
	::GetWindowRect( hWnd, &rect );

	if (left<=0)
		left=0;
	if (top<=0)
		top=0;
	if ((right<=0) || (right>rect.Width()))
		right=rect.Width();

	if ((bottom<=0) ||(bottom>rect.Height()))
		bottom=rect.Height();

	if(bGetData)
	{
		if (!GetDCData(hWnd))
			return FALSE;
	}

	//COLORREF color;
	//�Ա�ͼƬװ������
	::HBITMAP PicH=  (HBITMAP) LoadImage(NULL,Pic,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	BITMAP bm; 
	::GetObject(PicH,sizeof(BITMAP),&bm); 
	BYTE *tData;
	//ȡ�������飬�����Ϊ������ÿ��β�Զ���1�ֽڲ��㣬��Ҫȡ�����
	tData=new BYTE[ (bm.bmWidth * (bm.bmBitsPixel/8) + 3)/4*4* bm.bmHeight];
	long Ret1=GetBitmapBits (PicH,bm.bmWidth * bm.bmHeight * (bm.bmBitsPixel/8),&tData[0]);
	BOOL fFalse;
	//ѭ���Ƚ�
	for (int y=top; y<bottom-bm.bmHeight-1; y++)
	{
		for (int x=left; x<right-bm.bmWidth-1; x++)
		{

			fFalse=TRUE;
			for (int i=1;i<bm.bmHeight-1 ;i++)
			{
				if (!fFalse)
					break;
				for (int j=1;j<bm.bmWidth-1;j++)
				{
					if (!fFalse)
						break;
					//ȡ�������飬�����Ϊ������ÿ��β�Զ���1�ֽڲ��㣬��Ҫȡ�����
					/*b=tData[(i*bm.bmWidth +j)*3];
					g=tData[(i*bm.bmWidth +j)*3+1];
					r=tData[(i*bm.bmWidth +j)*3+2];*/
					/*b=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3];
					g=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+1];
					r=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+2];*/
					//int nn=i* ((bm.bmWidth *(bm.bmBitsPixel/8)/*+3*/)/4*4)+ j*(bm.bmBitsPixel/8);
					//int mm=i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3;

					if(bm.bmBitsPixel==32)
					{
						b=tData[ i* ((bm.bmWidth *(bm.bmBitsPixel/8)+3)/4*4)+ j*(bm.bmBitsPixel/8)];
						g=tData[ i* ((bm.bmWidth *(bm.bmBitsPixel/8)+3)/4*4)+ j*(bm.bmBitsPixel/8)+1];
						r=tData[ i* ((bm.bmWidth *(bm.bmBitsPixel/8)+3)/4*4)+ j*(bm.bmBitsPixel/8)+2];
					}
					else
					{
						b=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3];
						g=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+1];
						r=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+2];
					}

					now_r = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +2];
					now_g = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +1];
					now_b = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +0];


					if(r==GetRValue(color) && g==GetGValue(color) && b==GetBValue(color))
					{
						if (((abs(now_r - r)<xiangshi)&&(abs(now_g - g)<xiangshi)&&(abs(now_b -b)<xiangshi)) )
							fFalse=TRUE;
						else
							fFalse=FALSE;
					}
					else
					{
						if (((abs(now_r - GetRValue(color))<xiangshi)&&(abs(now_g - GetGValue(color))<xiangshi)&&(abs(now_b -GetBValue(color))<xiangshi)) )
							fFalse=FALSE;
						else
							fFalse=TRUE;
					}

				}

			}
			if (fFalse==TRUE) 
			{

				PT->x = x;
				PT->y = y;

				delete []tData;
				::DeleteObject (PicH);
				return TRUE;
			}
		}
	}
	PT->x=-1;
	PT->y=-2;
	delete []tData;
	::DeleteObject (PicH);
	return FALSE;
}





//���ݽ�������ȡ����֮ǰpid�б��е�pid(�����ִ�Сд)
DWORD CMyClass::GetProcessIdByName(CString in_processName,CString strOpenedGamePidLst)   
{    
	PROCESSENTRY32 processInfo;   
	processInfo.dwSize = sizeof(processInfo);

	CString strProcessId = _T("");
	CString strExeName;

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);   
	if (processesSnapshot == INVALID_HANDLE_VALUE)   
	{   
		return 0;   
	}   

	//Process First in snapshot and traverse the list.   
	Process32First(processesSnapshot, &processInfo);  
	strExeName.Format("%s",processInfo.szExeFile);
	if (in_processName.MakeLower()==strExeName.MakeLower())   
	{   
		//Process exists.   
		CloseHandle(processesSnapshot);   
		return processInfo.th32ProcessID;   
	}   

	while ( Process32Next(processesSnapshot, &processInfo) )   
	{   
		strExeName.Format("%s",processInfo.szExeFile);
		if (in_processName.MakeLower()==strExeName.MakeLower())     
		{
			strProcessId.Format(_T("%d"), processInfo.th32ProcessID);
			if(strOpenedGamePidLst.Find(strProcessId) == -1)	//����֮ǰ�򿪵���ϷPID�б���
			{
				CloseHandle(processesSnapshot);   
				return processInfo.th32ProcessID;   
			}
		}   
	}   

	CloseHandle(processesSnapshot);   
	return 0;   
}
//���ݽ��̺Ż�ȡ������
CString CMyClass::GetProcessNameById(DWORD dwProcessId)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot error\n");
		return 0;
	}

	PROCESSENTRY32 pe;
	CString ProcessName;


	pe.dwSize = sizeof(pe);

	if (!Process32First(hSnapshot, &pe))
	{
		CloseHandle(hSnapshot);
		return FALSE;
	}

	do 
	{
		if (pe.th32ProcessID == dwProcessId)
		{
			ProcessName.Format("%s",pe.szExeFile);
			CloseHandle(hSnapshot);
			return ProcessName;
		}
	} while(Process32Next(hSnapshot, &pe));

	CloseHandle(hSnapshot);
	return NULL;
}
//���ݲ��ֱ���,��ȡ�����б�
BOOL CMyClass::GetOpenedPIDLst(CString strPartTitle, CString &strOpenedPidLst)
{
	if ( strPartTitle.IsEmpty() )
		return FALSE;

	HWND hWnd = NULL;
	DWORD dwProcessIDTmp = 0;
	CString strRet = _T("");
	CString strProcessID = _T("");
	TCHAR szTitle[MAX_PATH] = {0};
	CString strTitle = _T("");

	strOpenedPidLst = _T("");

	hWnd = GetTopWindow(NULL);	
	if (!hWnd)
		return FALSE;

	hWnd = GetWindow( hWnd, GW_HWNDFIRST );
	while (hWnd)
	{
		if ( IsWindowVisible(hWnd) && GetWindowText(hWnd, szTitle, MAX_PATH) )	
		{
			strTitle.Format(_T("%s"), szTitle);
			strTitle.MakeUpper();
			strPartTitle.MakeUpper();
			//strTitle.Trim(_T(" "));
			if ( strTitle.Find(strPartTitle) != -1 )//�ҵ�ƥ��Ĵ��ڱ��� ����ƥ��
				//if ( strTitle.Compare(strPartTitle) == 0 )//�ҵ�ƥ��Ĵ��ڱ��� ��ȫƥ��
			{
				GetWindowThreadProcessId(hWnd, &dwProcessIDTmp);
				strProcessID.Format(_T("%d"), dwProcessIDTmp);
				if ( strOpenedPidLst.Find(strProcessID) == -1 )//������ʹ��PID�б���
				{
					strOpenedPidLst += strOpenedPidLst.IsEmpty() ? strProcessID : _T(",")+strProcessID;
				}
			}
		}

		hWnd = GetWindow( hWnd, GW_HWNDNEXT );
	}
	return TRUE;
}
//��ȡ����ϵͳ�汾��
CString CMyClass::GetOSVersion(void)
{
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(GetVersionEx(&osvi)==FALSE) 
	{
		return "";
	}
	if(osvi.dwPlatformId==VER_PLATFORM_WIN32s) 
	{
		return "95";
		//cout<<"windows95����ϵͳ!"<<endl;
	}
	if (osvi.dwMajorVersion==5 && osvi.dwMinorVersion==1)
		return "xp";
	//cout<<"���Ĳ���ϵͳΪ Windows XP"<<endl;
	if (osvi.dwMajorVersion==6 && osvi.dwMinorVersion==0)
		return "Vista";
	//cout<<"���Ĳ���ϵͳΪ Windows Vista"<<endl;
	if (osvi.dwMajorVersion==6 && osvi.dwMinorVersion==1)
		return "7";
	//cout<<"���Ĳ���ϵͳΪ Windows 7"<<endl;
	if (osvi.dwMajorVersion==8)
		return "8";
	//cout<<"���Ĳ���ϵͳΪ Windows 8"<<endl;
	/*CString strVersion;
	strVersion.Format("�汾��Ϊ: %d.%d.%d",osvi.dwMajorVersion,osvi.dwMinorVersion,osvi.dwBuildNumber);
	cout<<strVersion<<endl;

	cout<<"�û���: "<<GetWinUser()<<endl<<"�������: "
	<<GetComputerName()<<endl<<"IP��ַ: "<<GetIPAddress()<<endl;


	CString strDomain;
	DWORD dwSize=MAX_PATH;
	GetComputerNameExA(ComputerNameDnsFullyQualified,strDomain.GetBuffer(MAX_PATH),&dwSize);
	strDomain.ReleaseBuffer();
	cout<<"�����ļ�������� "<<strDomain<<endl;

	dwSize=MAX_PATH;
	GetComputerNameExA(ComputerNameDnsDomain,strDomain.GetBuffer(MAX_PATH),&dwSize);
	strDomain.ReleaseBuffer();
	cout<<"���������� "<<strDomain<<endl;*/
	return "";


	/*LPWSTR *strDnsDomain=new LPWSTR[50];
	NETSETUP_JOIN_STATUS buf;
	NetGetJoinInformation(NULL,strDnsDomain,&buf);
	if (NetSetupDomainName==buf)
	cout<<"���������� "<<strDnsDomain<<endl;*/
}
//��ȡIP��ַ
CString CMyClass::GetIPAddress(void)
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	::WSAStartup(sockVersion, &wsaData);

	char host_name[256];
	CString strIP;
	gethostname(host_name,sizeof(host_name));

	struct hostent  *hp;
	struct in_addr  sa;

	hp = gethostbyname(host_name);

	if (hp != NULL)
	{
		//ѭ����ȡ����������
		for (int i = 0; hp->h_addr_list[i]; i++) 
		{
			memcpy (&sa, hp->h_addr_list[i],hp->h_length);

			strIP = inet_ntoa(sa);
			//cout << "The host IP is:" << buf << endl;
		}
	}
	WSACleanup();
	return strIP;
}

//��ȡ�������λ�õľ��
HWND CMyClass::WindowFromCursor()
{
	POINT pt;
	::GetCursorPos(&pt);
	HWND hwnd = WindowFromPoint(pt);
	return hwnd;
}
//���ݾ����������رմ��� 
BOOL CMyClass::KillWindow(HWND hwnd)
{

	HANDLE hProcess=NULL;
	DWORD dwPid=0;

	for(int i=0;i<10;i++)
	{
		if (!IsWindow(hwnd))
			hwnd=GetHwndByPid(NULL,"��������");
		if (!IsWindow(hwnd))
			hwnd=GetHwndByPid(NULL,"������");
		if (!IsWindow(hwnd))
			hwnd=GetHwndByPid(NULL,"����");//��Ϸ����
		if (!IsWindow(hwnd))
			hwnd=GetHwndByPid(NULL,"��ȫ����");//��ҳ
		if (!IsWindow(hwnd))
			hwnd=GetHwndByPid(NULL,"�ű�����");//��ҳ
		if (IsWindow(hwnd))
		{
			//SendMessage(hwnd,WM_CLOSE,0,0);
			::PostMessage(hwnd,WM_CLOSE,0,0);
			Sleep(1000);
			::GetWindowThreadProcessId(hwnd,&dwPid);
			if (dwPid>0)
			{
				hProcess=::OpenProcess(PROCESS_TERMINATE,FALSE,dwPid);
				::TerminateProcess(hProcess,0);
				Sleep(1000);
			}
		}
	}
	if (IsWindow(hwnd))
		Sleep(1000);
	if (dwPid>0)
	{
		hProcess=::OpenProcess(PROCESS_TERMINATE,FALSE,dwPid);
		::TerminateProcess(hProcess,0);
	}
	dwPid=GetProcessIdByName("Client.exe");
	if(dwPid>0)
	{
		KillWindow("Client.exe");
		Sleep(5000);
	}
	dwPid=GetProcessIdByName("XClient.exe");
	if(dwPid>0)
	{
		KillWindow("XClient.exe");
		Sleep(1000);
	}
	::CloseHandle(hProcess);
	return TRUE;
}
//���ݽ������رմ���
BOOL CMyClass::KillWindow(CString strProcessName)
{
	strProcessName.Trim();
	if(strProcessName.IsEmpty())
		return FALSE;
	HANDLE hProcess=NULL;
	DWORD dwPid=0;

	for(int i=0;i<5;i++)
	{
		dwPid = GetProcessIdByName(strProcessName);
		if (dwPid>0)
		{
			hProcess=::OpenProcess(PROCESS_TERMINATE,FALSE,dwPid);
			::TerminateProcess(hProcess,0);
			Sleep(500);
		}
		else
			break;
	}
	::CloseHandle(hProcess);
	return TRUE;
}
// ��ֹ������������н�������,Ĭ��Ϊ������,�����ִ�Сд,��׺��ʡ��
void CMyClass::RefuseKillProcess(CString ProcessName)
{
	HWND hwnd,hwndMain;
	int  iItem=0;
	LVITEM lvitem, *plvitem;
	char ItemBuf[512],*pItem;
	DWORD PID;
	HANDLE hProcess;

	// �������������ListView���ھ��
	hwndMain=::FindWindow("#32770",_T("Windows ���������"));
	hwnd=::FindWindowExA(hwndMain,0,"#32770",0);
	hwnd=::FindWindowEx(hwnd,0,"SysListView32",0);

	// Windows�����������δ�����򷵻�
	if (!hwnd) 
		return;
	else
	{
		// û��ָ��Ŀ������򷵻�
		iItem=::SendMessage(hwnd,LVM_GETNEXTITEM,-1,LVNI_SELECTED);
		if (-1==iItem) 
			return;
		else
		{
			// ��ȡ���̾������ʧ���򷵻�
			GetWindowThreadProcessId(hwnd, &PID);
			hProcess=OpenProcess(PROCESS_ALL_ACCESS,false,PID);
			if (!hProcess)
				return;   
			else
			{
				plvitem=(LVITEM*)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);
				pItem=(char*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);

				// �޷������ڴ��򷵻�
				if ((!plvitem)||(!pItem))
					return;    
				else
				{
					lvitem.cchTextMax=512;
					lvitem.iSubItem=0;  //ProcessName
					lvitem.pszText=pItem;
					WriteProcessMemory(hProcess, plvitem, &lvitem, sizeof(LVITEM), NULL);
					::SendMessage(hwnd, LVM_GETITEMTEXT, (WPARAM)iItem, (LPARAM)plvitem);
					ReadProcessMemory(hProcess, pItem, ItemBuf, 512, NULL);

					// �Ƚ��ַ���,ƥ�����ӳ����
					CString str = (CString)ItemBuf;
					CString strName=ProcessName.IsEmpty()?AfxGetAppName():ProcessName;
					if (strName.Right(4).MakeLower()!=".exe")
						strName += ".exe";
					if(str.CompareNoCase(strName) == 0)
					{
						HWND hWnd=::FindWindow(NULL,_T("Windows ���������"));
						::SendMessage(hwndMain,WM_CLOSE,0,0);
						Sleep(100);
						::MessageBox(NULL,_T("��ֹ�ر�ϵͳ�ؼ�����!"),_T("��ʾ"),MB_ICONERROR | MB_OK);
					}
				}
			}
		}
	}
}
//��ȡָ���ļ�������
CString CMyClass::ReadFile ( CString strPathName )
{
	CString strData;
	strData = "";

	CFile file;
	BOOL bSuccess = file.Open( strPathName, CFile::modeRead | CFile::shareExclusive, NULL ) ;
	if ( !bSuccess ) 
		return "";

	DWORD dwFileSize = file.GetLength();
	char *pFileContent = new char[dwFileSize+1];
	memset ( pFileContent, 0, dwFileSize+1 );
	file.Read( pFileContent, dwFileSize );
	file.Close();

	if ( dwFileSize > 0 )
	{
		strData.Format("%s", pFileContent);
		strData = strData.Mid( 0, dwFileSize );
		strData	= strData + "\r\n";
	}

	delete pFileContent;
	pFileContent = NULL;

	return strData;
}




//��ȡָ���������ַ���֮�������(��keyName1��keyName2֮����ȡ)
CString CMyClass::FindStr(CString Str,CString StrS,CString StrE,CString keyName1,CString keyName2)
{
	int StartI=0;
	if (!keyName1.IsEmpty())
		StartI=Str.Find( keyName1,StartI)+keyName1.GetLength();
	if (!keyName1.IsEmpty())
		StartI=Str.Find( keyName2,StartI)+keyName2.GetLength();
	CString FStr="";
	int nStS,nStE;
	nStS=Str.Find( StrS,StartI);
	if (nStS==-1)
		return FStr;
	nStE=Str.Find (StrE,nStS+StrS.GetLength());
	if (nStE== 0 || StrE == "")
		nStE=Str.GetLength();
	FStr = Str.Mid( nStS + StrS.GetLength(),nStE - nStS - StrS.GetLength() );

	return FStr;
}
//���ҵ�index���ַ�(strS),ȡdirection(0��,1��)����
CString CMyClass::FindStr2(CString Str,CString StrS,int index,int direction)
{

	int StartI=0;

	CString FStr="";
	int nStS=-1;
	for(int i=0;i<index;i++)
	{

		nStS=Str.Find( StrS,StartI);
		StartI=nStS+1;
		//ShowMessage("%d",nStS);
	}
	if (nStS==-1)
		return FStr;
	if(direction==0)
		FStr = Str.Left( nStS);
	else if(direction==1)
		FStr = Str.Mid(nStS+1);
	char tt=StrS.GetAt(0);
	StartI=FStr.ReverseFind(tt);
	if(StartI>0)
		FStr=FStr.Mid(StartI+1);

	return FStr;
}
//���ڽ�ͼ
void CMyClass::CaptureGamePicture(CString strPictureName,HWND hwnd)
{
	int cx, cy;
	cx		= GetSystemMetrics(SM_CXSCREEN);
	cy		= GetSystemMetrics(SM_CYSCREEN);


	CRect rect;

	if (!IsWindow(hwnd))
		hwnd = ::GetDesktopWindow();

	::GetWindowRect( hwnd, &rect );

	int Width=rect.Width();
	int Height=rect.Height();
	if ( rect.right - rect.left <= 0 || 
		rect.bottom - rect.top <= 0 || 
		abs(rect.Width()) > cx || 
		abs(rect.Height()) > cy )
	{
		hwnd = ::GetDesktopWindow();
		Width=cx;
		Height=cy;
		/*rect.left = 0;
		rect.top = 0;
		rect.bottom = cy;
		rect.right = cx;*/
	}


	if (!GetDCData(hwnd))
	{
		//::AfxMessageBox("��ͼʧ��");
		return;
	}
	/*GetDIBits(pGDI->hMemDC,pGDI->hMemDIB,0,pGDI->BMIH.biHeight,pBMPData,
	(LPBITMAPINFO)&pGDI->BMIH,DIB_RGB_COLORS);*/
	LPBITMAPINFO lpbmih = new BITMAPINFO;//ͼ�θ�ʽ����
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = Width;//
	lpbmih->bmiHeader.biHeight = Height;
	lpbmih->bmiHeader.biPlanes = 1;//Ŀ���豸λ����������Ϊ1
	lpbmih->bmiHeader.biBitCount = 24;//��ɫ���
	lpbmih->bmiHeader.biCompression = BI_RGB;//λͼѹ������
	lpbmih->bmiHeader.biSizeImage = 0;//��С���ֽڵ�λ
	lpbmih->bmiHeader.biXPelsPerMeter = 0;//ˮƽ�ֱ���
	lpbmih->bmiHeader.biYPelsPerMeter = 0;//��ֱ�ֱ���
	lpbmih->bmiHeader.biClrUsed = 0;//λͼʵ��ʹ����ɫ������ɫ��
	lpbmih->bmiHeader.biClrImportant = 0;//λͼ��ʾ�бȽ���Ҫ����ɫ��

	BITMAPFILEHEADER bmfh;
	//bmfh.bfType=0x4D42;//������BMP�ļ�(�ַ���"BM")
	*((char *)&bmfh.bfType) = 'B';
	*(((char *)&bmfh.bfType) + 1) = 'M';//������BMP�ļ�
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//ƫ��λ��

	int iBMPBytes = (Width * (24 / 8)+3)/4*4*Height;//�����ļ���С
	bmfh.bfSize = bmfh.bfOffBits + iBMPBytes;//�ļ���С

	BOOL bShareClip = FALSE;
	CString strFileName, strJPEGFile;
	strFileName = strPictureName;
	CFile file;
	BOOL bSuccess = file.Open(strFileName,CFile::modeWrite | 
		CFile::shareExclusive | CFile::modeCreate,NULL);
	if(bSuccess)
	{
		file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
		file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
		file.Write(pData,iBMPBytes);
		file.Close();
	}

}
void CMyClass::CapturePictureInRect(HWND hwnd,CString strPictureName,int left,int top,int right,int bottom,BOOL bWaterMark,COLORREF WaterMarkColor,int WaterMarkLeft,int WaterMarkTop,int WaterMarkRight,int WaterMarkBottom,
									int WaterMarkLeft2,int WaterMarkTop2,int WaterMarkRight2,int WaterMarkBottom2,
									int WaterMarkLeft3,int WaterMarkTop3,int WaterMarkRight3,int WaterMarkBottom3)
{
	CRect rect;
	GetWindowRect(hwnd,&rect);
	if(left<=0)
	{
		//AfxMessageBox("000");
		left=rect.left+1;
	}

	if(top<0)
	{
		//AfxMessageBox("111");
		top=rect.top;
	}

	if(bottom>=rect.Height())
	{
		//AfxMessageBox("222");
		bottom=rect.Height()-1;
	}

	if(right>=rect.Width())
	{
		//AfxMessageBox("333");
		right=rect.Width()-1;
	}
	if(left>=right)
		return;

	//HWND hActiveWnd = hwnd;
	CString strLog;
	if(m_bGetData)
	{
		if (!GetDCData32(hwnd))
		{
			//::AfxMessageBox("��ͼʧ��");
			return;
		}
	}
	CRect argRect=CRect(left,top,right,bottom);
	//GetWindowRect(hwnd,&rect);

	BYTE *tData=new BYTE[(argRect.Width()*4+3)/4*4*argRect.Height()];
	int num=0;
	//ѭ������
	for (int y=bottom; y>top; y--)
	{
		for (int x=left; x<right; x++)
		{
			if(bWaterMark && ((x>WaterMarkLeft && y>WaterMarkTop && x<WaterMarkRight && y<WaterMarkBottom) 
				|| (x>WaterMarkLeft2 && y>WaterMarkTop2 && x<WaterMarkRight2 && y<WaterMarkBottom2)
				|| (x>WaterMarkLeft3 && y>WaterMarkTop3 && x<WaterMarkRight3 && y<WaterMarkBottom3)))
			{
				tData[num+2] = GetRValue(WaterMarkColor);
				tData[num+1] = GetGValue(WaterMarkColor);
				tData[num+0] = GetBValue(WaterMarkColor);
			}
			else
			{
				tData[num+2] = pData[rect.Width() * 4 * (rect.Height()- y ) + (x)*4 +2];
				tData[num+1] = pData[rect.Width() * 4 * (rect.Height()- y ) + (x)*4 +1];
				tData[num+0] = pData[rect.Width() * 4 * (rect.Height()- y ) + (x)*4 +0];
			}
			num+=4;
		}
	}

	LPBITMAPINFO lpbmih = new BITMAPINFO;//ͼ�θ�ʽ����
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = argRect.Width();//
	lpbmih->bmiHeader.biHeight = argRect.Height();
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

	int iBMPBytes = (argRect.Width() * 4+3)/4*4*argRect.Height();//�����ļ���С
	bmfh.bfSize = bmfh.bfOffBits + iBMPBytes;//�ļ���С

	CFile file;
	BOOL bSuccess = file.Open(strPictureName,CFile::modeCreate|CFile::modeWrite | 
		CFile::shareExclusive ,NULL);
	if(bSuccess)
	{
		file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
		file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
		file.Write(tData,iBMPBytes);
		file.Close();

	}
	delete []tData;

}
//��ͼƬ��ˮӡ
void CMyClass::WaterMark( CString strMark)
{
}
int CMyClass::GetColorNumFormText( COLORREF color,CString Outtext)
{

	int size=16,Interval=0,width=0,JiaCu=0,IntervalE=0;
	color=RGB(255,255,255);
	POINT PT;
	PT.x=-1;
	PT.y=-2;
	if (Outtext=="")
	{
		return FALSE;
	}

	//����λͼ����
	HDC hdc,hdcMem;//�豸
	HFONT ftUser;//�������;��
	HBITMAP hBitMap = NULL;//λͼҪ���豸��������Ҫת���ɴ�����
	CBitmap *pBitMap = NULL;//λͼ
	CDC *pMemDC = NULL; /*HDC����ı���ָ��һ���ڴ棬���   
						�ڴ���������һ���豸����ص����ݣ�����Ҳ����   
						��ΪHDC�������һ��ָ�룻��CDC�ඨ��һ������   
						�������ӵ��HDC�����һ���豸������ͬʱҲ��   
						����HDC��صĲ����ĺ����� */  


	BYTE *tData;
	//����ͼ�δ�С
	//if (Interval == 0)
	//	Interval=size;
	//int iWidth = (Outtext.GetLength() * ( size/2+Interval )+3) / 4  * 4;
	int iWidth = (Outtext.GetLength() * ( size+Interval )-7) / 8  * 4 +8;
	int iHeight =size;
	int iPixel = 24;
	//ͼ�θ�ʽ����
	LPBITMAPINFO lpbmih = new BITMAPINFO;//ͼ�θ�ʽ����
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = iWidth;//
	lpbmih->bmiHeader.biHeight = iHeight;
	lpbmih->bmiHeader.biPlanes = 1;//Ŀ���豸λ����������Ϊ1
	lpbmih->bmiHeader.biBitCount = iPixel;//��ɫ���
	lpbmih->bmiHeader.biCompression = BI_RGB;//λͼѹ������
	lpbmih->bmiHeader.biSizeImage = 0;//��С���ֽڵ�λ
	lpbmih->bmiHeader.biXPelsPerMeter = 0;//ˮƽ�ֱ���
	lpbmih->bmiHeader.biYPelsPerMeter = 0;//��ֱ�ֱ���
	lpbmih->bmiHeader.biClrUsed = 0;//λͼʵ��ʹ����ɫ������ɫ��
	lpbmih->bmiHeader.biClrImportant = 0;//λͼ��ʾ�бȽ���Ҫ����ɫ��


	hdc = CreateIC(TEXT("DISPLAY"),NULL,NULL,NULL);//���԰����ķ���ֵ���� HDC, ��������HDC�� ��ָ���Զ������в�ѯ������
	hdcMem = CreateCompatibleDC(hdc);//�ڴ�dc��
	hBitMap = CreateDIBSection(hdcMem,lpbmih,DIB_PAL_COLORS,(void **)&tData,NULL,0);//����һ��DIB
	if (color==0)
		memset(tData,255,iHeight * iWidth * iPixel/8);
	pBitMap = new CBitmap;
	pBitMap->Attach(hBitMap);//��λͼ����
	pMemDC = new CDC;
	pMemDC->Attach(hdcMem);//��DC�ڴ����
	pMemDC->SelectObject(pBitMap);//ѡ���ڴ��ַ

	pMemDC->SetBkMode(TRANSPARENT);//����ģʽ
	pMemDC->SetTextColor( color);//������ɫ
	ftUser = CreateFont(size,//�����С
		width,//�ֿ�
		0,//����ָ��ת��ʸ����escapement vector������ʾ�����x��ļнǣ���0.1Ϊ��λ��
		00,//����ָ���ַ����ߺ�x��ļнǣ���0.1Ϊ��λ����
		//FW_DONTCARE,//������
		JiaCu,
		FALSE, //����6��������bItalic����ָ�������Ƿ�Ϊб��Ĳ���ֵ
		FALSE,//��7��������bUnderline����ָ�������Ƿ�����»��ߵĲ���ֵ
		FALSE,//��8��������cStrickout����ָ���ַ��Ƿ����ɾ���ߵĲ���ֵ
		GB2312_CHARSET,//ָ����������弯
		OUT_DEFAULT_PRECIS,//����ָ������������ȷ��
		CLIP_DEFAULT_PRECIS,//������ȷ��
		DEFAULT_QUALITY,//�������������
		DEFAULT_PITCH|FF_MODERN,//�ַ������ַ�
		"����"//�������������
		); 
	(HFONT) SelectObject(hdcMem, ftUser);//ѡ����ʽ

	//��ͼƬ���������
	CRect rc;//�ͻ���
	rc.right=iWidth;
	rc.bottom=iHeight;
	rc.left =0;
	rc.top  =0;
	CString s;
	char* ss= Outtext.GetBuffer(Outtext.GetLength());
	int t=0;
	while ( t < Outtext.GetLength())
	{
		if(ss[t] <0)     //�Ǻ��� 
		{
			pMemDC->DrawText( Outtext.Mid(t,2), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			if (width== 0)
				rc.left=rc.left+size+Interval; 
			else
				rc.left=rc.left + width *2 + Interval;
			t++; 
		} 
		else 
		{
			pMemDC->DrawText( Outtext.Mid(t,1), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			if (width== 0)
				rc.left=rc.left+ size / 2 +IntervalE; 
			else
				rc.left=rc.left + width *2 + IntervalE;
		} 

		t++;

	}
	//pMemDC->DrawText( Outtext, &rc, DT_SINGLELINE|DT_LEFT );//����ı� 

	pMemDC->SelectObject(pBitMap);

	//AfxMessageBox("ͼƬ���ɳɹ���");
	/////////////////////////////////////�����ַ�ͼƬ
	//BITMAPFILEHEADER bmfh;
	//*((char *)&bmfh.bfType) = 'B';
	//*(((char *)&bmfh.bfType) + 1) = 'M';//������BMP�ļ�
	//bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//ƫ��λ��
	//bmfh.bfSize = bmfh.bfOffBits + (iWidth * iHeight) * iPixel / 8;//�ļ���С
	////TCHAR szBMPFileName[128];//�ļ�����
	//int iBMPBytes = iWidth * iHeight * iPixel / 8;//�����ļ���С
	//CFile file;
	//if(file.Open("F:\\aa.bmp",CFile::modeWrite | CFile::modeCreate))
	//{
	//	file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
	//	file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
	//	file.Write(tData,iBMPBytes);
	//	file.Close();
	//}
	//else
	//{
	//	AfxMessageBox("�޷����ɣ���ȷ������·���Ƿ���ȷ");
	//	return 1;
	//}
	BYTE r,g,b;
	int num=0;
	for (int i=0;i<iHeight;i++)
	{
		for (int j=0;j<iWidth;j++)
		{
			b=tData[ (iHeight-i-1) * (iWidth *3)+ j*3];
			g=tData[ (iHeight-i-1) * (iWidth *3)+ j*3+1];
			r=tData[ (iHeight-i-1) * (iWidth *3)+ j*3+2];
			if(b==255 && g==255 && r==255)
				num++;
		}
	}
	delete pMemDC; pMemDC = NULL;
	delete pBitMap; pBitMap = NULL;
	return num;
}
////���ɽ�ͼ�ļ���
//CString CMyClass::GetCaptureFileName()
//{
//	CString strFileName;
//	if ( m_strPictureDir.Right(1) != _T("\\")  )
//		m_strPictureDir += _T("\\");
//	CTime t = CTime::GetCurrentTime();
//	//����Ա����-��Ϸ��-������-��������-����Ա-yyyyMMddHHmmss.bmp
//
//	strFileName.Format( _T("%s%s-%s-%s-%s-%04d%02d%02d%02d%02d%02d-a.bmp"),
//						m_strPictureDir, m_strOPDN, m_strGameName, m_strOrderNo, m_strDeliverOpId,
//						t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond() );
//	return strFileName;
//}
//Ѱ�����֣���������֣�����ţ��ּ�࣬Ѱ�ҷ�Χ���ϣ��ң��£�������ͬRGBֵ��
POINT CMyClass::FindBMPText(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,int left,int top,int right,int bottom,int Interval,int JiaCu,int xiangshi,int width,int IntervalE)
{
	if (JiaCu!=700)
		JiaCu=0;

	POINT PT;
	PT.x=-1;
	PT.y=-2;
	if (Outtext=="")
	{
		return PT;
	}
	CRect rect;
	if (::IsWindow(hWnd))
	{
		GetWindowRect (hWnd,&rect);
	}
	else
	{
		return PT;
	}
	//����λͼ����
	HDC hdc,hdcMem;//�豸
	HFONT ftUser;//�������;��
	HBITMAP hBitMap = NULL;//λͼҪ���豸��������Ҫת���ɴ�����
	CBitmap *pBitMap = NULL;//λͼ
	CDC *pMemDC = NULL; /*HDC����ı���ָ��һ���ڴ棬���   
						�ڴ���������һ���豸����ص����ݣ�����Ҳ����   
						��ΪHDC�������һ��ָ�룻��CDC�ඨ��һ������   
						�������ӵ��HDC�����һ���豸������ͬʱҲ��   
						����HDC��صĲ����ĺ����� */  


	BYTE *tData;
	//����ͼ�δ�С
	//if (Interval == 0)
	//	Interval=size;
	//int iWidth = (Outtext.GetLength() * ( size/2+(Interval>0?1:0))+7) / 8  * 8;
	int iWidth = (Outtext.GetLength() * ( size+Interval )-7) / 8  * 4 +8;
	//iWidth = (Outtext.GetLength() * ( size/2+Interval ));
	int iHeight =size + 1;
	int iPixel = 24;
	//ͼ�θ�ʽ����
	LPBITMAPINFO lpbmih = new BITMAPINFO;//ͼ�θ�ʽ����
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = iWidth;//
	lpbmih->bmiHeader.biHeight = iHeight;
	lpbmih->bmiHeader.biPlanes = 1;//Ŀ���豸λ����������Ϊ1
	lpbmih->bmiHeader.biBitCount = iPixel;//��ɫ���
	lpbmih->bmiHeader.biCompression = BI_RGB;//λͼѹ������
	lpbmih->bmiHeader.biSizeImage = 0;//��С���ֽڵ�λ
	lpbmih->bmiHeader.biXPelsPerMeter = 0;//ˮƽ�ֱ���
	lpbmih->bmiHeader.biYPelsPerMeter = 0;//��ֱ�ֱ���
	lpbmih->bmiHeader.biClrUsed = 0;//λͼʵ��ʹ����ɫ������ɫ��
	lpbmih->bmiHeader.biClrImportant = 0;//λͼ��ʾ�бȽ���Ҫ����ɫ��


	hdc = CreateIC(TEXT("DISPLAY"),NULL,NULL,NULL);//���԰����ķ���ֵ���� HDC, ��������HDC�� ��ָ���Զ������в�ѯ������
	hdcMem = CreateCompatibleDC(hdc);//�ڴ�dc��
	hBitMap = CreateDIBSection(hdcMem,lpbmih,DIB_PAL_COLORS,(void **)&tData,NULL,0);//����һ��DIB
	if (color==0)
		memset(&tData[0],255,iHeight * iWidth * 3);
	pBitMap = new CBitmap;
	pBitMap->Attach(hBitMap);//��λͼ����
	pMemDC = new CDC;
	pMemDC->Attach(hdcMem);//��DC�ڴ����
	pMemDC->SelectObject(pBitMap);//ѡ���ڴ��ַ

	pMemDC->SetBkMode(TRANSPARENT);//����ģʽ
	pMemDC->SetTextColor( color);//������ɫ
	ftUser = CreateFont(size,//�����С
		width,//�ֿ�
		0,//����ָ��ת��ʸ����escapement vector������ʾ�����x��ļнǣ���0.1Ϊ��λ��
		00,//����ָ���ַ����ߺ�x��ļнǣ���0.1Ϊ��λ����
		//FW_DONTCARE,//������
		JiaCu,
		FALSE, //����6��������bItalic����ָ�������Ƿ�Ϊб��Ĳ���ֵ
		FALSE,//��7��������bUnderline����ָ�������Ƿ�����»��ߵĲ���ֵ
		FALSE,//��8��������cStrickout����ָ���ַ��Ƿ����ɾ���ߵĲ���ֵ
		GB2312_CHARSET,//ָ����������弯
		OUT_DEFAULT_PRECIS,//����ָ������������ȷ��
		CLIP_DEFAULT_PRECIS,//������ȷ��
		DEFAULT_QUALITY,//�������������
		DEFAULT_PITCH|FF_MODERN,//�ַ������ַ�
		ZiTi//"����"//�������������
		); 
	(HFONT) SelectObject(hdcMem, ftUser);//ѡ����ʽ

	//��ͼƬ���������
	CRect rc;//�ͻ���
	rc.right=iWidth;
	rc.bottom=iHeight;
	rc.left =1;
	rc.top = 1;
	CString s;
	char* ss= Outtext.GetBuffer(Outtext.GetLength());
	int t=0;
	while ( t < Outtext.GetLength())
	{
		if(ss[t] <0)     //�Ǻ��� 
		{
			pMemDC->DrawText( Outtext.Mid(t,2), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			if (width== 0)
				rc.left=rc.left+size+Interval; 
			else
				rc.left=rc.left + width *2 + Interval;
			t++; 
		} 
		else 
		{
			pMemDC->DrawText( Outtext.Mid(t,1), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			if (width== 0)
				rc.left=rc.left+ size / 2 +IntervalE; 
			else
				rc.left=rc.left + width *2 + IntervalE;
		} 

		t++;

	}
	//pMemDC->DrawText( Outtext, &rc, DT_SINGLELINE|DT_LEFT );//����ı� 

	pMemDC->SelectObject(pBitMap);

	//AfxMessageBox("ͼƬ���ɳɹ���");
	/////////////////////////////////////�����ַ�ͼƬ
	//BITMAPFILEHEADER bmfh;
	//*((char *)&bmfh.bfType) = 'B';
	//*(((char *)&bmfh.bfType) + 1) = 'M';//������BMP�ļ�
	//bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//ƫ��λ��
	//bmfh.bfSize = bmfh.bfOffBits + (iWidth * iHeight) * iPixel / 8;//�ļ���С
	////TCHAR szBMPFileName[128];//�ļ�����
	//int iBMPBytes = iWidth * iHeight * iPixel / 8;//�����ļ���С
	//CFile file;
	//if(file.Open("test2.bmp",CFile::modeWrite | CFile::modeCreate))
	//{
	//	file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
	//	file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
	//	file.Write(tData,iBMPBytes);
	//	file.Close();
	//}
	//else
	//{
	//	AfxMessageBox("�޷����ɣ���ȷ������·���Ƿ���ȷ");
	//	return PT;
	//}
	BYTE r, g, b, now_r, now_g, now_b;

	/*BYTE *pData=GetDCData(hWnd);*/
	if (!GetDCData(hWnd))
		return PT;
	//GetDCData(hWnd);

	if (left<=0)
		left=0;
	if (top<=0)
		top=0;
	if ((right<=0) || (right>rect.Width()))
		right=rect.Width();

	if ((bottom<=0) ||(bottom>rect.Height()))
		bottom=rect.Height();


	BOOL fFalse;
	//ѭ���Ƚ�
	for (int y=top; y<bottom-iHeight; y++)
	{
		for (int x=left; x<right-iWidth; x++)
		{
			fFalse=TRUE;
			for (int i=0;i<iHeight-1 ;i++)
			{
				if (!fFalse)
					break;
				for (int j=0;j<iWidth-1;j++)
				{
					if (!fFalse)
						break;
					b=tData[ (iHeight-i-1) * (iWidth *3)+ j*3];
					g=tData[ (iHeight-i-1) * (iWidth *3)+ j*3+1];
					r=tData[ (iHeight-i-1) * (iWidth *3)+ j*3+2];

					now_r = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +2];
					now_g = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +1];
					now_b = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +0];

					if (((abs(now_r - r)<xiangshi)&&(abs(now_g - g)<xiangshi)&&(abs(now_b -b)<xiangshi))  ||  ((b==tData[0]) && (g==tData[1])&&( r==tData[2]))  )
						fFalse=TRUE;
					else
						fFalse=FALSE;
				}

			}
			if (fFalse==TRUE) 
			{
				PT.x = x;
				PT.y = y;
				//delete pData;

				delete pMemDC; pMemDC = NULL;
				delete pBitMap; pBitMap = NULL;
				delete lpbmih; lpbmih = NULL;
				return PT;
			}
		}
	}
	//delete pData;
	delete pMemDC; pMemDC = NULL;
	delete pBitMap; pBitMap = NULL;
	delete lpbmih; lpbmih = NULL;
	return PT;

}
BOOL CMyClass::FindBMPText(HWND hWnd,CString ZiTi,CString Outtext,UINT size,COLORREF color,LPPOINT pt,UINT left,UINT top,UINT right,UINT bottom,BOOL bGetData,int Interval,int JiaCu,int xiangshi)
{
	pt->x=-1;
	pt->y=-2;
	if (JiaCu!=700)
		JiaCu=0;
	if (Outtext=="")
	{
		return FALSE;
	}
	CRect rect;
	if (::IsWindow(hWnd))
	{
		GetWindowRect (hWnd,&rect);
	}
	else
	{
		return FALSE;
	}
	//����λͼ����
	HDC hdc,hdcMem;//�豸
	HFONT ftUser;//�������;��
	HBITMAP hBitMap = NULL;//λͼҪ���豸��������Ҫת���ɴ�����
	CBitmap *pBitMap = NULL;//λͼ
	CDC *pMemDC = NULL; /*HDC����ı���ָ��һ���ڴ棬���   
						�ڴ���������һ���豸����ص����ݣ�����Ҳ����   
						��ΪHDC�������һ��ָ�룻��CDC�ඨ��һ������   
						�������ӵ��HDC�����һ���豸������ͬʱҲ��   
						����HDC��صĲ����ĺ����� */  

	int width=0,IntervalE=0;
	BYTE *tData;
	//����ͼ�δ�С
	//if (Interval == 0)
	//	Interval=size;
	//int iWidth = (Outtext.GetLength() * ( size/2+(Interval>0?1:0))+7) / 8  * 8;
	int iWidth = (Outtext.GetLength() * ( size+Interval )-7) / 8  * 4 +8;
	//iWidth = (Outtext.GetLength() * ( size/2+Interval ));
	int iHeight =size + 1;
	int iPixel = 24;
	//ͼ�θ�ʽ����
	LPBITMAPINFO lpbmih = new BITMAPINFO;//ͼ�θ�ʽ����
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = iWidth;//
	lpbmih->bmiHeader.biHeight = iHeight;
	lpbmih->bmiHeader.biPlanes = 1;//Ŀ���豸λ����������Ϊ1
	lpbmih->bmiHeader.biBitCount = iPixel;//��ɫ���
	lpbmih->bmiHeader.biCompression = BI_RGB;//λͼѹ������
	lpbmih->bmiHeader.biSizeImage = 0;//��С���ֽڵ�λ
	lpbmih->bmiHeader.biXPelsPerMeter = 0;//ˮƽ�ֱ���
	lpbmih->bmiHeader.biYPelsPerMeter = 0;//��ֱ�ֱ���
	lpbmih->bmiHeader.biClrUsed = 0;//λͼʵ��ʹ����ɫ������ɫ��
	lpbmih->bmiHeader.biClrImportant = 0;//λͼ��ʾ�бȽ���Ҫ����ɫ��


	hdc = CreateIC(TEXT("DISPLAY"),NULL,NULL,NULL);//���԰����ķ���ֵ���� HDC, ��������HDC�� ��ָ���Զ������в�ѯ������
	hdcMem = CreateCompatibleDC(hdc);//�ڴ�dc��
	hBitMap = CreateDIBSection(hdcMem,lpbmih,DIB_PAL_COLORS,(void **)&tData,NULL,0);//����һ��DIB
	if (color==0)
		memset(&tData[0],255,iHeight * iWidth * 3);
	pBitMap = new CBitmap;
	pBitMap->Attach(hBitMap);//��λͼ����
	pMemDC = new CDC;
	pMemDC->Attach(hdcMem);//��DC�ڴ����
	pMemDC->SelectObject(pBitMap);//ѡ���ڴ��ַ

	pMemDC->SetBkMode(TRANSPARENT);//����ģʽ
	pMemDC->SetTextColor( color);//������ɫ
	ftUser = CreateFont(size,//�����С
		width,//�ֿ�
		0,//����ָ��ת��ʸ����escapement vector������ʾ�����x��ļнǣ���0.1Ϊ��λ��
		00,//����ָ���ַ����ߺ�x��ļнǣ���0.1Ϊ��λ����
		//FW_DONTCARE,//������
		JiaCu,
		FALSE, //����6��������bItalic����ָ�������Ƿ�Ϊб��Ĳ���ֵ
		FALSE,//��7��������bUnderline����ָ�������Ƿ�����»��ߵĲ���ֵ
		FALSE,//��8��������cStrickout����ָ���ַ��Ƿ����ɾ���ߵĲ���ֵ
		GB2312_CHARSET,//ָ����������弯
		OUT_DEFAULT_PRECIS,//����ָ������������ȷ��
		CLIP_DEFAULT_PRECIS,//������ȷ��
		DEFAULT_QUALITY,//�������������
		DEFAULT_PITCH|FF_MODERN,//�ַ������ַ�
		ZiTi//"����"//�������������
		); 
	(HFONT) SelectObject(hdcMem, ftUser);//ѡ����ʽ

	//��ͼƬ���������
	CRect rc;//�ͻ���
	rc.right=iWidth;
	rc.bottom=iHeight;
	rc.left =1;
	rc.top = 1;
	CString s;
	char* ss= Outtext.GetBuffer(Outtext.GetLength());
	int t=0;
	while ( t < Outtext.GetLength())
	{
		if(ss[t] <0)     //�Ǻ��� 
		{
			pMemDC->DrawText( Outtext.Mid(t,2), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			if (width== 0)
				rc.left=rc.left+size+Interval; 
			else
				rc.left=rc.left + width *2 + Interval;
			t++; 
		} 
		else 
		{
			pMemDC->DrawText( Outtext.Mid(t,1), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			if (width== 0)
				rc.left=rc.left+ size / 2 +IntervalE; 
			else
				rc.left=rc.left + width *2 + IntervalE;
		} 

		t++;

	}
	//pMemDC->DrawText( Outtext, &rc, DT_SINGLELINE|DT_LEFT );//����ı� 

	pMemDC->SelectObject(pBitMap);

	//AfxMessageBox("ͼƬ���ɳɹ���");
	/////////////////////////////////////�����ַ�ͼƬ
	//BITMAPFILEHEADER bmfh;
	//*((char *)&bmfh.bfType) = 'B';
	//*(((char *)&bmfh.bfType) + 1) = 'M';//������BMP�ļ�
	//bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//ƫ��λ��
	//bmfh.bfSize = bmfh.bfOffBits + (iWidth * iHeight) * iPixel / 8;//�ļ���С
	////TCHAR szBMPFileName[128];//�ļ�����
	//int iBMPBytes = iWidth * iHeight * iPixel / 8;//�����ļ���С
	//CFile file;
	//if(file.Open("test3.bmp",CFile::modeWrite | CFile::modeCreate))
	//{
	//	file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
	//	file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
	//	file.Write(tData,iBMPBytes);
	//	file.Close();
	//}
	//else
	//{
	//	AfxMessageBox("�޷����ɣ���ȷ������·���Ƿ���ȷ");
	//	return FALSE;
	//}
	BYTE r, g, b, now_r, now_g, now_b;

	if(bGetData)
	{
		if (!GetDCData(hWnd))
			return FALSE;
	}

	if (left<=0)
		left=0;
	if (top<=0)
		top=0;
	if ((right<=0) || (right>rect.Width()))
		right=rect.Width();

	if ((bottom<=0) ||(bottom>rect.Height()))
		bottom=rect.Height();

	if(right<iWidth)
		return FALSE;

	BOOL fFalse;
	//ѭ���Ƚ�
	for (int y=top; y<bottom-iHeight; y++)
	{
		for (int x=left; x<right-iWidth; x++)
		{
			fFalse=TRUE;
			for (int i=0;i<iHeight;i++)
			{
				if (!fFalse)
					break;
				for (int j=0;j<iWidth-1;j++)
				{
					if (!fFalse)
						break;
					b=tData[ (iHeight-i-1) * (iWidth *3)+ j*3];
					g=tData[ (iHeight-i-1) * (iWidth *3)+ j*3+1];
					r=tData[ (iHeight-i-1) * (iWidth *3)+ j*3+2];

					now_r = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +2];
					now_g = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +1];
					now_b = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +0];

					if (((abs(now_r - r)<xiangshi)&&(abs(now_g - g)<xiangshi)&&(abs(now_b -b)<xiangshi))  ||  ((b==tData[0]) && (g==tData[1])&&( r==tData[2]))  )
						fFalse=TRUE;
					else
						fFalse=FALSE;
				}

			}
			if (fFalse==TRUE) 
			{
				pt->x = x;
				pt->y = y;
				//delete pData;

				delete pMemDC; pMemDC = NULL;
				delete pBitMap; pBitMap = NULL;
				delete lpbmih; lpbmih = NULL;
				return TRUE;
			}
		}
	}
	pt->x=-1;
	pt->y=-2;
	delete pMemDC; pMemDC = NULL;
	delete pBitMap; pBitMap = NULL;
	delete lpbmih; lpbmih = NULL;
	return FALSE;

}
//Ѱ�ҽ�ɫ����������֣�����ţ��ּ�࣬Ѱ�ҷ�Χ���ϣ��ң��£�������ͬRGBֵ1��
POINT CMyClass::FindTextRole(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,int left,int top,int right,int bottom,int Interval,int JiaCu,int xiangshi,int IntervalE)
{

	if (JiaCu!=700)
		JiaCu=0;

	POINT PT;
	PT.x=-1;
	PT.y=-2;
	CString strTemp=Outtext;
	if (strTemp.Trim()=="")
	{
		return PT;
	}
	CRect rect;
	if (::IsWindow(hWnd))
	{
		GetWindowRect (hWnd,&rect);
	}
	else
	{
		return PT;
	}

	//Outtext="  "+Outtext+"  ";

	//����λͼ����
	HDC hdc,hdcMem;//�豸
	HFONT ftUser;//�������;��
	HBITMAP hBitMap = NULL;//λͼҪ���豸��������Ҫת���ɴ�����
	CBitmap *pBitMap = NULL;//λͼ
	CDC *pMemDC = NULL; /*HDC����ı���ָ��һ���ڴ棬���   
						�ڴ���������һ���豸����ص����ݣ�����Ҳ����   
						��ΪHDC�������һ��ָ�룻��CDC�ඨ��һ������   
						�������ӵ��HDC�����һ���豸������ͬʱҲ��   
						����HDC��صĲ����ĺ����� */  


	BYTE *tData;
	int width=0;
	//����ͼ�δ�С
	//if (Interval == 0)
	//	Interval=size;
	//int iWidth = (Outtext.GetLength() * ( size/2+(Interval>0?1:0))+7) / 8  * 8;
	int iWidth = (Outtext.GetLength() * ( size+Interval )-7) / 8  * 4 +8;
	//int iWidth = (Outtext.GetLength() * ( size/2+Interval ));

	//int iWidth = (Outtext.GetLength() * ( size/2+Interval)+7) / 8  * 8;
	int iHeight =size ;
	int iPixel = 24;
	//ͼ�θ�ʽ����
	LPBITMAPINFO lpbmih = new BITMAPINFO;//ͼ�θ�ʽ����
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = iWidth;//
	lpbmih->bmiHeader.biHeight = iHeight;
	lpbmih->bmiHeader.biPlanes = 1;//Ŀ���豸λ����������Ϊ1
	lpbmih->bmiHeader.biBitCount = iPixel;//��ɫ���
	lpbmih->bmiHeader.biCompression = BI_RGB;//λͼѹ������
	lpbmih->bmiHeader.biSizeImage = 0;//��С���ֽڵ�λ
	lpbmih->bmiHeader.biXPelsPerMeter = 0;//ˮƽ�ֱ���
	lpbmih->bmiHeader.biYPelsPerMeter = 0;//��ֱ�ֱ���
	lpbmih->bmiHeader.biClrUsed = 0;//λͼʵ��ʹ����ɫ������ɫ��
	lpbmih->bmiHeader.biClrImportant = 0;//λͼ��ʾ�бȽ���Ҫ����ɫ��


	hdc = CreateIC(TEXT("DISPLAY"),NULL,NULL,NULL);//���԰����ķ���ֵ���� HDC, ��������HDC�� ��ָ���Զ������в�ѯ������
	hdcMem = CreateCompatibleDC(hdc);//�ڴ�dc��
	hBitMap = CreateDIBSection(hdcMem,lpbmih,DIB_PAL_COLORS,(void **)&tData,NULL,0);//����һ��DIB
	if (color==0)
		memset(&tData[0],255,iHeight * iWidth * 3);
	pBitMap = new CBitmap;
	pBitMap->Attach(hBitMap);//��λͼ����
	pMemDC = new CDC;
	pMemDC->Attach(hdcMem);//��DC�ڴ����
	pMemDC->SelectObject(pBitMap);//ѡ���ڴ��ַ

	pMemDC->SetBkMode(TRANSPARENT);//����ģʽ
	pMemDC->SetTextColor( color);//������ɫ
	ftUser = CreateFont(size,//�����С
		0,//�ֿ�
		0,//����ָ��ת��ʸ����escapement vector������ʾ�����x��ļнǣ���0.1Ϊ��λ��
		00,//����ָ���ַ����ߺ�x��ļнǣ���0.1Ϊ��λ����
		//FW_DONTCARE,//������
		JiaCu,
		FALSE, //����6��������bItalic����ָ�������Ƿ�Ϊб��Ĳ���ֵ
		FALSE,//��7��������bUnderline����ָ�������Ƿ�����»��ߵĲ���ֵ
		FALSE,//��8��������cStrickout����ָ���ַ��Ƿ����ɾ���ߵĲ���ֵ
		GB2312_CHARSET,//ָ����������弯
		OUT_DEFAULT_PRECIS,//����ָ������������ȷ��
		CLIP_DEFAULT_PRECIS,//������ȷ��
		DEFAULT_QUALITY,//�������������
		DEFAULT_PITCH|FF_MODERN,//�ַ������ַ�
		ZiTi//"����"//�������������
		); 
	(HFONT) SelectObject(hdcMem, ftUser);//ѡ����ʽ

	//��ͼƬ���������
	CRect rc;//�ͻ���
	rc.right=iWidth;
	rc.bottom=iHeight;
	rc.left =1;
	rc.top = 0;
	CString s;
	char* ss= Outtext.GetBuffer(Outtext.GetLength());
	int t=0;//�ֽ�

	while ( t < Outtext.GetLength())
	{
		if(ss[t] <0)     //�Ǻ��� 
		{
			pMemDC->DrawText( Outtext.Mid(t,2), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			if (width== 0)
				rc.left=rc.left+size+Interval; 
			else
				rc.left=rc.left + width *2 + Interval;
			t++; 

		} 
		else 
		{
			pMemDC->DrawText( Outtext.Mid(t,1), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			if (width== 0)
				rc.left=rc.left+ size / 2 +IntervalE; 
			else
				rc.left=rc.left + width *2 + IntervalE;
		} 

		t++;
		//num++;
	}

	//pMemDC->DrawText( Outtext, &rc, DT_SINGLELINE|DT_LEFT );//����ı� 

	pMemDC->SelectObject(pBitMap);

	//AfxMessageBox("ͼƬ���ɳɹ���");
	/////////////////////////////////////�����ַ�ͼƬ
	//BITMAPFILEHEADER bmfh;
	////bmfh.bfType=0x4D42;//������BMP�ļ�(�ַ���"BM")
	//*((char *)&bmfh.bfType) = 'B';
	//*(((char *)&bmfh.bfType) + 1) = 'M';//������BMP�ļ�
	//bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//ƫ��λ��
	//bmfh.bfSize = bmfh.bfOffBits + (iWidth * iHeight) * iPixel / 8;//�ļ���С
	////TCHAR szBMPFileName[128];//�ļ�����
	//int iBMPBytes = iWidth * iHeight * iPixel / 8;//�����ļ���С
	//CFile file;
	//if(file.Open("testRole.bmp",CFile::modeWrite | CFile::modeCreate))
	//{
	//	file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
	//	file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
	//	file.Write(tData,iBMPBytes);
	//	file.Close();
	//}
	//else
	//{
	//	AfxMessageBox("�޷����ɣ���ȷ������·���Ƿ���ȷ");
	//	return PT;
	//}
	BYTE r, g, b, now_r, now_g, now_b;
	if (!GetDCData(hWnd))
		return PT;
	//GetDCData(hWnd);


	if ((right==0) || (right>rect.Width()))
		right=rect.Width();

	if ((bottom==0) ||(bottom>rect.Height()))
		bottom=rect.Height();


	BOOL fFalse;
	//ѭ���Ƚ�
	for (int y=top; y<bottom-iHeight; y++)
	{
		for (int x=left; x<right-iWidth; x++)
		{
			fFalse=TRUE;
			for (int i=0;i<iHeight-1;i++)//������һ�в�ƥ��,��Ϊ��ɫ��������һ�п���û��
			{
				if (!fFalse)
					break;
				for (int j=1;j<rc.left;j++)
				{
					if (!fFalse)
						break;
					if(x==125 && y==76)
						std::cout<<"";

					b=tData[ (iHeight-i-1) * (iWidth *3)+ j*3];
					g=tData[ (iHeight-i-1) * (iWidth *3)+ j*3+1];
					r=tData[ (iHeight-i-1) * (iWidth *3)+ j*3+2];

					/*b=tData[ 0];
					g=tData[ 1];
					r=tData[ 2];*/


					now_r = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +2];
					now_g = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +1];
					now_b = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +0];

					if (((abs(now_r - r)<xiangshi)&&(abs(now_g - g)<xiangshi)&&(abs(now_b -b)<xiangshi)))
					{
						fFalse=TRUE;

					}
					else if((b==tData[0]) && (g==tData[1])&&( r==tData[2]))
					{
						if (((abs(now_r - GetRValue(color))<xiangshi)&&(abs(now_g - GetGValue(color))<xiangshi)&&(abs(now_b -GetBValue(color))<xiangshi)))
						{
							fFalse=FALSE;
						}
						else
							fFalse=TRUE;
					}
					else
						fFalse=FALSE;
				}

			}
			if (fFalse==TRUE) 
			{
				;
				PT.x = x;
				PT.y = y;
				//delete pData;

				delete pMemDC; pMemDC = NULL;
				delete pBitMap; pBitMap = NULL;
				delete lpbmih; lpbmih = NULL;
				return PT;
			}
		}
	}
	//delete pData;
	delete pMemDC; pMemDC = NULL;
	delete pBitMap; pBitMap = NULL;
	delete lpbmih; lpbmih = NULL;
	return PT;
}
//�����
void CMyClass::ActiveWindow( HWND hActiveWnd )
{
	if ( !IsWindow(hActiveWnd) ) 
		return;

	if ( IsIconic(hActiveWnd) )
	{
		/////////////���������Ϸ��С��ʱ�޷����������(������3�˺��������)//////////////
		/*BringWindowToTop(hActiveWnd);
		SetForegroundWindow(hActiveWnd);
		SetActiveWindow(hActiveWnd);
		ShowWindow( hActiveWnd, SW_SHOW );*/
		//////////////////////////////////////////////////////////////////////////////////
		ShowWindow(hActiveWnd, SW_RESTORE);
		Sleep(300);
	}
	::SetForegroundWindow( hActiveWnd );
	Sleep(300);
	HWND hwndForeground = GetForegroundWindow();
	for(int i=0;i<3;i++)
	{
		if( hwndForeground == hActiveWnd)
			break;
		Sleep(300);
		hwndForeground = GetForegroundWindow();
	}

	if( hwndForeground != hActiveWnd)
	{
		DWORD dwThreadIdAttachTo = GetWindowThreadProcessId(hwndForeground, NULL);
		DWORD dwThreadIdAttachToB = GetWindowThreadProcessId(hActiveWnd, NULL);
		if( dwThreadIdAttachTo != dwThreadIdAttachToB )
		{
			AttachThreadInput(dwThreadIdAttachToB, dwThreadIdAttachTo, TRUE);
			BringWindowToTop(hActiveWnd);
			SetForegroundWindow(hActiveWnd);
			SetActiveWindow(hActiveWnd);
			GetWindowThreadProcessId(hActiveWnd, NULL);
			AttachThreadInput(dwThreadIdAttachToB, dwThreadIdAttachTo, FALSE);
		}
	}

}
//�޸�ͼƬ����(����Ϊ��������)
void CMyClass::ChangeBgColor(CString strPic,BYTE pixelS,BYTE pixelE)
{
	::HBITMAP PicH=  (HBITMAP) LoadImage(NULL,strPic,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	BITMAP bm; 
	::GetObject(PicH,sizeof(BITMAP),&bm); 
	//ȡ�������飬�����Ϊ������ÿ��β�Զ���1�ֽڲ��㣬��Ҫȡ�����,�Ǵ����Ͻǿ�ʼȡ��
	BYTE *tData=new BYTE[(bm.bmWidth * 4 + 3)/4*4* bm.bmHeight];
	BYTE *tData1=new BYTE[(bm.bmWidth * 4 + 3)/4*4* bm.bmHeight];
	int nBitsPixel=32;
	GetBitmapBits (PicH,(bm.bmWidth * (nBitsPixel/8)+3)/4*4*bm.bmHeight,tData);
	//POINT pt;

	ZeroMemory(tData1,(bm.bmWidth * 4 + 3)/4*4* bm.bmHeight);
	//�������������Ƿ���,��������
	int num=(bm.bmWidth * (nBitsPixel/8)+3)/4*4*(bm.bmHeight-1);
	int num1=0;
	for(int i=0;i<bm.bmHeight;i++)
	{
		num=(bm.bmWidth * (nBitsPixel/8)+3)/4*4*(bm.bmHeight-1-i);
		for(int j=0;j<bm.bmWidth;j++)
		{
			tData1[num1+2]=tData[num+2];
			tData1[num1+1]=tData[num+1];
			tData1[num1+0]=tData[num+0];
			num+=nBitsPixel/8;
			num1+=nBitsPixel/8;
		}
	}
	delete []tData;
	tData=NULL;
	//num=0;
	//for(int i=0;i<bm.bmHeight;i++)
	//{
	//	for(int j=0;j<bm.bmWidth;j++)
	//	{
	//		pt.x=i;
	//		pt.y=j;
	//		//���λ�ھ����ı�֮�ڣ�����εĶ������������ϣ�
	//		//����Ϊ���ھ����ڲ�����λ�ھ��ε��Ҳ��ײ����ߣ�����Ϊ���ھ����ڲ�
	//		if(!PtInRect(rect1,pt) && !PtInRect(rect2,pt))//���Ǳ�������,�ı���ɫ
	//		{
	//			tData1[(bm.bmWidth * (nBitsPixel/8)+3)/4*4*(bm.bmHeight-1-i)+j*(nBitsPixel/8)+2]=255;
	//			tData1[(bm.bmWidth * (nBitsPixel/8)+3)/4*4*(bm.bmHeight-1-i)+j*(nBitsPixel/8)+1]=0;
	//			tData1[(bm.bmWidth * (nBitsPixel/8)+3)/4*4*(bm.bmHeight-1-i)+j*(nBitsPixel/8)+0]=0;
	//		}
	//		//num+=nBitsPixel/8;
	//	}
	//}
	BITMAPINFOHEADER BMPHeader;
	BMPHeader.biSize=sizeof(BITMAPINFOHEADER);
	BMPHeader.biWidth = bm.bmWidth;//
	BMPHeader.biHeight = bm.bmHeight;
	BMPHeader.biPlanes = 1;//Ŀ���豸λ����������Ϊ1
	BMPHeader.biBitCount = 32;//��ɫ���
	BMPHeader.biCompression = BI_RGB;//λͼѹ������
	BMPHeader.biClrUsed=0;



	BITMAPFILEHEADER bmfh;
	bmfh.bfType=0x4D42;//������BMP�ļ�(�ַ���"BM")
	//*((char *)&bmfh.bfType) = 'B';
	//*(((char *)&bmfh.bfType) + 1) = 'M';//������BMP�ļ�
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//ƫ��λ��

	int iBMPBytes = (bm.bmWidth * 4+3)/4*4*bm.bmHeight;//�����ļ���С,��λ�ֽ�,������4�ı���
	bmfh.bfSize = bmfh.bfOffBits + iBMPBytes;//�ļ���С

	CFile file;
	BOOL bSuccess = file.Open("F:\\�޸ı���.bmp",CFile::modeWrite | 
		CFile::shareExclusive | CFile::modeCreate,NULL);
	if(bSuccess)
	{
		file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
		file.Write(&BMPHeader,sizeof(BITMAPINFOHEADER));
		file.Write(tData1,iBMPBytes);
		file.Close();

	}
}
//��ȡ��������,ֱ�Ӷ�ȡ,������
CString CMyClass::GetURLContent(CString url)
{
	if (url.IsEmpty())
		return NULL;
	CString content="";
	CInternetSession session("HttpClient");
	//url = "http://www.ip138.com/ips.asp";
	CHttpFile *pfile = (CHttpFile *)session.OpenURL(url);
	DWORD dwStatusCode;
	pfile -> QueryInfoStatusCode(dwStatusCode);
	if(dwStatusCode == HTTP_STATUS_OK)
	{
		CString data;
		//���л�ȡ
		while (pfile -> ReadString(data))
			content = content+data+"\r\n";
	}
	else
	{
		/*::MessageBox(NULL,_T("������������"),_T("error"),MB_OK);*/
		WriteToFile("����������ַ���ִ���\r\n");
	}
	pfile -> Close();
	delete pfile;
	session.Close();
	return content;
}
//�����ַ���(�ް��)
void CMyClass::SendKeys(CString input,int waitTime)
{
	for(int i=0;i<input.GetLength();i++)
	{
		char ch=input.GetAt(i);
		if(ch<0)
		{
			AfxMessageBox("�ú���ֻ���������ַ�");
			return;
		}
		SHORT ks = VkKeyScan(ch);
		BYTE key = ks & 0xFF;
		BOOL bShift = FALSE;

		if(ch==126 || ch==33 || ch==64 || ch==35 || ch==36 || ch==37 || 
			ch==94 || ch==38 || ch==42 || ch==40 || ch==41 || ch==95 ||
			ch==43 || ch==58 || ch==124 || ch==125 || ch==123 || ch==63 ||
			ch==62 || ch==60 || ch=='_' || ch=='+' || ch=='~')
		{
			bShift = TRUE;			
		}

		if(ch>=65 && ch<=90)
		{
			bShift = TRUE;
		}
		if(bShift)
		{
			keybd_event(VK_SHIFT,0,0,0); 
		}
		keybd_event(key,MapVirtualKey(key,0),0,0);
		Sleep(50);
		keybd_event(key,MapVirtualKey(key,0),KEYEVENTF_KEYUP,0);
		if(bShift)
		{
			keybd_event(VK_SHIFT,0,KEYEVENTF_KEYUP,0); 
		}
		Sleep(waitTime);
	}
}
//�����ַ�����,Ĭ�ϰ�һ��
void CMyClass::SendKeys(char key,int n)
{
	SHORT ks = VkKeyScan(key);
	BYTE key1 = ks & 0xFF;
	for(int i=0;i<n;i++)
	{
		keybd_event(key1,MapVirtualKey(key1,0),0,0);
		Sleep(50);
		keybd_event(key1,MapVirtualKey(key1,0),KEYEVENTF_KEYUP,0);
		Sleep(300);
	}
	return;
}
void CMyClass::SendFuncKey(char key,int n)
{

	for(int i=0;i<n;i++)
	{
		keybd_event(key,MapVirtualKey(key,0),0,0);
		Sleep(50);
		keybd_event(key,MapVirtualKey(key,0),KEYEVENTF_KEYUP,0);
		Sleep(300);
	}
	return;
}

//������ϼ�,1ctrl,2alt
void CMyClass::Send2Keys(char FuncKey,char ch)
{
	//CString input(ch);
	//input.MakeUpper();
	//ch=input.GetAt(0);
	SHORT ks = VkKeyScan(ch);
	BYTE key1 = ks & 0xFF;

	keybd_event(FuncKey,MapVirtualKey(FuncKey,0),0,0);
	Sleep(100);
	keybd_event(key1,MapVirtualKey(key1,0),0,0);
	Sleep(100);
	keybd_event(key1,MapVirtualKey(key1,0),KEYEVENTF_KEYUP,0);
	keybd_event(FuncKey,MapVirtualKey(FuncKey,0),KEYEVENTF_KEYUP,0);

}
//�����ַ���(֧��ȫ��,��Ҫ��ȡ�����ľ��,��������Ƶ������)
void CMyClass::SendString(CString str)
{
	HWND hWnd=WindowFromCursor();

	CStringIterator strIter(str);
	while (strIter.HasNext())
	{
		SendMessage(hWnd, WM_IME_CHAR, strIter.Next(), 0);
		Sleep(100);
	}
}
//���Ը�ʽ����messagebox
void CMyClass::ShowMessage ( CString strFormat, ...)
{
	CString strMessage;
	va_list pArgList ;
	va_start (pArgList, strFormat) ;
	_vsntprintf ( strMessage.GetBuffer(MAX_PATH), MAX_PATH,strFormat, pArgList);
	strMessage.ReleaseBuffer();
	va_end (pArgList) ;
	MessageBox(GetForegroundWindow(), strMessage, "��ʾ", MB_ICONINFORMATION);
}
//�޸�INI�ļ�������
BOOL CMyClass::WriteINIFile (int num,LPCTSTR fileName,...)
{
	va_list arg_ptr;
	va_start(arg_ptr,num);
	int i=0;
	while(i<num-1)
	{
		LPCTSTR n=va_arg(arg_ptr,LPCTSTR);
		std::cout<<"n="<<n<<std::endl;
		i++;
	}
	return TRUE;
}
//�������,��ȫʱ���
POINT CMyClass::FindBMPNumber(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,int left,int top,int right,int bottom,int Interval,int JiaCu,int xiangshi)
{

	if (JiaCu!=700)
		JiaCu=0;

	POINT PT;
	PT.x=-1;
	PT.y=-2;
	if (Outtext=="")
	{
		return PT;
	}

	CRect rect;
	if (::IsWindow(hWnd))
	{
		GetWindowRect (hWnd,&rect);
	}
	else
	{
		return PT;
	}


	//����λͼ����
	HDC hdc,hdcMem;//�豸
	HFONT ftUser;//�������;��
	HBITMAP hBitMap = NULL;//λͼҪ���豸��������Ҫת���ɴ�����
	CBitmap *pBitMap = NULL;//λͼ
	CDC *pMemDC = NULL; /*HDC����ı���ָ��һ���ڴ棬���   
						�ڴ���������һ���豸����ص����ݣ�����Ҳ����   
						��ΪHDC�������һ��ָ�룻��CDC�ඨ��һ������   
						�������ӵ��HDC�����һ���豸������ͬʱҲ��   
						����HDC��صĲ����ĺ����� */  


	BYTE *tData;
	//����ͼ�δ�С
	//if (Interval == 0)
	//	Interval=size;
	int iWidth = (Outtext.GetLength() * ( size/2+(Interval>0?1:0))+7) / 8  * 8;
	int iHeight =size + 1;
	int iPixel = 24;
	//ͼ�θ�ʽ����
	LPBITMAPINFO lpbmih = new BITMAPINFO;//ͼ�θ�ʽ����
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = iWidth;//
	lpbmih->bmiHeader.biHeight = iHeight;
	lpbmih->bmiHeader.biPlanes = 1;//Ŀ���豸λ����������Ϊ1
	lpbmih->bmiHeader.biBitCount = iPixel;//��ɫ���
	lpbmih->bmiHeader.biCompression = BI_RGB;//λͼѹ������
	lpbmih->bmiHeader.biSizeImage = 0;//��С���ֽڵ�λ
	lpbmih->bmiHeader.biXPelsPerMeter = 0;//ˮƽ�ֱ���
	lpbmih->bmiHeader.biYPelsPerMeter = 0;//��ֱ�ֱ���
	lpbmih->bmiHeader.biClrUsed = 0;//λͼʵ��ʹ����ɫ������ɫ��
	lpbmih->bmiHeader.biClrImportant = 0;//λͼ��ʾ�бȽ���Ҫ����ɫ��


	hdc = CreateIC(TEXT("DISPLAY"),NULL,NULL,NULL);//���԰����ķ���ֵ���� HDC, ��������HDC�� ��ָ���Զ������в�ѯ������
	hdcMem = CreateCompatibleDC(hdc);//�ڴ�dc��
	hBitMap = CreateDIBSection(hdcMem,lpbmih,DIB_PAL_COLORS,(void **)&tData,NULL,0);//����һ��DIB
	if (color==0)
		memset(&tData[0],255,iHeight * iWidth * 3);
	pBitMap = new CBitmap;
	pBitMap->Attach(hBitMap);//��λͼ����
	pMemDC = new CDC;
	pMemDC->Attach(hdcMem);//��DC�ڴ����
	pMemDC->SelectObject(pBitMap);//ѡ���ڴ��ַ

	pMemDC->SetBkMode(TRANSPARENT);//����ģʽ
	pMemDC->SetTextColor( color);//������ɫ
	ftUser = CreateFont(size,//�����С
		0,//�ֿ�
		0,//����ָ��ת��ʸ����escapement vector������ʾ�����x��ļнǣ���0.1Ϊ��λ��
		00,//����ָ���ַ����ߺ�x��ļнǣ���0.1Ϊ��λ����
		//FW_DONTCARE,//������
		JiaCu,
		FALSE, //����6��������bItalic����ָ�������Ƿ�Ϊб��Ĳ���ֵ
		FALSE,//��7��������bUnderline����ָ�������Ƿ�����»��ߵĲ���ֵ
		FALSE,//��8��������cStrickout����ָ���ַ��Ƿ����ɾ���ߵĲ���ֵ
		GB2312_CHARSET,//ָ����������弯
		OUT_DEFAULT_PRECIS,//����ָ������������ȷ��
		CLIP_DEFAULT_PRECIS,//������ȷ��
		DEFAULT_QUALITY,//�������������
		DEFAULT_PITCH|FF_MODERN,//�ַ������ַ�
		ZiTi//"����"//�������������
		); 
	(HFONT) SelectObject(hdcMem, ftUser);//ѡ����ʽ

	//��ͼƬ���������
	CRect rc;//�ͻ���
	rc.right=iWidth;
	rc.bottom=iHeight;
	rc.left =1;
	rc.top = 1;
	CString s;
	char* ss= Outtext.GetBuffer(Outtext.GetLength());
	int t=0;//�ֽ�
	int num=0;//�ַ�
	while ( t < Outtext.GetLength())
	{
		if(ss[t] <0)     //�Ǻ��� 
		{
			pMemDC->DrawText( Outtext.Mid(t,2), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			rc.left=rc.left+size;
			switch(Interval)
			{
			case 2:
				if(3==num || 7==num || 11==num || 15==num)
					rc.left++;
				break;
			case 3:
				if(2==num || 5==num || 7==num || 10==num || 13==num || 16==num)
					rc.left++;
				break;
			case 4:
				if(num%2==0)
					rc.left++;
				break;
			default:
				rc.left+=Interval; 
			}
			t++; 
		} 
		else 
		{
			pMemDC->DrawText( Outtext.Mid(t,1), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			rc.left=rc.left+size/2;
			switch(Interval)
			{
			case 2:
				if(3==num || 7==num || 11==num || 15==num)
					rc.left++;
				break;
			case 3:
				if(2==num || 5==num || 7==num || 10==num || 13==num || 16==num)
					rc.left++;
				break;
			case 4:
				if(num%2==0)
					rc.left++;
				break;
			default:
				rc.left+=Interval; 
			}
		} 

		t++;
		num++;
	}
	//pMemDC->DrawText( Outtext, &rc, DT_SINGLELINE|DT_LEFT );//����ı� 

	pMemDC->SelectObject(pBitMap);

	//AfxMessageBox("ͼƬ���ɳɹ���");
	///////////////////////////////////�����ַ�ͼƬ
	//BITMAPFILEHEADER bmfh;
	////bmfh.bfType=0x4D42;//������BMP�ļ�(�ַ���"BM")
	//*((char *)&bmfh.bfType) = 'B';
	//*(((char *)&bmfh.bfType) + 1) = 'M';//������BMP�ļ�
	//bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//ƫ��λ��
	//bmfh.bfSize = bmfh.bfOffBits + (iWidth * iHeight) * iPixel / 8;//�ļ���С
	////TCHAR szBMPFileName[128];//�ļ�����
	//int iBMPBytes = iWidth * iHeight * iPixel / 8;//�����ļ���С
	//CFile file;
	//if(file.Open("testRole.bmp",CFile::modeWrite | CFile::modeCreate))
	//{
	//	file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
	//	file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
	//	file.Write(tData,iBMPBytes);
	//	file.Close();
	//}
	//else
	//{
	//	AfxMessageBox("�޷����ɣ���ȷ������·���Ƿ���ȷ");
	//	return PT;
	//}
	BYTE r, g, b, now_r, now_g, now_b;
	if (!GetDCData(hWnd))
		return PT;
	//GetDCData(hWnd);


	if ((right==0) || (right>rect.Width()))
		right=rect.Width();

	if ((bottom==0) ||(bottom>rect.Height()))
		bottom=rect.Height();


	BOOL fFalse;
	//ѭ���Ƚ�
	for (int y=top; y<bottom-iHeight; y++)
	{
		for (int x=left; x<right-iWidth; x++)
		{
			fFalse=TRUE;
			for (int i=1;i<iHeight-1 ;i++)
			{
				if (!fFalse)
					break;
				for (int j=1;j<iWidth-1;j++)
				{
					if (!fFalse)
						break;
					b=tData[ (iHeight-i-1) * (iWidth *3)+ j*3];
					g=tData[ (iHeight-i-1) * (iWidth *3)+ j*3+1];
					r=tData[ (iHeight-i-1) * (iWidth *3)+ j*3+2];

					now_r = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +2];
					now_g = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +1];
					now_b = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +0];
					if (((abs(now_r - r)<xiangshi)&&(abs(now_g - g)<xiangshi)&&(abs(now_b -b)<xiangshi)))
					{
						fFalse=TRUE;

					}
					else if((b==tData[0]) && (g==tData[1])&&( r==tData[2]))
					{
						if (((abs(now_r - GetRValue(color))<xiangshi)&&(abs(now_g - GetGValue(color))<xiangshi)&&(abs(now_b -GetBValue(color))<xiangshi)))
							fFalse=FALSE;
						else
							fFalse=TRUE;
					}
					else
						fFalse=FALSE;
				}

			}
			if (fFalse==TRUE) 
			{
				;
				PT.x = x;
				PT.y = y;
				//delete pData;

				delete pMemDC; pMemDC = NULL;
				delete pBitMap; pBitMap = NULL;
				delete lpbmih; lpbmih = NULL;
				return PT;
			}
		}
	}
	//delete pData;
	delete pMemDC; pMemDC = NULL;
	delete pBitMap; pBitMap = NULL;
	delete lpbmih; lpbmih = NULL;
	return PT;
}
//�������,��ȫʱ���
CString CMyClass::GetNumberInBMP(COLORREF color,int left,int top,int right,int bottom,CString ziti,int size,int interval)
{
	CString strNum,strResult="";
	POINT pt;

	//int interval=0,size=12,left=236,top=403,right=270,bottom=422;
	int nLast=1280;//��������ǰһ���ֵ�����

	for (int j=0;j<right;)
	{
		//cout<<"left="<<left<<endl;
		j=left+size-2;//ÿ�β��Ҳ��������ַ��ķ�Χ
		for(int i=0;i<10;i++)
		{
			strNum.Format("%d",i);
			pt=FindBMPNumber(GetForegroundWindow(),ziti,strNum,size,color,left,top,j,bottom);
			//pt=FindBMPText(GetForegroundWindow(),ziti,strNum,size,color,left,top,j,bottom);
			if (pt.x>0 && pt.y>0)
			{
				if(pt.x-nLast>size/2+interval)//��ǰһ���ֵļ�����һ���ַ�,����
					return strResult;
				nLast=pt.x;
				left=pt.x+size/2-1;
				//cout<<strNum<<endl;
				//cout<<"pt.x="<<pt.x<<endl;
				strResult+=strNum;
				break;
			}
			else if(i==9)
				left++;
		}
	}

	return strResult;
}
//ע����л�ȡ�ѹ����뷨·��
CString CMyClass::GetSougouPath(void)
{
	HKEY hKey;
	DWORD type=REG_SZ;
	DWORD len=MAX_PATH;
	CString KeyValue,KeyValue1;
	if (ERROR_SUCCESS == RegOpenKeyEx (HKEY_LOCAL_MACHINE,"SOFTWARE\\SogouInput",NULL,KEY_READ,&hKey))
	{
		long ret1=::RegQueryValueExA(hKey, "",NULL, &type , (BYTE *)KeyValue.GetBuffer(len), &len); 
		KeyValue.ReleaseBuffer();
		len=MAX_PATH;
		RegQueryValueExA(hKey, "Version",NULL, &type , (BYTE *)KeyValue1.GetBuffer(len), &len);
		KeyValue1.ReleaseBuffer();
		RegCloseKey(hKey);
		return KeyValue+"\\"+KeyValue1;
	}
	else
	{
		AfxMessageBox("�����ѹ�·��ʧ��");
	}
	RegCloseKey(hKey);
	return KeyValue;
}
//���ݽ������򴰿ڱ����ȡ·��
CString CMyClass::GetOtherProgPath(CString strTitle,CString strProcessName)
{
	CString strPath="";
	DWORD dwPid;
	HANDLE pHandle;
	if(strTitle.IsEmpty() && strProcessName.IsEmpty())
		return strPath;

	if(!strTitle.IsEmpty())
	{
		HWND hWnd = ::FindWindow(NULL,strTitle);
		if (hWnd)
		{
			GetWindowThreadProcessId(hWnd, &dwPid);
			pHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, dwPid);
			::GetModuleFileNameEx(pHandle,NULL,strPath.GetBuffer(MAX_PATH),MAX_PATH);
			strPath.ReleaseBuffer();
			strPath = strPath.Left(strPath.ReverseFind('\\'));		
		}
	}
	else
	{
		dwPid=GetProcessIdByName(strProcessName);
		if (0==dwPid)
			return strPath;
		pHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, dwPid);
		::GetModuleFileNameExA(pHandle,NULL,strPath.GetBuffer(MAX_PATH),MAX_PATH);
		strPath.ReleaseBuffer();
		strPath = strPath.Left(strPath.ReverseFind('\\'));
	}
	::CloseHandle(pHandle);
	return strPath;
}
//�޸������ļ�������(�����޸�,û�������)
BOOL CMyClass::CreateTextFile (CString FileName,CString FileContent)
{
	CFile file;
	BOOL bSuccess = file.Open( FileName, CFile::modeCreate |  CFile::modeWrite , NULL ) ;
	if ( !bSuccess ) 
		return FALSE;

	file.Write( FileContent, FileContent.GetLength() );
	file.Close();

	return TRUE;
}
//���ַ����е�pt.x,pt.y�滻�����һ�ε����ֻ���ͼ������
BOOL CMyClass::PickXY(CString &strScr)
{
	POINT pt,ptInGame;
	ptInGame.x=100;
	ptInGame.y=100;
	pt.x=-1;
	pt.y=-2;
	static CStringArray tmpScr;

	strScr=strScr.Right (strScr.GetLength ()-strScr.Find ("=")-1);
	SplitString(strScr,",",tmpScr);

	strScr="";
	for(int i=0;i<tmpScr.GetSize();i++)
	{
		if (tmpScr[i].Find("pt.x")>=0)
			tmpScr[i].Format("%d",ptInGame.x+atoi(tmpScr[i].Mid(tmpScr[i].Find("pt.x")+4)));
		else if (tmpScr[i].Find("pt.y")>=0)
			tmpScr[i].Format("%d",ptInGame.y+atoi(tmpScr[i].Mid(tmpScr[i].Find("pt.y")+4)));
		strScr+=tmpScr[i]+",";

	}

	return TRUE;
}
int CMyClass::SplitString(CString strScr,CString strFG,CStringArray &strArray)
{

	int n=0;
	int num=0;
	while(1)
	{
		n=strScr.Find (strFG);
		if (n<0)
		{
			strArray.Add(strScr);
			break;
		}
		strArray.Add(strScr.Left(n));
		strScr=strScr.Right(strScr.GetLength()-n-1);
		if(strScr.IsEmpty())
			break;
		num++;
	}
	return num;
}
void CMyClass::CheckIME()
{
	cout<<"�Ѱ�װ�����뷨��:"<<endl;
	HKEY hKey,hKey1;
	DWORD cp = 16;
	char  lp[15];
	CString szID;
	CString szKeyName, szKeyName1;
	szKeyName	= "Keyboard Layout\\Preload";
	szKeyName1	= "System\\CurrentControlSet\\Control\\Keyboard Layouts\\";

	int	 i = 1;
	szID.Format("%d", i);

	DWORD lpT = REG_SZ;
	if(::RegOpenKey(HKEY_CURRENT_USER,szKeyName,&hKey)==ERROR_SUCCESS)
	{
		while(::RegQueryValueEx(hKey, szID, NULL, &lpT, (LPBYTE)lp, &cp)==ERROR_SUCCESS)
		{
			CString szTempName;
			szTempName = szKeyName1 + (LPCTSTR)(LPTSTR)lp;
			if(RegOpenKey(HKEY_LOCAL_MACHINE,szTempName,&hKey1)==ERROR_SUCCESS)
			{
				char lpD[100];
				DWORD lpS = 100;
				if(RegQueryValueExA(hKey1,"Layout Text",NULL,&lpT,(LPBYTE)lpD,&lpS)==ERROR_SUCCESS)
				{
					CString strID, strName;
					strID.Format("%s",lp);
					strName.Format("%s", lpD );
					cout<<strName<<endl;
					if(strID=="00000804")
					{
						meishiID=strID;

					}
					if ( strName.Find ( "�ѹ�ƴ��", 0 ) != -1 )
					{
						sougouID=strID;
					}

				}
			}
			::RegCloseKey(hKey1);
			i++;
			szID.Format("%d",i);
		}
	}
	::RegCloseKey(hKey);
}
//�ر����뷨
BOOL CMyClass::SelectIME(LPCTSTR strID)
{

	if(strID=="")
	{
		ShowMessage("û�а�װ�����뷨");
		return FALSE;
	}
	HKL	hkl, hkl1;	
	DWORD pId; 
	hkl=LoadKeyboardLayoutA(strID,KLF_ACTIVATE);//װ�ؼ����������뷨
	pId = GetWindowThreadProcessId(GetForegroundWindow(),NULL);   
	hkl1 = GetKeyboardLayout(pId);//��ȡ���㴰��ʹ�õ����뷨
	int i=0;
	while(hkl!=hkl1)
	{
		if(i>1)
			return FALSE;
		::SendMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, (WPARAM)TRUE, (LPARAM)hkl);
		Sleep(300);
		hkl1 = GetKeyboardLayout(pId);
		i++;
	}
	return TRUE;
}
//����������,MOUSE_MOVE
void CMyClass::PressMouseKey( HWND hWnd , int x, int y,int nTimes ,int nWaitTime)
{
	if ( hWnd == NULL )
		hWnd=::GetForegroundWindow();

	if ( (x!=0) && (y!=0) )
	{

		MoveMouse(hWnd,x,y);
		//::SetCursorPos(rcView.left + x, rcView.top + y);
	}

	for(int i=0;i<nTimes;i++)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		Sleep(200);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
		Sleep(nWaitTime);
	}
}
//����������,SetCursorPos
void CMyClass::PressMouse( HWND hWnd , int x, int y,int nTimes ,int nWaitTime)
{
	if ( hWnd == NULL )
		hWnd=::GetForegroundWindow();


	RECT rect;
	GetWindowRect (hWnd,&rect);
	SetCursorPos(rect.left + x, rect.top + y);


	for(int i=0;i<nTimes;i++)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		Sleep(500);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
		Sleep(nWaitTime);
	}
}
//2,����;3����;
void CMyClass::PressMouseKey( int type)
{
	if(type==2)
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
	else if(type==3)
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);	
}
//��������Ҽ�
void CMyClass::PressRightKey( HWND hWnd , int x, int y, int nTimes )
{
	if ( hWnd == NULL )
		hWnd=::GetDesktopWindow();

	if ( (x>0) && (y>0) )
	{
		CRect rcView;
		::GetWindowRect( hWnd, &rcView );
		::SetCursorPos(rcView.left + x, rcView.top + y);
		Sleep(1000);
	}

	for(int i=0;i<nTimes;i++)
	{
		mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
		Sleep(200);
		mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
		Sleep(100);
	}
	mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
}
//���ݽ��̺źͲ��ֱ�������ȡ���
HWND CMyClass::GetHwndByPid(DWORD Pid,CString PartTitle)
{
	if(Pid==0 && PartTitle=="")
		return 0;
	PartTitle.MakeUpper();
	CString tmp;
	char strTitle[64];
	HWND hwnd;
	DWORD lngProcID;
	hwnd = ::GetDesktopWindow();
	hwnd = ::GetWindow(hwnd, GW_CHILD);

	CRect rect;
	CString strLog;

	while (hwnd)
	{
		if (IsWindowVisible(hwnd))
		{
			GetWindowText (hwnd, strTitle, 63);
			tmp.Format ("%s",strTitle);
			if (tmp.MakeUpper().Find (PartTitle) >= 0)
			{
				::GetWindowRect(hwnd,&rect);
				//����С��200*200ʱ�Զ����ԣ�������С��ʱrect��Ϊ������Ҫ�������ۡ�
				if ((rect.right-rect.left)<200 || (rect.bottom-rect.top)<200)
				{
					if (!(rect.left<0 && rect.top<0 && rect.right<0 && rect.bottom<0))//������С�����ڣ��������ҡ�
					{
						hwnd = ::GetWindow(hwnd, GW_HWNDNEXT);
						continue;
					}
				}
				if(Pid>0)//��Ҫƥ����̺�
				{
					::GetWindowThreadProcessId (hwnd, &lngProcID);
					if (lngProcID==DWORD(Pid))
						return hwnd;
				}
				else
					return hwnd;
			}
		}
		hwnd = ::GetWindow(hwnd, GW_HWNDNEXT);
	}
	return 0;
}
//���ݽ�������ȡ���
HWND CMyClass::GetHwndByPid(CString ProcessName)
{
	if(ProcessName=="")
		return 0;
	DWORD lngProcID=GetProcessIdByName(ProcessName);
	return GetHwndByPid(lngProcID);
}

CString CMyClass::SetPicPath(CString str,CString OrdNo)
{
	CString strFileName;

	CTime t = CTime::GetCurrentTime();
	strFileName.Format("%s%04d_%02d\\%02d\\%s\\",str,t.GetYear(), t.GetMonth(), t.GetDay(),OrdNo);
	str =strFileName;
	MakeSureDirectoryPathExists(str);
	//#pragma   comment(lib, "imagehlp.lib")
	//#include <imagehlp.h>

	//strFileName.Format( _T("%04d%02d\\%02d\\%s\\%02d%02d%02d.bmp"),
	//t.GetYear(), t.GetMonth(), t.GetDay(),OrdNo,t.GetHour(), t.GetMinute(), t.GetSecond() );
	strFileName.Format("%02d%02d%02d.bmp",t.GetHour(), t.GetMinute(), t.GetSecond() );
	//if MakeSureDirectoryPathExists()

	return str + strFileName;
}
int CMyClass::getcolor(HWND hwnd,COLORREF color,int left,int top,int right,int bottom,int xiangshi)
{
	int nResult=0;
	CRect rect;
	if (::IsWindow(hwnd))
	{
		GetWindowRect (hwnd,&rect);
	}
	else
	{
		return nResult;
	}
	BYTE r,g,b,now_r, now_g, now_b;
	r=GetRValue(color);
	g=GetGValue(color);
	b=GetBValue(color);

	GetDCData(hwnd);
	for (int y=top; y<bottom; y++)
	{
		for (int x=left; x<right; x++)
		{


			now_r = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y -1) + x*3 +2];
			now_g = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y -1) + x*3 +1];
			now_b = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y -1) + x*3 +0];
			if (((abs(now_r - r)<xiangshi)&&(abs(now_g - g)<xiangshi)&&(abs(now_b -b)<xiangshi)) )
				nResult++;

		}
	}

	return nResult;

}
BOOL CMyClass::IsProcessID(DWORD dwPid)
{
	//CString strLog;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) 
	{
		return NULL;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL fOk;
	CString  str_szExeFile,str_pName;
	for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe))
	{

		if (pe.th32ProcessID == dwPid)
		{
			CloseHandle(hSnapshot);
			return TRUE;
		}
	}
	CloseHandle(hSnapshot);
	//strLog.Format("����[%d]�Ѿ�������\r\n",dwPid);
	//WriteToFile(strLog);
	return FALSE;
}
POINT CMyClass::FindBmpBlackWhite (HWND hWnd,CString Pic,int left ,int top ,int right ,int bottom,int xiangshi)
{


	POINT PT;
	PT.x=-1;
	PT.y=-2;
	if (_access(Pic,0)==-1 || !IsWindow(hWnd))
		return PT;
	/*if (!IsWindowVisible(hWnd) || ::IsIconic(hWnd))
	return PT;*/



	BYTE r, g, b, now_r, now_g, now_b;

	CRect rect;
	::GetWindowRect( hWnd, &rect );



	if ((right==0) || (right>rect.Width()))
		right=rect.Width();

	if ((bottom==0) ||(bottom>rect.Height()))
		bottom=rect.Height();

	if (!GetDCData(hWnd))
		return PT;

	//COLORREF color;
	//�Ա�ͼƬװ������
	::HBITMAP PicH=  (HBITMAP) LoadImage(NULL,Pic,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	BITMAP bm; 
	::GetObject(PicH,sizeof(BITMAP),&bm); 
	BYTE *tData;
	//ȡ�������飬�����Ϊ������ÿ��β�Զ���1�ֽڲ��㣬��Ҫȡ�����
	tData=new BYTE[ (bm.bmWidth * (bm.bmBitsPixel/8) + 3)/4*4* bm.bmHeight];
	long Ret1=GetBitmapBits (PicH,bm.bmWidth * bm.bmHeight * (bm.bmBitsPixel/8),&tData[0]);
	BOOL fFalse;
	//ѭ���Ƚ�
	for (int y=top; y<bottom-bm.bmHeight-1; y++)
	{
		for (int x=left; x<right-bm.bmWidth-1; x++)
		{

			fFalse=TRUE;
			for (int i=0;i<bm.bmHeight-1 ;i++)
			{
				if (!fFalse)
					break;
				for (int j=0;j<bm.bmWidth-1;j++)
				{
					if (!fFalse)
						break;
					//ȡ�������飬�����Ϊ������ÿ��β�Զ���1�ֽڲ��㣬��Ҫȡ�����
					/*b=tData[(i*bm.bmWidth +j)*3];
					g=tData[(i*bm.bmWidth +j)*3+1];
					r=tData[(i*bm.bmWidth +j)*3+2];*/
					b=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3];
					g=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+1];
					r=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+2];

					if(b>100 || g>100 || r>100)//��ԭͼ�󲿷���Ϊ0,0�Ĳ��ֺ���
						b=0,g=0,r=0;
					else
						b=255,g=255,r=255;
					now_r = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +2];
					now_g = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +1];
					now_b = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +0];

					if(now_b>100 && now_g>100 && now_r>100)//����Ϸ���ڴ󲿷���Ϊ255
						now_b=0,now_g=0,now_r=0;
					else
						now_b=255,now_g=255,now_r=255;

					if (b==0 || (now_r == r && now_g == g && now_b ==b) )
						fFalse=TRUE;
					else
						fFalse=FALSE;
				}

			}
			if (fFalse==TRUE) 
			{
				PT.x = x;
				PT.y = y;

				delete []tData;
				::DeleteObject (PicH);
				return PT;
			}
		}
	}


	delete []tData;
	::DeleteObject (PicH);
	return PT;
}



//�Ƚ�ָ������ɫ,�����ط���������ָ����ɫ
int CMyClass::GetBmpNum (HWND hWnd,CString Pic,CArray<CPoint,CPoint&> &ptArray,int left ,int top ,int right ,int bottom,int xiangshi,BOOL bColor,COLORREF color)
{

	int nResult=0;
	CPoint PT(-1,-2);
	if (_access(Pic,0)==-1 || !IsWindow(hWnd))
		return nResult;
	if (!IsWindowVisible(hWnd) || ::IsIconic(hWnd))
		return nResult;



	BYTE r, g, b, now_r, now_g, now_b;

	CRect rect;
	::GetWindowRect( hWnd, &rect );



	if ((right==0) || (right>rect.Width()))
		right=rect.Width();

	if ((bottom==0) ||(bottom>rect.Height()))
		bottom=rect.Height();

	if (!GetDCData(hWnd))
		return nResult;

	//COLORREF color;
	//�Ա�ͼƬװ������
	::HBITMAP PicH=  (HBITMAP) LoadImage(NULL,Pic,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	BITMAP bm; 
	::GetObject(PicH,sizeof(BITMAP),&bm); 
	BYTE *tData;
	//ȡ�������飬�����Ϊ������ÿ��β�Զ���1�ֽڲ��㣬��Ҫȡ�����
	tData=new BYTE[ (bm.bmWidth * (bm.bmBitsPixel/8) + 3)/4*4* bm.bmHeight];
	long Ret1=GetBitmapBits (PicH,bm.bmWidth * bm.bmHeight * (bm.bmBitsPixel/8),&tData[0]);
	BOOL fFalse;
	ptArray.SetSize(0);
	//ѭ���Ƚ�
	for (int y=top; y<bottom-bm.bmHeight-1; y++)
	{
		for (int x=left; x<right-bm.bmWidth-1; x++)
		{
			fFalse=TRUE;
			for (int i=0;i<bm.bmHeight-1 ;i++)
			{
				if (!fFalse)
					break;
				for (int j=0;j<bm.bmWidth-1;j++)
				{
					if (!fFalse)
						break;
					/*if(x==745 && y==127)
					std::cout<<"";*/
					//ȡ�������飬�����Ϊ������ÿ��β�Զ���1�ֽڲ��㣬��Ҫȡ�����
					b=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3];
					g=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+1];
					r=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+2];

					now_r = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +2];
					now_g = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +1];
					now_b = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +0];

					if(bColor)
					{
						if(abs(r-GetRValue(color))<xiangshi && abs(g-GetGValue(color))<xiangshi && abs(b-GetBValue(color))<xiangshi)
						{
							if (((abs(now_r - r)<xiangshi)&&(abs(now_g - g)<xiangshi)&&(abs(now_b -b)<xiangshi)) )
								fFalse=TRUE;
							else
								fFalse=FALSE;
						}
						else
						{
							if (((abs(now_r - GetRValue(color))<xiangshi)&&(abs(now_g - GetGValue(color))<xiangshi)&&(abs(now_b -GetBValue(color))<xiangshi)) )
								fFalse=FALSE;
							else
								fFalse=TRUE;
						}
					}
					else
					{
						if (((abs(now_r - r)<xiangshi)&&(abs(now_g - g)<xiangshi)&&(abs(now_b -b)<xiangshi))  ||  ((b==tData[0]) && (g==tData[1])&&( r==tData[2]))  )
							fFalse=TRUE;
						else
							fFalse=FALSE;
					}
				}

			}
			if (fFalse==TRUE) 
			{
				PT.SetPoint(x,y);
				ptArray.Add(PT);
				nResult++;
				x+=bm.bmWidth;
				continue;
			}
		}
	}
	delete []tData;
	::DeleteObject (PicH);
	return nResult;
}
LPCTSTR CMyClass::GetCoin(HWND hwnd,CString PicPath,int left ,int top ,int right ,int bottom,COLORREF color)
{
	POINT pt;
	POINT ptArray[10];//ptArray.x����,y���ҵ�����
	CString strCoin="";
	GetDCData(hwnd);
	CString strPic;
	int num=0;
	for(int i=0;i<10;i++)
	{
		strPic.Format("%s\\%d.bmp",PicPath,i);
		int leftTemp=left;
		for(int j=0;j<10;j++)
		{
			if(FindBmp(hwnd,PicPath,&pt,color,leftTemp,top,right,bottom,FALSE))
			{
				leftTemp=pt.x+1;
				ptArray[num].x=pt.x;
				ptArray[num].y=i;
				num++;
			}
			else
				break;
		}
	}

	//����
	for(int i=0;i<num;i++)
	{
		for(int j=i+1;j<num;j++)
		{
			if(ptArray[i].x>ptArray[j].x)
			{
				pt.x=ptArray[i].x;
				pt.y=ptArray[i].y;
				ptArray[i].x=ptArray[j].x;
				ptArray[i].y=ptArray[j].y;
				ptArray[j].x=pt.x;
				ptArray[j].y=pt.y;
			}
		}
	}
	for(int i=0;i<num;i++)
	{
		CString strTemp;
		strTemp.Format("%d",ptArray[i].y);
		strCoin+=strTemp;
	}
	return strCoin;
}
int CMyClass::GetBmpNum (HWND hWnd,CString Pic,int left ,int top ,int right ,int bottom,int xiangshi)
{

	int nResult=0;
	if (_access(Pic,0)==-1 || !IsWindow(hWnd))
		return nResult;
	if (!IsWindowVisible(hWnd) || ::IsIconic(hWnd))
		return nResult;



	BYTE r, g, b, now_r, now_g, now_b;

	CRect rect;
	::GetWindowRect( hWnd, &rect );



	if ((right==0) || (right>rect.Width()))
		right=rect.Width();

	if ((bottom==0) ||(bottom>rect.Height()))
		bottom=rect.Height();

	if (!GetDCData(hWnd))
		return nResult;

	//COLORREF color;
	//�Ա�ͼƬװ������
	::HBITMAP PicH=  (HBITMAP) LoadImage(NULL,Pic,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	BITMAP bm; 
	::GetObject(PicH,sizeof(BITMAP),&bm); 
	BYTE *tData;
	//ȡ�������飬�����Ϊ������ÿ��β�Զ���1�ֽڲ��㣬��Ҫȡ�����
	tData=new BYTE[ (bm.bmWidth * (bm.bmBitsPixel/8) + 3)/4*4* bm.bmHeight];
	long Ret1=GetBitmapBits (PicH,bm.bmWidth * bm.bmHeight * (bm.bmBitsPixel/8),&tData[0]);
	BOOL fFalse;

	//ѭ���Ƚ�
	for (int y=top; y<bottom-bm.bmHeight-1; y++)
	{
		for (int x=left; x<right-bm.bmWidth-1; x++)
		{
			fFalse=TRUE;
			for (int i=0;i<bm.bmHeight-1 ;i++)
			{
				if (!fFalse)
					break;
				for (int j=0;j<bm.bmWidth-1;j++)
				{
					if (!fFalse)
						break;
					//ȡ�������飬�����Ϊ������ÿ��β�Զ���1�ֽڲ��㣬��Ҫȡ�����
					b=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3];
					g=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+1];
					r=tData[ i* (bm.bmWidth *3+(bm.bmWidth % 2))+ j*3+2];

					now_r = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +2];
					now_g = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +1];
					now_b = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +0];


					if (((abs(now_r - r)<xiangshi)&&(abs(now_g - g)<xiangshi)&&(abs(now_b -b)<xiangshi))  ||  ((b==tData[0]) && (g==tData[1])&&( r==tData[2]))  )
						fFalse=TRUE;
					else
						fFalse=FALSE;
				}

			}
			if (fFalse==TRUE) 
			{
				nResult++;
				continue;
			}
		}
	}
	delete []tData;
	::DeleteObject (PicH);
	return nResult;
}
//Ѱ�����ָ���
int CMyClass::GetTextNum(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,CArray<CPoint,CPoint&> &ptArray,int left,int top,int right,int bottom,int Interval,int JiaCu,int xiangshi,int width,int IntervalE)
{
	int nResult=0;
	if (JiaCu!=700)
		JiaCu=0;
	CPoint PT(-1,-2);
	if (Outtext=="")
	{
		return nResult;
	}
	CRect rect;
	if (::IsWindow(hWnd))
	{
		GetWindowRect (hWnd,&rect);
	}
	else
	{
		return nResult;
	}
	//����λͼ����
	HDC hdc,hdcMem;//�豸
	HFONT ftUser;//�������;��
	HBITMAP hBitMap = NULL;//λͼҪ���豸��������Ҫת���ɴ�����
	CBitmap *pBitMap = NULL;//λͼ
	CDC *pMemDC = NULL; /*HDC����ı���ָ��һ���ڴ棬���   
						�ڴ���������һ���豸����ص����ݣ�����Ҳ����   
						��ΪHDC�������һ��ָ�룻��CDC�ඨ��һ������   
						�������ӵ��HDC�����һ���豸������ͬʱҲ��   
						����HDC��صĲ����ĺ����� */  


	BYTE *tData;
	//����ͼ�δ�С
	//if (Interval == 0)
	//	Interval=size;
	//int iWidth = (Outtext.GetLength() * ( size/2+(Interval>0?1:0))+7) / 8  * 8;
	int iWidth = (Outtext.GetLength() * ( size+Interval )-7) / 8  * 4 +8;
	//iWidth = (Outtext.GetLength() * ( size/2+Interval ));
	int iHeight =size + 1;
	int iPixel = 24;
	//ͼ�θ�ʽ����
	LPBITMAPINFO lpbmih = new BITMAPINFO;//ͼ�θ�ʽ����
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = iWidth;//
	lpbmih->bmiHeader.biHeight = iHeight;
	lpbmih->bmiHeader.biPlanes = 1;//Ŀ���豸λ����������Ϊ1
	lpbmih->bmiHeader.biBitCount = iPixel;//��ɫ���
	lpbmih->bmiHeader.biCompression = BI_RGB;//λͼѹ������
	lpbmih->bmiHeader.biSizeImage = 0;//��С���ֽڵ�λ
	lpbmih->bmiHeader.biXPelsPerMeter = 0;//ˮƽ�ֱ���
	lpbmih->bmiHeader.biYPelsPerMeter = 0;//��ֱ�ֱ���
	lpbmih->bmiHeader.biClrUsed = 0;//λͼʵ��ʹ����ɫ������ɫ��
	lpbmih->bmiHeader.biClrImportant = 0;//λͼ��ʾ�бȽ���Ҫ����ɫ��


	hdc = CreateIC(TEXT("DISPLAY"),NULL,NULL,NULL);//���԰����ķ���ֵ���� HDC, ��������HDC�� ��ָ���Զ������в�ѯ������
	hdcMem = CreateCompatibleDC(hdc);//�ڴ�dc��
	hBitMap = CreateDIBSection(hdcMem,lpbmih,DIB_PAL_COLORS,(void **)&tData,NULL,0);//����һ��DIB
	if (color==0)
		memset(&tData[0],255,iHeight * iWidth * 3);
	pBitMap = new CBitmap;
	pBitMap->Attach(hBitMap);//��λͼ����
	pMemDC = new CDC;
	pMemDC->Attach(hdcMem);//��DC�ڴ����
	pMemDC->SelectObject(pBitMap);//ѡ���ڴ��ַ

	pMemDC->SetBkMode(TRANSPARENT);//����ģʽ
	pMemDC->SetTextColor( color);//������ɫ
	ftUser = CreateFont(size,//�����С
		width,//�ֿ�
		0,//����ָ��ת��ʸ����escapement vector������ʾ�����x��ļнǣ���0.1Ϊ��λ��
		00,//����ָ���ַ����ߺ�x��ļнǣ���0.1Ϊ��λ����
		//FW_DONTCARE,//������
		JiaCu,
		FALSE, //����6��������bItalic����ָ�������Ƿ�Ϊб��Ĳ���ֵ
		FALSE,//��7��������bUnderline����ָ�������Ƿ�����»��ߵĲ���ֵ
		FALSE,//��8��������cStrickout����ָ���ַ��Ƿ����ɾ���ߵĲ���ֵ
		GB2312_CHARSET,//ָ����������弯
		OUT_DEFAULT_PRECIS,//����ָ������������ȷ��
		CLIP_DEFAULT_PRECIS,//������ȷ��
		DEFAULT_QUALITY,//�������������
		DEFAULT_PITCH|FF_MODERN,//�ַ������ַ�
		ZiTi//"����"//�������������
		); 
	(HFONT) SelectObject(hdcMem, ftUser);//ѡ����ʽ

	//��ͼƬ���������
	CRect rc;//�ͻ���
	rc.right=iWidth;
	rc.bottom=iHeight;
	rc.left =1;
	rc.top = 1;
	CString s;
	char* ss= Outtext.GetBuffer(Outtext.GetLength());
	int t=0;
	while ( t < Outtext.GetLength())
	{
		if(ss[t] <0)     //�Ǻ��� 
		{
			pMemDC->DrawText( Outtext.Mid(t,2), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			if (width== 0)
				rc.left=rc.left+size+Interval; 
			else
				rc.left=rc.left + width *2 + Interval;
			t++; 
		} 
		else 
		{
			pMemDC->DrawText( Outtext.Mid(t,1), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			if (width== 0)
				rc.left=rc.left+ size / 2 +IntervalE; 
			else
				rc.left=rc.left + width *2 + IntervalE;
		} 

		t++;

	}
	//pMemDC->DrawText( Outtext, &rc, DT_SINGLELINE|DT_LEFT );//����ı� 

	pMemDC->SelectObject(pBitMap);

	//AfxMessageBox("ͼƬ���ɳɹ���");
	/////////////////////////////////////�����ַ�ͼƬ
	//BITMAPFILEHEADER bmfh;
	//*((char *)&bmfh.bfType) = 'B';
	//*(((char *)&bmfh.bfType) + 1) = 'M';//������BMP�ļ�
	//bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//ƫ��λ��
	//bmfh.bfSize = bmfh.bfOffBits + (iWidth * iHeight) * iPixel / 8;//�ļ���С
	////TCHAR szBMPFileName[128];//�ļ�����
	//int iBMPBytes = iWidth * iHeight * iPixel / 8;//�����ļ���С
	//CFile file;
	//if(file.Open("test2.bmp",CFile::modeWrite | CFile::modeCreate))
	//{
	//	file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
	//	file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
	//	file.Write(tData,iBMPBytes);
	//	file.Close();
	//}
	//else
	//{
	//	AfxMessageBox("�޷����ɣ���ȷ������·���Ƿ���ȷ");
	//	return PT;
	//}
	BYTE r, g, b, now_r, now_g, now_b;

	/*BYTE *pData=GetDCData(hWnd);*/
	if (!GetDCData(hWnd))
		return nResult;
	//GetDCData(hWnd);

	if (left<=0)
		left=0;
	if (top<=0)
		top=0;
	if ((right<=0) || (right>rect.Width()))
		right=rect.Width();

	if ((bottom<=0) ||(bottom>rect.Height()))
		bottom=rect.Height();


	BOOL fFalse;
	ptArray.SetSize(0);
	//ѭ���Ƚ�
	for (int y=top; y<bottom-iHeight; y++)
	{
		for (int x=left; x<right-iWidth; x++)
		{
			fFalse=TRUE;
			for (int i=0;i<iHeight-1 ;i++)
			{
				if (!fFalse)
					break;
				for (int j=0;j<iWidth-1;j++)
				{
					if (!fFalse)
						break;
					b=tData[ (iHeight-i-1) * (iWidth *3)+ j*3];
					g=tData[ (iHeight-i-1) * (iWidth *3)+ j*3+1];
					r=tData[ (iHeight-i-1) * (iWidth *3)+ j*3+2];

					now_r = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +2];
					now_g = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +1];
					now_b = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +0];

					if (((abs(now_r - r)<xiangshi)&&(abs(now_g - g)<xiangshi)&&(abs(now_b -b)<xiangshi)) /* ||  ((b==tData[0]) && (g==tData[1])&&( r==tData[2])) */ )
						fFalse=TRUE;
					else if((b==tData[0]) && (g==tData[1])&&( r==tData[2]))
					{
						if (((abs(now_r - GetRValue(color))<xiangshi)&&(abs(now_g - GetGValue(color))<xiangshi)&&(abs(now_b -GetBValue(color))<xiangshi)))
						{
							fFalse=FALSE;
						}
						else
							fFalse=TRUE;
					}
					else
						fFalse=FALSE;
				}

			}
			if (fFalse==TRUE) 
			{
				PT.SetPoint(x,y);
				ptArray.Add(PT);
				nResult++;
				continue;
			}
		}
	}
	//delete pData;
	delete pMemDC; pMemDC = NULL;
	delete pBitMap; pBitMap = NULL;
	delete lpbmih; lpbmih = NULL;
	return nResult;

}
//Ѱ�����ָ���(��ȷ����)
int CMyClass::GetRoleNum(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,CArray<CPoint,CPoint&> &ptArray,int left,int top,int right,int bottom,int Interval,int JiaCu,int xiangshi,int width,int IntervalE)
{
	int nResult=0;
	if (JiaCu!=700)
		JiaCu=0;
	CPoint PT(-1,-2);
	if (Outtext=="")
	{
		return nResult;
	}
	CRect rect;
	if (::IsWindow(hWnd))
	{
		GetWindowRect (hWnd,&rect);
	}
	else
	{
		return nResult;
	}
	//����λͼ����
	HDC hdc,hdcMem;//�豸
	HFONT ftUser;//�������;��
	HBITMAP hBitMap = NULL;//λͼҪ���豸��������Ҫת���ɴ�����
	CBitmap *pBitMap = NULL;//λͼ
	CDC *pMemDC = NULL; /*HDC����ı���ָ��һ���ڴ棬���   
						�ڴ���������һ���豸����ص����ݣ�����Ҳ����   
						��ΪHDC�������һ��ָ�룻��CDC�ඨ��һ������   
						�������ӵ��HDC�����һ���豸������ͬʱҲ��   
						����HDC��صĲ����ĺ����� */  


	BYTE *tData;
	//����ͼ�δ�С
	//if (Interval == 0)
	//	Interval=size;
	//int iWidth = (Outtext.GetLength() * ( size/2+(Interval>0?1:0))+7) / 8  * 8;
	int iWidth =0;
	for(int i=0;i<Outtext.GetLength();i++)
	{
		if(Outtext.GetAt(i)<0)
		{
			iWidth+=size+Interval;
			i++;
		}
		else
			iWidth+=size/2+IntervalE;
	}
	iWidth=(iWidth+3)/4*4;
	//int iWidth = (Outtext.GetLength() * ( size+Interval )-7) / 8  * 4 +8;
	//iWidth = (Outtext.GetLength() * ( size/2+Interval ));
	int iHeight =size + 1;
	int iPixel = 24;
	//ͼ�θ�ʽ����
	LPBITMAPINFO lpbmih = new BITMAPINFO;//ͼ�θ�ʽ����
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = iWidth;//
	lpbmih->bmiHeader.biHeight = iHeight;
	lpbmih->bmiHeader.biPlanes = 1;//Ŀ���豸λ����������Ϊ1
	lpbmih->bmiHeader.biBitCount = iPixel;//��ɫ���
	lpbmih->bmiHeader.biCompression = BI_RGB;//λͼѹ������
	lpbmih->bmiHeader.biSizeImage = 0;//��С���ֽڵ�λ
	lpbmih->bmiHeader.biXPelsPerMeter = 0;//ˮƽ�ֱ���
	lpbmih->bmiHeader.biYPelsPerMeter = 0;//��ֱ�ֱ���
	lpbmih->bmiHeader.biClrUsed = 0;//λͼʵ��ʹ����ɫ������ɫ��
	lpbmih->bmiHeader.biClrImportant = 0;//λͼ��ʾ�бȽ���Ҫ����ɫ��


	hdc = CreateIC(TEXT("DISPLAY"),NULL,NULL,NULL);//���԰����ķ���ֵ���� HDC, ��������HDC�� ��ָ���Զ������в�ѯ������
	hdcMem = CreateCompatibleDC(hdc);//�ڴ�dc��
	hBitMap = CreateDIBSection(hdcMem,lpbmih,DIB_PAL_COLORS,(void **)&tData,NULL,0);//����һ��DIB
	if (color==0)
		memset(&tData[0],255,iHeight * iWidth * 3);
	pBitMap = new CBitmap;
	pBitMap->Attach(hBitMap);//��λͼ����
	pMemDC = new CDC;
	pMemDC->Attach(hdcMem);//��DC�ڴ����
	pMemDC->SelectObject(pBitMap);//ѡ���ڴ��ַ

	pMemDC->SetBkMode(TRANSPARENT);//����ģʽ
	pMemDC->SetTextColor( color);//������ɫ
	ftUser = CreateFont(size,//�����С
		width,//�ֿ�
		0,//����ָ��ת��ʸ����escapement vector������ʾ�����x��ļнǣ���0.1Ϊ��λ��
		00,//����ָ���ַ����ߺ�x��ļнǣ���0.1Ϊ��λ����
		//FW_DONTCARE,//������
		JiaCu,
		FALSE, //����6��������bItalic����ָ�������Ƿ�Ϊб��Ĳ���ֵ
		FALSE,//��7��������bUnderline����ָ�������Ƿ�����»��ߵĲ���ֵ
		FALSE,//��8��������cStrickout����ָ���ַ��Ƿ����ɾ���ߵĲ���ֵ
		GB2312_CHARSET,//ָ����������弯
		OUT_DEFAULT_PRECIS,//����ָ������������ȷ��
		CLIP_DEFAULT_PRECIS,//������ȷ��
		DEFAULT_QUALITY,//�������������
		DEFAULT_PITCH|FF_MODERN,//�ַ������ַ�
		ZiTi//"����"//�������������
		); 
	(HFONT) SelectObject(hdcMem, ftUser);//ѡ����ʽ

	//��ͼƬ���������
	CRect rc;//�ͻ���
	rc.right=iWidth;
	rc.bottom=iHeight;
	rc.left =1;
	rc.top = 1;
	CString s;
	char* ss= Outtext.GetBuffer(Outtext.GetLength());
	int t=0;
	while ( t < Outtext.GetLength())
	{
		if(ss[t] <0)     //�Ǻ��� 
		{
			pMemDC->DrawText( Outtext.Mid(t,2), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			if (width== 0)
				rc.left=rc.left+size+Interval; 
			else
				rc.left=rc.left + width *2 + Interval;
			t++; 
		} 
		else 
		{
			pMemDC->DrawText( Outtext.Mid(t,1), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			if (width== 0)
				rc.left=rc.left+ size / 2 +IntervalE; 
			else
				rc.left=rc.left + width *2 + IntervalE;
		} 

		t++;

	}
	//pMemDC->DrawText( Outtext, &rc, DT_SINGLELINE|DT_LEFT );//����ı� 

	pMemDC->SelectObject(pBitMap);

	//AfxMessageBox("ͼƬ���ɳɹ���");
	///////////////////////////////////�����ַ�ͼƬ
	//BITMAPFILEHEADER bmfh;
	//*((char *)&bmfh.bfType) = 'B';
	//*(((char *)&bmfh.bfType) + 1) = 'M';//������BMP�ļ�
	//bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//ƫ��λ��
	//bmfh.bfSize = bmfh.bfOffBits + (iWidth * iHeight) * iPixel / 8;//�ļ���С
	////TCHAR szBMPFileName[128];//�ļ�����
	//int iBMPBytes = iWidth * iHeight * iPixel / 8;//�����ļ���С
	//CFile file;
	//if(file.Open("D:\\test2.bmp",CFile::modeWrite | CFile::modeCreate))
	//{
	//	file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
	//	file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
	//	file.Write(tData,iBMPBytes);
	//	file.Close();
	//}
	//else
	//{
	//	AfxMessageBox("�޷����ɣ���ȷ������·���Ƿ���ȷ");
	//	//return PT;
	//}
	BYTE r, g, b, now_r, now_g, now_b;

	/*BYTE *pData=GetDCData(hWnd);*/
	if (!GetDCData(hWnd))
		return nResult;
	//GetDCData(hWnd);

	if (left<=0)
		left=0;
	if (top<=0)
		top=0;
	if ((right<=0) || (right>rect.Width()))
		right=rect.Width();

	if ((bottom<=0) ||(bottom>rect.Height()))
		bottom=rect.Height();


	BOOL fFalse;
	ptArray.SetSize(0);
	//ѭ���Ƚ�
	for (int y=top; y<bottom-iHeight; y++)
	{
		for (int x=left; x<right-iWidth; x++)
		{
			fFalse=TRUE;
			for (int i=0;i<iHeight-1 ;i++)
			{
				if (!fFalse)
					break;
				for (int j=0;j<iWidth-1;j++)
				{
					if (!fFalse)
						break;
					b=tData[ (iHeight-i-1) * (iWidth *3)+ j*3];
					g=tData[ (iHeight-i-1) * (iWidth *3)+ j*3+1];
					r=tData[ (iHeight-i-1) * (iWidth *3)+ j*3+2];

					now_r = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +2];
					now_g = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +1];
					now_b = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +0];

					if (((abs(now_r - r)<xiangshi)&&(abs(now_g - g)<xiangshi)&&(abs(now_b -b)<xiangshi)))
					{
						fFalse=TRUE;

					}
					else if((b==tData[0]) && (g==tData[1])&&( r==tData[2]))
					{
						if (((abs(now_r - GetRValue(color))<xiangshi)&&(abs(now_g - GetGValue(color))<xiangshi)&&(abs(now_b -GetBValue(color))<xiangshi)))
						{
							fFalse=FALSE;
						}
						else
							fFalse=TRUE;
					}
					else
						fFalse=FALSE;
				}

			}
			if (fFalse==TRUE) 
			{
				PT.SetPoint(x,y);
				ptArray.Add(PT);
				nResult++;
				continue;
			}
		}
	}
	//delete pData;
	delete pMemDC; pMemDC = NULL;
	delete pBitMap; pBitMap = NULL;
	delete lpbmih; lpbmih = NULL;
	return nResult;

}
POINT CMyClass::FindTextBlackWhite(HWND hWnd,CString ZiTi,CString Outtext,int size,COLORREF color,int left,int top,int right,int bottom,BOOL bGetData,int Interval,int JiaCu,int xiangshi,int width,int IntervalE)
{
	if (JiaCu!=700)
		JiaCu=0;

	POINT PT;
	PT.x=-1;
	PT.y=-2;
	if (Outtext=="")
	{
		return PT;
	}
	CRect rect;
	if (::IsWindow(hWnd))
	{
		GetWindowRect (hWnd,&rect);
	}
	else
	{
		return PT;
	}
	//����λͼ����
	HDC hdc,hdcMem;//�豸
	HFONT ftUser;//�������;��
	HBITMAP hBitMap = NULL;//λͼҪ���豸��������Ҫת���ɴ�����
	CBitmap *pBitMap = NULL;//λͼ
	CDC *pMemDC = NULL; /*HDC����ı���ָ��һ���ڴ棬���   
						�ڴ���������һ���豸����ص����ݣ�����Ҳ����   
						��ΪHDC�������һ��ָ�룻��CDC�ඨ��һ������   
						�������ӵ��HDC�����һ���豸������ͬʱҲ��   
						����HDC��صĲ����ĺ����� */  


	BYTE *tData;
	//����ͼ�δ�С
	//if (Interval == 0)
	//	Interval=size;
	//int iWidth = (Outtext.GetLength() * ( size/2+(Interval>0?1:0))+7) / 8  * 8;
	int iWidth = (Outtext.GetLength() * ( size+Interval )-7) / 8  * 4 +8;
	//iWidth = (Outtext.GetLength() * ( size/2+Interval ));
	int iHeight =size + 1;
	int iPixel = 24;
	//ͼ�θ�ʽ����
	LPBITMAPINFO lpbmih = new BITMAPINFO;//ͼ�θ�ʽ����
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = iWidth;//
	lpbmih->bmiHeader.biHeight = iHeight;
	lpbmih->bmiHeader.biPlanes = 1;//Ŀ���豸λ����������Ϊ1
	lpbmih->bmiHeader.biBitCount = iPixel;//��ɫ���
	lpbmih->bmiHeader.biCompression = BI_RGB;//λͼѹ������
	lpbmih->bmiHeader.biSizeImage = 0;//��С���ֽڵ�λ
	lpbmih->bmiHeader.biXPelsPerMeter = 0;//ˮƽ�ֱ���
	lpbmih->bmiHeader.biYPelsPerMeter = 0;//��ֱ�ֱ���
	lpbmih->bmiHeader.biClrUsed = 0;//λͼʵ��ʹ����ɫ������ɫ��
	lpbmih->bmiHeader.biClrImportant = 0;//λͼ��ʾ�бȽ���Ҫ����ɫ��


	hdc = CreateIC(TEXT("DISPLAY"),NULL,NULL,NULL);//���԰����ķ���ֵ���� HDC, ��������HDC�� ��ָ���Զ������в�ѯ������
	hdcMem = CreateCompatibleDC(hdc);//�ڴ�dc��
	hBitMap = CreateDIBSection(hdcMem,lpbmih,DIB_PAL_COLORS,(void **)&tData,NULL,0);//����һ��DIB
	if (color==0)
		memset(&tData[0],255,iHeight * iWidth * 3);
	pBitMap = new CBitmap;
	pBitMap->Attach(hBitMap);//��λͼ����
	pMemDC = new CDC;
	pMemDC->Attach(hdcMem);//��DC�ڴ����
	pMemDC->SelectObject(pBitMap);//ѡ���ڴ��ַ

	pMemDC->SetBkMode(TRANSPARENT);//����ģʽ
	pMemDC->SetTextColor( color);//������ɫ
	ftUser = CreateFont(size,//�����С
		width,//�ֿ�
		0,//����ָ��ת��ʸ����escapement vector������ʾ�����x��ļнǣ���0.1Ϊ��λ��
		00,//����ָ���ַ����ߺ�x��ļнǣ���0.1Ϊ��λ����
		//FW_DONTCARE,//������
		JiaCu,
		FALSE, //����6��������bItalic����ָ�������Ƿ�Ϊб��Ĳ���ֵ
		FALSE,//��7��������bUnderline����ָ�������Ƿ�����»��ߵĲ���ֵ
		FALSE,//��8��������cStrickout����ָ���ַ��Ƿ����ɾ���ߵĲ���ֵ
		GB2312_CHARSET,//ָ����������弯
		OUT_DEFAULT_PRECIS,//����ָ������������ȷ��
		CLIP_DEFAULT_PRECIS,//������ȷ��
		DEFAULT_QUALITY,//�������������
		DEFAULT_PITCH|FF_MODERN,//�ַ������ַ�
		ZiTi//"����"//�������������
		); 
	(HFONT) SelectObject(hdcMem, ftUser);//ѡ����ʽ

	//��ͼƬ���������
	CRect rc;//�ͻ���
	rc.right=iWidth;
	rc.bottom=iHeight;
	rc.left =1;
	rc.top = 1;
	CString s;
	char* ss= Outtext.GetBuffer(Outtext.GetLength());
	int t=0;
	while ( t < Outtext.GetLength())
	{
		if(ss[t] <0)     //�Ǻ��� 
		{
			pMemDC->DrawText( Outtext.Mid(t,2), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			if (width== 0)
				rc.left=rc.left+size+Interval; 
			else
				rc.left=rc.left + width *2 + Interval;
			t++; 
		} 
		else 
		{
			pMemDC->DrawText( Outtext.Mid(t,1), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			if (width== 0)
				rc.left=rc.left+ size / 2 +IntervalE; 
			else
				rc.left=rc.left + width *2 + IntervalE;
		} 

		t++;

	}
	//pMemDC->DrawText( Outtext, &rc, DT_SINGLELINE|DT_LEFT );//����ı� 

	pMemDC->SelectObject(pBitMap);

	//AfxMessageBox("ͼƬ���ɳɹ���");
	/////////////////////////////////////�����ַ�ͼƬ
	//BITMAPFILEHEADER bmfh;
	//*((char *)&bmfh.bfType) = 'B';
	//*(((char *)&bmfh.bfType) + 1) = 'M';//������BMP�ļ�
	//bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//ƫ��λ��
	//bmfh.bfSize = bmfh.bfOffBits + (iWidth * iHeight) * iPixel / 8;//�ļ���С
	////TCHAR szBMPFileName[128];//�ļ�����
	//int iBMPBytes = iWidth * iHeight * iPixel / 8;//�����ļ���С
	//CFile file;
	//if(file.Open("test2.bmp",CFile::modeWrite | CFile::modeCreate))
	//{
	//	file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
	//	file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
	//	file.Write(tData,iBMPBytes);
	//	file.Close();
	//}
	//else
	//{
	//	AfxMessageBox("�޷����ɣ���ȷ������·���Ƿ���ȷ");
	//	return PT;
	//}
	BYTE r, g, b, now_r, now_g, now_b;

	/*BYTE *pData=GetDCData(hWnd);*/
	if (!GetDCData(hWnd))
		return PT;
	//GetDCData(hWnd);

	if (left<=0)
		left=0;
	if (top<=0)
		top=0;
	if ((right<=0) || (right>rect.Width()))
		right=rect.Width();

	if ((bottom<=0) ||(bottom>rect.Height()))
		bottom=rect.Height();


	BOOL fFalse;
	//ѭ���Ƚ�
	for (int y=top; y<bottom-iHeight; y++)
	{
		for (int x=left; x<right-iWidth; x++)
		{
			fFalse=TRUE;
			for (int i=0;i<iHeight-1 ;i++)
			{
				if (!fFalse)
					break;
				for (int j=0;j<iWidth-1;j++)
				{
					if (!fFalse)
						break;
					b=tData[ (iHeight-i-1) * (iWidth *3)+ j*3];
					g=tData[ (iHeight-i-1) * (iWidth *3)+ j*3+1];
					r=tData[ (iHeight-i-1) * (iWidth *3)+ j*3+2];
					if(b==0 && g==0 && r==0)
						b=0,g=0,r=0;
					else
						b=255,g=255,r=255;

					now_r = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +2];
					now_g = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +1];
					now_b = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +0];
					if(now_b<30 && now_g>90 && now_r<30)
						now_b=255,now_g=255,now_r=255;
					else
						now_b=0,now_g=0,now_r=0;

					if (b==0 || (now_r == r && now_g == g && now_b ==b) )
						fFalse=TRUE;
					else
						fFalse=FALSE;
				}

			}
			if (fFalse==TRUE) 
			{
				PT.x = x;
				PT.y = y;
				//delete pData;

				delete pMemDC; pMemDC = NULL;
				delete pBitMap; pBitMap = NULL;
				delete lpbmih; lpbmih = NULL;
				return PT;
			}
		}
	}
	//delete pData;
	delete pMemDC; pMemDC = NULL;
	delete pBitMap; pBitMap = NULL;
	delete lpbmih; lpbmih = NULL;
	return PT;

}
void CMyClass::DragMouse(HWND hWnd,int ToX,int ToY,int FromX,int FromY,int nWaitTime)
{
	if ( !IsWindowVisible(hWnd) )
		return;
	CRect rcView;
	GetWindowRect( hWnd, &rcView );
	POINT pt;
	if(FromX<0 || FromY<0)
	{
		GetCursorPos(&pt);
		FromX=pt.x-rcView.left;
		FromY=pt.y-rcView.top;
	}
	SetCursorPos(rcView.left + FromX, rcView.top + FromY);
	Sleep(300);
	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
	Sleep(100);
	int kFenZi = ToY-FromY;
	int kFenMu = ToX-FromX;
	int bFenZi = ToY*kFenMu-kFenZi*ToX;
	if(kFenMu>=-2 && kFenMu<=2)
	{
		for(int i=1;i<abs(FromY-ToY);i++)
		{
			Sleep(20);
			int y=FromY>ToY?FromY-i:FromY+i;
			SetCursorPos(rcView.left + FromX, rcView.top + y);
			//mouse_event(MOUSEEVENTF_MOVE,0,-1,0,0);
		}

		//return;
	}
	for(int i=1;i<abs(FromX-ToX);i++)
	{
		Sleep(20);
		int x=FromX>ToX?FromX-i:FromX+i;
		int y=(kFenZi*x+bFenZi)/kFenMu;
		SetCursorPos(rcView.left + x, rcView.top + y);
	}
	Sleep(1000);//ͣ��
	mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
	Sleep(100);
	return;
}


void CMyClass::MoveMouse(HWND hwnd,int x,int y,int absx)
{
	if(x<0 || y<0)
		return;
	RECT rect;
	if (!IsWindowVisible(hwnd))
		return;


	GetWindowRect (hwnd,&rect);


	x+=rect.left;
	y+=rect.top;
	if(x<0 || y<0 || x>rect.right || y>rect.bottom)
		return;
	POINT pt;
	GetCursorPos(&pt);
	while (abs(pt.x - x)>absx || abs(pt.y - y)>absx)
	{
		int mx=1;
		int my=1;
		if (abs(pt.x - x) > 20)
			mx = (x - pt.x) / 5;
		else if (abs(pt.x - x) > 5)
			mx = (x - pt.x) / 2;
		else if (abs(pt.x - x) <=absx)
			mx=0;
		else if (pt.x > x)
			mx=-1;

		if (abs(pt.y - y) > 20)
			my= (y - pt.y) / 5;
		else if (abs(pt.y - y) > 5)
			my= (y - pt.y) / 2;
		else if (abs(pt.y - y) <= absx)
			my=0;
		else if  (pt.y > y)
			my=-1;

		//M_MoveR2(handle1,mx,my);
		mouse_event(MOUSE_MOVE_ABSOLUTE, mx,my,NULL,NULL);
		Sleep(100);
		//M_ResetMousePos(handle1);
		mouse_event(MOUSE_MOVE_ABSOLUTE, 0,0,NULL,NULL);
		GetCursorPos(&pt);
	}
	return ;
}
POINT CMyClass::FindTextBySingle(HWND hWnd,CString ZiTi,CString Outtext,UINT size,COLORREF color,UINT left ,UINT top,UINT right ,UINT bottom)
{
	POINT PT;
	PT.x=-1;
	PT.y=-2;
	if (Outtext=="")
		return PT;
	CRect rect;
	if (::IsWindow(hWnd))
	{
		GetWindowRect (hWnd,&rect);
	}
	else
	{
		return PT;
	}
	//����λͼ����
	HDC hdc,hdcMem;//�豸
	HFONT ftUser;//�������;��
	HBITMAP hBitMap = NULL;//λͼҪ���豸��������Ҫת���ɴ�����
	CBitmap *pBitMap = NULL;//λͼ
	CDC *pMemDC = NULL; /*HDC����ı���ָ��һ���ڴ棬���   
						�ڴ���������һ���豸����ص����ݣ�����Ҳ����   
						��ΪHDC�������һ��ָ�룻��CDC�ඨ��һ������   
						�������ӵ��HDC�����һ���豸������ͬʱҲ��   
						����HDC��صĲ����ĺ����� */  


	BYTE *tData;
	//����ͼ�δ�С
	int iWidth = (Outtext.GetLength() * (( size+1)/2)+7)/4*4;
	int iHeight =size+1;
	int iPixel = 24;
	//ͼ�θ�ʽ����
	LPBITMAPINFO lpbmih = new BITMAPINFO;//ͼ�θ�ʽ����
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = iWidth;//
	lpbmih->bmiHeader.biHeight = iHeight;
	lpbmih->bmiHeader.biPlanes = 1;//Ŀ���豸λ����������Ϊ1
	lpbmih->bmiHeader.biBitCount = iPixel;//��ɫ���
	lpbmih->bmiHeader.biCompression = BI_RGB;//λͼѹ������
	lpbmih->bmiHeader.biSizeImage = 0;//��С���ֽڵ�λ
	lpbmih->bmiHeader.biXPelsPerMeter = 0;//ˮƽ�ֱ���
	lpbmih->bmiHeader.biYPelsPerMeter = 0;//��ֱ�ֱ���
	lpbmih->bmiHeader.biClrUsed = 0;//λͼʵ��ʹ����ɫ������ɫ��
	lpbmih->bmiHeader.biClrImportant = 0;//λͼ��ʾ�бȽ���Ҫ����ɫ��


	hdc = CreateIC(TEXT("DISPLAY"),NULL,NULL,NULL);//���԰����ķ���ֵ���� HDC, ��������HDC�� ��ָ���Զ������в�ѯ������
	hdcMem = CreateCompatibleDC(hdc);//�ڴ�dc��
	hBitMap = CreateDIBSection(hdcMem,lpbmih,DIB_PAL_COLORS,(void **)&tData,NULL,0);//����һ��DIB
	if (color==0)
		memset(&tData[0],255,iHeight * iWidth * 3);
	pBitMap = new CBitmap;
	pBitMap->Attach(hBitMap);//��λͼ����
	pMemDC = new CDC;
	pMemDC->Attach(hdcMem);//��DC�ڴ����
	pMemDC->SelectObject(pBitMap);//ѡ���ڴ��ַ

	pMemDC->SetBkMode(TRANSPARENT);//����ģʽ
	pMemDC->SetTextColor( color);//������ɫ
	ftUser = CreateFont(size,//�����С
		0,//�ֿ�
		0,//����ָ��ת��ʸ����escapement vector������ʾ�����x��ļнǣ���0.1Ϊ��λ��
		00,//����ָ���ַ����ߺ�x��ļнǣ���0.1Ϊ��λ����
		//FW_DONTCARE,//������
		0,
		FALSE, //����6��������bItalic����ָ�������Ƿ�Ϊб��Ĳ���ֵ
		FALSE,//��7��������bUnderline����ָ�������Ƿ�����»��ߵĲ���ֵ
		FALSE,//��8��������cStrickout����ָ���ַ��Ƿ����ɾ���ߵĲ���ֵ
		GB2312_CHARSET,//ָ����������弯
		OUT_DEFAULT_PRECIS,//����ָ������������ȷ��
		CLIP_DEFAULT_PRECIS,//������ȷ��
		DEFAULT_QUALITY,//�������������
		DEFAULT_PITCH|FF_MODERN,//�ַ������ַ�
		"����"//�������������
		); 
	(HFONT) SelectObject(hdcMem, ftUser);//ѡ����ʽ

	//��ͼƬ���������
	CRect rc;//�ͻ���
	rc.right=iWidth;
	rc.bottom=iHeight;
	rc.left =1;
	rc.top = 1;
	CString s;
	char* ss= Outtext.GetBuffer(Outtext.GetLength());
	int t=0;
	CStringArray arrText;
	while ( t < Outtext.GetLength())
	{
		if(ss[t] <0)     //�Ǻ��� 
		{
			pMemDC->DrawText( Outtext.Mid(t,2), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			rc.left=rc.left+size; 
			arrText.Add(Outtext.Mid(t,2));
			t++; 
		} 
		else 
		{
			pMemDC->DrawText( Outtext.Mid(t,1), &rc, DT_SINGLELINE|DT_LEFT );//����ı�
			rc.left=rc.left+ size / 2 ; 
			arrText.Add(Outtext.Mid(t,1));
		} 

		t++;

	}

	pMemDC->SelectObject(pBitMap);

	//AfxMessageBox("ͼƬ���ɳɹ���");
	/////////////////////////////////////�����ַ�ͼƬ
	//BITMAPFILEHEADER bmfh;
	//bmfh.bfType = 0x4d42;  // 'BM'
	//bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//ƫ��λ��
	//bmfh.bfSize = bmfh.bfOffBits + (iWidth * iHeight) * iPixel / 8;//�ļ���С
	////TCHAR szBMPFileName[128];//�ļ�����
	//int iBMPBytes = iWidth * iHeight * (iPixel / 8);//�����ļ���С
	//CFile file;
	//if(file.Open("test2.bmp",CFile::modeWrite | CFile::modeCreate))
	//{
	//	file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
	//	file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
	//	file.Write(tData,iBMPBytes);
	//	file.Close();
	//}
	//else
	//{
	//	AfxMessageBox("�޷����ɣ���ȷ������·���Ƿ���ȷ");
	//	return PT;
	//}
	BYTE r, g, b, now_r, now_g, now_b;

	/*BYTE *pData=GetDCData(hWnd);*/
	GetDCData(hWnd);

	if ((right==0) || (right>rect.Width()))
		right=rect.Width();

	if ((bottom==0) ||(bottom>rect.Height()))
		bottom=rect.Height();


	BOOL fFalse;
	int nChar=0,nLastX=0,nSleft=0,nSright=arrText.GetAt(0).GetLength()*(size/2);

	//ѭ���Ƚ�
	for (int y=top; y<bottom-iHeight; y++)
	{
		for (int x=left; x<right-iWidth; x++)
		{
			fFalse=TRUE;
			for (int i=0;i<iHeight-1 ;i++)
			{
				if (!fFalse)
					break;
				for (int j=0;j<arrText.GetAt(nChar).GetLength()*(size/2);j++)
				{
					if (!fFalse)
						break;
					b=tData[ (iHeight-i-1) * (iWidth *3)+ (j+nSleft)*3];
					g=tData[ (iHeight-i-1) * (iWidth *3)+ (j+nSleft)*3+1];
					r=tData[ (iHeight-i-1) * (iWidth *3)+ (j+nSleft)*3+2];

					now_r = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +2];
					now_g = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +1];
					now_b = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-i -1) + (x+j)*3 +0];
					if (((abs(now_r - r)<24)&&(abs(now_g - g)<24)&&(abs(now_b -b)<24)))
						fFalse=TRUE;
					else if((b==tData[0]) && (g==tData[1])&&( r==tData[2]))
					{
						/*if (j>0 && (abs(now_r - GetRValue(color))<24)&&(abs(now_g - GetGValue(color))<24)&&(abs(now_b -GetBValue(color))<24))
						fFalse=FALSE;
						else*/
						fFalse=TRUE;
					}
					else
						fFalse=FALSE;
				}

			}
			if (fFalse==TRUE) 
			{
				if(nChar==0)
				{
					BOOL bColor=FALSE;
					for(int k=0;k<size;k++)//��һ���ַ�ǰ�Ƿ�����ͬ����,���в���
					{
						for(int p=0;p<size;p++)
						{
							now_r = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-p -1) + (x-k)*3 +2];
							now_g = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-p -1) + (x-k)*3 +1];
							now_b = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-p -1) + (x-k)*3 +0];
							if (((abs(now_r - GetRValue(color))<24)&&(abs(now_g - GetGValue(color))<24)&&(abs(now_b -GetBValue(color))<24)))
							{
								bColor=TRUE;
								break;
							}
						}
						if(bColor)
							break;
					}
					if(bColor)
						continue;
					PT.x = x;
					PT.y = y;
				}
				else
				{
					if((x-nLastX)>(arrText.GetAt(nChar-1).GetLength()*(size/2)+1) || (x-nLastX)<(arrText.GetAt(nChar-1).GetLength()*(size/2)-1))
					{
						nChar=0;
						nSleft=0;
						nSright=arrText.GetAt(0).GetLength()*(size/2);
						continue;
					}
				}
				if(nChar==arrText.GetUpperBound())
				{
					BOOL bColor=FALSE;
					for(int k=0;k<12;k++)//���һ���ַ����Ƿ�����ͬ����
					{
						for(int p=0;p<size;p++)
						{
							now_r = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-k -1) + (x+arrText.GetAt(nChar).GetLength()*(size/2)+p)*3 +2];
							now_g = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-k -1) + (x+arrText.GetAt(nChar).GetLength()*(size/2)+p)*3 +1];
							now_b = pData[((rect.Width() * 3 + 3 ) / 4 * 4 ) * (rect.Height()- y-k -1) + (x+arrText.GetAt(nChar).GetLength()*(size/2)+p)*3 +0];
							if (((abs(now_r - GetRValue(color))<24)&&(abs(now_g - GetGValue(color))<24)&&(abs(now_b -GetBValue(color))<24)))
							{
								bColor=TRUE;
								break;
							}
						}
						if(bColor)
							break;
					}
					if(bColor)
					{
						nChar=0;
						nSleft=0;
						nSright=arrText.GetAt(0).GetLength()*(size/2);
						continue;
					}
					delete pMemDC; pMemDC = NULL;
					delete pBitMap; pBitMap = NULL;
					delete lpbmih; lpbmih = NULL;
					return PT;
				}
				nLastX=x;
				nSleft +=arrText.GetAt(nChar).GetLength()*(size/2);//����1����϶
				nChar++;
				nSright+=arrText.GetAt(nChar).GetLength()*(size/2);
			}
		}
	}
	delete pMemDC; pMemDC = NULL;
	delete pBitMap; pBitMap = NULL;
	delete lpbmih; lpbmih = NULL;
	return PT;
}
//��strReplace�滻�ļ���strS��strE֮�������
void CMyClass::ReplaceFileText (LPCTSTR fileName,CString strReplace,CString strS,CString strE)
{
	CStdioFile myFile;
	CString strText="";
	CString szLine="";
	int nFindS,nFindE;
	if(!myFile.Open(fileName,CFile::modeReadWrite))
		return;
	while(myFile.ReadString(szLine))
	{
		if(szLine.IsEmpty())
			continue;
		nFindS=szLine.Find(strS);
		if(nFindS>=0)
		{
			CString tt="";
			nFindE=szLine.Find(strE,nFindS+strS.GetLength());
			if(nFindE>0)
				tt=szLine.Mid(nFindE);
			szLine=szLine.Left(nFindS)+strS+strReplace+tt;
		}
		strText+=szLine+"\n";
	}
	myFile.SetLength(0);
	myFile.Write(strText,strText.GetLength());
	myFile.Close();
	return;
}
void CMyClass::ClearFolder(CString strFolder)
{
	CFileFind finder;
	if(!finder.FindFile(strFolder+"\\*.*"))
		return;
	while(finder.FindNextFileA())
	{
		if(!finder.IsDirectory() && !finder.IsDots())
			DeleteFile(finder.GetFilePath());
	}
	DeleteFile(finder.GetFilePath());
	finder.Close();
	return;
}
int CMyClass::GetFileNum(CString strFolder)
{
	int num=0;
	CFileFind finder;
	if(!finder.FindFile(strFolder+"\\*.jpg"))
		return num;
	while(finder.FindNextFileA())
	{
		if(!finder.IsDirectory() && !finder.IsDots())
			num++;
	}
	if(num>0)
		num++;
	finder.Close();
	return num;
}
// ����ĳЩ��֮ǰ�Ľ�ͼ���ļ���
void CMyClass::DeleteLog(LPCTSTR strPath, int nDays  )
{
	CString			strFileName;
	CString			strToday;
	CString			strLogPath;

	CTime t = CTime::GetCurrentTime();
	strFileName.Format("%04d_%02d_%02d\\",t.GetYear(), t.GetMonth(), t.GetDay());

	strLogPath = strPath;
	COleDateTime		dt;
	COleDateTimeSpan	ts;

	WIN32_FIND_DATA	fd,fd2;

	ZeroMemory( &fd, sizeof( WIN32_FIND_DATA ) );
	ZeroMemory( &fd2, sizeof( WIN32_FIND_DATA ) );

	HANDLE	hFind	= ::FindFirstFile( strLogPath + _T( "*.*" ), &fd );

	if ( hFind == INVALID_HANDLE_VALUE )
	{
		goto __FINAL;
	}

	ts.SetDateTimeSpan( nDays, 0, 0, 0 );

	dt	= COleDateTime::GetCurrentTime();
	dt	-= ts;


	strToday.Format("%04d_%02d_%02d",dt.GetYear(),dt.GetMonth(),dt.GetDay());

	do
	{
		strFileName	= fd.cFileName;
		if (strFileName.GetLength()>2 && strFileName <= strToday )
		{
			DeleteFolder(strLogPath +strFileName);
		}

	} while ( ::FindNextFile( hFind, &fd ) );

__FINAL:
	::FindClose( hFind );

}

//ɾ���ļ��м��ļ������ļ�
BOOL CMyClass::DeleteFolder(LPCTSTR lpszPath) 
{ 
	if(!PathFileExists(lpszPath))
		return TRUE;
	int nLength = strlen(lpszPath);
	char *NewPath = new char[nLength+2];
	strcpy(NewPath,lpszPath);
	NewPath[nLength] = '\0';
	NewPath[nLength+1] = '\0';
	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags = FOF_NOCONFIRMATION;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = NewPath;
	FileOp.pTo = NULL;
	FileOp.wFunc = FO_DELETE;
	return SHFileOperation(&FileOp) == 0;
}
void CMyClass::SetDisplay()
{
	if(CXSCREEN==1280 && CYSCREEN==1024)
		return;
	//�޸�   
	DEVMODE lpDevMode;
	lpDevMode.dmBitsPerPel = 32;
	lpDevMode.dmPelsWidth = 1280;
	lpDevMode.dmPelsHeight = 1024;
	lpDevMode.dmSize = sizeof(lpDevMode);
	lpDevMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
	LONG result;
	result = ChangeDisplaySettings(&lpDevMode, 0);
	if (result == DISP_CHANGE_SUCCESSFUL)
	{
		//MessageBox("�޸ĳɹ�!");   
		ChangeDisplaySettings(&lpDevMode, CDS_UPDATEREGISTRY);
		//          ����//ʹ��CDS_UPDATEREGISTRY��ʾ���޸��ǳ־õģ�   
		//          ����//����ע�����д������ص�����   

	}
}
BOOL GetFileVersionN(HMODULE hModule, WORD *pBuffer)
{
	TCHAR fname[MAX_PATH];
	VS_FIXEDFILEINFO *pVi;
	DWORD dwHandle;
	CString str;

	if (::GetModuleFileName(hModule, fname, MAX_PATH))
	{
		int size = GetFileVersionInfoSize(fname, &dwHandle);

		if (size > 0) {
			BYTE *buffer = new BYTE[size];

			if (GetFileVersionInfo(fname, dwHandle, size, buffer)) {
				if (VerQueryValue(buffer, _T("\\"), (LPVOID *)&pVi, (PUINT)&size)) {
					pBuffer[0] = HIWORD(pVi->dwFileVersionMS);
					pBuffer[1] = LOWORD(pVi->dwFileVersionMS);
					pBuffer[2] = HIWORD(pVi->dwFileVersionLS);
					pBuffer[3] = LOWORD(pVi->dwFileVersionLS);

					delete []buffer;
					return true;
				}
			}

			delete []buffer;
		}
	}

	return false;
}
CString CMyClass::GetFileVersion()
{
	HMODULE hModule = GetModuleHandle(NULL);
	CString str;
	WORD buffer[4];

	if (GetFileVersionN(hModule, buffer))
	{
		char str2[32];

		for (int i = 0; i < sizeof(buffer)/sizeof(WORD); i++)
		{
			itoa(buffer[i], str2, 10);
			str += str2;

			if (i != sizeof(buffer)/sizeof(WORD) - 1)
			{
				str += ".";
			}
		}
	}

	return str;
}
void CMyClass::BMPToJPG(CString strBMPFile,BOOL bDelete)
{
	CImage image;
	image.Load(strBMPFile);
	image.Save(strBMPFile.Left(strBMPFile.ReverseFind('.')) + ".jpg",Gdiplus::ImageFormatJPEG);
	if(bDelete)
		DeleteFile(strBMPFile);

} 
