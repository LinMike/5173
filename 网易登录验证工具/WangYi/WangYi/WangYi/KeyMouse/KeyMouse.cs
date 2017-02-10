﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Windows.Forms;
using System.Threading;
using GamePic;
//using System.Net;
namespace GTR
{
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


    public class KeyMouse
    {
        public struct RECT
        {
            public int left;
            public int top;
            public int right;
            public int bottom;
        }
        [DllImport("user32.dll")]
        static extern bool SetCursorPos(int X, int Y);
        [DllImport("user32.dll")]
        static extern void mouse_event(MouseEventFlag flags, int dx, int dy, uint data, UIntPtr extraInfo);
        [DllImport("user32.dll", EntryPoint = "GetWindowRect")]
        public static extern bool GetWindowRect(IntPtr hWnd, out RECT lpRect);
        [DllImport("user32.dll", EntryPoint = "GetDesktopWindow", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern IntPtr GetDesktopWindow();



        public static void SendStringKeys(string strText, int delayTime)
        {
            string strCh = "";
            for (int i = 0; i < strText.Length; i++)
            {
                strCh += "{" + strText.Substring(i, 1) + "}";
            }
            SendKeys.SendWait(strCh);
            SendKeys.Flush();
            Thread.Sleep(delayTime);
        }

        public static void SendFunKey(char ch, bool ctrl, bool alt, bool shift, bool funKey)
        {
            String temp = "";
            temp = string.Format("^{0}", ch);
            if (ctrl) {  SendKeys.SendWait(temp);}
            if (alt) {  SendKeys.SendWait(temp); }
            if (shift) { SendKeys.SendWait(temp);}
            SendKeys.Flush(); 
            Sleep(20);
        }

        public static void SendTabKey()
        {
            SendKeys.SendWait("{TAB}");
            SendKeys.Flush();
        }

        public static void SendEnterKey()
        {
            SendKeys.SendWait("{ENTER}");
            SendKeys.Flush();
        }

        public static void SendEscKey()
        {
            SendKeys.SendWait("{ESC}"); 
            SendKeys.Flush();
        }
        public static void SendCtrlC()
        {
            SendKeys.SendWait("^c");
            SendKeys.Flush();
        }

        public static void SendCtrlV()
        {
            SendKeys.SendWait("^v");
            SendKeys.Flush();
        }

        public static void SendCtrlA()
        {
            SendKeys.SendWait("^a"); 
            SendKeys.Flush();
        }

        public static void SendBackSpaceKey(int times)
        {
            string temp = "";
            temp = string.Format("{{BACKSPACE {0}}}",times);
            SendKeys.SendWait(temp); 
            Sleep(10);
            SendKeys.Flush();
        }

        public static void SendDeleteKey(int times)
        {
            string temp = "";
            temp = string.Format("{{DELETE {0}}}", times);
            SendKeys.SendWait(temp);
            Sleep(10);
            SendKeys.Flush();
        }

        private static void Sleep(int time)
        {
            Thread.Sleep(time);
        }


        public static void MouseMove(IntPtr hWnd, Point pt)
        {
            if (User32API.IsWindow(hWnd))
            {
                RECT rect;
                GetWindowRect(hWnd, out rect);
                pt.X += rect.left;
                pt.Y += rect.top;
            }
            SetCursorPos(pt.X, pt.Y);
            Sleep(50);
        }

        public static void MouseMove(RECT rect,int x, int y)
        {
            x += rect.left;
            y += rect.top;
            SetCursorPos(x, y);
            Sleep(50);
        }
        public static void MouseMove(IntPtr hWnd, int x, int y)
        {
            if (User32API.IsWindow(hWnd))
            {
                RECT rect;
                GetWindowRect(hWnd, out rect);
                x += rect.left;
                y += rect.top;
                SetCursorPos(x, y);
                Sleep(50);
            }
        }
        public static void MouseClickMove(IntPtr hWnd, int x, int y, int ax, int ay)
        {
            if (User32API.IsWindow(hWnd))
            {
                RECT rect,rect1;
                GetWindowRect(hWnd, out rect);
                x += rect.left;
                y += rect.top;
               // SetCursorPos(x, y);
                GetWindowRect(GetDesktopWindow(), out rect1);
                MouseEventFlag MouseKeyDown = MouseEventFlag.LeftDown;
                MouseEventFlag MouseKeyUp = MouseEventFlag.LeftUp;
                MouseEventFlag MouseKeyMove = MouseEventFlag.Move;
                MouseEventFlag MouseKeyAbsolute = MouseEventFlag.Absolute;
                mouse_event(MouseKeyMove | MouseKeyAbsolute, (x) * 65535 / (rect1.right + rect1.left), y * 65535 / (rect1.bottom + rect1.top), 0, UIntPtr.Zero);
                Sleep(50);
                mouse_event(MouseKeyDown, 0, 0, 0, UIntPtr.Zero);//点击 
                Sleep(100);
                //for (int p = x; p < ax; p++)
                //{
                //    mouse_event(MouseKeyMove | MouseKeyAbsolute, (p) * 65535 / (rect1.right + rect1.left), y * 65535 / (rect1.bottom + rect1.top), 0, UIntPtr.Zero);
                //    Sleep(30);
                //}
                //Sleep(5000);
                //mouse_event(MouseKeyUp, 0, 0, 0, UIntPtr.Zero);//抬起
                //Sleep(100);
                //mouse_event(MouseKeyUp, 0, 0, 0, UIntPtr.Zero);//抬起
                //Sleep(100);



               //return;
                int last = ax - x;
                int ad = (ax - x) / 20;
                int tm = (ax - x) / 200;
                int bd = ad;
                int i = 0;
                while (last >= i)
                {
                    Random rm = new Random();
                    int n = rm.Next(4);
                    //FileRW.WriteToFile(n.ToString());
                    int md = 0;
                    if (n == 0)
                        md = 10;
                    if (n == 1)
                        md = 14;
                    if (n == 2)
                        md = 18;
                    if (n == 3)
                        md = 22;
                    mouse_event(MouseKeyMove | MouseKeyAbsolute, (x + i) * 65535 / (rect1.right + rect1.left), y * 65535 / (rect1.bottom + rect1.top), 0, UIntPtr.Zero);
                    Sleep(md * (bd - ad + 1) * (2 - tm));
                    ad = (ax - x - i) / 20;
                    i++;
                }
                
                //ScreenCapture.PrtWindowHWND(hWnd, @"e:\test\\",1);
                Sleep(5000);
                mouse_event(MouseKeyUp, 0, 0, 0, UIntPtr.Zero);//抬起
                Sleep(100);
                mouse_event(MouseKeyUp, 0, 0, 0, UIntPtr.Zero);//抬起
                Sleep(100);
                return;
            }
        }
       public static void MouseClick(IntPtr hWnd,int x, int y, uint tb, uint tc, int sleepTime)
        {
            MouseMove(hWnd, x, y);
            MouseEventFlag MouseKeyDown = MouseEventFlag.LeftDown;
            MouseEventFlag MouseKeyUp = MouseEventFlag.LeftUp;
            switch (tb)
            {
                case 1:
                    break;
                case 2:
                    MouseKeyDown = MouseEventFlag.RightDown;
                    MouseKeyUp = MouseEventFlag.RightUp;
                    break;
                default:
                    break;
            }
            switch (tc)
            {
                case 1:

                    mouse_event(MouseKeyDown, 0, 0, 0, UIntPtr.Zero);//点击 
                    Sleep(10);
                    mouse_event(MouseKeyUp, 0,0, 0, UIntPtr.Zero);//抬起
                    Sleep(sleepTime);
                    break;
                case 2:
                    mouse_event(MouseKeyDown, 0,0, 0, UIntPtr.Zero);//点击 
                    Sleep(10);
                    mouse_event(MouseKeyUp, 0,0, 0, UIntPtr.Zero);//抬起
                    Sleep(10);
                    mouse_event(MouseKeyDown, 0,0, 0, UIntPtr.Zero);//点击 
                    Sleep(10);
                    mouse_event(MouseKeyUp, 0,0, 0, UIntPtr.Zero);//抬起
                    Sleep(sleepTime);
                    break;
                default:
                    break;
            }
        }
        public enum TYPE_CLICK : uint
        {
            BUTTON_CLICK = 1,		//单击
            BUTTON_DOUBLE_CLICK,		//双击
            BUTTON_DOWN,				//按下
            BUTTON_UP					//弹起
        }
        public enum TYPE_BUTTON : uint
        {
            L_BUTTON = 1,			//左键
            R_BUTTON,					//右键
            M_BUTTON					//中键
        }
        
    }
    //public class WinApi
    //{
    //    //public static UInt32 VK_ESCAPE = 0x1B;
    //    //public static UInt32 VK_SHIFT = 0x10;
    //    //public static UInt32 VK_CONTROL = 0x11;
    //    //public static UInt32 VK_MENU = 0x12;
    //    //public static UInt32 VK_LEFT = 0x25;
    //    //public static UInt32 VK_UP = 0x26;
    //    //public static UInt32 VK_RIGHT = 0x27;
    //    //public static UInt32 VK_RETURN = 0x0D;
    //    //public static UInt32 VK_BACK = 0x08;
    //    //public static UInt32 VK_TAB = 0x09;
    //    //public static UInt32 VK_DOWN = 0x28;
    //    //public static UInt32 VK_PRINT = 0x2A;
    //    //public static UInt32 VK_DELETE = 0x2E;

    //    //[DllImport("Kernel32.dll", EntryPoint = "GetLastError", CallingConvention = CallingConvention.Winapi)]
    //    //public static extern UInt32 GetLastError();

    //    //[DllImport("user32.dll")]
    //    //public static extern bool GetCursorPos(out Point pt);

    //    //[DllImport("user32.dll", EntryPoint = "GetWindowRect")]
    //    //public static extern bool GetWindowRect(IntPtr hWnd, out RECT lpRect);

    //    public struct RECT
    //    {
    //        public int left;
    //        public int top;
    //        public int right;
    //        public int bottom;
    //    }
    //}
}
