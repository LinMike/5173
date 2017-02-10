using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Text.RegularExpressions;

namespace CallDemo
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            ////string strDemo = "WBCheck_Project2.exe 001 790567436 1";
            //string strDemo = "WBCheck_Project2.exe";
            //Process p = new Process();
            //p.StartInfo.UseShellExecute = false;
            //p.StartInfo.RedirectStandardOutput = true;
            //p.StartInfo.FileName = strDemo;
            //p.StartInfo.CreateNoWindow = true;
            ////p.StartInfo.Arguments = "001 790567436 0";
            //p.StartInfo.Arguments = "001 1274222966 0";
            //p.Start();
            //p.WaitForExit();
            //string output = p.StandardOutput.ReadToEnd();
            //output = Regex.Match(output, "\\d{3}[/*]{6}\\d{2}").ToString();//匹配手机号格式的字符串
            //MessageBox.Show(output);//返回的结果
            //System.Environment.Exit(0);

            Base64 b64 = new Base64();
            //Console.WriteLine("编码: " + b64.EncodeBase64("用户名"));
            //Console.WriteLine("解码: " + b64.DecodeBase64(b64.EncodeBase64("用户名")));
            //return;

            string strDemo = "GDI.exe";
            Process p = new Process();
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.FileName = strDemo;
            p.StartInfo.CreateNoWindow = true;
            //p.StartInfo.Arguments = "用户名" + "@@" + "密码" + "@@" + "问题1" + "@@" + "问题2" + "@@" + "问题3";
            //p.StartInfo.Arguments = b64.EncodeBase64("yuezijunhehe@163.com") + " " + b64.EncodeBase64("he03295014") + " " + b64.EncodeBase64("") + " " + b64.EncodeBase64("") + " " + b64.EncodeBase64("");

            p.StartInfo.Arguments = b64.EncodeBase64("q120094545@163.com") + " " + b64.EncodeBase64("5201314qq..") + " ";

            p.StartInfo.Arguments += b64.EncodeBase64("") + " ";
            p.StartInfo.Arguments += b64.EncodeBase64("") + " ";
            p.StartInfo.Arguments += b64.EncodeBase64("") + " ";

            p.StartInfo.Arguments += b64.EncodeBase64("") + " ";
            p.StartInfo.Arguments += b64.EncodeBase64("") + " ";
            p.StartInfo.Arguments += b64.EncodeBase64("");

            p.Start();
            p.WaitForExit();
            string output = p.StandardOutput.ReadToEnd();

            //output = Regex.Match(output, "\\d{3}[/*]{6}\\d{2}").ToString();//匹配手机号格式的字符串
            MessageBox.Show(output);//返回的结果
            System.Environment.Exit(0);
        }
    }





    public sealed class Base64
    {
        public string EncodeBase64(string source)
        {
            if (!string.IsNullOrEmpty(source))
            {
                string a = source;
                byte[] b = System.Text.Encoding.Default.GetBytes(a);
                return Convert.ToBase64String(b);
            }
            return "";
        }

        public string DecodeBase64(string result)
        {
            if (!string.IsNullOrEmpty(result))
            {
                string a = result;
                byte[] c = Convert.FromBase64String(a);
                return System.Text.Encoding.Default.GetString(c);
            }
            return "";
        }
    }
}
