using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using mshtml;
using System.Windows.Forms;
using System.Drawing;

namespace GDI
{
    class PicGetSet
    {
        int UnMatchLeft;
        int UnMatchRigth;


        public Boolean EnumPic(string strPic, PictureBox pb1, PictureBox pb2)
        {
            Boolean isMatch = false;
            try
            {
                if (strPic.ToLower().Contains("sample")) { return false; }//样式图无视
                if (!strPic.ToLower().Contains(".jpg")) { return false; }
                pb1.Load(strPic);
                isMatch = ComparePic(pb1, pb2);
                return isMatch;
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }


        /// <summary>
        /// 像素比较
        /// </summary>
        /// <param name="pCenter"></param>
        /// <returns></returns>
        public Boolean ComparePic(PictureBox pb1, PictureBox pb2)
        {
            Boolean isMatch = true;//默认匹配
            Bitmap bmp1 = pb1.Image as Bitmap;
            Bitmap bmp2 = pb2.Image as Bitmap;
            int unMatchPixel = 0;
            int MatchCol = 0;
            try
            {
                //满足差异像素的像素点总数      
                int MatchPMin = 80;

                //为了防止缺失的图片正好在扫描列 所以扫描2次

                //扫描三列像素   有2列满足90个像素匹配就通过
                UnMatchRigth = 0;
                for (int j = 0; j < bmp1.Height; j++)
                {
                    Color CurrentCol1 = bmp1.GetPixel(10, j);
                    Color CurrentCol2 = bmp2.GetPixel(10, j);
                    if (MatchColor(CurrentCol1, CurrentCol2, 10))
                    {
                        unMatchPixel++;
                    }
                }
                if (unMatchPixel >= MatchPMin) MatchCol++;


                //扫描像素   x坐标50
                unMatchPixel = 0;
                for (int j = 0; j < bmp1.Height; j++)
                {
                    Color CurrentCol1 = bmp1.GetPixel(50, j);
                    Color CurrentCol2 = bmp2.GetPixel(50, j);
                    if (MatchColor(CurrentCol1, CurrentCol2, 10))
                    {
                        unMatchPixel++;
                    }
                }
                if (unMatchPixel >= MatchPMin) MatchCol++;

                //扫描像素   x坐标50
                unMatchPixel = 0;
                for (int j = 0; j < bmp1.Height; j++)
                {
                    Color CurrentCol1 = bmp1.GetPixel(90, j);
                    Color CurrentCol2 = bmp2.GetPixel(90, j);
                    if (MatchColor(CurrentCol1, CurrentCol2, 10))
                    {
                        unMatchPixel++;
                    }
                }
                if (unMatchPixel >= MatchPMin) MatchCol++;

                if (MatchCol >= 2) { isMatch = true; } else { isMatch = false; }
                return isMatch;

            }
            catch (Exception ex)
            {
                throw ex;
            }
        }


        /// <summary>
        /// 匹配像素点相似度
        /// </summary>
        /// <param name="c1"></param>
        /// <param name="c2"></param>
        /// <returns></returns>
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

        ///// <summary>
        ///// 匹配像素点相似度    更宽松
        ///// </summary>
        ///// <param name="c1"></param>
        ///// <param name="c2"></param>
        ///// <returns></returns>
        //public Boolean MatchColorV2(Color c1, Color c2, int OfferSet)
        //{
        //    Boolean isMatch = true;
        //    if (OfferSet < 0) OfferSet = 30;
        //    try
        //    {
        //        //像素点颜色的 Alpha 值
        //        byte alpha1 = c1.A;
        //        //颜色的 RED 分量值
        //        byte red1 = c1.R;
        //        //颜色的 GREEN 分量值
        //        byte green1 = c1.G;
        //        //颜色的 BLUE 分量值
        //        byte blue1 = c1.B;

        //        //像素点颜色的 Alpha 值
        //        byte alpha2 = c2.A;
        //        //颜色的 RED 分量值
        //        byte red2 = c2.R;
        //        //颜色的 GREEN 分量值
        //        byte green2 = c2.G;
        //        //颜色的 BLUE 分量值
        //        byte blue2 = c2.B;

        //        if ((Math.Abs(alpha1 - alpha2) + Math.Abs(red1 - red2) + Math.Abs(green1 - green2) + Math.Abs(blue1 - blue2)) > OfferSet)
        //        {
        //            isMatch = false;
        //        }
        //        return isMatch;

        //    }
        //    catch (Exception ex)
        //    {
        //        throw ex;
        //    }
        //}


        /// <summary>
        /// 计算有效差异区
        /// </summary>
        /// <param name="bmp"></param>
        /// <param name="x"></param>
        /// <returns></returns>
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
                    if (bmp.GetPixel(k, j) == Common.cDiffArea)
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
                    if (bmp.GetPixel(x, k) == Common.cDiffArea)
                    {
                        MatchP++;
                    }
                }

                return MatchP;
            }
            catch(Exception ex)
            {
                throw ex;
            }
        }

        public Bitmap calcPoint(Bitmap bmp1,Bitmap bmp2,out int pCenter, out int UnMatchLeft,out int UnMatchRigth)
        {
            //初始化
            UnMatchLeft = bmp2.Width;
            UnMatchRigth = 0;
            pCenter = 0;
            //满足差异像素的像素点总数
            int MatchPMin = 20;

            //遍历所有点

            for (int i = 0; i < bmp1.Width; i++)
            {
                for (int j = 0; j < bmp1.Height; j++)
                {
                    Color CurrentCol1 = bmp1.GetPixel(i, j);
                    Color CurrentCol2 = bmp2.GetPixel(i, j);

                    //如果是差异区域像素
                    if (bmp2.GetPixel(i, j) == Common.cDiffArea)
                    {
                        int MatchP = 0;

                        //if (i == 246)
                        //{
                        //    Console.WriteLine("调试点");
                        //}
                        //相近像素是不是差异区，是的话计数。行和列分别
                        MatchP = calcPixel(bmp2, i, j);

                        //for (int k = 0; k < bmp2.Height; k++)
                        //{
                        //    if (bmp2.GetPixel(i, k) == cDiffArea)
                        //    {
                        //        MatchP++;
                        //    }
                        //}
                        //如果满足一列大于MatchPMin个 那么赋值


                        if (MatchP >= MatchPMin)
                        {
                            if (UnMatchLeft > i)
                            {
                                UnMatchLeft = i;
                            }
                            if (UnMatchRigth < i)
                            {
                                UnMatchRigth = i;
                            }
                        }
                    }
                }
            }
            //输出结果
            pCenter = (UnMatchLeft + UnMatchRigth) / 2;
            //Console.WriteLine(pCenter);
            //txtPosition.Text = pCenter.ToString();
            //txt_Left.Text = UnMatchLeft.ToString();
            //txt_Right.Text = UnMatchRigth.ToString();

            //对图2画线
            for (int i = 0; i < bmp2.Height; i++)
            {
                bmp2.SetPixel(pCenter, i, Common.cCenterLine);
            }
            return bmp2;
            //pictureBox2.Image = Image.FromHbitmap(bmp2.GetHbitmap());
        
        }
    }
}
