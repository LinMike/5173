using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using mshtml;
using System.IO;
using System.Drawing.Imaging;

namespace GDI
{
    public partial class CollectPic : Form
    {
        PicGetSet pgs = new PicGetSet();
        int UnMatchLeft;
        int UnMatchRigth;
        Bll bll = new Bll();

        public CollectPic()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
           
        }

        private void CollectPic_Load(object sender, EventArgs e)
        {
            wbMain.Url = new Uri("http://reg.163.com/");
        }

        private void btn_Save_Click(object sender, EventArgs e)
        {
            Image img = pictureBox2.Image;
            img.Save(Common.PicPath + DateTime.Now.ToString("yyyyMMddhhmmss") + ".jpg", System.Drawing.Imaging.ImageFormat.Jpeg);//第二个参数是保存格式，根据自己需要修改

        }

        private void btn_LoadPic_Click(object sender, EventArgs e)
        {
            LoadWBPic("login");
        }

        private void btn_Search_Click(object sender, EventArgs e)
        {
            //遍历文件夹
            string strDir = AppDomain.CurrentDomain.BaseDirectory + "\\ImgBase\\";
            DirectoryInfo TheFolder = new DirectoryInfo(strDir);
            //遍历文件
            foreach (FileInfo NextFile in TheFolder.GetFiles())
            {
                lbl_PicName1.Text = "文件名:" + NextFile.Name;
                if (pgs.EnumPic(strDir + NextFile.Name,pictureBox1,pictureBox2))
                {
                    break;
                }
                pictureBox1.Image = null;
                lbl_PicName1.Text = "没有匹配的图片";
            }
        }

        private void Btn_Match_Click(object sender, EventArgs e)
        {
            Bitmap bmp1 = new Bitmap(pictureBox1.Image.Width, pictureBox1.Image.Height, PixelFormat.Format24bppRgb);
            bmp1 = pictureBox1.Image as Bitmap;
            Bitmap bmp2 = new Bitmap(pictureBox1.Image.Width, pictureBox1.Image.Height, PixelFormat.Format24bppRgb);
            bmp2 = pictureBox2.Image.Clone() as Bitmap;


            //用于去除错误噪点的变量
            Bitmap bmpBak = pictureBox2.Image.Clone() as Bitmap;
            int BmpSumX = 0;//保存所有噪点的X总值
            int BmpSumY = 0;//保存所有噪点的Y总值
            int NoisyPointCount = 0;    //噪点总数

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

            //对错误噪点去除的处理 20170117   //缺失部分范围大小60*100
            int BmpAreaX = 60; int BmpAreaY = 100;
            int BmpAvgX = BmpSumX / NoisyPointCount;
            int BmpAvgY = BmpSumY / NoisyPointCount;

            //对图2画线 //调试的时候看方便 实际用请注释掉
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
                        NoisyPointCount++;
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
            pictureBox2.Image = Image.FromHbitmap(bmp2.GetHbitmap());
            Console.WriteLine("共有" + NoisyPointCount + "个像素点有差异");
        }

        private void btn_Calc_Click(object sender, EventArgs e)
        {

            Bitmap bmp1 = pictureBox1.Image as Bitmap;
            Bitmap bmp2 = pictureBox2.Image as Bitmap;
            //初始化
            UnMatchLeft = bmp2.Width;
            UnMatchRigth = 0;
            int pCenter;

            bmp2 = pgs.calcPoint(bmp1, bmp2, out pCenter, out UnMatchLeft, out UnMatchRigth);
            
            //Console.WriteLine(pCenter);
            txtPosition.Text = pCenter.ToString();
            txt_Left.Text = UnMatchLeft.ToString();
            txt_Right.Text = UnMatchRigth.ToString();

            //对图2画线
            for (int i = 0; i < bmp2.Height; i++)
            {
                bmp2.SetPixel(pCenter, i, Common.cCenterLine);
            }
            pictureBox2.Image = Image.FromHbitmap(bmp2.GetHbitmap());
        }

        private void btn_SavePicBox_Click(object sender, EventArgs e)
        {
            Image img = pictureBox1.Image;
            img.Save(Common.PicPath + "Sample_Tmp1" + ".jpg", System.Drawing.Imaging.ImageFormat.Jpeg);

            img = pictureBox2.Image;
            img.Save(Common.PicPath + "Sample_Tmp2" + ".jpg", System.Drawing.Imaging.ImageFormat.Jpeg);
        }

        private void btn_LoadPicBox_Click(object sender, EventArgs e)
        {
            pictureBox1.Load(Common.PicPath + "Sample_Tmp1" + ".jpg");
            pictureBox2.Load(Common.PicPath + "Sample_Tmp2" + ".jpg");
        }

        private void Btn_TestRange_Click(object sender, EventArgs e)
        {
            Bitmap bmp1 = pictureBox1.Image as Bitmap;
            Bitmap bmp2 = pictureBox2.Image as Bitmap;

            int k = int.Parse(txt_Range.Text);

            for (int j = 0; j < bmp1.Height; j++)
            {
                bmp1.SetPixel(k, j, Common.cDiffArea);
            }
            pictureBox1.Image = Image.FromHbitmap(bmp1.GetHbitmap());
        }

        private void btn_SaveNewLeft_Click(object sender, EventArgs e)
        {
            Bitmap bmp1 = pictureBox1.Image as Bitmap;
            Bitmap bmp2 = pictureBox2.Image as Bitmap;

            int k = int.Parse(txt_Range.Text);
            for (int i = 0; i <= k; i++)
            {
                for (int j = 0; j < bmp1.Height; j++)
                {
                    Color CurrentCol2 = bmp2.GetPixel(i, j);
                    bmp1.SetPixel(i, j, CurrentCol2);
                }
            }
            
            pictureBox1.Image = Image.FromHbitmap(bmp1.GetHbitmap());
            bmp1.Dispose(); 
            Image img = pictureBox1.Image;
            img.Save(Common.PicPath + lbl_PicName1.Text.Replace("文件名:","new"), System.Drawing.Imaging.ImageFormat.Jpeg);

        }

        private void btn_SaveNewRight_Click(object sender, EventArgs e)
        {
            Bitmap bmp1 = pictureBox1.Image as Bitmap;
            Bitmap bmp2 = pictureBox2.Image as Bitmap;

            int k = int.Parse(txt_Range.Text);
            for (int i = k; i < bmp1.Width; i++)
            {
                for (int j = 0; j < bmp1.Height; j++)
                {
                    Color CurrentCol2 = bmp2.GetPixel(i, j);
                    bmp1.SetPixel(i, j, CurrentCol2);
                }
            }
            
            pictureBox1.Image = Image.FromHbitmap(bmp1.GetHbitmap());
            bmp1.Dispose(); 
            Image img = pictureBox1.Image;
            img.Save(Common.PicPath + lbl_PicName1.Text.Replace("文件名:", "new"), System.Drawing.Imaging.ImageFormat.Jpeg);

        }

        private void Btn_Show_Click(object sender, EventArgs e)
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

        //流程 同步自主窗体
        //Step1   加载Webbrowser里的图片
        private void LoadWBPic(string Action)
        {
            try
            {
                //SetLog("LoadWBPic加载", 0);
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
                            //EndApp("自动审核失败，错误001");
                            lbl_Msg.Text = "自动审核失败，错误001";
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
                        //pictureBox_Bak.Load(strPic);
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
                                //pictureBox_Bak.Load(strPicSafeQ);
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
                //EndApp("自动审核失败(加载" + Action + "时图片错误)");
            }
        }

        private void btn_newPic_Click(object sender, EventArgs e)
        {
            wbMain.Url = new Uri(Common.Page1);
        }
    }
}
