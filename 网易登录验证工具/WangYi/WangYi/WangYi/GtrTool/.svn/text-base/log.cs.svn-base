using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;

namespace GTR
{
    class MyStr
    {
        //
        public static string GetCut(string strSource)
        {
            char[] anychar = new char[1];
            anychar[0]='\0';
            int len = strSource.IndexOfAny(anychar);
            if (len > 0)
            {
                return strSource.Substring(0,len);
            }
            return strSource;
        }
        public static string FindStr(string strSource, string strKeyBegin, string strKeyEnd)
        {
            int iStart = 0;
            if (strKeyBegin != "")
            {
                iStart = strSource.IndexOf(strKeyBegin);
                if (iStart == -1)  { return ""; }
            }
            strSource = strSource.Substring(iStart);
            int iEnd = strSource.Length;
            if (strKeyEnd != "")
            {
                iEnd = strSource.IndexOf(strKeyEnd);
                if (iEnd == -1) {return ""; }
            }
            iStart = strKeyBegin.Length;
            if (iEnd - iStart < 0) { return ""; }
            
            return strSource.Substring(iStart, iEnd - iStart);
        }
        
    }
    class FileRW
    {
        private static string _strPath = Application.ProductName + ".log";

        public static void InitLog()
        {
            using (File.Create(_strPath))
            { }
        }
        public static void WriteToFile(string strLog)
        {
            if (_strPath.Trim() == "")
            {
                return;
            }

            if (!File.Exists(_strPath))
                InitLog();
            StreamWriter sw=null;

            try
            {
                //sw = File.AppendText(_strPath);
                sw = new StreamWriter(_strPath, true, Encoding.Default);
                sw.WriteLine("{0}[{1}] ===> {2}", DateTime.Now.ToString("HH:mm:ss"), Application.ProductVersion, strLog);
            }
            catch (Exception)
            {
                return;
            }
            finally
            {
                if (sw != null)
                    sw.Close();
            }
        }
        public static void WriteToFile(string strLog, string strPath)
        {
            StreamWriter Asw = null;
            try 
            {
                Asw = new StreamWriter(strPath, false, Encoding.Default);
                Asw.Write(strLog);
            }
            catch (Exception e)
            {
                throw e;
                //return;
            }
            finally
            {
                if (Asw != null)
                    Asw.Close();
            }
        }
        public static void WriteError(Exception ex)
        {
            WriteToFile(ex.ToString());
        }
        public static void WriteError(string strex)
        {
            WriteToFile(strex);
        }
        public static string ReadFile(string strPathName)
        {
            StreamReader sr;
            try
            {
                //sr = File.OpenText(strPathName);
                sr = new StreamReader(strPathName, Encoding.Default);
                string tmp = sr.ReadToEnd();
                sr.Close();
                return tmp;
            }
            catch (Exception ex)
            {
                WriteError(ex);
            }
            
            return "";
         
        }
    }
}
