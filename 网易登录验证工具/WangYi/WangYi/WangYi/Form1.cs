using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using GamePic;
using GTR;
using BaseCom;
using System.Threading;
using System.IO;
using mshtml;
using System.Collections;
using System.Net;
using System.Security.Cryptography;
using System.Diagnostics;
namespace WangYi
{
    public partial class Form1 : Form
    {

        //D:\QQ信息截图\2015_07\01\DB20140912975293087\ DB20140912975293087 1 1 游戏帐号：zxc365868911@126.com；游戏密码：zxc2621555；身份证号码：370683199207087210；帐号安全码：asd123456；物品锁：2312386；密保问题一：您父亲的名字是？；密保答案一：爸爸；密保问题二：您母亲的名字是？；密保答案二：妈妈；密保问题三：对你影响最大的人是？；密保答案三：老师；
        //[DllImport("wininet.dll", CharSet = CharSet.Auto, SetLastError = true)]
        //public static extern bool InternetSetCookie(string lpszUrlName, string lbszCookieName, string lpszCookieData);
        //[DllImport("KERNEL32.DLL ")]
        //public static extern bool Kernel32API.WritePrivateProfileString(string lpAppName, string lpKeyName, string lpString, string lpFileName);
        public static string PicPath = AppDomain.CurrentDomain.BaseDirectory + "\\ImgBase\\";


        public static Color cDiffArea = Color.FromArgb(255, 255, 255);   //差异区域填充色  由下列几个默认色填充  根据图片名
        public static Color cCenterLine = Color.FromArgb(0, 0, 0);   //竖线
        //以下为备用填充色
        public static Color cWhite = Color.FromArgb(255, 255, 255);   //差异区域填充色
        public static Color cBlack = Color.FromArgb(0, 0, 0);   //差异区域中点
        public static Color cGolden = Color.FromArgb(167, 126, 67);   //金色
        public static Color cRed = Color.FromArgb(255, 0, 0);   //红色
        public static Color cGreen = Color.FromArgb(0, 255, 0);   //绿色
        int UnMatchLeft;
        int UnMatchRigth;


        private static Thread Fun1Proc;
        public static WangYi.WebReference.QuestionReceiver Rs;
        //HtmlElement ClickBtn = null;
        public static string strExePath = System.Windows.Forms.Application.StartupPath + "\\roleInfo.ini";
        public static string strBMPpaht = System.Windows.Forms.Application.StartupPath + "\\WangYi\\";
        //public static string url = "http://reg.163.com/UserLogin.shtml";
        public static string url1 = "https://reg.163.com/mibao/controller/goIndex.jsp";//安全状态
        public static string url2 = "http://reg.163.com/chg/ShowBaomiInfo.jsp";//信息页面
        public static string url3 = "http://reg.163.com/mibao2/reset.do";//安全问题没有绑定手机
        //public static string url3 = "https://reg.163.com/mibao/controller/mob/index.jsp";//安全问题没有绑定手机
        public static string url4 = "https://reg.163.com/change/setPasswordInfoToAuth.do?noheader=0&username=XXXXXX&type=2&chg_which=0";//安全码
        public static string url5 = "https://reg.163.com/mibao/mob/replaceByPhone_1.do?noheader=0&url=";//安全问题绑定手机
        public static string url6 = "https://reg.163.com/mibao/controller/smdj/index.jsp";
        public static string url7 = "http://reg.163.com/UserLogin.shtml";
        public static string url8 = " https://reg.163.com/account/mutipleAcctMgr.jsp";
        public static string url9 = " http://email.163.com";
        public static StringBuilder url = new StringBuilder("http://reg.163.com");
        // public static StringBuilder url = new StringBuilder("https://reg.163.com/Main.jsp");
        //public static StringBuilder url = new StringBuilder("http://email.163.com/");
        //public static StringBuilder url = new StringBuilder("http://you.163.com/u/login");
        // public static StringBuilder url = new StringBuilder("http://www.kaola.com/login.html");
        public static string strPicPath = "";
        private int pageNo = 0;
        public static int picNo = 0;
        public static int waitn = 0;
        public static int KeyNum = 0;
        private int urlNum = 0;

        public string strPic1 = string.Empty;
        public string strPic2 = string.Empty;
        //private string strcookie = "";
        public static StringBuilder m_strAcount = new StringBuilder();
        public static StringBuilder m_strPaswod = new StringBuilder();
        public static StringBuilder m_strSafeNo = new StringBuilder();
        public static StringBuilder m_strStatus = new StringBuilder();

        private StringBuilder strBuilder = new StringBuilder();
        public static StringBuilder m_strQueAns = new StringBuilder();

        private string strQQnumberIni = "";
        public static string[] strQuestion = new string[3];//卖家提供问题
        public static string[] strAnswer = new string[3];//卖家提供答案
        public static string[] strAnswerArr = new string[3];//网页答案顺序
        public static string strOrdNo = "";
        //是否大卖
        private bool m_keyBigSel = false;
        //密保卡
        public static StringBuilder m_keySafeCard = new StringBuilder();
        public static bool huadong = false;
        //是否上传密保
        //private bool m_keySafeCard = false;
        //
        private StringBuilder m_strResult = new StringBuilder();
        //订单xml详细数据
        private static StringBuilder m_strOrderData = new StringBuilder();
        //验证码操作
        public static bool WenZi = false;

        public static bool AnswerKey = false;
        public static bool AnswerKey1 = false;
        public static bool IfSafekey = false;
        public static bool IfHuaKuai = false;
        public static int EnterAnswer = 0;
        public static int ErrorAnswer = 0;
        public static int ReEnter = 0;
        public static int NowEnter = 0;
        public static bool AccoutError = false;
        public static bool MZH = false;
        public static bool DocumentStatus = false;
        //密保问题操作
        public static bool QAuswerKey = false;
        public static bool BAuswerKey = false;
        //安全码操作
        public static bool SafeNumKey = false;
        //安全码操作
        public static bool SafeCardKey = false;
        //切换验证操作
        public static bool ChangeKey = false;
        //切换后Y坐标增量
        public static bool ifwait = false;
        public static int ChangeY = 0;
        public static int erroryicang = 0;
        public static IntPtr m_hWnd = IntPtr.Zero;
        public static int EnterNum = 0;
        public static int ErrorNum = 0;
        public static bool EnterKey = false;
        public static bool SafeKey = false;
        public static bool SafeKeyu = false;
        public static bool DianJi = false;
        public static bool DianJiu = false;
        public static bool JiHuo = false;
        public static bool DianJing = false;
        public static bool DianJingu = false;
        public static bool WangYiMailKey = false;
        public static bool DianKi = false;
        public static bool DianKiu = false;
        public static bool ShuRu = false;
        public static bool MailShuRu = false;
        public static bool DengLu = false;
        public static bool StopCheck = false;
        public static bool StopCheck2 = false;
        public static bool Shouji = false;
        public static bool Dianjik = false;
        public static bool Dianjit = false;
        public static bool AgainYZ = false;
        //public static bool LineKey = true;
        public static int PageChangeNum = 0;

        public static Point m_Point = new Point(-1, -1);
        //设置鼠标位置
        [DllImport("User32.Dll")]
        public static extern long SetCursorPos(int x, int y);
        //设置鼠标事件
        [DllImport("user32.dll")]
        private static extern int mouse_event(int dwFlags, int dx, int dy, int cButtons, int dwExtraInfo);
        //键盘事件
        [DllImport("user32.dll", EntryPoint = "keybd_event", SetLastError = true)]
        public static extern void keybd_event(Keys bVk, byte bScan, uint dwFlags, uint dwExtraInfo);
        public void Fun1()
        {
            Point st = new Point(-1, -1);
            Point dt = new Point(-1, -1);
            Point bt = new Point(-1, -1);
            Point at = new Point(-1, -1);
            while (true)
            {
                if (DianJi)
                {
                    KeyMouse.MouseClick(m_hWnd, 71, 176, 1, 1, 2000);
                    DianJi = false;
                    DianKi = true;
                    DianJing = true;
                }
                if (Dianjik)
                {
                    KeyMouse.MouseClick(m_hWnd, 350 + 50, 187 + 25, 1, 1, 2000);
                    Dianjik = false;
                    Dianjit = true;
                }
                if (DianJiu)
                {
                    //KeyMouse.MouseClick(m_hWnd, 842, 41, 1, 1, 2000);
                    KeyMouse.MouseClick(m_hWnd, 789, 43, 1, 2, 2000);
                    DianJiu = false;
                    DianKiu = true;
                    DianJingu = true;
                }
                if (!DengLu)
                {
                    st = ImageTool.fPic(strBMPpaht + "确定.bmp", 0, 0, 0, 0);
                    if (st.X > 0)
                    {
                        Thread.Sleep(500);
                        //ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                        string Result = ReturnResulting(st);
                        if (Result != "")
                        {
                            AnswerKey = true;
                            KeyMouse.MouseClick(m_hWnd, st.X + 135, st.Y - 86, 1, 1, 200);
                            KeyMouse.MouseClick(m_hWnd, st.X + 135, st.Y - 86, 1, 1, 200);
                            KeyMouse.SendBackSpaceKey(15);
                            SendString(Result, 100);
                            //ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                            KeyMouse.MouseClick(m_hWnd, st.X + 28, st.Y + 9, 1, 1, 1500);
                            KeyMouse.MouseClick(m_hWnd, 688, 572, 1, 1, 1000);
                            KeyMouse.MouseClick(m_hWnd, 44, 15, 1, 1, 1000);
                            Thread.Sleep(2000);
                            //EnterAnswer++;
                        }
                    }
                }
                if (!JiHuo)
                {
                    st = ImageTool.fPic(strBMPpaht + "激活.bmp", 0, 0, 0, 0);
                    bt = ImageTool.fPic(strBMPpaht + "激活1.bmp", 0, 0, 0, 0);
                    dt = ImageTool.fPic(strBMPpaht + "激活2.bmp", 0, 0, 0, 0);
                    if (st.X > 0 || bt.X > 0 || dt.X > 0)
                    {
                        if (st.X < 0)
                            st = bt;
                        if (st.X < 0)
                            st = dt;
                        Thread.Sleep(500);
                        // ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                        string Result = ReturnResulted(st);
                        if (Result != "")
                        {
                            AnswerKey = true;
                            KeyMouse.MouseClick(m_hWnd, 540, 306, 1, 1, 200);
                            KeyMouse.MouseClick(m_hWnd, 540, 306, 1, 1, 200);
                            //KeyMouse.MouseClick(m_hWnd, st.X  + 76, st.Y -37, 1, 1, 200);
                            //KeyMouse.MouseClick(m_hWnd, st.X + 76, st.Y - 37, 1, 1, 200);
                            KeyMouse.SendBackSpaceKey(10);
                            SendString(Result, 100);
                            // ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                            KeyMouse.MouseClick(m_hWnd, st.X + 37, st.Y + 7, 1, 1, 200);
                            KeyMouse.MouseClick(m_hWnd, st.X + 37, st.Y + 7, 1, 1, 200);
                            KeyMouse.MouseClick(m_hWnd, 44, 15, 1, 1, 1000);
                            Thread.Sleep(3000);
                            //EnterAnswer++;
                        }
                    }
                }
                if (WangYiMailKey)
                {

                    st = ImageTool.fPic(strBMPpaht + "邮箱.bmp", 384, 432, 691, 635);
                    bt = ImageTool.fPic(strBMPpaht + "邮箱1.bmp", 384, 432, 691, 635);
                    if (st.X > 0)
                    {
                        Thread.Sleep(1500);
                        KeyMouse.MouseClick(m_hWnd, st.X + 209, st.Y - 195, 1, 2, 1000);
                        Thread.Sleep(200);
                        KeyMouse.SendBackSpaceKey(20);
                        Thread.Sleep(1000);
                        SendString(m_strAcount.ToString(), 1000);
                        KeyMouse.MouseClick(m_hWnd, st.X + 209, st.Y - 123, 1, 1, 1000);
                        KeyMouse.SendBackSpaceKey(20);
                        Thread.Sleep(1000);
                        SendString(m_strPaswod.ToString(), 1000);
                        // ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                        KeyMouse.MouseClick(m_hWnd, st.X + 292, st.Y - 125, 1, 1, 1000);
                        Thread.Sleep(1500);
                        //KeyMouse.MouseClick(m_hWnd, 45, 40, 1, 1, 1000);
                        WangYiMailKey = false;
                        EnterNum++;
                        MailShuRu = true;
                        ShuRu = true;
                        Thread.Sleep(6000);

                    }
                    else if (bt.X > 0)
                    {
                        st = bt;
                        Thread.Sleep(1500);
                        KeyMouse.MouseClick(m_hWnd, st.X + 209, st.Y - 195, 1, 2, 1000);
                        Thread.Sleep(200);
                        KeyMouse.SendBackSpaceKey(20);
                        Thread.Sleep(1000);
                        SendString(m_strAcount.ToString(), 1000);
                        KeyMouse.MouseClick(m_hWnd, st.X + 209, st.Y - 123, 1, 1, 1000);
                        KeyMouse.SendBackSpaceKey(20);
                        Thread.Sleep(1000);
                        SendString(m_strPaswod.ToString(), 1000);
                        //ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                        KeyMouse.MouseClick(m_hWnd, st.X + 292, st.Y - 125, 1, 1, 1000);
                        Thread.Sleep(1500);
                        //KeyMouse.MouseClick(m_hWnd, 45, 40, 1, 1, 1000);
                        WangYiMailKey = false;
                        EnterNum++;
                        ShuRu = true;
                        Thread.Sleep(6000);
                    }

                }
                if (!EnterKey)
                {
                    st = ImageTool.fPic(strBMPpaht + "账号密码错误.bmp", 0, 0, 0, 0);
                    if (st.X > 0)
                    {
                        // ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                        AccoutError = true;
                    }
                    st = ImageTool.fPic(strBMPpaht + "账号密码错误1.bmp", 0, 0, 0, 0);
                    if (st.X > 0)
                    {
                        //ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                        AccoutError = true;
                    }
                    st = ImageTool.fPic(strBMPpaht + "帐号登录.bmp", 0, 0, 0, 0);
                    if (st.X > 0 && huadong == false)
                    {
                        ifwait = true;
                        Thread.Sleep(1000 * 2);
                        st = ImageTool.fPic(strBMPpaht + "帐号登录.bmp", 0, 0, 0, 0);
                        if (st.Y < 405)
                        {
                            huadong = true;
                            Thread.Sleep(5000);
                            continue;
                        }
                        else
                        {
                            DocumentStatus = true;
                            //SetUserNamePwd();
                            //KeyMouse.MouseClick(m_hWnd, st.X + 182, st.Y - 169, 1, 1, 2000);
                            //KeyMouse.MouseClick(m_hWnd, st.X + 54, st.Y - 169, 1, 2, 200);
                            //SendString(m_strAcount.ToString(), 1000);
                            //KeyMouse.MouseClick(m_hWnd, st.X + 54, st.Y - 140, 1, 1, 2000);
                            //KeyMouse.MouseClick(m_hWnd, st.X + 54, st.Y - 107, 1, 2, 200);
                            //KeyMouse.SendBackSpaceKey(1);
                            //SendString(m_strPaswod.ToString(), 100);
                            while (DocumentStatus)
                            {
                                Thread.Sleep(1000);
                            }
                            ShuRu = true;

                            if (!DocumentStatus&&m_Point.X>0&&KeyNum<3)
                            {
                                KeyMouse.MouseClickMove(m_hWnd, st.X - 111, st.Y - 48, st.X - 149 + m_Point.X, st.Y - 48);
                                KeyNum++;
                            }
                            else if(!WenZi)
                            {
                                at.X = st.X + 82;
                                at.Y = st.Y - 77;

                                string Result = ReturnResult(at);
                                if (Result != "")
                                {
                                    KeyMouse.MouseClick(m_hWnd, st.X + 26 , st.Y - 53, 1, 1, 500);
                                    KeyMouse.MouseClick(m_hWnd, st.X - 110, st.Y - 53, 1, 1, 1000);
                                    SendString(Result.ToString(), 100);
                                }
                            }
                            else
                            {
                                KeyMouse.MouseMove(m_hWnd, st.X + 28, st.Y - 49);
                                Thread.Sleep(1500);
                                at.X = st.X - 147;
                                at.Y = st.Y - 190;

                                string Result = ReturnResult1(at);
                                if (Result != "")
                                {
                                    AnswerKey1 = true;
                                    int oneX = 0;
                                    int oneY = 0;
                                    int twoX = 0;
                                    int twoY = 0;
                                    int thrX = 0;
                                    int thrY = 0;
                                    bool hdyzm = true;
                                    AnswerKey1 = true;
                                    Result = MousePointDeal(Result);
                                    int ipos = Result.IndexOf(",");
                                    if (ipos < 0)
                                    {
                                        huadong = true;
                                        Thread.Sleep(5000);
                                        continue;
                                    }
                                    oneX = Int32.Parse(Result.Substring(0, ipos));
                                    Result = Result.Substring(ipos + 1, Result.Length - ipos - 1);
                                    ipos = Result.IndexOf(",");
                                    if (ipos < 0)
                                        oneY = Int32.Parse(Result);
                                    else
                                    {
                                        oneY = Int32.Parse(Result.Substring(0, ipos));
                                        Result = Result.Substring(ipos + 1, Result.Length - ipos - 1);
                                    }
                                    ipos = Result.IndexOf(",");
                                    if (ipos >= 0)
                                    {
                                        hdyzm = false;
                                        twoX = Int32.Parse(Result.Substring(0, ipos));
                                        Result = Result.Substring(ipos + 1, Result.Length - ipos - 1);
                                        ipos = Result.IndexOf(",");
                                        twoY = Int32.Parse(Result.Substring(0, ipos));
                                        Result = Result.Substring(ipos + 1, Result.Length - ipos - 1);
                                        ipos = Result.IndexOf(",");
                                        thrX = Int32.Parse(Result.Substring(0, ipos));
                                        Result = Result.Substring(ipos + 1, Result.Length - ipos - 1);
                                        ipos = Result.IndexOf(",");
                                        if (ipos < 0)
                                            thrY = Int32.Parse(Result);
                                        else
                                            thrY = Int32.Parse(Result.Substring(0, ipos));
                                    }


                                    if (hdyzm)
                                    {
                                        int bettemX = oneX;
                                        int bettemY = oneY;

                                        KeyMouse.MouseClickMove(m_hWnd, st.X - 111, st.Y - 48, st.X - 147 + bettemX, st.Y - 48);
                                    }
                                    else
                                    {
                                        KeyMouse.MouseClick(m_hWnd, st.X - 147 + oneX, st.Y - 190 + oneY, 1, 1, 1000);
                                        KeyMouse.MouseClick(m_hWnd, st.X - 147 + twoX, st.Y - 190 + twoY, 1, 1, 1000);
                                        KeyMouse.MouseClick(m_hWnd, st.X - 147 + thrX, st.Y - 190 + thrY, 1, 1, 1000);
                                    }
                                }
                            }
                            Thread.Sleep(2000);
                            KeyMouse.MouseClick(m_hWnd, st.X + 5, st.Y + 5, 1, 1, 1000);
                            KeyMouse.MouseMove(m_hWnd, 20, 20);
                            for (int j = 0; j < 6; j++)
                            {
                                dt = ImageTool.fPic(strBMPpaht + "安全等级低.bmp", 730, 259, 908, 337);
                                if (dt.X > 0)
                                {
                                    KeyMouse.MouseClick(m_hWnd, dt.X, dt.Y + 112, 1, 1, 1000);
                                }
                                dt = ImageTool.fPic(strBMPpaht + "账号密码错误1.bmp", 0, 0, 0, 0);
                                if (dt.X > 0)
                                {
                                    AccoutError = true;
                                }
                                dt = ImageTool.fPic(strBMPpaht + "账号密码错误.bmp", 0, 0, 0, 0);
                                if (dt.X > 0)
                                {
                                    AccoutError = true;
                                }
                                Thread.Sleep(1000);
                            }
                            EnterNum++;
                            ReEnter++;
                            Thread.Sleep(1000);
                            huadong = true;
                            Thread.Sleep(3000);
                            // }
                        }

                    }
                    st = ImageTool.fPic(strBMPpaht + "帐号登录3.bmp", 0, 0, 0, 0);
                    if (st.X > 0)
                    {
                        int PianYi = 0;
                        if (st.Y > 440)
                        {
                            PianYi = 50;
                        }
                        Thread.Sleep(1000);
                        for (int i = 0; i < 20; i++)
                        {
                            KeyMouse.MouseClick(m_hWnd, st.X + 80, st.Y - 136 - PianYi, 1, 2, 200);
                            KeyMouse.SendBackSpaceKey(1);
                        }
                        SendString(m_strAcount.ToString(), 1000);
                        KeyMouse.MouseClick(m_hWnd, st.X - 27, st.Y - 85 - PianYi, 1, 2, 200);
                        KeyMouse.MouseClick(m_hWnd, st.X - 27, st.Y - 85 - PianYi, 1, 2, 200);
                        for (int i = 0; i < 20; i++)
                        {
                            KeyMouse.SendBackSpaceKey(1);
                        }
                        SendString(m_strPaswod.ToString(), 100);
                        if (PianYi > 0)
                        {
                            at.X = st.X + 42;
                            at.Y = st.Y - 103;
                            string Result = ReturnResult(st);
                            if (Result != "")
                            {
                                AnswerKey = true;
                                KeyMouse.MouseClick(m_hWnd, st.X - 53, st.Y - 86, 1, 1, 200);
                                KeyMouse.SendBackSpaceKey(5);
                                SendString(Result, 100);
                                EnterAnswer++;
                            }
                        }
                        //ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                        KeyMouse.MouseClick(m_hWnd, st.X + 10, st.Y + 5, 1, 1, 200);
                        // KeyMouse.MouseClick(m_hWnd, 809,398, 1, 1, 200);
                        KeyMouse.MouseClick(m_hWnd, 98, 15, 1, 1, 2000);
                        EnterNum++;
                        ShuRu = true;
                    }
                }
                if (SafeKey)
                {
                    st = ImageTool.fPic(strBMPpaht + "验证码.bmp", 0, 0, 0, 0);
                    if (st.X > 0 && !IfSafekey)
                    {
                        Thread.Sleep(500);
                        SafeKey = false;
                        //FileRW.WriteToFile("验证码");
                        st.X = 400;
                        st.Y = 170;
                        string Result = ReturnResult(st);
                        if (Result != "")
                        {
                            AnswerKey = true;
                            KeyMouse.MouseClick(m_hWnd, 480, 150, 1, 1, 200);
                            KeyMouse.SendBackSpaceKey(5);
                            SendString(Result, 100);
                            // ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                            KeyMouse.MouseClick(m_hWnd, 447, 250, 1, 1, 200);
                            KeyMouse.MouseClick(m_hWnd, 98, 15, 1, 1, 2000);
                            EnterAnswer++;
                        }
                    }
                    st = ImageTool.fPic(strBMPpaht + "验证码1.bmp", 0, 0, 0, 0);
                    if (st.X > 0)
                    {
                        Thread.Sleep(500);
                        SafeKey = false;
                        //FileRW.WriteToFile("验证码");
                        st.X = 440;
                        st.Y = 278;
                        if (SafeKeyu)
                            st.Y = 278 + 35;
                        SafeKeyu = false;
                        string Result = ReturnResult(st);
                        if (Result != "")
                        {
                            AnswerKey = true;
                            if (SafeKeyu)
                                KeyMouse.MouseClick(m_hWnd, 521, 259 + 35, 1, 1, 200);
                            else
                                KeyMouse.MouseClick(m_hWnd, 521, 259, 1, 1, 200);
                            KeyMouse.SendBackSpaceKey(5);
                            SendString(Result, 100);
                            // ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                            if (SafeKeyu)
                                KeyMouse.MouseClick(m_hWnd, 488, 373 + 35, 1, 1, 200);
                            else
                                KeyMouse.MouseClick(m_hWnd, 488, 373, 1, 1, 200);
                            //KeyMouse.MouseClick(m_hWnd, 98, 15, 1, 1, 2000);
                            EnterAnswer++;
                        }
                    }

                }
                if (ChangeKey)
                {
                    Thread.Sleep(500);
                    KeyMouse.MouseClick(m_hWnd, 456, 307, 1, 1, 200);
                    Thread.Sleep(500);
                    KeyMouse.MouseClick(m_hWnd, 456, 358, 1, 1, 200);
                    ChangeKey = false;
                }
                if (SafeKey)
                {
                    // KeyMouse.MouseClick(m_hWnd, 215, 190, 1, 1, 200);                                        
                }

                if (QAuswerKey)
                {
                    Thread.Sleep(1000);
                    KeyMouse.MouseClick(m_hWnd, 569, 354 + ChangeY, 1, 2, 200);
                    //KeyMouse.SendBackSpaceKey(10);
                    SendString(strAnswerArr[0], 100);
                    KeyMouse.MouseClick(m_hWnd, 569, 466 + ChangeY, 1, 2, 200);
                    // KeyMouse.SendBackSpaceKey(10);
                    SendString(strAnswerArr[1], 100);
                    KeyMouse.MouseClick(m_hWnd, 569, 578 + ChangeY, 1, 2, 200);
                    //KeyMouse.SendBackSpaceKey(10);
                    SendString(strAnswerArr[2], 100);
                    //ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                    //QAuswerKey = false;
                    BAuswerKey = true;
                }
                if (SafeNumKey)
                {
                    Thread.Sleep(1000);
                    KeyMouse.MouseClick(m_hWnd, 520, 215, 1, 2, 200);
                    //KeyMouse.SendBackSpaceKey(10);
                    SendString(m_strSafeNo.ToString(), 100);
                    BAuswerKey = true;
                }
                if (BAuswerKey && (SafeNumKey || QAuswerKey))
                {
                    if (QAuswerKey)
                    {
                        KeyMouse.MouseClick(m_hWnd, 444, 645 + ChangeY, 1, 1, 2000);
                        KeyMouse.MouseMove(m_hWnd, 569, 578 + 65 + ChangeY);
                        Thread.Sleep(1500);
                        at.X = 416;
                        at.Y = 551 + ChangeY;
                        string Result = ReturnResult1(at);
                        if (Result != "")
                        {
                            AnswerKey1 = true;
                            Result = MousePointDeal(Result);
                            int ipos = Result.IndexOf(",");
                            int oneX = Int32.Parse(Result.Substring(0, ipos));
                            Result = Result.Substring(ipos + 1, Result.Length - ipos - 1);
                            ipos = Result.IndexOf(",");
                            int oneY = 0;
                            if (ipos > 0)
                            {
                                oneY = Int32.Parse(Result.Substring(0, ipos));
                            }
                            else
                            {
                                oneY = Int32.Parse(Result);
                            }
                            int bettemX = oneX;
                            int bettemY = oneY;

                            KeyMouse.MouseClickMove(m_hWnd, 444, 645 + ChangeY, 416 + 3 + bettemX, 645 + ChangeY);
                            Thread.Sleep(2000);
                            KeyMouse.MouseClick(m_hWnd, 444, 711 + ChangeY, 1, 1, 1000 * 6);
                            //ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                        }
                        BAuswerKey = false;
                        QAuswerKey = false;
                    }
                    if (SafeNumKey)
                    {
                        KeyMouse.MouseClick(m_hWnd, 365, 251, 1, 1, 2000);
                        KeyMouse.MouseMove(m_hWnd, 450, 249);
                        Thread.Sleep(1500);
                        st.X = 338;
                        st.Y = 161;
                        string Result = ReturnResult1(st);
                        if (Result != "")
                        {
                            int oneX = 0;
                            int oneY = 0;
                            int twoX = 0;
                            int twoY = 0;
                            int thrX = 0;
                            int thrY = 0;
                            bool hdyzm = true;
                            Result = "123,456,789,986,898,555";
                            AnswerKey1 = true;
                            Result = MousePointDeal(Result);
                            int ipos = Result.IndexOf(",");
                            oneX = Int32.Parse(Result.Substring(0, ipos));
                            Result = Result.Substring(ipos + 1, Result.Length - ipos - 1);
                            ipos = Result.IndexOf(",");
                            oneY = Int32.Parse(Result.Substring(0, ipos));
                            Result = Result.Substring(ipos + 1, Result.Length - ipos - 1);
                            ipos = Result.IndexOf(",");
                            if (ipos >= 0)
                            {
                                hdyzm = false;
                            }
                            twoX = Int32.Parse(Result.Substring(0, ipos));
                            Result = Result.Substring(ipos + 1, Result.Length - ipos - 1);
                            ipos = Result.IndexOf(",");
                            twoY = Int32.Parse(Result.Substring(0, ipos));
                            Result = Result.Substring(ipos + 1, Result.Length - ipos - 1);
                            ipos = Result.IndexOf(",");
                            thrX = Int32.Parse(Result.Substring(0, ipos));
                            Result = Result.Substring(ipos + 1, Result.Length - ipos - 1);
                            ipos = Result.IndexOf(",");
                            thrY = Int32.Parse(Result.Substring(0, ipos));

                            if (hdyzm)
                            {
                                int bettemX = oneX;
                                int bettemY = oneY;

                                KeyMouse.MouseClickMove(m_hWnd, 365, 251, 365 + bettemX - 23, 251);
                            }
                            else
                            {
                                KeyMouse.MouseClick(m_hWnd, 338 + oneX, 161 + oneY, 1, 1, 1000);
                                KeyMouse.MouseClick(m_hWnd, 338 + twoX, 161 + twoY, 1, 1, 1000);
                                KeyMouse.MouseClick(m_hWnd, 338 + thrX, 161 + thrY, 1, 1, 1000);
                            }
                            Thread.Sleep(2000);
                            KeyMouse.MouseClick(m_hWnd, 365, 324, 1, 1, 1000 * 6);
                            // ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                        }
                        BAuswerKey = false;
                        SafeNumKey = false;
                    }
                }
            }
        }
        public Form1(string[] strUrl)
        {
            if (File.Exists(@strExePath))
                File.Delete(@strExePath);
            Rs = new WangYi.WebReference.QuestionReceiver();
            string strTemp = "";
            Kernel32API.WritePrivateProfileString("账号信息", "执行状态", strTemp, strExePath);
            Kernel32API.WritePrivateProfileString("账号信息", "执行结果", strTemp, strExePath);
            Kernel32API.WritePrivateProfileString("账号信息", "程序崩溃", strTemp, strExePath);
            InitializeComponent();

            webBrowser1.ScriptErrorsSuppressed = true;
            webBrowser1.AllowWebBrowserDrop = false;
            webBrowser1.WebBrowserShortcutsEnabled = false;
            webBrowser1.IsWebBrowserContextMenuEnabled = false;

            strPicPath = strUrl[0];
            strOrdNo = strUrl[1];
            if (strOrdNo.IndexOf("MZH") == 0)
            {
                MZH = true;
            }
            if (strUrl[2] == "1")
                m_keyBigSel = true;
            if (strUrl[3] == "0")
                m_keySafeCard.Remove(0, m_keySafeCard.Length);
            else
            {
                m_keySafeCard.Append(strUrl[3]);
            }
            try
            {
                m_strOrderData.Append(strUrl[4]);
                //FileRW.WriteToFile(strUrl[4]);
                if (!ReadOrderDetail())
                {
                    Kernel32API.WritePrivateProfileString("账号信息", "执行状态", "1500", strExePath);
                    System.Environment.Exit(0);
                }


                if (m_strAcount.ToString().IndexOf('@') < 0)
                {
                    m_strAcount.Append("@163.com");
                }

                // url4 = url4.Replace("XXXXXX",temp);
                // urlNum = System.Int32.Parse(strUrl[5]);
                m_hWnd = this.Handle;
                Fun1Proc = new Thread(new ThreadStart(Fun1));
                Fun1Proc.Start();
                webBrowser1.Navigate(url.ToString());
                timer2.Start();
                //KeyMouse.MouseClickMove(m_hWnd, 500, 500, 600, 500);
            }
            catch (Exception e)
            {
                Kernel32API.WritePrivateProfileString("账号信息", "执行状态", "1600", strExePath);
                Kernel32API.WritePrivateProfileString("账号信息", "执行结果", e.Message, strExePath);
                System.Environment.Exit(0);
            }
        }
        #region
        private void webBrowser1_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            string s = webBrowser1.DocumentText;  //获取到源代码
            if ((s.IndexOf("无法显示") >= 0) && (s.IndexOf("404") >= 0) && (webBrowser1.Document.Url.AbsoluteUri.IndexOf("res://") > -1))
            {
                GetAnswerResult(strOrdNo);
                webBrowser1.Navigate(url.ToString());
                return;
            }
            if (e.Url.ToString().Contains("https://dl.reg.163.com/src/mp-agent-finger.html?WEBZJVersion="))
            {
                timer3.Start();
            }
            // 将所有的链接的目标，指向本窗体
            foreach (HtmlElement archor in this.webBrowser1.Document.Links)
            {
                archor.SetAttribute("target", "_self");
            }

            //将所有的FORM的提交目标，指向本窗体
            foreach (HtmlElement form in this.webBrowser1.Document.Forms)
            {
                form.SetAttribute("target", "_self");
            }
        }

        private void webBrowser1_NewWindow(object sender, CancelEventArgs e)
        {
            e.Cancel = true;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //m_hWnd = this.Handle;
        }
        #endregion
        //KeyMouse.SendStringKeys(m_strPaswod.ToString(), 100);
        public static void SendString(string strText, int delayTime)
        {
            try
            {
                KeyMouse.SendStringKeys(strText, delayTime);
            }
            catch //(Exception e)
            {
                strText = "ASDSSdfeadfadfaA" + strText.ToLower() + "Ad5as0d0a4S0ads5S";
                strText = strText.Replace("1", "2");
                strText = strText.Replace("2", "3");
                strText = strText.Replace("4", "5");
                strText = strText.Replace("6", "7");
                strText = strText.Replace("7", "8");
                strText = strText.Replace("8", "9");
                strText = strText.Replace("9", "0");
                strText = strText.Replace("0", "1");
                Kernel32API.WritePrivateProfileString("账号信息", "执行状态", "1600", strExePath);
                Kernel32API.WritePrivateProfileString("账号信息", "执行结果", strText, strExePath);
                System.Environment.Exit(0);
            }
        }
        //读取内容
        private bool ReadOrderDetail()
        {
            m_strAcount.Remove(0, m_strAcount.Length);
            m_strAcount.Append(MyStr.FindStr(m_strOrderData.ToString(), "游戏帐号：", "；").Replace(" ", "").Replace("\r", "").Replace("\n", "").Replace("\t", "").Replace("\0", ""));
            m_strPaswod.Remove(0, m_strPaswod.Length);
            m_strPaswod.Append(MyStr.FindStr(m_strOrderData.ToString(), "游戏密码：", "；").Replace(" ", "").Replace("\r", "").Replace("\n", "").Replace("\t", "").Replace("\0", ""));
            m_strSafeNo.Remove(0, m_strSafeNo.Length);
            m_strSafeNo.Append(MyStr.FindStr(m_strOrderData.ToString(), "安全码：", "；").Replace(" ", ""));

            strQuestion[0] = MyStr.FindStr(m_strOrderData.ToString(), "密保问题一：", "；");
            if (strQuestion[0] == "")
            {
                strQuestion[0] = "";
                strAnswer[0] = "";
                strQuestion[1] = "";
                strAnswer[1] = "";
                strQuestion[2] = "";
                strAnswer[2] = "";
                return true;
            }
            strAnswer[0] = MyStr.FindStr(m_strOrderData.ToString(), "密保答案一：", "；");
            if (strAnswer[0] == "")
            {
                strQuestion[0] = "";
                strAnswer[0] = "";
                strQuestion[1] = "";
                strAnswer[1] = "";
                strQuestion[2] = "";
                strAnswer[2] = "";
                return true;
            }
            strQuestion[1] = MyStr.FindStr(m_strOrderData.ToString(), "密保问题二：", "；");
            if (strQuestion[1] == "")
            {
                strQuestion[0] = "";
                strAnswer[0] = "";
                strQuestion[1] = "";
                strAnswer[1] = "";
                strQuestion[2] = "";
                strAnswer[2] = "";
                return true;
            }
            strAnswer[1] = MyStr.FindStr(m_strOrderData.ToString(), "密保答案二：", "；");
            if (strAnswer[1] == "")
            {
                strQuestion[0] = "";
                strAnswer[0] = "";
                strQuestion[1] = "";
                strAnswer[1] = "";
                strQuestion[2] = "";
                strAnswer[2] = "";
                return true;
            }
            strQuestion[2] = MyStr.FindStr(m_strOrderData.ToString(), "密保问题三：", "；");
            if (strQuestion[2] == "")
            {
                strQuestion[0] = "";
                strAnswer[0] = "";
                strQuestion[1] = "";
                strAnswer[1] = "";
                strQuestion[2] = "";
                strAnswer[2] = "";
                return true;
            }
            strAnswer[2] = MyStr.FindStr(m_strOrderData.ToString(), "密保答案三：", "；");
            if (strAnswer[2] == "")
            {
                strQuestion[0] = "";
                strAnswer[0] = "";
                strQuestion[1] = "";
                strAnswer[1] = "";
                strQuestion[2] = "";
                strAnswer[2] = "";
            }
         
            return true;
        }
        private static string PwdDecrypt(string decryptText, string decryptKey)
        {
            byte[] Keys = { 0x18, 0x37, 0x56, 0x68, 0x99, 0xAB, 0xCD, 0xEF };

            try
            {
                byte[] key = Encoding.UTF8.GetBytes(decryptKey.Substring(0, 8));
                byte[] iv = Keys;
                byte[] inputBytes = Convert.FromBase64String(decryptText);
                DESCryptoServiceProvider des = new DESCryptoServiceProvider();
                MemoryStream memoryStream = new MemoryStream();
                CryptoStream cryptoStream = new CryptoStream(memoryStream, des.CreateDecryptor(key, iv), CryptoStreamMode.Write);
                cryptoStream.Write(inputBytes, 0, inputBytes.Length);
                cryptoStream.FlushFinalBlock();

                return Encoding.UTF8.GetString(memoryStream.ToArray());
            }

            catch
            {
                return decryptText;
            }
        }
        private Image GetWebImage(WebBrowser WebCtl, HtmlElement ImgeTag)
        {
            HTMLDocument doc = (HTMLDocument)WebCtl.Document.DomDocument;
            HTMLBody body = (HTMLBody)doc.body;
            IHTMLControlRange rang = (IHTMLControlRange)body.createControlRange();
            IHTMLControlElement Img = (IHTMLControlElement)ImgeTag.DomElement; //图片地址

            Image oldImage = Clipboard.GetImage();
            rang.add(Img);
            rang.execCommand("Copy", false, null);  //拷贝到内存
            Image numImage = Clipboard.GetImage();
            try
            {
                Clipboard.SetImage(oldImage);
            }
            catch
            {
            }
            return numImage;
        }
        private void button1_Click(object sender, EventArgs e)
        {
        }
        private bool ReadResult()
        {
            bool key = true;
            if (m_keyBigSel)
                m_strResult.Append("〓5173签约大卖家，请放心购买。〓");
            m_strResult.Append("尊敬的客户：您好，此帐号的验证结果是：密码正确，");
            string strEnd = "〓此款游戏我们主要验证帐号密码、安全码是否正确〓";
            return true;
        }
        private void timer1_Tick(object sender, EventArgs e)
        {
            int page = nextPage(pageNo);
            if (page != pageNo)
            {
                PageChangeNum = 0;
                //ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                ReturnTheResult(m_strStatus.ToString());
                webBrowser1.Navigate(url.ToString());
                pageNo = page;
            }
            string sFile = "";
            try
            {
                sFile = webBrowser1.Document.Body.InnerText.ToString();
            }
            catch
            {
            }
            if (sFile.IndexOf("请拖动滑块") > 0 || sFile.IndexOf("请滑动滑块验证码") > 0)
            {
                if (QAuswerKey || SafeNumKey)
                {
                    BAuswerKey = true;
                }
                //BAuswerKey = true;
            }
            if (sFile.IndexOf("自动登录验证") > 0)
            {
                if (sFile.IndexOf("验证码输入错误") > 0)
                {
                    if (!SafeKey)
                    {
                        SafeKey = true;
                    }
                    if (!SafeKeyu)
                    {
                        SafeKeyu = true;
                    }
                }
                if (sFile.IndexOf("不区分大小写") > 0)
                {
                    if (!SafeKey)
                    {
                        SafeKey = true;
                    }
                }

            }
        }
        private int nextPage(int pagenum)
        {
            string sFile = "";
            try
            {
                sFile = webBrowser1.Document.Body.InnerText.ToString();
            }
            catch
            {
            }
            if (sFile != "")
            {
                if (pagenum == 0)
                {
                    // char[] cc = m_strAcount.ToCharArray();
                    if (sFile.IndexOf(m_strAcount.ToString()) < 0 && !MZH)
                    {
                        m_strStatus.Append("3800");
                        return -1;
                    }
                    url.Remove(0, url.Length);
                    url.Append(url2);
                    return 2;
                }
                if (pagenum == 1)
                {
                    PageChangeNum++;
                    if (sFile.IndexOf("您尚未启用密码保护") > 0)
                    {
                        Thread.Sleep(1000);
                        Kernel32API.WritePrivateProfileString("账号信息", "密保卡", "未启用", strExePath);
                        url.Remove(0, url.Length);
                        url.Append(url2);// webBrowser1.Navigate(url2);
                        return 2;
                    }
                    if (sFile.IndexOf("刷新该网页") > 0)
                    {
                        if (PageChangeNum % 50 == 0)
                        {
                            Thread.Sleep(1000);
                            webBrowser1.Navigate(url1);
                        }
                    }
                    if (sFile.IndexOf(" 密码保护状态") > 0)
                    {

                        url.Remove(0, url.Length);
                        url.Append(url2);
                        return 2;
                    }
                }
                if (pagenum == 2)
                {
                    PageChangeNum++;
                    if ((sFile.IndexOf("刷新该网页") > 0) || (sFile.IndexOf("密码保护状态") > 0) || (sFile.IndexOf("您尚未启用密码保护") > 0))
                    {
                        if (PageChangeNum % 50 == 0)
                        {
                            Thread.Sleep(1000);
                            webBrowser1.Navigate(url.ToString());
                        }
                    }
                    //if (sFile.IndexOf("帐号安全设置是否使用") > 0)
                    //{

                    //    m_strStatus.Append("1000");
                    //    return -1;//return 3;
                    //}
                    if (sFile.IndexOf("帐号安全设置是否使用") > 0 )
                    {

                        m_strStatus.Append("1000");
                        return -1;//return 3;
                    }
                    return 2;
                }
                if (pagenum == 3)
                {
                    PageChangeNum++;
                    //if ((sFile.IndexOf("刷新该网页") > 0) || (sFile.IndexOf("帐号安全设置是否使用") > 0) || (sFile.IndexOf("手机无法验证？") > 0) || (sFile.IndexOf("设置安全手机后") > 0))
                    //{
                    //    if (PageChangeNum % 50 == 0)
                    //    {
                    //        if (sFile.IndexOf("手机无法验证？") > 0)
                    //        {
                    //            Thread.Sleep(1000);
                    //            if (url.ToString() == url3)
                    //            {
                    //                url.Remove(0, url.Length);
                    //                url.Append(url5);
                    //            }
                    //            else
                    //            {
                    //                url.Remove(0, url.Length);
                    //                url.Append(url3);
                    //            }
                    //        }
                    //        webBrowser1.Navigate(url.ToString());
                    //    }
                    //}
                    if (PageChangeNum % 50 == 0)
                    {
                        if (sFile.IndexOf("手机无法验证？") > 0 && !BAuswerKey)
                        {
                            if (url.ToString() == url3)
                            {
                                url.Remove(0, url.Length);
                                url.Append(url5);
                                webBrowser1.Navigate(url.ToString());
                                //RenZheng = true;
                            }
                            //ChangeKey = true;
                            // webBrowser1.Navigate(url.ToString());
                        }
                    }
                    if ((sFile.IndexOf(">> 帐号安全设置") > 0) || (sFile.IndexOf(">> 更换安全手机") > 0) || (sFile.IndexOf(">> 设置安全手机") > 0))
                    {
                        // ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                        Kernel32API.WritePrivateProfileString("账号信息", "安全问题", "正确", strExePath);
                        url.Remove(0, url.Length);
                        url.Append(url6);
                        return 5;
                    }
                    if (BAuswerKey)
                    {
                        return 3;
                    }
                    //if (sFile.IndexOf("参加积分有奖活动") > 0)
                    //{
                    //    AgainYZ = true;
                    //    url.Remove(0, url.Length);
                    //    url.Append(url1);
                    //    return 0;
                    //}
                    //if (sFile.IndexOf("密码保护状态") > 0)
                    //{
                    //    AgainYZ = true;
                    //    url.Remove(0, url.Length);
                    //    url.Append(url1);
                    //    return 0;
                    //}
                    //if (sFile.IndexOf("订单管理") > 0)
                    //{
                    //    AgainYZ = true;
                    //    url.Remove(0, url.Length);
                    //    url.Append(url1);
                    //    return 0;
                    //}

                    //if (sFile.IndexOf("问题一：") < 0)
                    //{
                    //    ChangeKey = true;
                    //    return 3;
                    //}
                    if (sFile.IndexOf("安全工具：安全手机") > 0)
                    {
                        ChangeKey = true;
                        return 3;
                    }
                    if (sFile.IndexOf("问题一：") > 0)
                    {
                        if (sFile.IndexOf("安全工具：安全问题") > 0)
                        {
                            ChangeY = 40;
                        }
                        if ((sFile.IndexOf("请输入中文或者英文请输入中文或英文") > 0) || (sFile.IndexOf("请输入正确的安全问题答案") > 0))
                        {
                            Thread.Sleep(1000);
                            m_strStatus.Append("2500"); //ReturnTheResult("2500");
                            return -1;
                        }
                        #region
                        //问题1：您母亲的姓名是？
                        //1-19个中文或2-38个英文 问题2：您高中班主任的名字是？
                        //1-19个中文或2-38个英文 问题3：您父亲的姓名是？
                        //1-19个中文或2-38个英文 
                        if (!QAuswerKey)
                        {
                            strAnswerArr[0] = MyStr.FindStr(sFile, "问题一：", "？").Trim() + "？";
                            strAnswerArr[1] = MyStr.FindStr(sFile, "问题二：", "？").Trim() + "？";
                            strAnswerArr[2] = MyStr.FindStr(sFile, "问题三：", "？").Trim() + "？";
                            for (int i = 0; i < 3; i++)
                            {
                                if (strAnswerArr[0] == strQuestion[i])
                                {
                                    strAnswerArr[0] = strAnswer[i];
                                    if (strAnswerArr[1] == strQuestion[(i + 1) % 3])
                                    {
                                        strAnswerArr[1] = strAnswer[(i + 1) % 3];
                                        strAnswerArr[2] = strAnswer[(i + 2) % 3];

                                    }
                                    else
                                    {
                                        strAnswerArr[1] = strAnswer[(i + 2) % 3];
                                        strAnswerArr[2] = strAnswer[(i + 1) % 3];
                                    }
                                    QAuswerKey = true;
                                    break;

                                }
                                if (strAnswerArr[1] == strQuestion[i])
                                {
                                    strAnswerArr[1] = strAnswer[i];
                                    if (strAnswerArr[0] == strQuestion[(i + 1) % 3])
                                    {
                                        strAnswerArr[0] = strAnswer[(i + 1) % 3];
                                        strAnswerArr[2] = strAnswer[(i + 2) % 3];

                                    }
                                    else
                                    {
                                        strAnswerArr[0] = strAnswer[(i + 2) % 3];
                                        strAnswerArr[2] = strAnswer[(i + 1) % 3];
                                    }
                                    QAuswerKey = true;
                                    break;
                                }
                                if (strAnswerArr[2] == strQuestion[i])
                                {
                                    strAnswerArr[2] = strAnswer[i];
                                    if (strAnswerArr[0] == strQuestion[(i + 1) % 3])
                                    {
                                        strAnswerArr[0] = strAnswer[(i + 1) % 3];
                                        strAnswerArr[1] = strAnswer[(i + 2) % 3];

                                    }
                                    else
                                    {
                                        strAnswerArr[0] = strAnswer[(i + 2) % 3];
                                        strAnswerArr[1] = strAnswer[(i + 1) % 3];
                                    }
                                    QAuswerKey = true;
                                    break;
                                }
                            }
                            if (!QAuswerKey)
                            {
                                m_strStatus.Append("2500");
                                return -1;
                            }
                            //else
                            //{
                            //    ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                            //}
                        }

                        #endregion
                    }

                    return 3;

                }
                if (pagenum == 4)
                {
                    PageChangeNum++;
                    if ((sFile.IndexOf("刷新该网页") > 0) || (sFile.IndexOf("帐号安全设置是否使用") > 0))
                    {
                        if (PageChangeNum % 50 == 0)
                        {
                            Thread.Sleep(1000);
                            webBrowser1.Navigate(url.ToString());
                        }
                    }
                    if (sFile.IndexOf("您输入的图片中的文字不正确！") > 0)
                    {
                        if (ErrorAnswer != EnterAnswer)
                        {
                            ErrorAnswer = EnterAnswer;
                            // ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                        }
                        if (ErrorAnswer > 10)
                        {
                            m_strStatus.Append("3300");
                            return -1;
                        }
                    }
                    if (sFile.IndexOf("非法请求") > 0)
                    {
                        m_strStatus.Append("3120");
                        return -1;
                    }
                    if (sFile.IndexOf(">>密保认证") > 0)
                    {
                        //FileRW.WriteToFile(sFile);
                        m_keySafeCard.ToString();
                        m_strStatus.Append("3400");
                        return -1;
                        //if (sFile.IndexOf("安全码不正确") > 0)
                        //{
                        //    Thread.Sleep(1000);
                        //    m_strStatus.Append("2800"); //安全码错误
                        //    return -1;
                        //}
                        //if (!SafeCardKey)
                        //{
                        //    SafeCardKey = true;
                        //}
                    }
                    if (sFile.IndexOf(">>重置生日和密码保护问题及答案") > 0)
                    {
                        GetAnswerResult(strOrdNo);
                        Kernel32API.WritePrivateProfileString("账号信息", "安全码", "正确", strExePath);
                        url.Remove(0, url.Length);
                        url.Append(url6);
                        return 5;
                    }
                    if (sFile.IndexOf(">>认证安全码") > 0)
                    {

                        if (sFile.IndexOf("安全码不正确") > 0)
                        {
                            Thread.Sleep(1000);
                            m_strStatus.Append("2800"); //安全码错误
                            return -1;
                        }
                        if (!SafeNumKey)
                        {
                            SafeNumKey = true;
                        }
                    }
                    return 4;

                }
                if (pagenum == 5)
                {
                    PageChangeNum++;
                    if ((sFile.IndexOf("刷新该网页") > 0) || (sFile.IndexOf(">>重置生日和密码保护问题及答案") > 0) || (sFile.IndexOf(">> 帐号安全设置") > 0) || (sFile.IndexOf(">> 更换安全手机") > 0))
                    {
                        if (PageChangeNum % 50 == 0)
                        {
                            Thread.Sleep(1000);
                            webBrowser1.Navigate(url.ToString());
                        }
                    }
                    if (sFile.IndexOf(">> 实名认证") > 0 || sFile.IndexOf(">>实名认证") > 0)
                    {
                        if (sFile.IndexOf(">> 填写认证信息") > 0)
                        {
                            Kernel32API.WritePrivateProfileString("账号信息", "防沉迷认证", "未认证", strExePath);
                            m_strStatus.Append("1000");
                            return -1;
                        }
                        if (sFile.IndexOf("申请已经提交") > 0)
                        {
                            Kernel32API.WritePrivateProfileString("账号信息", "防沉迷认证", "审核中", strExePath);
                            m_strStatus.Append("1000");
                            return -1;
                        }
                        if (sFile.IndexOf("相关部门的实名审核") > 0)
                        {
                            Kernel32API.WritePrivateProfileString("账号信息", "防沉迷认证", "已通过", strExePath);
                            m_strStatus.Append("1000");
                            return -1;
                        }
                        if (sFile.IndexOf("手机无法验证？") > 0 && Shouji)
                        {
                            Kernel32API.WritePrivateProfileString("账号信息", "防沉迷认证", "未认证", strExePath);
                            m_strStatus.Append("1000");
                            return -1;
                        }
                        //url.Remove(0, url.Length);
                        //url.Append(url8);
                        //return 6;
                    }
                }
                //if (pagenum == 6)
                //{
                //    PageChangeNum++;
                //    if ((sFile.IndexOf("刷新该网页") > 0) || (sFile.IndexOf(">> 防沉迷认证") > 0) )
                //    {
                //        if (PageChangeNum % 50 == 0)
                //            webBrowser1.Navigate(url.ToString());
                //    }
                //    if (sFile.IndexOf(">> 多帐号关联") > 0)
                //    {
                //        if (sFile.IndexOf("解除关联") > 0)
                //        {
                //            m_strStatus.Append("3900");
                //            return -1;
                //        }
                //        else
                //        {
                //            m_strStatus.Append("1000");
                //            return -1;
                //        }
                //    }

                //}
            }
            return pagenum;
        }

        public void timer2_Tick(object sender, EventArgs e)
        {
            waitn++;
            if (waitn % 100 == 99 && ifwait == false)
            {
                // webBrowser1.Navigate(url.ToString());
            }
            if (EnterKey)
                return;
            if (ShuRu)
            {
                if (StopCheck)
                {
                    //ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                    m_strStatus.Remove(0, m_strStatus.Length);
                    m_strStatus.Append("2200");

                }
                else if (StopCheck2)
                {
                    // ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                    m_strStatus.Remove(0, m_strStatus.Length);
                    m_strStatus.Append("1800");
                }
                else if (MailShuRu)
                {
                    if (m_strAcount.ToString().IndexOf("163.com") < 0 && m_strAcount.ToString().IndexOf("126.com") < 0 && m_strAcount.ToString().IndexOf("yeah.net") < 0)
                    {
                        // ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                        GetAnswerResult(strOrdNo);
                        m_strStatus.Append("3700"); //ReturnTheResult("2000");
                    }
                }
            }
            if (ErrorNum > 1)
            {
                //ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                GetAnswerResult(strOrdNo);
                m_strStatus.Append("2000"); //ReturnTheResult("2000");
            }
            if (AccoutError)
            {
                // ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                GetAnswerResult(strOrdNo);
                m_strStatus.Append("2000"); //ReturnTheResult("2000");
            }
            if (ErrorAnswer > 10)
            {
                //ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                m_strStatus.Append("3300");
            }
            ReturnTheResult(m_strStatus.ToString());
            string sFile = "";
            try
            {
                sFile = webBrowser1.Document.Body.InnerText.ToString();
            }
            catch
            { }

            if (sFile.IndexOf("帐号安全提醒") > 0)
            {
                if (sFile.IndexOf("不区分大小写") > 0)
                {
                    if (!SafeKey)
                    {
                        //FileRW.WriteToFile(sFile);
                        SafeKey = true;
                    }
                }
            }
            if (sFile.IndexOf("自动登录验证") > 0)
            {
                if (sFile.IndexOf("不区分大小写") > 0)
                {
                    if (!SafeKey)
                    {
                        //FileRW.WriteToFile(sFile);
                        SafeKey = true;
                    }
                }
            }
            if (sFile.IndexOf("该网站遇到了问题") > 0 || huadong)
            {
                erroryicang++;
                if (erroryicang > 2)
                {
                    m_strStatus.Append("3120");
                }
                webBrowser1.Navigate(url.ToString());
            }
            if (huadong)
            {
                webBrowser1.Navigate(url.ToString());
                huadong = false;
            }
            if (IfHuaKuai)
            {
                webBrowser1.Navigate(url.ToString());
                IfHuaKuai = false;
            }
            if (ReEnter != NowEnter)
            {
                NowEnter = ReEnter;
                webBrowser1.Navigate(url.ToString());
            }
            if (sFile.IndexOf("已取消到该网页的导航") > 0)
            {
                erroryicang++;
                if (erroryicang > 2)
                {
                    m_strStatus.Append("3120");
                }
                webBrowser1.Navigate(url.ToString());
            }
            if (sFile.IndexOf("错误") > 0)
            {
                if (ErrorAnswer != EnterAnswer)
                {
                    ErrorAnswer = EnterAnswer;
                    //ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                }
            }
            if (AccoutError || sFile.IndexOf("您的密码不正确") > 0 || sFile.IndexOf("帐号或密码错误") > 0 || sFile.IndexOf("请输入正确的用户名") > 0 || sFile.IndexOf("密码不正确") > 0 || sFile.IndexOf("用户名不存在") > 0)
            //if (sFile.IndexOf("请输入正确的") > 0)
            {
                EnterAnswer = 0;
                if (ErrorNum != EnterNum)
                {
                    ErrorNum = EnterNum;
                    GetAnswerResult(strOrdNo);
                    // ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                }
            }
            if (sFile.IndexOf("收件箱") > 0)
            {
                if (!DengLu)
                    DengLu = true;
                JiHuo = true;
                WangYiMailKey = false;
                if (!DianKi)
                    DianJi = true;
                if (DianJing)
                {
                    webBrowser1.Navigate(url7);
                    DianJing = false;
                }
            }
            if (sFile.IndexOf("订单管理") > 0 || sFile.IndexOf("我的订单") > 0)
            {
                if (!DengLu)
                    DengLu = true;
                //JiHuo = true;
                //WangYiMailKey = false;
                if (!DianKiu)
                    DianJiu = true;
                if (DianJingu)
                {
                    webBrowser1.Navigate(url7);
                    DianJingu = false;
                }
            }
            if (sFile.IndexOf("注册网易免费") > 0)
            {
                // ScreenCapture.PrtWindowHWND(m_hWnd, strPicPath, ++picNo);
                WangYiMailKey = true;

            }
            if (sFile.IndexOf("存在安全风险") > 0)
            {
                webBrowser1.Navigate(url9);
            }
            if (sFile.IndexOf("开启二次验证") > 0)
            {
                Dianjik = true;
                if (Dianjit)
                {
                    webBrowser1.Navigate(url7);
                }
            }
            if (sFile.IndexOf("安全退出") > 0 && !StopCheck && !StopCheck2)
            {
                ErrorAnswer = 0;
                EnterAnswer = 0;
                IfSafekey = true;
                Game.tskill("iexplore");
                EnterKey = true;
                GetAnswerResult(strOrdNo);
                timer2.Stop();
                timer3.Stop();
                timer1.Start();
            }

        }
        private void button2_Click(object sender, EventArgs e)
        {
        }
        public static string ReturnResulting(Point st)
        {
            if (File.Exists(@"D:\\1.jpg"))
                File.Delete(@"D:\\1.jpg");
            Thread.Sleep(1000);
            GamePic.RECT rect;
            rect.left = st.X + 220;
            rect.top = st.Y - 120;
            rect.right = rect.left + 145;
            rect.bottom = rect.top + 72;
            ScreenCapture.PrtWindowRET(m_hWnd, "D:\\1.jpg", rect);
            string imgBytesIn = SaveImage("D:\\1.jpg");
            if (imgBytesIn != "")
            {
                imgBytesIn = Rs.receiveScriptQuestion(strOrdNo, "1004", 1, "", imgBytesIn, 90);

                if (imgBytesIn == "0")
                    Thread.Sleep(5000);
                string strResult = "";
                for (int i = 0; i < 100; i++)
                {
                    strResult = GetWebClient(strOrdNo);
                    int ipos = strResult.IndexOf("\r\n");
                    if (ipos > 0)
                    {
                        strResult = strResult.Substring(0, ipos);
                        if (strResult == "-1")
                        {
                            break;
                        }
                        else
                            if (strResult == "-2")
                            {
                                break;
                            }
                            else
                                if (strResult == "null")
                                {
                                    Thread.Sleep(1000);
                                }
                                else
                                    break;
                    }
                    else
                    {
                        Thread.Sleep(1000);
                    }
                }
                if (strResult == "" || strResult == "*" || strResult == "-1" || strResult == "-2" || strResult == "null")
                {
                }
                else
                {
                    return strResult;
                }
            }
            return "";
        }
        public static string ReturnResulted(Point st)
        {
            if (File.Exists(@"D:\\1.jpg"))
                File.Delete(@"D:\\1.jpg");
            Thread.Sleep(1000);
            GamePic.RECT rect;
            rect.left = 430;
            rect.top = 230;
            rect.right = rect.left + 158;
            rect.bottom = rect.top + 56;
            //rect.left = st.X-52;
            //rect.top = st.Y-120;
            //rect.right = rect.left + 190;
            //rect.bottom = rect.top + 70;
            ScreenCapture.PrtWindowRET(m_hWnd, "D:\\1.jpg", rect);
            string imgBytesIn = SaveImage("D:\\1.jpg");
            if (imgBytesIn != "")
            {
                imgBytesIn = Rs.receiveScriptQuestion(strOrdNo, "1004", 1, "", imgBytesIn, 90);

                if (imgBytesIn == "0")
                    Thread.Sleep(5000);
                string strResult = "";
                for (int i = 0; i < 100; i++)
                {
                    strResult = GetWebClient(strOrdNo);
                    int ipos = strResult.IndexOf("\r\n");
                    if (ipos > 0)
                    {
                        strResult = strResult.Substring(0, ipos);
                        if (strResult == "-1")
                        {
                            break;
                        }
                        else
                            if (strResult == "-2")
                            {
                                break;
                            }
                            else
                                if (strResult == "null")
                                {
                                    Thread.Sleep(1000);
                                }
                                else
                                    break;
                    }
                    else
                    {
                        Thread.Sleep(1000);
                    }
                }
                if (strResult == "" || strResult == "*" || strResult == "-1" || strResult == "-2" || strResult == "null")
                {
                }
                else
                {
                    return strResult;
                }
            }
            return "";
        }
        //加载webBeowser里的图片
        private void LoadWBPic(string Action)
        {
            try
            {
                switch (Action)
                {
                    case "login":
                        //IHTMLDocument3 baiduDoc = CorssDomainHelper.GetDocumentFromWindow(frames[0].DomWindow as IHTMLWindow2);
                        //mshtml.IHTMLElementCollection hc = baiduDoc.getElementsByTagName("input");
                        HtmlWindowCollection frames = webBrowser1.Document.Window.Frames;
                        if (frames.Count <= 0)
                        {
                            //iFramee还没加载好
                            FileRW.WriteToFile("自动审核失败，错误001");
                        }
                        IHTMLDocument2 baiduDoc = (IHTMLDocument2)CorssDomainHelper.GetDocumentFromWindow(frames[1].DomWindow as IHTMLWindow2);
                        //Console.WriteLine("页面源码:" + baiduDoc.body.innerHTML);
                        //Console.WriteLine("标题:" + baiduDoc.title);
                        ////mshtml.IHTMLElementCollection hc = baiduDoc.all.tags("INPUT");
                        // mshtml.IHTMLElementCollection hc = baiduDoc.all;

                        //Console.WriteLine("获取Tag总数" + hc.length);
                        //Console.WriteLine("标题:" + baiduDoc.title);
                        //Console.WriteLine("全文:" + baiduDoc.body.innerHTML);

                        string BodyHtml = baiduDoc.body.innerHTML;
                        string strPic = ImageTool.CheckPic(BodyHtml);
                        pictureBox2.Load(strPic);
                        pictureBox_Bak.Load(strPic);
                        break;
                    case "safeQuestion":
                        foreach (HtmlElement f in webBrowser1.Document.Images)
                        {
                            //Console.WriteLine(f.GetAttribute("src"));
                            if (f.GetAttribute("src").Contains("://nos.netease.com/nisp-captcha/"))//2张拼图的前一张
                            {
                                string strPicSafeQ = f.GetAttribute("src");
                                //Console.WriteLine(strPicSafeQ);
                                int i = strPicSafeQ.IndexOf("?NOSAccessKeyId");
                                strPicSafeQ = strPicSafeQ.Substring(0, i);
                                pictureBox2.Load(strPicSafeQ);
                                pictureBox_Bak.Load(strPicSafeQ);
                                break;
                            }
                        }
                        break;
                    default:

                        break;
                }
            }
            catch (Exception ex)
            {
                FileRW.WriteToFile(ex.ToString());
            }
        }
        //搜索图库
        private void SearchPicBase()
        {
            try
            {
                //遍历文件夹
                string strDir = AppDomain.CurrentDomain.BaseDirectory + "\\ImgBase\\";
                DirectoryInfo TheFolder = new DirectoryInfo(strDir);
                //遍历文件
                foreach (FileInfo NextFile in TheFolder.GetFiles())
                {
                    if (ImageTool.EnumPic(strDir + NextFile.Name, pictureBox1, pictureBox2))
                    {
                        ImageTool.chooseCorlr(NextFile.Name);
                        //bll.chooseCorlr(NextFile.Name);
                        break;
                    }
                    pictureBox1.Image = null;
                }
                //如果没有匹配的图片 那么下载网页的图片
                //if (Common.isDebug) { pictureBox1.Image = null; }
                if (pictureBox1.Image == null)
                {
                    ImageTool.BackPic(pictureBox2);
                }
            }
            catch (Exception ex)
            {
                FileRW.WriteToFile(ex.ToString());
            }
        }
        // 填充差异
        private void Match()
        {
            try
            {
                Bitmap bmp1 = new Bitmap(pictureBox1.Image.Width, pictureBox1.Image.Height, PixelFormat.Format24bppRgb);
                bmp1 = pictureBox1.Image as Bitmap;
                Bitmap bmp2 = new Bitmap(pictureBox1.Image.Width, pictureBox1.Image.Height, PixelFormat.Format24bppRgb);
                bmp2 = pictureBox2.Image.Clone() as Bitmap;


                //用于去除错误噪点的变量
                Bitmap bmpBak = pictureBox2.Image.Clone() as Bitmap;
                pictureBox_Bak.Image = (Image)bmpBak;
                int BmpSumX = 0;//保存所有噪点的X总值
                int BmpSumY = 0;//保存所有噪点的Y总值
                int NoisyPointCount = 0;    //噪点总数

                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //和图库的图对比像素
                for (int i = 0; i < bmp1.Width; i++)
                {
                    for (int j = 0; j < bmp1.Height; j++)
                    {
                        Color CurrentCol1 = bmp1.GetPixel(i, j);
                        Color CurrentCol2 = bmp2.GetPixel(i, j);


                        if (!MatchColor(CurrentCol1, CurrentCol2, 10))
                        {
                            NoisyPointCount++;
                            //设置像素点颜色  
                            bmp2.SetPixel(i, j, cDiffArea);

                            BmpSumX += i;
                            BmpSumY += j;
                            //记录差异区域    还是重新扫描吧
                            //setPoint(i, j);
                        }
                    }
                }
                //Console.WriteLine("共有" + k + "个像素点有差异");
                //pictureBox2.Image = Image.FromHbitmap(bmp2.GetHbitmap());
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////


                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //去掉孤立的点(左右分别10像素 + 上下分别10像素 共计40像素范围比较. 返回颜色相同的噪点数目)
                for (int i = 0; i < bmp1.Width; i++)
                {
                    for (int j = 0; j < bmp1.Height; j++)
                    {
                        Color CurrentPixel = bmp2.GetPixel(i, j);
                        if (cDiffArea != CurrentPixel) continue;

                        int MatchP =calcPixel(bmp2, i, j);
                        if (MatchP < 10)//相同颜色的噪点小于10 ,那么认为是孤立的点
                        {
                            Color CurrentCol2 = bmpBak.GetPixel(i, j);
                            bmp2.SetPixel(i, j, CurrentCol2);
                            NoisyPointCount--;
                            BmpSumX -= i;
                            BmpSumY -= j;
                        }
                    }
                }
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////


                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //对错误噪点去除的处理 20170117   //缺失部分范围大小60*100
                int BmpAreaX = 60; int BmpAreaY = 100;
                int BmpAvgX = BmpSumX / NoisyPointCount;
                int BmpAvgY = BmpSumY / NoisyPointCount;

                ////对图2画线 //调试的时候看方便 实际用请注释掉
                //for (int i = 0; i < bmp2.Height; i++)
                //{
                //    bmp2.SetPixel(BmpAvgX, i, Common.cGolden);
                //}
                //for (int i = 0; i < bmp2.Width; i++)
                //{
                //    bmp2.SetPixel(i, BmpAvgY, Common.cGolden);
                //}


                //算法:对所有噪点坐标进行绝对平均值计算.把bmp2对比bmpBak对比像素,凡是超出验证码范围的噪点进行还原像素值
                for (int i = 0; i < bmp1.Width; i++)
                {
                    for (int j = 0; j < bmp1.Height; j++)
                    {
                        Color CurrentCol1 = bmp2.GetPixel(i, j);
                        Color CurrentCol2 = bmpBak.GetPixel(i, j);

                        if (CurrentCol1 != CurrentCol2)
                        {
                            //如果超出缺失部分范围的噪点 恢复原来的颜色
                            if (Math.Abs(i - BmpAvgX) > BmpAreaX)
                            {
                                //设置像素点颜色  
                                bmp2.SetPixel(i, j, CurrentCol2);
                            }

                            if (Math.Abs(j - BmpAvgY) > BmpAreaY)
                            {
                                //设置像素点颜色  
                                bmp2.SetPixel(i, j, CurrentCol2);
                            }
                        }
                    }
                }
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




                pictureBox2.Image = Image.FromHbitmap(bmp2.GetHbitmap());
            }
            catch (Exception ex)
            {
                //备份图片
               BackPic(pictureBox1, pictureBox2);
            }
        }
        public Boolean MatchColor(Color c1, Color c2, int OfferSet)
        {
            Boolean isMatch = true;
            if (OfferSet < 0) OfferSet = 30;
            try
            {
                //像素点颜色的 Alpha 值
                byte alpha1 = c1.A;
                //颜色的 RED 分量值
                byte red1 = c1.R;
                //颜色的 GREEN 分量值
                byte green1 = c1.G;
                //颜色的 BLUE 分量值
                byte blue1 = c1.B;

                //像素点颜色的 Alpha 值
                byte alpha2 = c2.A;
                //颜色的 RED 分量值
                byte red2 = c2.R;
                //颜色的 GREEN 分量值
                byte green2 = c2.G;
                //颜色的 BLUE 分量值
                byte blue2 = c2.B;

                if (Math.Abs(alpha1 - alpha2) > OfferSet || Math.Abs(red1 - red2) > OfferSet || Math.Abs(green1 - green2) > OfferSet || Math.Abs(blue1 - blue2) > OfferSet)
                {
                    isMatch = false;
                }
                return isMatch;

            }
            catch (Exception ex)
            {
                throw ex;
            }
        }
        public int calcPixel(Bitmap bmp, int x, int j)
        {
            int MatchP = 0;
            int offset = 10;
            try
            {
                if (x <= offset || x > bmp.Width - offset)
                {
                    return 0;
                }

                if (j <= offset || j > bmp.Height - offset)
                {
                    return 0;
                }

                //遍历差异区计数
                //横向
                for (int k = x - offset; k < x + offset; k++)
                {
                    if (bmp.GetPixel(k, j) ==cDiffArea)
                    {
                        MatchP++;
                    }
                }
                if (MatchP < 10) return 0;

                ////遍历差异区计数
                ////横向
                //for (int k = x - offset; k < x + offset; k++)
                //{
                //    if (bmp.GetPixel(k, j) == Common.cDiffArea)
                //    {
                //        MatchP++;
                //    }
                //}
                //if (MatchP < 10) return 0;

                //纵向
                for (int k = j - offset; k < j + offset; k++)
                {
                    if (bmp.GetPixel(x, k) == cDiffArea)
                    {
                        MatchP++;
                    }
                }

                return MatchP;
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }
        public void BackPic(PictureBox pic1, PictureBox pic2)
        {
            try
            {
                Image img = null;
                //备份图片
                if (pic1.Image != null)
                {
                    img = pic1.Image;
                    img.Save(PicPath + "Sample_Tmp1" + ".jpg", System.Drawing.Imaging.ImageFormat.Jpeg);
                }
                if (pic2.Image != null)
                {
                    img = pic2.Image;
                    img.Save(PicPath + "Sample_Tmp2" + ".jpg", System.Drawing.Imaging.ImageFormat.Jpeg);
                }
                img.Dispose();
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }
        //计算差异
        private void Clac()
        {
            try
            {
                Bitmap bmp1 = pictureBox1.Image as Bitmap;
                Bitmap bmp2 = pictureBox2.Image as Bitmap;

                //初始化
                UnMatchLeft = bmp2.Width;
                UnMatchRigth = 0;
                int pCenter;

                bmp2 = ImageTool.calcPoint(bmp1, bmp2, out pCenter, out UnMatchLeft, out UnMatchRigth);
                ////输出结果
                //pCenter = (UnMatchLeft + UnMatchRigth) / 2;
                //Console.WriteLine(pCenter);
                m_Point.X = int.Parse(pCenter.ToString()) * 350 / 320;
                ////对图2画线
                //for (int i = 0; i < bmp2.Height; i++)
                //{
                //    bmp2.SetPixel(pCenter, i, cCenterLine);
                //}
                pictureBox2.Image = Image.FromHbitmap(bmp2.GetHbitmap());
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }
        //滑动鼠标验证
        private void SetVeryCodeLogin()
        {
            try
            {
                Point sPoint = new Point(1182, 363);
                Point ePoint = new Point(sPoint.X - 39 + m_Point.X, sPoint.Y);
                int offset = 2300 + 1000;

                System.Threading.Timer threadTimer1 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 1, "" }, 1000 + offset, 0);
                System.Threading.Timer threadTimer2 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 2, "" }, 3000 + offset, 0);
                for (int i = 3100; i < 3800; i = i + 50)
                {
                    //if (sPoint.X < ePoint.X + 39)
                    //{
                    sPoint.X = sPoint.X + 10;
                    //}
                    //else
                    //{
                    //    break;
                    //}
                    System.Threading.Timer threadTimer21 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 1, "" }, i + offset, 0);

                }
                System.Threading.Timer threadTimer3 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { ePoint, 1, "" }, 4000 + offset, 0);
                System.Threading.Timer threadTimer4 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { ePoint, 3, "" }, 6000 + offset, 0);

                //调试
                //txtPosition.Text = "320";
                //SetCursorPos(1182 + int.Parse(txtPosition.Text), 400);
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }
        //Step6 滑动鼠标验证
        private void SetVeryCodeSafeQuestion()
        {
            //图片宽226 总拉伸长度176   起始位置226-176  /2 = 25
            try
            {
                Point sPoint = new Point(668, 363);
                Point ePoint = new Point(sPoint.X - 25 + m_Point.X, sPoint.Y);
                int offset = 2300 + 1000;

                System.Threading.Timer threadTimer1 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 1, "" }, 1000 + offset, 0);
                System.Threading.Timer threadTimer2 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 2, "" }, 3000 + offset, 0);
                for (int i = 3100; i < 3800; i = i + 50)
                {
                    sPoint.X = sPoint.X + 10;
                    System.Threading.Timer threadTimer21 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 1, "" }, i + offset, 0);

                }
                System.Threading.Timer threadTimer3 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { ePoint, 1, "" }, 4000 + offset, 0);
                System.Threading.Timer threadTimer4 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { ePoint, 3, "" }, 6000 + offset, 0);
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }
        //鼠标操作函数
        public static void setMouse(object obj)
        {
            int MOUSEEVENTF_LEFTDOWN = 0x2;
            int MOUSEEVENTF_LEFTUP = 0x4;

            //int KEYEVENTF_KEYUP =  Convert.ToInt32( "0xH2", 16);
            //int KEYEVENTF_EXTENDEDKEY = Convert.ToInt32("0xH1", 16);

            Point p = (Point)((object[])(obj))[0];
            int actionType = int.Parse(((object[])(obj))[1].ToString());
            string arg = ((object[])(obj))[2].ToString();

            //Console.WriteLine("鼠标操作:" + actionType);
            //SetLog("鼠标操作:" + actionType + "", 0);
            try
            {
                switch (actionType)
                {
                    //移动鼠标
                    case 1:
                        SetCursorPos(p.X, p.Y);
                        break;
                    //LEFTDOWN
                    case 2:
                        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                        break;
                    //LEFTUP
                    case 3:
                        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                        break;
                    //Ctrl+A
                    case 4:
                        keybd_event(Keys.ControlKey, 0, 0, 0);
                        keybd_event(Keys.A, 0x1e, 0, 0);
                        keybd_event(Keys.A, 0x1e, 2, 0);
                        keybd_event(Keys.ControlKey, 0, 2, 0);
                        break;
                    //Ctrl+V
                    case 5:
                        keybd_event(Keys.ControlKey, 0, 0, 0);
                        keybd_event(Keys.V, 0x1e, 0, 0);
                        keybd_event(Keys.V, 0x1e, 2, 0);
                        keybd_event(Keys.ControlKey, 0, 2, 0);
                        break;
                        break;
                    default:
                        break;

                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }
        public static string ReturnResult(Point st)
        {
            if (File.Exists(@"D:\\1.jpg"))
                File.Delete(@"D:\\1.jpg");
            Thread.Sleep(1000);
            GamePic.RECT rect;
            rect.left = st.X;
            rect.top = st.Y;
            rect.right = rect.left + 125;
            rect.bottom = rect.top + 50;
            ScreenCapture.PrtWindowRET(m_hWnd, "D:\\1.jpg", rect);
            string imgBytesIn = SaveImage("D:\\1.jpg");
            if (imgBytesIn != "")
            {
                imgBytesIn = Rs.receiveScriptQuestion(strOrdNo, "1004", 1, "", imgBytesIn, 90);

                if (imgBytesIn == "0")
                    Thread.Sleep(5000);
                string strResult = "";
                for (int i = 0; i < 90; i++)
                {
                    strResult = GetWebClient(strOrdNo);
                    int ipos = strResult.IndexOf("\r\n");
                    if (ipos > 0)
                    {
                        strResult = strResult.Substring(0, ipos);
                        if (strResult == "-1")
                        {
                            break;
                        }
                        else
                            if (strResult == "-2")
                            {
                                break;
                            }
                            else
                                if (strResult == "null")
                                {
                                    Thread.Sleep(1000);
                                }
                                else
                                    break;
                    }
                    else
                    {
                        Thread.Sleep(1000);
                    }
                }
                if (strResult == "" || strResult == "*" || strResult == "-1" || strResult == "-2" || strResult == "null")
                {
                }
                else
                {
                    return strResult;
                }
            }
            return "";
        }
        public static string ReturnResult1(Point st)
        {
            if (File.Exists(@"D:\\1.jpg"))
                File.Delete(@"D:\\1.jpg");
            Thread.Sleep(1000);
            GamePic.RECT rect;
            rect.left = st.X;
            rect.top = st.Y;
            rect.right = rect.left + 352;
            rect.bottom = rect.top + 150;
            ScreenCapture.PrtWindowRET(m_hWnd, "D:\\1.jpg", rect);
            string imgBytesIn = SaveImage("D:\\1.jpg");
            if (imgBytesIn != "")
            {
                imgBytesIn = Rs.receiveScriptQuestion(strOrdNo, "1004", 3, "", imgBytesIn, 90);

                if (imgBytesIn == "0")
                    Thread.Sleep(5000);
                string strResult = "";
                for (int i = 0; i < 90; i++)
                {
                    strResult = GetWebClient(strOrdNo);
                    int ipos = strResult.IndexOf("\r\n");
                    if (ipos > 0)
                    {
                        strResult = strResult.Substring(0, ipos);
                        if (strResult == "-1")
                        {
                            break;
                        }
                        else
                            if (strResult == "-2")
                            {
                                break;
                            }
                            else
                                if (strResult == "null")
                                {
                                    Thread.Sleep(1000);
                                }
                                else
                                    break;
                    }
                    else
                    {
                        Thread.Sleep(1000);
                    }
                }
                if (strResult == "" || strResult == "*" || strResult == "-1" || strResult == "-2" || strResult == "null")
                {
                }
                else
                {
                    return strResult;
                }
            }
            return "";
        }
        public static string ReturnResult2(Point st)
        {
            if (File.Exists(@"D:\\1.jpg"))
                File.Delete(@"D:\\1.jpg");
            Thread.Sleep(1000);
            GamePic.RECT rect;
            rect.left = st.X;
            rect.top = st.Y;
            rect.right = rect.left + 351;
            rect.bottom = rect.top + 116;
            ScreenCapture.PrtWindowRET(m_hWnd, "D:\\1.jpg", rect);
            string imgBytesIn = SaveImage("D:\\1.jpg");
            if (imgBytesIn != "")
            {
                imgBytesIn = Rs.receiveScriptQuestion(strOrdNo, "1004", 3, "", imgBytesIn, 90);

                if (imgBytesIn == "0")
                    Thread.Sleep(5000);
                string strResult = "";
                for (int i = 0; i < 90; i++)
                {
                    strResult = GetWebClient(strOrdNo);
                    int ipos = strResult.IndexOf("\r\n");
                    if (ipos > 0)
                    {
                        strResult = strResult.Substring(0, ipos);
                        if (strResult == "-1")
                        {
                            break;
                        }
                        else
                            if (strResult == "-2")
                            {
                                break;
                            }
                            else
                                if (strResult == "null")
                                {
                                    Thread.Sleep(1000);
                                }
                                else
                                    break;
                    }
                    else
                    {
                        Thread.Sleep(1000);
                    }
                }
                if (strResult == "" || strResult == "*" || strResult == "-1" || strResult == "-2" || strResult == "null")
                {
                }
                else
                {
                    return strResult;
                }
            }
            return "";
        }
        public static string MousePointDeal(string sm)
        {
            int len = sm.Length;
            char[] strTemp = sm.ToCharArray();
            while ((strTemp[0] < 47 || strTemp[0] > 58) && (len > 0))
            {
                sm.Remove(0, 1);
                strTemp = sm.ToCharArray();
                len = sm.Length;
            }
            return sm;
        }
        #region
        public Point GetOffset(HtmlElement el)
        {
            //get element pos
            Point pos = new Point(el.OffsetRectangle.Left, el.OffsetRectangle.Top);

            //get the parents pos
            HtmlElement tempEl = el.OffsetParent;
            while (tempEl != null)
            {
                pos.X += tempEl.OffsetRectangle.Left;
                pos.Y += tempEl.OffsetRectangle.Top;
                tempEl = tempEl.OffsetParent;
            }

            return pos;
        }
        public static Point ScreenCursorPosition //获取光标相对于显示器的位置 
        {
            get
            {
                Point showPoint = new Point();
                User32API.GetCursorPos(out showPoint);
                return showPoint;
            }
        }
        //将图片以二进制流
        public static string SaveImage(String path)
        {
            if (File.Exists(@path))
            {
                FileStream fs = new FileStream(path, FileMode.Open, FileAccess.Read); //将图片以文件流的形式进行保存
                BinaryReader br = new BinaryReader(fs);
                byte[] imgBytesIn = br.ReadBytes((int)fs.Length);  //将流读入到字节数组中
                fs.Close();
                string str;
                string strRet = "";
                foreach (byte b in imgBytesIn)
                {
                    str = b.ToString("x");
                    if (str.Length != 2)
                    {
                        str = "0" + str;
                    }
                    strRet += str;
                }
                return strRet;
            }
            else
                return "";
        }

        public static string GetWebClient(string url)
        {
            string strUrl = "";
            strUrl = string.Format("{0}{1}", "http://gtr.5173.com/GTR_RC_ORDERINFO/Answer.aspx?OrderNO=", url);
            string strHTML = "";
            WebClient myWebClient = new WebClient();
            Stream myStream = myWebClient.OpenRead(strUrl);
            StreamReader sr = new StreamReader(myStream, System.Text.Encoding.GetEncoding("utf-8"));
            strHTML = sr.ReadToEnd();
            myStream.Close();
            return strHTML;
        }
        public static void GetAnswerResult(string ordNo)
        {
            //发送答题结果
            if (AnswerKey)
            {
                Rs.correctAnswer(ordNo, "1004", 1);
            }
            if (AnswerKey1)
            {
                Rs.correctAnswer(ordNo, "1004", 3);
            }
            AnswerKey = false;
        }
        private void Form1_Load_1(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Fun1Proc.Abort();
            timer1.Stop();
            timer2.Stop();
            timer3.Stop();
            System.Environment.Exit(0);
        }
        private void ReturnTheResult(string sResult)
        {
            if (sResult != "")
            {
                if (sResult == "1000")
                {
                    GamePic.RECT rect;
                    ScreenCapture.GetWindowRect(m_hWnd, out rect);
                    rect.left = 35;
                    rect.right = 35 + 875;
                    rect.top = 95;
                    rect.bottom = 513;

                    ScreenCapture.PrtWindowBMP(m_hWnd, strPicPath + "审核信息.bmp", rect);
                }
                Fun1Proc.Abort();
                timer1.Stop();

                if ((sResult == "1000") || (sResult == "4010"))
                {
                    if (!ReadResult())
                        sResult = "4010";
                    Kernel32API.WritePrivateProfileString("账号信息", "执行结果", m_strResult.ToString(), strExePath);
                    Kernel32API.WritePrivateProfileString("账号信息", "执行结果", m_strResult.ToString(), strQQnumberIni);
                }
                Kernel32API.WritePrivateProfileString("账号信息", "执行状态", sResult, strExePath);
                System.Environment.Exit(0);
            }
        }
        #endregion

        private void timer3_Tick(object sender, EventArgs e)
        {
            if (!DocumentStatus)
            {
                return;
            }
            HtmlWindowCollection frames = webBrowser1.Document.Window.Frames;
            if (frames.Count <= 0)
            {
                //iFramee还没加载好
               // FileRW.WriteToFile("自动审核失败，错误001");
                return;
            }
            IHTMLDocument2 baiduDoc = (IHTMLDocument2)CorssDomainHelper.GetDocumentFromWindow(frames[1].DomWindow as IHTMLWindow2);
            string BodyHtml = baiduDoc.body.innerHTML;
            if (BodyHtml == null)
                return;
            if (BodyHtml.IndexOf("nisp-captcha.nosdn.127.net") > 0 || BodyHtml.IndexOf("nos.netease.com/nisp-captcha") > 0)
            {
                if (BodyHtml.IndexOf("请依次") > 0)
                {
                    WenZi = true;
                }
                else
                {
                    LoadWBPic("login");
                    //Step2 搜索图库
                    SearchPicBase();
                    //Step3 填充差异
                    Match();
                    //Step4 计算差异
                    Clac();
                }
            }
            SetUserNamePwd();
            DocumentStatus = false;
        }
        private void SetUserNamePwd()
        {
            try
            {
                //js实现填写账号密码(跨域Iframe)
                HtmlWindowCollection frames = webBrowser1.Document.Window.Frames;
                IHTMLDocument3 baiduDoc3 = CorssDomainHelper.GetDocumentFromWindow(frames[0].DomWindow as IHTMLWindow2);
                IHTMLDocument2 baiduDoc2 = (IHTMLDocument2)CorssDomainHelper.GetDocumentFromWindow(frames[1].DomWindow as IHTMLWindow2);
                IHTMLDocument baiduDoc = (IHTMLDocument)CorssDomainHelper.GetDocumentFromWindow(frames[1].DomWindow as IHTMLWindow2);
                //mshtml.IHTMLElementCollection hc = baiduDoc.getElementsByTagName("input");
                //Console.WriteLine("页面源码:" + baiduDoc2.body.innerHTML);//检测账号密码错误的提示
                //var Btn_Submit = baiduDoc3.getElementById("dologin");
                IHTMLWindow2 vWindow = (IHTMLWindow2)baiduDoc2.parentWindow;
                try
                {
                    string strJscript = string.Format("var eByName = document.getElementsByName('email');eByName[0].value = '{0}';", m_strAcount.ToString());
                    strJscript += string.Format("var eByName = document.getElementsByName('password');eByName[0].value = '{0}';", m_strPaswod.ToString());
                    vWindow.execScript(strJscript, "javascript");
                }
                catch (Exception e)
                {
                    throw e;
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }

        }

        //对IHTMLDocument2对象写入JS脚本
        public string GetScriptValue(WebBrowser browser, string ValueName)
        {
            IHTMLDocument2 vDocument = browser.Document.DomDocument as IHTMLDocument2;
            IHTMLWindow2 vWindow = (IHTMLWindow2)vDocument.parentWindow;
            Type vWindowType = vWindow.GetType();
            object xpt = vWindowType.InvokeMember(ValueName, System.Reflection.BindingFlags.GetProperty, null, vWindow, new object[] { });
            return xpt as string;
        }


    }
}