using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Runtime.InteropServices;

namespace GDI
{
    class PrintScreen
    {
        //[StructLayout(LayoutKind.Sequential)]
        //private struct RECT { internal int Left; internal int Top; internal int Right; internal int Bottom;}
        //[DllImport("user32.dll")]
        //private static extern IntPtr GetWindowDC(IntPtr hwnd);
        //public static extern bool GetWindowRect(IntPtr hWnd, ref RECT rect);
        //[DllImport("gdi32.dll")]
        //static public extern IntPtr CreateCompatibleBitmap(IntPtr hDC, int Width, int Heigth);

        //public static Bitmap PrtWindow(IntPtr hWnd)
        //{

        //    IntPtr hscrdc = GetWindowDC(hWnd);

        //    RECT rect ;

        //    GetWindowRect(hWnd, ref rect);

        //    IntPtr hbitmap = CreateCompatibleBitmap(hscrdc, rect.Right - rect.Left, rect.Bottom - rect.Top);

        //    IntPtr hmemdc = CreateCompatibleDC(hscrdc);

        //    SelectObject(hmemdc, hbitmap);

        //    PrintWindow(hWnd, hmemdc, 0);

        //    Bitmap bmp = Bitmap.FromHbitmap(hbitmap);

        //    DeleteDC(hscrdc);

        //    DeleteDC(hmemdc);

        //    return bmp;

        //}
    }

}
