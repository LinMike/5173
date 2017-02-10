using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Data.SqlClient;
using System.Data;

namespace GDI
{
    class Common
    {
        //剩余的开发问题
        //鼠标拉验证码次数多了会错误(拉动的过程中 滑块和鼠标的对应位置会 和初始时候不一样  决绝办法 拖动使用MouseMove的API)
        //服务器上 在登录成功页面转 不加载完成 需要刷新下的功能(网速慢的时候会出现)
        //页面长时间停止的监控(MouseUp消息被吃掉引起的 很少出现 未解决)

        public static string PicPath = AppDomain.CurrentDomain.BaseDirectory + "\\ImgBase\\";
   

        public static Color cDiffArea = Color.FromArgb(255, 255, 255);   //差异区域填充色  由下列几个默认色填充  根据图片名
        public static Color cCenterLine = Color.FromArgb(0, 0, 0);   //竖线
        //以下为备用填充色
        public static Color cWhite = Color.FromArgb(255, 255, 255);   //差异区域填充色
        public static Color cBlack = Color.FromArgb(0, 0, 0);   //差异区域中点
        public static Color cGolden = Color.FromArgb(167, 126, 67);   //金色
        public static Color cRed = Color.FromArgb(255, 0, 0);   //红色
        public static Color cGreen = Color.FromArgb(0, 255, 0);   //绿色

        //状态参数
        public static DateTime dtStart = new DateTime();
        public static string mq5173 = @"D:\自动审核\MQ5173.txt";
        public static string Page1 = @"http://reg.163.com/";
        public static string Page2 = "http://reg.163.com/mibao/mob/replaceByPhone_1.do?noheader=0&url=";//安全问题验证页面
        public static string Page3 = "http://reg.163.com/account/accountInfo.jsp";//基本信息页
        public static Boolean VeryCodePass = false;//安全问题验证成功时候设置True  那么再次跳到安全问题页面但是验证成功  那么说明安全问题错误
        public static Boolean MQ5173Status = false;//是否队列中等待的进程已经轮到自己启动
        public static Boolean DocumentStatus = false;//登陆页面加载是否完成的状态
        public static Boolean YZMCheckWorkFlowComplete = false;//验证码步骤完成标记(需要确认验证码状态 执行后续的 触发登陆按钮操作 触发后改回Flase状态)
        public static Boolean LoginFaild = false;//第一次账号登录失败时设置True     等登陆页加载的时候判断这个值为True的话就重新识别并登录
        public static Boolean isNeedReset = false;//是否立即中断登陆过程重新开始
        public static int intOption;//Timer做操作时候使用的操作类型
        public static Boolean isDebug = false;//无参数时设置调试模式
        public static string strQuestionHtml;
        //配置参数
        public static int PubOffset = 500;//总延时设置
        public static int PubMaxSeconds = 60;//超时关闭的最长秒数
        public static int PubMaxVeryErrNumber = 5;//最多三次登陆验证码错误就退出
        public static int PubVeryErrRetryTimes = 0;//尝试的登陆验证码错误次数
        //输入参数
        //public static string strUn = "fan909947@qq.com";
        //public static string strPwd = "a123s456d789";
        public static string strUn = "nishimengxin@163.com";
        public static string strPwd = "513521482.";
        //public static string strUn = "podvcd2@163.com";            //登陆确认点击测试账号
        //public static string strPwd = "11141114vcd";
        //public static string strUn = "gaolongfc1216@163.com";     //登陆确认点击测试账号
        //public static string strPwd = "778899";
        //public static string strUn = "fan917734@163.com";
        //public static string strPwd = "a123s456d789";
        public static string strOrderID = "DB20160321201100047";

        public static string strQ2 = "您父亲的生日是？";//问题顺序会变 要判断哦
        public static string strA2 = "19501105";
        public static string strQ3 = "您母亲的生日是？";
        public static string strA3 = "19501225";
        public static string strQ1 = "您配偶的生日是？";
        public static string strA1 = "19801205";
        //输出参数
        public static string StrBindInfo = "";

        //2个问题拼起来一起返回
        public static string strStatus = "";// 100100这样子
        public static string isQuestionCurrent = "0";//密保问题
    }

    /// <summary>
    /// 暂时没使用
    /// </summary>
    public class DB
    {
        public void buildSQL(string Msg)
        {
            int i = Msg.IndexOf("返回内容:") + "返回内容:".Length;
            Msg = Msg.Substring(i).Replace("\r\n", "");
            Boolean boolNotAllNumber = false;
            string output = Msg;
            for (int stri = 0; stri < output.Length; stri++)
            {
                if (!Char.IsNumber(output, stri))
                {
                    boolNotAllNumber = true;
                    break;
                }
            }
            if (!boolNotAllNumber)
            {
                string strStep2;
                string strStep5;
                strStep2 = output.Substring(0, 6);
                strStep5 = "3";
                if (output.Substring(2, 1) == "3")
                {
                    strStep5 = "2";
                }
                if (output.Length >= 7)
                {
                    strStep5 = output.Substring(6, 1);
                }
                string strSQL = "UPDATE T_AuditResult_WangYiTrade SET step2 = " + strStep2 + ",step5=" + strStep5 + ",EmailAddress = 'CPU@1.com' WHERE OrderId = '" + Common.strOrderID + "'";
                execString("", strSQL);
            }
        }


        public string execString(string strConnString, string strSQL)
        {
            try
            {
                string strReturnValue = "";
                SqlConnection con = new SqlConnection();
                if (string.IsNullOrEmpty(strConnString))
                {
                    //strConnString = "server=192.168.206.218;Database=AutoAudit;User Id=AutoAudit;Password=zzjy@5173.com";
                    strConnString = "server=192.168.130.26;Database=AutoAudit;User Id=autotest;Password=123456;";
                }


                con.ConnectionString = strConnString;
                con.Open();
                SqlCommand com = new SqlCommand();
                com.Connection = con;
                com.CommandType = CommandType.Text;
                com.CommandText = strSQL;

                object obj = com.ExecuteScalar();
                if (obj != null)
                    strReturnValue = obj.ToString();
                else
                    strReturnValue = "";
                com.Dispose();
                con.Close();
                con.Dispose();
                return strReturnValue;
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }
    }
    //JS脚本

//    function getElementsByClassName(n) { 
//var classElements = [],allElements = document.getElementsByTagName('*'); 
//for (var i=0; i< allElements.length; i++ ) 
//{ 
//if (allElements[i].className == n ) { 
//        classElements[classElements.length] = allElements[i];
//  return allElements[i];;
//}
//} 
//return classElements; 
//}

//function getElementsBySrc(n) { 
//var classElements = [],allElements = document.getElementsByTagName('*'); 
//for (var i=0; i< allElements.length; i++ ) 
//{ 
//if (allElements[i].src == n ) { 
//        classElements[classElements.length] = allElements[i];
//  return allElements[i];;
//}
//} 
//return classElements; 
//}

//function _click(obj){
//   var key=$(obj).attr("key");
//      if(key==2){
//          two_click();
//      }else{
//          one_click();
//      }
//      $(obj).attr("key",(key==2?"1":"2"));
//      function one_click(){
//        alert(1);
//      }
//      function two_click(){
//       alert(2);
//      }
//}

//function mouseoutHandler(oEvent,objTagName)
//        {
//        var oEvent = oEvent ? oEvent : window.event 
//        var oElem = oEvent.toElement ? oEvent.toElement : oEvent.relatedTarget; // 此做法是为了兼容FF浏览器
//        alert(oElem.id);  // 当鼠标移出Div1时，会弹出提示框提示鼠标移动到的另一个元素的ID
//        }

        
        //测试模拟划动的脚本  还没能实现
        //var imgDiv = getElementsByClassName('ncpt_slide_fg ncpt_anim_left');	//小图所在层
        ////指定层的zindex值设置
        //var zindex  = getElementsByClassName('ckbox m-ckcnt slidebox f-cb');	//设置大层的Z轴
        //zindex.style.zIndex = '501';
        ////设置验证码大图层属性
        //var yzmPic  = getElementsByClassName('ncpt_puzzle_bg');								//验证码大图对象
        //yzmPic.style.display = 'block';
        ////yzmPic.style.display = 'none';
        ////设置验证码小图的位置
        //var yzmPicSmall  = getElementsByClassName('ncpt_puzzle_fg');					//延验证码小图对象
        //yzmPicSmall.style.left='200px'
        ////滑块的Css修改
        ////imgDiv.mousedown();
        //imgDiv.className = "ncpt_slide_fg";
        //imgDiv.style.left='200px';
        ////imgDiv.click();
        ////_click(this);
        ////获取滑块图片对象 
        //var imgObj = getElementsBySrc('https://captcha.reg.163.com/v2/images/slider.png');	//滑块图片对象

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////测试区
        //////////////////////////////////////////////////////////////////////////////////////////////
        //imgDiv.click(function(){
        //    alert('绑定点击事件');
        //});

        ////imgDiv.click;
        ////imgObj.tigger();
        ////
        ////var alertObj = getEventListeners(imgDiv);
        //alert(imgDiv);

        //填写用户名密码和提交按钮的JS
        ////var eByName = document.getElementsByName('email');
        ////eByName[0].value = 'fan909947@163.com1';
        ////var eByName = document.getElementsByName('password');
        ////eByName[0].value = 'a123s456d789';
        ////document.getElementById('dologin').click();
        ////初始的Css : ncpt_slide_fg ncpt_anim_left
        ////拉动成功后的Css : ncpt_slide_fg
}
