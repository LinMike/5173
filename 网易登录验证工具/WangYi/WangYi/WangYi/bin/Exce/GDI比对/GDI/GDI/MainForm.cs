using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;
using mshtml;
using System.Collections;
using System.Runtime.InteropServices;
using System.Threading;
using System.IO;

namespace GDI
{
    delegate void SetTextCallBack0();
    delegate void SetTextCallBack(string text);
    delegate void SetTextCallBack2(string text,int ActionType);

    public partial class MainForm : Form
    {
        //如果要暂停这个程序的队列  D:\自动审核\MQ5173.txt 的文档首行输入任意字符暂停。还原后继续


        #region 模拟鼠标移动  Win API
        [Flags]
        enum MouseEventFlag : uint
        {
            Move = 0x0001,
            LeftDown = 0x0002,
            LeftUp = 0x0004,
            RightDown = 0x0008,
            RightUp = 0x0010,
            MiddleDown = 0x0020,
            MiddleUp = 0x0040,
            XDown = 0x0080,
            XUp = 0x0100,
            Wheel = 0x0800,
            VirtualDesk = 0x4000,
            Absolute = 0x8000
        }
        /// <summary>
        /// 功能比较全面的鼠标功能API
        /// </summary>
        /// <param name="flags"></param>
        /// <param name="dx"></param>
        /// <param name="dy"></param>
        /// <param name="data"></param>
        /// <param name="extraInfo"></param>
        [DllImport("user32.dll")]
        static extern void mouse_event(MouseEventFlag flags, int dx, int dy, uint data, UIntPtr extraInfo);
 
        #endregion
 
        #region 配置
        static class MouseConfig
        {
            //鼠标指针每秒移动多少像素点
            public static int MovelForSecond = 100;
        }
        #endregion



        #region 移动到指定的屏幕坐标
        /// <summary>
        /// 移动到指定的屏幕坐标（带轨迹）
        /// </summary>
        /// <param name="targetX">X坐标</param>
        /// <param name="targetY">Y坐标</param>
        public new void Move(int targetX, int targetY)
        {
            Thread th = new Thread(() =>
            {
                int count = MouseConfig.MovelForSecond;
                while (count != 0)
                {
                    Thread.Sleep(10);
                    int stepx = (targetX - Cursor.Position.X) / count;
                    int stepy = (targetY - Cursor.Position.Y) / count;
                    count--;
                    if (count != 0)
                        mouse_event(MouseEventFlag.Move, stepx, stepy, 0, UIntPtr.Zero);
                }
 
            });
            th.IsBackground = true;
            th.Start();
        }
        #endregion

        #region 移动到指定的屏幕坐标(公开方法)
        /// <summary>
        /// 移动到指定的元素
        /// </summary>
        /// <param name="element">元素</param>
        //public static void Move(UIElement element)
        //{
        //    //获取该控件在屏幕上的坐标
        //    Point coordinate = element.PointToScreen(new Point());
        //    //将鼠标位置定到控件的中心位置
        //    coordinate = new Point(coordinate.X + element.RenderSize.Width / 2, coordinate.Y + element.RenderSize.Height / 2);
        //    Move((int)coordinate.X, (int)coordinate.Y);
        //}
        #endregion

        #region 获得鼠标当前的屏幕位置需要的代码
        [StructLayout(LayoutKind.Sequential)]
        public struct POINT
        {
            public int X;
            public int Y;

            public POINT(int x, int y)
            {
                this.X = x;
                this.Y = y;
            }

            public override string ToString()
            {
                return ("X:" + X + ", Y:" + Y);
            }
        }

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern bool GetCursorPos(out POINT pt);
        #endregion

        //方法2
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = false)]
        static extern IntPtr SendMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);
        [DllImport("user32.dll", SetLastError = true)]
        static extern IntPtr GetWindow(IntPtr hWnd, uint uCmd);
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static extern int GetClassName(IntPtr hWnd, StringBuilder lpClassName, int nMaxCount);
        //设置鼠标位置
        [DllImport("User32.Dll")]
        public static extern long SetCursorPos(int x, int y);
        
        //设置鼠标事件
        [DllImport("user32.dll")]
        private static extern int mouse_event(int dwFlags, int dx, int dy, int cButtons, int dwExtraInfo);
        //键盘事件
        [DllImport("user32.dll", EntryPoint = "keybd_event", SetLastError = true)]
        public static extern void keybd_event(Keys bVk, byte bScan, uint dwFlags, uint dwExtraInfo);


        IO io = new IO();
        PicGetSet pgs = new PicGetSet();
        Bll bll = new Bll();
        Base64 b64 = new Base64();

        public string strPic1 = string.Empty;
        public string strPic2 = string.Empty;

        int UnMatchLeft;
        int UnMatchRigth;
        string[] args = null;

        #region 窗体事件
        public MainForm(string[] args)
        {
            try
            {
                //args = new string[] { b64.EncodeBase64("jmsfeige1@163.com"), b64.EncodeBase64("qq8596321"), "", "", "" };
                //MessageBox.Show("原文 " + args[0] + "解码 " + b64.DecodeBase64(args[0]));
                //Clipboard.SetText(args[0]);
                //System.Environment.Exit(0);
                //args = new string[] { b64.EncodeBase64("jmsfeige1@163.com"), b64.EncodeBase64("qq8596321"), "", "", "" };
                InitializeComponent();
                //接收参数
                this.args = args;

                SetLog("args.Length:" + args.Length.ToString() + "读取完成", 0);
                if (this.args.Length != 0)
                {
                    Common.strUn = b64.DecodeBase64(args[0].ToString());
                    Common.strPwd = b64.DecodeBase64(args[1].ToString());
                    Common.strOrderID = b64.DecodeBase64(args[2].ToString());
                    if (args.Length == 9)
                    {
                        Common.strA1 = b64.DecodeBase64(args[3].ToString());
                        Common.strA2 = b64.DecodeBase64(args[4].ToString());
                        Common.strA3 = b64.DecodeBase64(args[5].ToString());

                        Common.strQ1 = b64.DecodeBase64(args[6].ToString());
                        Common.strQ2 = b64.DecodeBase64(args[7].ToString());
                        Common.strQ3 = b64.DecodeBase64(args[8].ToString());
                    }
                    else
                    {
                        Common.strA1 = "空";
                        Common.strA2 = "空";
                        Common.strA3 = "空";
                        Common.strQ1 = "空";
                        Common.strQ2 = "空";
                        Common.strQ3 = "空";
                    }

                    if (!Common.strUn.Contains("@"))
                    {
                        Common.strUn = Common.strUn + "@163.com";
                    }
                    if (!Common.strUn.Contains("."))
                    {
                        Common.strUn = Common.strUn + ".com";
                    }

                    //为避免需要验证密保问题  但是密保问题为空的情况下在页面卡死
                    if (string.IsNullOrEmpty(Common.strA1)) { Common.strA1 = "空"; }
                    if (string.IsNullOrEmpty(Common.strA2)) { Common.strA2 = "空"; }
                    if (string.IsNullOrEmpty(Common.strA3)) { Common.strA3 = "空"; }

                    if (string.IsNullOrEmpty(Common.strQ1)) { Common.strA1 = ""; }
                    if (string.IsNullOrEmpty(Common.strQ2)) { Common.strA2 = ""; }
                    if (string.IsNullOrEmpty(Common.strQ3)) { Common.strA3 = ""; }
                }
                else
                {
                    Common.isDebug = true;
                    lbl_isDebug.Text = "isDebug:是";
                }
                SetLog("Common.isDebug状态:" + Common.isDebug.ToString(), 0);
                SetLog("Common.strUn:" + Common.strUn, 0);
                SetLog("Common.strPwd:" + Common.strPwd, 0);
                SetLog("Common.strOrderID:" + Common.strOrderID, 0);
                SetLog("Common.strQ1:" + Common.strQ1, 0);
                SetLog("Common.strA1:" + Common.strA1, 0);
                SetLog("Common.strQ2:" + Common.strQ2, 0);
                SetLog("Common.strA2:" + Common.strA2, 0);
                SetLog("Common.strQ3:" + Common.strQ3, 0);
                SetLog("Common.strA3:" + Common.strA3, 0);
                //窗体总在最前面
                if (!Common.isDebug)
                {
                    this.TopMost = false;
                    this.BringToFront();
                    this.TopMost = true;
                }
                SetLog(!Common.isDebug ? "设置窗体最前" : "不设置窗体最前", 0);
                lbl_Msg.Text = Common.strUn + " " + Common.strPwd;
                lbl_AppMsg.Text = "无信息";


                //启动计时器 超时返回错误自动关闭
                Common.dtStart = DateTime.Now;
                timerAutoClose.Enabled = true;
                //添加操作队列
                io.Append(System.Diagnostics.Process.GetCurrentProcess().Id.ToString());
                //初始化参数
                wbMain.ScriptErrorsSuppressed = true;
            }
            catch (Exception ex)
            { 
                SetLog(ex.Message,0);
                EndApp("程序错误,退出"); 
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            wbMain.Url = new Uri(Common.Page1);
            this.Location = new Point(0, 0);//定位
            lbl_Clipboard.TextChanged += new System.EventHandler(this.lbl_Clipboard_TextChanged);//验证码文本框改变的事件
        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            EndApp("手动关闭");
        }
        #endregion

        #region 按钮事件
        private void btn_LoadPic_Click(object sender, EventArgs e)
        {
            string strDir = AppDomain.CurrentDomain.BaseDirectory;
            strPic1 = strDir + "\\ImgBase\\Sample_Tmp1.jpg";
            strPic2 = strDir + "\\ImgBase\\Sample_Tmp2.jpg";

            pictureBox1.Load(strPic1);
            pictureBox2.Load(strPic2);

            //设置默认色
        }

        //填充区域
        private void Btn_Match_Click(object sender, EventArgs e)
        {
            Match();
        }

        /// <summary>
        /// 扣出来网页的图片
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_LoadPicfromWB_Click(object sender, EventArgs e)
        {
            LoadWBPic("login");
        }

        private void btn_setMouse_Click(object sender, EventArgs e)
        {
            SetVeryCodeLogin();
            //LoginAgain(); 部分账号要再次登录的
        }

        //登陆状态确认
        private void btn_Start_Click(object sender, EventArgs e)
        {
            Console.WriteLine(ConfirmLogin());
        }

        //鼠标结束位置
        private void button1_Click(object sender, EventArgs e)
        {
            SetUserNamePwd();
        }

        //显示2号窗口
        private void button2_Click(object sender, EventArgs e)
        {
            //刷新网页
            wbMain.Url = new Uri(Common.Page1);
            Common.PubMaxSeconds = 80000;
            //打开窗体
            CollectPic cp = new CollectPic();
            cp.Show();
        }

        //登陆
        private void Btn_Login_Click(object sender, EventArgs e)
        {
            Login();
        }

        private void Btn_TestRange_Click(object sender, EventArgs e)
        {
            Bitmap bmp2 = pictureBox2.Image as Bitmap;

            int k = int.Parse(txt_Range.Text);

            for (int j = 0; j < bmp2.Height; j++)
            {
                bmp2.SetPixel(k, j, Common.cDiffArea);
            }
            pictureBox2.Image = Image.FromHbitmap(bmp2.GetHbitmap());
        }

        private void button3_Click(object sender, EventArgs e)
        {
            io.BackPic(pictureBox1, pictureBox2);
        }

        private void Btn_TestRange2_Click(object sender, EventArgs e)
        {
            Bitmap bmp2 = pictureBox2.Image as Bitmap;

            int k = int.Parse(txt_Range.Text);

            for (int j = 0; j < bmp2.Width; j++)
            {
                bmp2.SetPixel(j, k, Common.cDiffArea);
            }
            pictureBox2.Image = Image.FromHbitmap(bmp2.GetHbitmap());
        }

        private void btn_Exit_Click(object sender, EventArgs e)
        {
            EndApp("手动退出");
        }

        private void btn_Temp_Click(object sender, EventArgs e)
        {
            ////截图
            ////timerSafeQuestionWorkFlow.Enabled = true;

            //SetLog((wbMain.ReadyState == WebBrowserReadyState.Complete).ToString(), 0);
            ////wbMain.Visible = false;
            //// 获取网页高度和宽度,也可以自己设置
            //int height = wbMain.Document.Body.ScrollRectangle.Height;
            //int width = wbMain.Document.Body.ScrollRectangle.Width;
            //// 调节webBrowser的高度和宽度
            //wbMain.Height = height;
            //wbMain.Width = width;

            //Bitmap bitmap = new Bitmap(width, height);  // 创建高度和宽度与网页相同的图片
            //Rectangle rectangle = new Rectangle(0, 0, width, height);  // 绘图区域
            //wbMain.DrawToBitmap(bitmap, rectangle);  // 截图

            //// 保存图片对话框
            //SaveFileDialog saveFileDialog = new SaveFileDialog();
            //saveFileDialog.Filter = "JPEG (*.jpg)|*.jpg|PNG (*.png)|*.png";
            //saveFileDialog.ShowDialog();
            //bitmap.Save(saveFileDialog.FileName);  // 保存图片


            //设置用户名密码
            //SetVeryCodeLogin();

            //移动鼠标
            Move(500, 500);
        }

        /// <summary>
        /// 找匹配的图片
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_Search_Click(object sender, EventArgs e)
        {
            SearchPicBase();
        }
        #endregion

        /// <summary>
        /// 设置不匹配区域
        /// </summary>
        /// <param name="pLeft"></param>
        /// <param name="pRight"></param>
        public void setPoint(int pLeft, int pRight)
        {
            try
            {
                if (UnMatchLeft == 0) { UnMatchLeft = pLeft; }
                if (UnMatchRigth == 0) { UnMatchRigth = pRight; }

                if (pLeft > UnMatchLeft) { UnMatchLeft = pLeft; }
                if (pRight > UnMatchRigth) { UnMatchRigth = pRight; }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        /// <summary>
        /// 计算差异中心坐标
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_Calc_Click(object sender, EventArgs e)
        {
            Clac();
        }


        //鼠标操作函数
        public void setMouse(object obj)
        {
            int MOUSEEVENTF_LEFTDOWN = 0x2;
            int MOUSEEVENTF_LEFTUP = 0x4;

            //int KEYEVENTF_KEYUP =  Convert.ToInt32( "0xH2", 16);
            //int KEYEVENTF_EXTENDEDKEY = Convert.ToInt32("0xH1", 16);

            Point p = (Point)((object[])(obj))[0];
            int actionType = int.Parse(((object[])(obj))[1].ToString());
            string arg = ((object[])(obj))[2].ToString();

            //Console.WriteLine("鼠标操作:" + actionType);
            //if (actionType != 1) { SetLog("鼠标操作:" + actionType + "", 1); }
            try
            {
                switch (actionType)
                {
                    //移动鼠标
                    case 1:
                        SetLog("鼠标操作:移动鼠标.X:" + p.X + " Y:" + p.Y + "[" + DateTime.Now.Second.ToString() + "]",1);
                        SetCursorPos(p.X, p.Y);
                        break;
                    //LEFTDOWN
                    case 2:
                        SetLog("鼠标操作:LEFTDOWN" + "[" + DateTime.Now.Second.ToString() + "]", 1);
                        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                        break;
                    //LEFTUP
                    case 3://验证码动作的鼠标弹起
                    case 8://网页按钮点击的鼠标弹起
                        if (actionType == 3) { Common.YZMCheckWorkFlowComplete = true; }//验证码类型的鼠标弹起需要变更 Common.YZMCheckWorkFlowComplete的状态为True
                        SetLog("鼠标操作:LEFTUP" + "[" + DateTime.Now.Second.ToString() + "]", 1); 
                        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                        break;
                    //Ctrl+A
                    case 4:
                        SetLog("键盘操作:Ctrl+A" + "[" + DateTime.Now.Second.ToString() + "]", 1);
                        //SendKeys.Send("^{A}");
                        keybd_event(Keys.ControlKey, 0, 0, 0);
                        keybd_event(Keys.A, 0x1e, 0, 0);
                        keybd_event(Keys.A, 0x1e, 2, 0);
                        keybd_event(Keys.ControlKey, 0, 2, 0);
                        break;
                    //Ctrl+V
                    case 5:
                        SetLog("键盘操作:Ctrl+V" + "[" + DateTime.Now.Second.ToString() + "]", 1);
                        SetLog(arg, 2);
                        //SetLog2(arg);//设置剪切板
                        keybd_event(Keys.ControlKey, 0, 0, 0);
                        keybd_event(Keys.V, 0x1e, 0, 0);
                        keybd_event(Keys.V, 0x1e, 2, 0);
                        keybd_event(Keys.ControlKey, 0, 2, 0);
                        break;
                    case 6:
                        SetLog("设置剪切板,应该用不到了", 1);
                        Clipboard.SetDataObject(Common.strPwd);
                        break;
                    case 7:
                        SetLog("滑动鼠标", 1);
                        Move(p.X, p.Y);
                        //setJsInvoke();
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

        /// <summary>
        /// 获得返回的验证码触发
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void lbl_Clipboard_TextChanged(object sender, EventArgs e)
        {
            try
            {
                Clipboard.SetText(lbl_Clipboard.Text);
                SetLog("剪切板被设置为:" + lbl_Clipboard.Text, 0);
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        //保存WBMain的图片
        public void SavePic()
        {
            // 获取网页高度和宽度,也可以自己设置
            int height = wbMain.Document.Body.ScrollRectangle.Height;
            int width = wbMain.Document.Body.ScrollRectangle.Width;
            // 调节webBrowser的高度和宽度
            wbMain.Height = height;
            wbMain.Width = width;

            Bitmap bitmap = new Bitmap(width, height);  // 创建高度和宽度与网页相同的图片
            Rectangle rectangle = new Rectangle(0, 0, width, height);  // 绘图区域
            wbMain.DrawToBitmap(bitmap, rectangle);  // 截图

            // 保存图片对话框
            //SaveFileDialog saveFileDialog = new SaveFileDialog();
            //saveFileDialog.Filter = "JPEG (*.jpg)|*.jpg|PNG (*.png)|*.png";
            //saveFileDialog.ShowDialog();
            string strFile = Common.PicPath.Replace("ImgBase", "GDILog") + Common.strUn + "_log.jpg";
            bitmap.Save(strFile);  // 保存图片
        }



        #region 不要代码
        //public Boolean EnumPic(string strPic)
        //{
        //    Boolean isMatch = false;
        //    try
        //    {
        //        if (strPic.ToLower().Contains("sample")) { return false; }//样式图无视
        //        if (!strPic.ToLower().Contains(".jpg")) { return false; }//样式图无视
        //        pictureBox1.Load(strPic);
        //        isMatch = ComparePic();
        //        return isMatch;
        //    }
        //    catch (Exception ex)
        //    {
        //        throw ex;
        //    }
        //}

        ///// <summary>
        ///// 像素比较
        ///// </summary>
        ///// <param name="pCenter"></param>
        ///// <returns></returns>
        //public Boolean ComparePic()
        //{
        //    Boolean isMatch = false;//默认不匹配
        //    Bitmap bmp1 = pictureBox1.Image as Bitmap;
        //    Bitmap bmp2 = pictureBox2.Image as Bitmap;
        //    int unMatchPixel = 0;
        //    try
        //    {
        //        //初始化
        //        UnMatchLeft = bmp2.Width;
        //        UnMatchRigth = 0;

        //        //满足差异像素的像素点总数  
        //        int MatchPMin = 80;

        //        //为了防止缺失的图片正好在扫描列 所以扫描2次

        //        //只扫描一列像素   x坐标10
        //        for (int j = 0; j < bmp1.Height; j++)
        //        {
        //            Color CurrentCol1 = bmp1.GetPixel(10, j);
        //            Color CurrentCol2 = bmp2.GetPixel(10, j);
        //            if (MatchColorV2(CurrentCol1, CurrentCol2,30))
        //            {
        //                unMatchPixel++;
        //            }
        //        }
        //        if (unMatchPixel >= MatchPMin) return true;


        //        //只扫描一列像素   x坐标90
        //        unMatchPixel = 0;
        //        for (int j = 0; j < bmp1.Height; j++)
        //        {
        //            Color CurrentCol1 = bmp1.GetPixel(90, j);
        //            Color CurrentCol2 = bmp2.GetPixel(90, j);
        //            if (MatchColorV2(CurrentCol1, CurrentCol2, 30))
        //            {
        //                unMatchPixel++;
        //            }
        //        }
        //        if (unMatchPixel >= MatchPMin) return true;

        //        return isMatch;

        //    }
        //    catch (Exception ex)
        //    {
        //        throw ex;
        //    }
        //}
        #endregion

       
        //画三根线
        private void Btn_Show_Click(object sender, EventArgs e)
        {
            try
            {
                Bitmap bmp2 = pictureBox2.Image as Bitmap;
                //中间线
                int k = int.Parse(txtPosition.Text);
                for (int j = 0; j < bmp2.Height; j++)
                {
                    bmp2.SetPixel(k, j, Common.cDiffArea);
                }
                //pictureBox2.Image = Image.FromHbitmap(bmp2.GetHbitmap());

                //左线
                k = int.Parse(txt_Left.Text);
                for (int j = 0; j < bmp2.Height; j++)
                {
                    bmp2.SetPixel(k, j, Common.cDiffArea);
                }
                //pictureBox2.Image = Image.FromHbitmap(bmp2.GetHbitmap());
                //右线
                k = int.Parse(txt_Right.Text);
                for (int j = 0; j < bmp2.Height; j++)
                {
                    bmp2.SetPixel(k, j, Common.cDiffArea);
                }
                pictureBox2.Image = Image.FromHbitmap(bmp2.GetHbitmap());
            }
            catch
            {
                io.BackPic(pictureBox1, pictureBox2);
            }
        }


        //退出程序
        private void EndApp(string strErrmsg)
        {
            try
            {
                SetLog("EndApp调用 strErrmsg:" + strErrmsg, 0);
                SetLog(":" + Common.LoginFaild, 0);
                //if (Common.isNeedReset)
                //{
                //    Common.isNeedReset = false;
                //    wbMain.Url = new Uri(Common.Page1);
                //    timerLoginWorkFlow.Enabled = true;
                //    return 
                //}

                if (string.IsNullOrEmpty(strErrmsg))
                {
                    SetLog("strErrmsg为空 调用ConfirmLogin()加载",0);
                    strErrmsg = ConfirmLogin();
                }
                bll.delMQ5173();
                //这句是返回信息 不能删除
                Console.WriteLine(b64.EncodeBase64(Common.strUn) + "@@" + b64.EncodeBase64(Common.strPwd) + "返回内容:" + strErrmsg);
                lbl_Msg.Text = strErrmsg;
                io.AppendLog("退出信息:" + strErrmsg + "\r\n" + txt_OptionLog.Text);

                //DB db = new DB();
                //db.buildSQL(strErrmsg);
                //SavePic();//保存截图
                System.Environment.Exit(0);
            }
            catch
            {
                Console.WriteLine("-2");
                System.Environment.Exit(0);
            }
        }

        //浏览器加载页面完成函数
        private void wbMain_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            try
            {
                //加载浏览器里的验证码图片
                if (e.Url.ToString().Contains("https://dl.reg.163.com/src/mp-agent-finger.html?WEBZJVersion="))
                {
                    SetLog("发现浏览器验证码图片被加载", 0);
                    Common.DocumentStatus = true;
                    if (Common.LoginFaild)
                    {
                        Common.PubVeryErrRetryTimes++;
                        if (Common.PubVeryErrRetryTimes >= Common.PubMaxVeryErrNumber)
                        {
                            EndApp("无法自动识别(验证错误次数过多)");
                        }

                        SetLog("Common.LoginFaild为真,再次登录", 0);
                        timerLoginWorkFlow.Enabled = true;
                        lbl_Msg.Text = "再次登陆";
                        Common.LoginFaild = false;
                    }
                }

                //登陆成功  跳转到信息页  //有个Ajax在服务器上总是卡死需要特殊处理
                if (e.Url.ToString().Contains("http://reg.163.com/Main.jsp"))
                {
                    SetLog("登陆成功  跳转到信息页", 0);
                    //判断规则
                    string strResult = bll.ValidateBindInfo(wbMain.DocumentText);
                    if (!string.IsNullOrEmpty(strResult))
                    {
                        SetLog("即将退出 原因:" + strResult, 0);
                        EndApp("" + strResult);
                    }
                    wbMain.Url = new Uri("http://reg.163.com/chg/ShowBaomiInfo.jsp");
                }

                //数据页加载完毕 准备抓取
                if (e.Url.ToString().Contains("//reg.163.com/chg/ShowBaomiInfo.jsp"))
                {
                    SetLog("数据页加载完毕 准备抓取", 0);

                    //基本信息
                    /// 手机（0老帐号未绑定手机，1老帐号手机绑定，2新帐号手机绑定，3新帐号未绑定手机）
                    /// 身份证（0未设置，1已设置 3无法验证   目前只有1和3两种状态）
                    /// 安全码(老账号的生日和密码提示问题)/安全问题（0老帐号安全码未设置，1老帐号安全码已设置，2新帐号安全问题已设置，3新帐号安全问题未设置）
                    /// 密保绑定（1将军令，2帐号通，3密保卡，0无密保）   总是设置0
                    /// 密保邮箱（5未设置，4已设置）
                    /// QQ号码（0未设置，1已设置）
                    /// atau49@163.com  302050
                    string bindInfo = bll.LoadUserMsg(wbMain.DocumentText);
                    Common.StrBindInfo = bindInfo;
                    string errMsg = "";
                    Boolean isPass = bll.isPassRule(wbMain.DocumentText, out errMsg);
                    if (!isPass)
                    {
                        EndApp(errMsg);
                        return;
                    }

                    ////网易的帐号
                    ////是新帐号的
                    ////验证的时候，如果身份证显示未设置的，点填写查询并截图
                    ////如果要输入手机验证码或发手机短信的，验证结果中写 实名认证未设置
                    ////如果认证中或打钩的，写实名认证已设置
                    ////老帐号的如果外面显示未打钩的，都写实名是否设置无法验证
                    //if (!"23".Contains(bindInfo.Substring(0, 1)) && bindInfo.Substring(1, 1) == "0")//老账号 + //未设置身份证的
                    //{
                    //    Common.StrBindInfo = Common.StrBindInfo.Substring(0, 1) + "3" + Common.StrBindInfo.Substring(2);
                    //}

                    //if ("23".Contains(bindInfo.Substring(0, 1)) && bindInfo.Substring(1, 1) == "0")//新账号 + //未设置身份证的
                    //{

                    //    wbMain.Url = new Uri("https://reg.163.com/mibao/controller/smdj/index.jsp");//实名认证页面去确认
                    //}

                    //else//如果是新账号认证中或打钩的，写实名认证已设置，或者老账号
                    //{
                    //    //是否要查询密保问题
                    //    if (bindInfo.Substring(2, 1) == "2")
                    //    {
                    //        wbMain.Url = new Uri(Common.Page3);
                    //    }
                    //    else//直接返回结果
                    //    {
                    //        EndApp(bindInfo);
                    //        return;
                    //    }
                    //}

                    //现在不管是新老账号,都去实名认证页面判断下
                    wbMain.Url = new Uri("https://reg.163.com/mibao/controller/smdj/index.jsp");//实名认证页面去确认
                }

                //实名认证页
                if (e.Url.ToString().Contains("controller/smdj/index.jsp"))
                {
                    SetLog("实名认证页面加载完毕", 0);

                    string errMsg = "";
                    Boolean isPass = bll.isRealName_Authentication(wbMain.DocumentText, out errMsg);
                    SetLog(errMsg, 0);
                    //返回 已实名 true  和  不能验证 false 两种状态   现在就2种状态  1是确认已经实名的  3是无法确认的  0未设置这种状态无法确定了
                    if (isPass)
                    {
                        Common.StrBindInfo = Common.StrBindInfo.Substring(0, 1) + "1" + Common.StrBindInfo.Substring(2);
                    }
                    else
                    {
                        Common.StrBindInfo = Common.StrBindInfo.Substring(0, 1) + "3" + Common.StrBindInfo.Substring(2);
                    }

                    //是否要查询密保问题
                    if (Common.StrBindInfo.Substring(2, 1) == "2")
                    {
                        wbMain.Url = new Uri(Common.Page3);
                    }
                    else//直接返回结果
                    {
                        EndApp(Common.StrBindInfo);
                        return;
                    }
                }

                //基本信息页
                if (e.Url.ToString().Contains("account/accountInfo.jsp"))
                {
                    SetLog("基本信息页加载完毕 isPassRule2准备判定", 0);

                    string errMsg = "";
                    Boolean isPass = bll.isPassRule2(wbMain.DocumentText, out errMsg);
                    if (!isPass)
                    {
                        SetLog("isPassRule2判定不合格", 0);
                        EndApp(errMsg);
                        return;
                    }
                    wbMain.Url = new Uri(Common.Page2);
                }

                //验证密保问题页面加载完毕
                if (e.Url.ToString().Contains("//reg.163.com/mibao/mob/replaceByPhone_1.do?noheader=0&url="))
                {
                    SetLog("验证密保问题页面加载完毕", 0);

                    //20170112新增:   如果跳转到这个页面 那么就是需要验证密保.
                    //判断密保问题 和答案  如果不符合规则 直接退出  密保答案不能为[空] 不能小于2个字符
                    //密保问题不能为 [未设置]
                    if (Common.strA1 == "空" || Common.strA2 == "空" || Common.strA3 == "空" || Common.strA1.Length < 2 || Common.strA2.Length < 2 || Common.strA3.Length < 2)
                    {
                        EndApp(Common.StrBindInfo + "0");
                        return;
                    }
                    if (Common.strQ1 == "未设置" || Common.strA2 == "未设置" || Common.strA3 == "未设置")
                    {
                        EndApp(Common.StrBindInfo + "0");
                        return;
                    }

                    //如果验证已提交但是又回到这个页面 那么就是安全问题不对，退出程序
                    if (Common.VeryCodePass)
                    {
                        EndApp(Common.StrBindInfo + "0");
                        return;
                    }

                    string strContent = wbMain.DocumentText;
                    if (Common.isDebug)
                    {
                        if (File.Exists(@"c:\HtmlTest.txt"))
                        {
                            strContent = File.ReadAllText(@"c:\HtmlTest.txt", Encoding.UTF8);
                        }
                    }

                    //设置下拉框
                    SetDdlByInnerText("安全问题", "Click");

                    bll.calcSafeQuestionSort(strContent);
                    string[] answer123 = bll.calcSafeTxtBoxSort(strContent);

                    SetLog(Common.strQ1 + " " + Common.strA1, 0);
                    SetLog(Common.strQ2 + " " + Common.strA2, 0);
                    SetLog(Common.strQ3 + " " + Common.strA3, 0);
                    SetLog(answer123[0] + answer123[1] + answer123[2], 0);
                    //设置密保答案
                    //sortAnswer(); 不需要排序 本身问题文本框就是123有ID编号的
                    SetValueByID(answer123[0], Common.strA1);
                    SetValueByID(answer123[1], Common.strA2);
                    SetValueByID(answer123[2], Common.strA3);
                    //Y轴滚动条到底
                    Common.intOption = 1;
                    timerOption.Interval = 500;
                    timerOption.Enabled = true;

                    //保存当前页面的HTML   如果验证错误，输出日志
                    Common.strQuestionHtml = strContent;

                    //验证    需要去掉注释vcd
                    timerSafeQuestionWorkFlow.Enabled = true;
                }

                //如果跳转到这个页面 那么密保验证提交成功，根据内容判断密保问题是不是正确
                if (e.Url.ToString().Contains("reg.163.com/mibao/verifyMibao2.do"))
                {
                    SetLog("密保验证成功提交确认页面加载完毕", 0);
                    //判断是不是验证正确 判断依据为
                    //回答错误包含
                    //为了您的帐号安全，请先从以下安全工具中任选1种进行身份验证
                    //为了您的帐号安全，请先进行安全验证
                    //回答正确包含
                    //设置安全手机</h2>
                    //<h2 class="clum_title"><a href="http://reg.163.com/account/accountInfo.jsp">帐号管理</a> &gt;&gt; <a href="/mibao/controller/mob/index.jsp">手机服务</a> &gt;&gt; 更换安全手机</h2>

                    string strContent = wbMain.DocumentText;

                    if (strContent.Contains("对不起，您的密保认证错误次数过多，请过一段时间再试吧"))
                    {
                        SetLog("密保认证错误次数过多", 0);
                        EndApp(Common.StrBindInfo + "0");
                        return;
                    }

                    if (strContent.Contains("为了您的帐号安全，请先"))
                    {
                        //输出验证问题页面的HTML
                        io.AppendLog("输出密保提交错误页面的相关HTML\r\n" + Common.strQuestionHtml);
                        //SetLog(Common.strQuestionHtml + " " + Common.strA1, 0);

                        EndApp(Common.StrBindInfo + "0");
                        return;
                    }
                    if (strContent.Contains("手机服务</a>"))
                    {
                        SetLog("验证密保问题正确", 0);
                        EndApp(Common.StrBindInfo + "1");
                        return;
                    }

                    EndApp("验证密保问题无法解析");
                    return;
                }
            }
            catch (Exception ex)
            {
                io.AppendLog("订单" + Common.strOrderID + "遇到错误,错误内容为:\r\n" + ex.ToString());
                EndApp("ErrorCode:009!请联系技术人员修复");
            }
            //Console.WriteLine(e.Url.ToString());
        }


        #region 登录流程/安全问题认证流程
        //Step1   加载Webbrowser里的图片
        private void LoadWBPic(string Action)
        {
            try
            {
                SetLog("LoadWBPic加载", 0);
                switch (Action)
                {
                    case "login":
                        var doc = this.wbMain.Document;
                        var frames = doc.Window.Frames;
                        //IHTMLDocument3 baiduDoc = CorssDomainHelper.GetDocumentFromWindow(frames[0].DomWindow as IHTMLWindow2);
                        //mshtml.IHTMLElementCollection hc = baiduDoc.getElementsByTagName("input");
                        if (frames.Count <= 0)
                        { 
                            //iFramee还没加载好
                            EndApp("自动审核失败，错误001");
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
                        string strPic = bll.CheckPic(BodyHtml);
                        pictureBox2.Load(strPic);
                        pictureBox_Bak.Load(strPic);
                        break;
                    case "safeQuestion":
                        foreach (HtmlElement f in wbMain.Document.Images)
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
            catch(Exception ex)
            {
                EndApp("ErrorCode:002!" + ex.Message);
                //EndApp("自动审核失败(加载" + Action + "时图片错误)");
            }
        }


        //Step2 搜索图库
        private void SearchPicBase()
        {
            try
            {
                SetLog("SearchPicBase加载", 0);
                //遍历文件夹
                string strDir = AppDomain.CurrentDomain.BaseDirectory + "\\ImgBase\\";
                DirectoryInfo TheFolder = new DirectoryInfo(strDir);
                //遍历文件
                foreach (FileInfo NextFile in TheFolder.GetFiles())
                {
                    lbl_PicName1.Text = "文件名:" + NextFile.Name;
                    if (pgs.EnumPic(strDir + NextFile.Name, pictureBox1, pictureBox2))
                    {
                        bll.chooseCorlr(NextFile.Name);
                        //bll.chooseCorlr(NextFile.Name);
                        break;
                    }
                    pictureBox1.Image = null;
                    lbl_PicName1.Text = "没有匹配的图片";
                }
                //如果没有匹配的图片 那么下载网页的图片
                //if (Common.isDebug) { pictureBox1.Image = null; }
                if (pictureBox1.Image == null)
                {
                    io.BackPic(pictureBox2);
                    Common.isNeedReset = true;  //需要重置
                    EndApp("找不到匹配的图片 无法自动识别");
                    
                }
            }
            catch (Exception ex)
            {
                //throw ex;
                EndApp("SearchPicBase" + ex.Message);
            }
        }

        //Step3 填充差异
        private void Match()
        {
            try
            {
                SetLog("Match加载", 0);
                Bitmap bmp1 = new Bitmap(pictureBox1.Image.Width,pictureBox1.Image.Height, PixelFormat.Format24bppRgb);
                bmp1 = pictureBox1.Image as Bitmap;
                Bitmap bmp2 = new Bitmap(pictureBox1.Image.Width,pictureBox1.Image.Height, PixelFormat.Format24bppRgb);
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


                        if (!pgs.MatchColor(CurrentCol1, CurrentCol2, 10))
                        {
                            NoisyPointCount++;
                            //设置像素点颜色  
                            bmp2.SetPixel(i, j, Common.cDiffArea);

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
                        if (Common.cDiffArea != CurrentPixel) continue;

                        int MatchP = pgs.calcPixel(bmp2, i, j);
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
            catch(Exception ex)
            {
                //备份图片
                io.BackPic(pictureBox1, pictureBox2);
                EndApp("ErrorCode:003!" + ex.Message);
            }
        }

        //Step4 计算差异
        private void Clac()
        {
            try
            {
                SetLog("Clac加载", 0);
                Bitmap bmp1 = pictureBox1.Image as Bitmap;
                Bitmap bmp2 = pictureBox2.Image as Bitmap;

                //初始化
                UnMatchLeft = bmp2.Width;
                UnMatchRigth = 0;
                int pCenter;

                bmp2 = pgs.calcPoint(bmp1, bmp2, out pCenter, out UnMatchLeft, out UnMatchRigth);
                ////输出结果
                //pCenter = (UnMatchLeft + UnMatchRigth) / 2;
                //Console.WriteLine(pCenter);
                txtPosition.Text = pCenter.ToString();
                txtPosition350.Text = ((pCenter * 350 / 320)).ToString();
                txtPosition226.Text = ((pCenter * 226 / 320)).ToString();
                txt_Left.Text = UnMatchLeft.ToString();
                txt_Right.Text = UnMatchRigth.ToString();

                ////对图2画线
                //for (int i = 0; i < bmp2.Height; i++)
                //{
                //    bmp2.SetPixel(pCenter, i, cCenterLine);
                //}
                pictureBox2.Image = Image.FromHbitmap(bmp2.GetHbitmap());
            }
            catch (Exception ex)
            {
                //throw ex;
                EndApp("ErrorCode:004!" + ex.Message);
            }
        }

        //Step5 填写账号密码
        private void SetUserNamePwd()
        {
            try
            {
                //鼠标键盘模拟输入账户密码
                //SetLog("SetUserNamePwd加载", 0);
                //int offset = Common.PubOffset;
                ////填写账号
                //Point sPoint = new Point(1362, 280);
                ////Clipboard.SetDataObject(Common.strUn);
                //SetLog("设置剪切板:" + Clipboard.GetText(), 0);
                //System.Threading.Timer threadTimer1 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 1, "" }, offset, 0);
                //System.Threading.Timer threadTimer2 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 2, "" }, 300 + offset, 0);
                //System.Threading.Timer threadTimer3 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 4, "" }, 600 + offset, 0);//Ctrl+A
                //System.Threading.Timer threadTimer4 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 5, Common.strUn }, 900 + offset, 0);//Ctrl+V

                //////填写密码
                //sPoint = new Point(1362, 340);
                ////System.Threading.Timer threadTimer01 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 6, "" }, 1000 + offset, 0);
                ////myThread.SetApartmentState(ApartmentState.STA); 
                //System.Threading.Timer threadTimer11 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 1, "" }, 1200 + offset, 0);
                //System.Threading.Timer threadTimer22 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 2, "" }, 1500 + offset, 0);
                //System.Threading.Timer threadTimer33 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 4, "" }, 1800 + offset, 0);//Ctrl+A
                //System.Threading.Timer threadTimer44 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 5, Common.strPwd }, 2100 + offset, 0);//Ctrl+V

                //js实现填写账号密码(跨域Iframe)
                var doc = this.wbMain.Document;
                var frames = doc.Window.Frames;
                IHTMLDocument3 baiduDoc3 = CorssDomainHelper.GetDocumentFromWindow(frames[0].DomWindow as IHTMLWindow2);
                IHTMLDocument2 baiduDoc2 = (IHTMLDocument2)CorssDomainHelper.GetDocumentFromWindow(frames[1].DomWindow as IHTMLWindow2);
                IHTMLDocument baiduDoc = (IHTMLDocument)CorssDomainHelper.GetDocumentFromWindow(frames[1].DomWindow as IHTMLWindow2);
                //mshtml.IHTMLElementCollection hc = baiduDoc.getElementsByTagName("input");
                //Console.WriteLine("页面源码:" + baiduDoc2.body.innerHTML);//检测账号密码错误的提示
                //var Btn_Submit = baiduDoc3.getElementById("dologin");
                IHTMLWindow2 vWindow = (IHTMLWindow2)baiduDoc2.parentWindow;
                try
                {
                    string strJscript = string.Format("var eByName = document.getElementsByName('email');eByName[0].value = '{0}';",Common.strUn);
                    strJscript += string.Format("var eByName = document.getElementsByName('password');eByName[0].value = '{0}';", Common.strPwd);
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

        //Step6 滑动鼠标验证
        private void SetVeryCodeLogin()
        {
            try
            {
                SetLog("SetVeryCodeLogin加载", 0);
                Point sPoint = new Point(1182, 400);
                Point ePoint = new Point(sPoint.X - 39 + int.Parse(txtPosition350.Text), sPoint.Y);
                //Point ePoint = new Point(sPoint.X - 59 + int.Parse(txtPosition350.Text), sPoint.Y);//故意错误的验证码位置 搜索关键字:错误坐标
                int offset = Common.PubOffset;

                ////调试文本信息
                //txt_OptionLog.Text = "";
                //SetLog("320的图对应中间线在:" + txtPosition.Text, 0);
                //SetLog("350的图对应中间线在:" + txtPosition350.Text, 0);
                ////int calc = int.Parse(txtPosition.Text) * 350  / 320;
                //SetLog("计算:" + txtPosition.Text + " * 350 / 320得到" + txtPosition350.Text, 0);
                //SetLog("鼠标起始坐标:" + sPoint.X.ToString(), 0);
                //SetLog("鼠标最终坐标:" + ePoint.X.ToString(), 0);

                //具体执行
                System.Threading.Timer threadTimer1 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 1, "" }, 100 + offset, 0);
                System.Threading.Timer threadTimer2 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 2, "" }, 500 + offset, 0);
                //移动鼠标使用参数7 新的API
                System.Threading.Timer threadTimer3 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { ePoint, 7, "" }, 800 + offset, 0);
                System.Threading.Timer threadTimer4 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { ePoint, 1, "" }, 1400 + offset, 0);
                System.Threading.Timer threadTimer5 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { ePoint, 3, "" }, 1600 + offset, 0);
                //登陆
                //System.Threading.Timer threadTimer6 = new System.Threading.Timer(new System.Threading.TimerCallback(Login), null, 2000 + offset, 0);
                //调试
                //txtPosition.Text = "320";
                //SetCursorPos(1182 + int.Parse(txtPosition.Text), 400);
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }


        //Step6 滑动鼠标验证(再次确认登陆按钮  提示语句为：当前安全手机：********104，若已换号请及时更换手机。)
        private void LoginAgain()
        {
            try
            {
                SetLog("LoginAgain加载", 0);
                Point sPoint = new Point(1182, 430);
                //Point ePoint = new Point(sPoint.X - 39 + int.Parse(txtPosition350.Text), sPoint.Y);
                int offset = Common.PubOffset;

                System.Threading.Timer threadTimer1 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 1, "" }, 100 + offset, 0);
                System.Threading.Timer threadTimer2 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 2, "" }, 300 + offset, 0);
                System.Threading.Timer threadTimer4 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 8, "" }, 600 + offset, 0);
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        //Step6 滑动鼠标验证(再次确认登陆按钮  提示语句为：包含这两句话：[帐号安全等级低]和[建议你绑定手机] 也要点击确认。)
        private void LoginAgainType2()
        {
            try
            {
                SetLog("LoginAgainType2加载", 0);
                Point sPoint = new Point(1200, 490);
                //Point ePoint = new Point(sPoint.X - 39 + int.Parse(txtPosition350.Text), sPoint.Y);
                int offset = Common.PubOffset;
                System.Threading.Timer threadTimer1 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 1, "" }, 100 + offset, 0);
                System.Threading.Timer threadTimer2 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 2, "" }, 300 + offset, 0);
                System.Threading.Timer threadTimer4 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 8, "" }, 600 + offset, 0);

                //用JS触发登陆  感觉暂时没必要
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        //Step6 滑动鼠标验证(密保问题验证码)
        private void SetVeryCodeSafeQuestion()
        {
            //图片宽226 总拉伸长度176   起始位置226-176  /2 = 25
            try
            {
                SetLog("SetVeryCodeSafeQuestion加载", 0);
                Point sPoint = new Point(895, 440);
                Point ePoint = new Point(sPoint.X - 25 + int.Parse(txtPosition226.Text), sPoint.Y);
                int offset = 300 + Common.PubOffset;

                System.Threading.Timer threadTimer1 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 1, "" }, 100 + offset, 0);
                System.Threading.Timer threadTimer2 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 2, "" }, 300 + offset, 0);

                System.Threading.Timer threadTimer3 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { ePoint, 7, "" }, 500 + offset, 0);
                System.Threading.Timer threadTimer4 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { ePoint, 1, "" }, 1100 + offset, 0);
                System.Threading.Timer threadTimer5 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { ePoint, 3, "" }, 1300 + offset, 0);
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        //Step7 登陆
        private void Login()
        //private void Login(object obj)
        {
            try
            {
                SetLog("Login加载", 0);
                //var doc = this.wbMain.Document;
                //var frames = doc.Window.Frames;
                //IHTMLDocument3 baiduDoc3 = CorssDomainHelper.GetDocumentFromWindow(frames[0].DomWindow as IHTMLWindow2);
                //IHTMLDocument2 baiduDoc2 = (IHTMLDocument2)CorssDomainHelper.GetDocumentFromWindow(frames[1].DomWindow as IHTMLWindow2);
                ////mshtml.IHTMLElementCollection hc = baiduDoc.getElementsByTagName("input");
                //Console.WriteLine("页面源码:" + baiduDoc2.body.innerHTML);//检测账号密码错误的提示
                //var Btn_Submit = baiduDoc3.getElementById("dologin");
                ////Console.WriteLine(Btn_Submit.getAttribute("id"));
                //int offset = 1600 + Common.PubOffset;
                ////填写账号
                //Point sPoint = new Point(1362, 465);
                //System.Threading.Timer threadTimer2 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 1, "" }, 100 + offset, 0);
                //System.Threading.Timer threadTimer3 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 2, "" }, 200 + offset, 0);//MouseDown
                //System.Threading.Timer threadTimer4 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { sPoint, 3, "" }, 300 + offset, 0);//MouseUp

                //委托方式执行 调用浏览器的JS
                if (this.wbMain.InvokeRequired)
                {
                    SetTextCallBack0 stcb = new SetTextCallBack0(Login);
                    this.Invoke(stcb, null);
                }
                else
                {
                    //Console.WriteLine("执行JS登陆");
                    var doc = this.wbMain.Document;
                    var frames = doc.Window.Frames;
                    IHTMLDocument3 baiduDoc3 = CorssDomainHelper.GetDocumentFromWindow(frames[0].DomWindow as IHTMLWindow2);
                    IHTMLDocument2 baiduDoc2 = (IHTMLDocument2)CorssDomainHelper.GetDocumentFromWindow(frames[1].DomWindow as IHTMLWindow2);
                    IHTMLDocument baiduDoc = (IHTMLDocument)CorssDomainHelper.GetDocumentFromWindow(frames[1].DomWindow as IHTMLWindow2);
                    IHTMLWindow2 vWindow = (IHTMLWindow2)baiduDoc2.parentWindow;
                    try
                    {
                        string strJscript = string.Format("document.getElementById('dologin').click();;");
                        vWindow.execScript(strJscript, "javascript");
                    }
                    catch (Exception e)
                    {
                        throw e;
                    }
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        //Step8 确认登陆结果
        private string ConfirmLogin()
        {
            //请滑动验证码        //<DIV class=ferrorhead id=auto-id-1481521221376>请滑动验证码</DIV></DIV>
            //帐号或密码错误      //<DIV class=ferrorhead>帐号或密码错误</DIV></DIV>
            string strLoginErrResult = string.Empty;
            try
            {
                SetLog("ConfirmLogin加载", 0);
                var doc = this.wbMain.Document;
                var frames = doc.Window.Frames;

                //判断条件2似乎足够了 但是为了保证兼容原来的思路判断条件1保留
                if (frames.Count <= 0) { return "No frames"; }//不能返回未知错误 不然提交后跳转到Main页面 监测登陆状态认为异常退出了
                if (wbMain.Url.ToString().Contains("reg.163.com/Main.jsp")) { return "No frames"; }

                IHTMLDocument2 baiduDoc = (IHTMLDocument2)CorssDomainHelper.GetDocumentFromWindow(frames[1].DomWindow as IHTMLWindow2);

                //SetLog("页面源码:" + baiduDoc.body.innerHTML, 0);
                //SetLog("(预估错误点)当前地址" + wbMain.Url.ToString(), 0);
                //SetLog("(预估错误点)doc.Window.Frames总数" + frames.Count, 0);
                //SetLog("(预估错误点)baiduDoc等于Nul吗" + (baiduDoc == null).ToString(), 0);
                //SetLog("标题:" + baiduDoc.title, 0);

                if (baiduDoc.body.innerHTML.Contains("请滑动验证码</DIV></DIV>") || baiduDoc.body.innerHTML.Contains("请输入密码"))
                {
                    strLoginErrResult = "验证码错误";
                    //把错误的验证码存起来
                    io.BackPic(pictureBox_Bak);
                    strLoginErrResult = "请滑动验证码";
                    Common.LoginFaild = true;//如果重新加载到验证码图片会启动登陆步骤
                }
                else if (baiduDoc.body.innerHTML.Contains("帐号或密码错误</DIV></DIV>"))
                {
                    strLoginErrResult = "帐号或密码错误";//处理器收到账号密码错误会重启程序跑一遍 以免是误报 验证码多次错误会有这个现象
                }
                else if (baiduDoc.body.innerHTML.Contains("当前安全手机") && baiduDoc.body.innerHTML.Contains("若已换号请及时更换手机"))
                {
                    strLoginErrResult = "<a>登陆</a>";
                }
                else if (baiduDoc.body.innerHTML.Contains("帐号安全等级低") && baiduDoc.body.innerHTML.Contains("建议你绑定手机"))
                {
                    strLoginErrResult = "<a>登陆2</a>";
                }
                //请用手机<SPAN id=pp_mb>********723</SPAN>进行安全验证
                else if (baiduDoc.body.innerHTML.Contains("请用手机") && baiduDoc.body.innerHTML.Contains("进行安全验证"))
                {
                    strLoginErrResult = "需要手机验证码";
                }
                else if (baiduDoc.body.innerHTML.Contains("操作超时") && baiduDoc.body.innerHTML.Contains("请刷新页面重"))
                {
                    strLoginErrResult = "刷新页面重试";
                    Common.LoginFaild = true;//如果重新加载到验证码图片会启动登陆步骤
                }
                else if (baiduDoc.body.innerHTML.Contains("帐号格式错误"))
                {
                    strLoginErrResult = "帐号格式错误";
                    Common.LoginFaild = true;//如果重新加载到验证码图片会启动登陆步骤
                }
                else
                {
                    Common.LoginFaild = true;//如果重新加载到验证码图片会启动登陆步骤  //有时候登陆失败 啥提示也没 这时候也重新登录
                    strLoginErrResult = "ConfirmLogin解析未知错误.错误内容：" + baiduDoc.body.innerHTML;
                }
                return strLoginErrResult;
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        //点击下一步
        private string ConfirmSafeQuestion()
        {
            SetLog("ConfirmSafeQuestion加载", 0);
            string strLoginErrResult = string.Empty;
            //2行分别为认证成功和未成功的区别
            //<div class="statusTxt" style="height: 22.6px; margin-left: 238px; line-height: 22.6px; display: block;"></div>
            //<div class="statusTxt" style="height: 22.6px; margin-left: 238px; line-height: 22.6px; display: none;"></div>
            try
            {
                lbl_Msg.Text = "安全问题点下一步";
                string strHtml = wbMain.Document.Body.OuterHtml.ToLower();
                //io.AppendLog(strHtml);
                //Console.WriteLine(strHtml);
                //不停浏览器这个DIV各个属性顺序不一样  所以要取statusTxt所在的<>内 找display: block; 或者  display: none;
                int i = strHtml.IndexOf("statustxt");
                i = strHtml.LastIndexOf("<",i);
                int j = strHtml.IndexOf(">",i);
                strHtml = strHtml.Substring(i, j - i);

                if (strHtml.Contains("display: none;"))
                {
                    //Console.WriteLine("验证失败");
                    wbMain.Url = new Uri(Common.Page2);
                }
                //验证成功
                else if (strHtml.Contains("display: block;"))
                {
                    Common.VeryCodePass = true;
                    //Console.WriteLine("验证成功");
                    //点击下一步
                    foreach (HtmlElement he in wbMain.Document.GetElementsByTagName("input"))
                    {
                        if (he.GetAttribute("value") != null)
                        {
                            //Console.WriteLine(he.GetAttribute("value"));
                            if (he.GetAttribute("value") == "下一步")
                            {
                                he.InvokeMember("click");
                            }
                        }
                    }
                }
                else
                {
                    lbl_Msg.Text = "HTML分析错误" + strHtml.Contains("statustxt").ToString();
                }
                return strLoginErrResult;
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }


        #endregion


        #region 网页控件操作

        //问题排序
        public void sortAnswer()
        {
            string strTmp;
            try
            {
                string strHtml = wbMain.DocumentText;
                int Q1 = strHtml.IndexOf(Common.strQ1);
                int Q2 = strHtml.IndexOf(Common.strQ2);
                int Q3 = strHtml.IndexOf(Common.strQ3);
                if (Q3 < Q1) { strTmp = Common.strA3; Common.strA3 = Common.strA1; Common.strA1 = strTmp; }
                if (Q2 < Q1) { strTmp = Common.strA2; Common.strA2 = Common.strA1; Common.strA1 = strTmp; }
                if (Q3 < Q2) { strTmp = Common.strA3; Common.strA3 = Common.strA2; Common.strA2 = strTmp; }

            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        //根据控件ID设置值
        public void SetValueByID(string key, string value)
        {
            try
            {
                wbMain.Document.GetElementById(key).SetAttribute("value", value);
            }
            catch
            {

            }
        }

        //根据超链接内容获取超链接对象
        public void SetDdlByInnerText(string key, string action)
        {
            try
            {
                for (int i = 0; i < wbMain.Document.Links.Count; i++)
                {
                    if (wbMain.Document.Links[i].InnerText == key && !wbMain.Document.Links[i].GetAttribute("href").Contains("accountSecQuestionIndex.do"))
                    {
                        //用swtich是防止大小写问题 并且可以组合自定义动作
                        switch (action.ToLower())
                        {
                            case "click":
                                wbMain.Document.Links[i].InvokeMember("click");
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
            catch
            {

            }
        }
        #endregion


        #region Timer系列
        //其他延时操作
        private void timerOption_Tick(object sender, EventArgs e)
        {
            string strResult = string.Empty;
            try
            {
                switch (Common.intOption)
                {
                    //安全问题滚动条下拉
                    case 1:
                        int height = wbMain.Document.Body.ScrollRectangle.Height;
                        wbMain.Document.Window.ScrollTo(320, height);

                        Common.intOption = 0;
                        timerOption.Enabled = false;
                        break;
                    case 2:
                        if (Common.YZMCheckWorkFlowComplete)
                        {
                            Common.YZMCheckWorkFlowComplete = false;
                            ConfirmSafeQuestion();
                            Common.intOption = 0;
                            timerOption.Enabled = false;
                        }
                        break;
                    //验证登陆状态，验证码错误的话继续刷新页面
                    case 3:
                        strResult = ConfirmLogin();
                        //SetLog("ConfirmLogin()获取结果:" + strResult, 0);
                        if (strResult == "验证码错误" || strResult == "请滑动验证码" || strResult == "刷新页面重试")
                        {
                            wbMain.Url = new Uri(Common.Page1);
                        }
                        if (strResult == "帐号或密码错误")
                        {
                            EndApp("无法自动识别,帐号或密码错误");
                        }
                        if (strResult == "需要手机验证码")
                        {
                            EndApp("需要手机验证码才能登陆");
                        }
                        if (strResult == "帐号或密码错误")
                        {
                            EndApp("帐号或密码错误");
                        }
                        if (strResult == "帐号格式错误")
                        {
                            EndApp("帐号格式错误");
                        }
                        if (strResult == "<a>登陆</a>")
                        {
                            LoginAgain();//再次确认登录按钮
                        }
                        if (strResult == "<a>登陆2</a>")
                        {
                            LoginAgainType2();//再次确认登录按钮
                        }
                        if (strResult.Contains("ConfirmLogin解析未知错误.错误内容："))
                        {
                            SetLog("ConfirmLogin解析未知错误.错误内容",0);
                            //io.AppendLog(strResult);
                            wbMain.Url = new Uri(Common.Page1);
                            //EndApp("未知错误!(ConfirmLogin 可能是改版或未知的格式)");
                        }
                        if (strResult == "No frames")
                        { 
                            //跳转到登陆成功页面去了 不需要处理
                        }

                        Common.intOption = 0;
                        timerOption.Enabled = false;
                        break;
                        //验证码拉动完成,触发登陆操作
                    case 4:
                        //补丁: 如果长期False状态的话  发送一个鼠标弹起操作 因为上一个未知原因被吃掉了
                        //System.Threading.Timer threadTimer5 = new System.Threading.Timer(new System.Threading.TimerCallback(setMouse), new object[] { ePoint, 3, "" }, 1600 + offset, 0);

                        //SetLog("监视验证码验证步骤是否完成", 0);
                        if (Common.YZMCheckWorkFlowComplete)
                        {
                            SetLog("验证码验证步骤完成,触发登陆", 0);
                            Common.intOption = 3;
                            Login();//这个有个已经修复的Bug Login后一秒才执行判断登陆状态  这时候可能已经跳转到登陆成功页  也可能还在验证中 需要在ConfirmLogin里添加判断
                            Common.YZMCheckWorkFlowComplete = false;
                        }
                        break;
                    default:

                        Common.intOption = 0;
                        timerOption.Enabled = false;
                        break;
                }
            }
            catch (Exception ex)
            {
                //throw ex;
                SetLog("当前Common.intOption" + Common.intOption.ToString() + ex.ToString() ,0);
                EndApp("ErrorCode:007!" + ex.Message);
            }
            finally
            {

            }
        }

        //启动自动登录步骤 延时调用加载
        private void timerLoginWorkFlow_Tick(object sender, EventArgs e)
        {
            try
            {
                //return;//调试总开关    搜索关键字:总开关

                //Step1   加载Webbrowser里的图片
                LoadWBPic("login");
                //Step2 搜索图库
                SearchPicBase();
                //Step3 填充差异
                Match();
                //Step4 计算差异
                Clac();
                //Step5 填写账号密码
                SetUserNamePwd();
                //Step6 滑动鼠标验证
                SetVeryCodeLogin();     //1800毫秒后执行完毕
                SetLog("登录操作开始", 0);
                //Step7 登陆
                //Login();//Login之后收不到提交的消息  所以用延时读取的方式进行下一步
                //Step8 确认状态    延时10000 + Common.PubOffset   配置在超时函数里       //验证验证码通过的话要点击确认按钮
                Common.intOption = 4;
                timerOption.Interval = 1000;
                timerOption.Enabled = true;

                timerLoginWorkFlow.Enabled = false;
                //判断结果
            }
            catch (Exception ex)
            {
                EndApp("ErrorCode:006!" + ex.Message + ex.StackTrace);
            }
        }

        //超时自动关闭
        private void timerAutoClose_Tick(object sender, EventArgs e)
        {
            int intMaxSeconds = Common.PubMaxSeconds;
            try
            {
                //程序开启超时
                DateTime dt = DateTime.Now;
                TimeSpan tsP = new TimeSpan(Common.dtStart.Ticks);
                TimeSpan tsF = new TimeSpan(dt.Ticks);
                TimeSpan ts = tsF.Subtract(tsP).Duration();
                int CountSec = ts.Minutes * 60 + ts.Seconds;//计算两个时间的时间差 只记分和秒的差值
                lbl_AutoCloseMsg.Text = "处理时间:" + CountSec.ToString();
                lbl_isDebug.Text = "isDebug:" + (Common.isDebug ? "是" : "否");

                if (CountSec > intMaxSeconds / 2)
                {
                    lbl_AutoCloseMsg.Text = "处理时间:" + CountSec.ToString() + ".剩余时间:" + (intMaxSeconds - CountSec).ToString() + "秒后自动关闭";
                }

                //SetLog("tsP:" + tsP.ToString(), 0);
                //SetLog("tsF:" + tsF.ToString(), 0);
                //SetLog("DateTime.Now:" + DateTime.Now.ToString(), 0);
                //SetLog("监控关闭:" + (ts.Seconds > intMaxSeconds).ToString(), 0);

                if (CountSec > intMaxSeconds)
                {
                    //SetLog("最大执行秒数:" + intMaxSeconds.ToString(),0);
                    //SetLog("当前执行秒数:" + ts.Seconds.ToString(), 0);
                    //lbl_AutoCloseMsg.Text = "处理时间:" + ts.Seconds.ToString() + ".执行自动关闭";
                    EndApp("超时关闭");
                }
                //监控队列
                if (!Common.MQ5173Status){lbl_Msg.Text = "还没轮到我";}
                if (!Common.MQ5173Status && Common.DocumentStatus && bll.CurrentMQ())
                {
                    Common.MQ5173Status = true;
                    timerLoginWorkFlow.Enabled = true;
                    lbl_Msg.Text = "启动登陆";
                }

                //刷新当前鼠标所在位置(调试完注释掉吧)
                POINT currentPosition = new POINT();
                GetCursorPos(out currentPosition);
                //System.Diagnostics.Debug.WriteLine("Current mouse point: " + currentPosition.ToString());
                lbl_AppMsg.Text = "当前鼠标X:" + currentPosition.X.ToString();
            }
            catch (Exception ex)
            {
                //throw ex;
                EndApp("ErrorCode:005!" + ex.Message);
            }
        }
        #endregion

        private void timerSafeQuestionWorkFlow_Tick(object sender, EventArgs e)
        {
            try
            {
                //Step1   加载Webbrowser里的图片
                LoadWBPic("safeQuestion");
                //Step2 搜索图库
                SearchPicBase();
                //Step3 填充差异
                Match();
                //Step4 计算差异
                Clac();
                //Step5 填写账号密码
                ////SetUserNamePwd();//安全问题不需要此步骤
                //Step6 滑动鼠标验证
                SetVeryCodeSafeQuestion();
                lbl_Msg.Text = "安全问题验证啦";
                //Step7 登陆
                ////Login();
                //Step8 确认状态    延时8000 + Common.PubOffset   配置在超时函数里
                //Step9 安全问题验证确认状态  延时调用ConfirmSafeQuestion();
                Common.intOption = 2;
                timerOption.Interval = Common.PubOffset;
                timerOption.Enabled = true;
                //Step9 处理完自动点击下一步
                timerSafeQuestionWorkFlow.Enabled = false;
            }
            catch (Exception ex)
            {
                EndApp("ErrorCode:008!" + "timerSafeQuestionWorkFlow_Tick:" + ex.Message);
                //throw ex;
            }
        }

        //日志
        private void SetLog(string strMsg,int type)
        {
            try
            {
                //普通的输出日志
                if (type == 0)
                {
                    txt_OptionLog.Text = strMsg + "\r\n" + txt_OptionLog.Text;
                }
                //普通的委托输出日志 似乎会引起timer线程无规律没有被调用的现象
                if (type == 1)
                {
                    //if (this.txt_OptionLog.InvokeRequired)
                    //{
                    //    SetTextCallBack2 stcb = new SetTextCallBack2(SetLog);
                    //    this.Invoke(stcb, new object[] { strMsg, type });
                    //}
                    //else
                    //{
                    //    this.txt_OptionLog.Text = strMsg + "\r\n" + txt_OptionLog.Text;
                    //}
                }
                //设置lbl_Clipboard触发change时间来设置剪切板
                if (type == 2)
                {
                    if (this.lbl_Clipboard.InvokeRequired)
                    {
                        SetTextCallBack2 stcb = new SetTextCallBack2(SetLog);
                        this.Invoke(stcb, new object[] { strMsg, type });
                    }
                    else
                    {
                        this.lbl_Clipboard.Text = strMsg;
                    }
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

    }
}
