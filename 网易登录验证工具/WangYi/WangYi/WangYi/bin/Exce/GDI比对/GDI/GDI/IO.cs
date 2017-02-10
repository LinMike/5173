using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.IO;

namespace GDI
{
    class IO
    {
        public void BackPic(PictureBox pic1,PictureBox pic2)
        {
            try
            {
                Image img = null;
                //备份图片
                if (pic1.Image != null)
                {
                    img = pic1.Image;
                    img.Save(Common.PicPath + "Sample_Tmp1" + ".jpg", System.Drawing.Imaging.ImageFormat.Jpeg);
                }
                if (pic2.Image != null)
                {
                    img = pic2.Image;
                    img.Save(Common.PicPath + "Sample_Tmp2" + ".jpg", System.Drawing.Imaging.ImageFormat.Jpeg);
                }
                img.Dispose();
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        //保存图片到未识别图库
        public void BackPic(PictureBox pic)
        {
            try
            {
                Image img = null;
                //备份图片
                if (pic.Image != null)
                {
                    img = pic.Image;
                    img.Save(Common.PicPath + @"\NoMatch\" + DateTime.Now.ToString("yyyMMddHHmmss") + ".jpg", System.Drawing.Imaging.ImageFormat.Jpeg);
                }
                img.Dispose();
                pic.Dispose();
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public void AppendLog(string Msg)
        {
            try
            {
                //string strFile = Common.PicPath.Replace("ImgBase","GDILog") + DateTime.Now.ToString("yyyyMMddHHmmss") + "_log.txt";
                string strFile = Common.PicPath.Replace("ImgBase", "GDILog") + Common.strUn + "_log.txt";
                FileStream fs = new FileStream(strFile, FileMode.Append);
                StreamWriter sw = new StreamWriter(fs, Encoding.Default);
                sw.Write(Msg + "\r\n");
                sw.Close();
                fs.Close();
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public void Append(string ProcessID)
        {
            try
            {
                string strFile = Common.mq5173;
                FileStream fs = new FileStream(strFile, FileMode.Append);
                StreamWriter sw = new StreamWriter(fs, Encoding.Default);
                sw.Write(ProcessID + "\r\n");
                sw.Close();
                fs.Close();
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public void DelMQ5173(string ProcessID)
        {
            string str = null;
            try
            {
                using (System.IO.StreamReader sr = new System.IO.StreamReader(Common.mq5173))
                {
                    str = sr.ReadToEnd();
                    str = str.Replace(ProcessID + "\r\n", "");
                }
                string strFile = Common.mq5173;
                FileStream fs = new FileStream(strFile, FileMode.Truncate);
                StreamWriter sw = new StreamWriter(fs, Encoding.Default);
                sw.Write(str);
                sw.Close();
                fs.Close();
             
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public Boolean CurrentMQ5173(string ProcessID)
        {
            string str = null;
            try
            {
                using (System.IO.StreamReader sr = new System.IO.StreamReader(Common.mq5173))
                {
                    str = sr.ReadLine();
                }
                if (str == ProcessID)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }
    }
}
