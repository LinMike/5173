using System;
using System.Collections.Generic;
using System.Windows.Forms;
using GTR;
namespace WangYi
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main(string[] arg)
        {
            try
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new Form1(arg));
            }
            catch (Exception ex)
            {
                FileRW.WriteToFile(ex.ToString());
            }
        }
    }
}