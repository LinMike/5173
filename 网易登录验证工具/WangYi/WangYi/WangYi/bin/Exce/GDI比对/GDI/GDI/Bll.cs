using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace GDI
{
    class Bll
    {
        IO io = new IO();

        //删除消息队列
        public Boolean delMQ5173()
        {
            try
            {
                io.DelMQ5173(System.Diagnostics.Process.GetCurrentProcess().Id.ToString());
                return true;
            }
            catch (Exception ex)
            {
                return false;
            }
        }

        //查看队列消息，当前进程是不是队列第一条
        public Boolean CurrentMQ()
        {
            try
            {
                return io.CurrentMQ5173(System.Diagnostics.Process.GetCurrentProcess().Id.ToString());
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        

        //分析页面:帐号安全设置(http://reg.163.com/chg/ShowBaomiInfo.jsp)
        public string LoadUserMsg(string strHtml)
        {
            string bindInfo = "";
            /// 手机（0老帐号未绑定手机，1老帐号手机绑定，2新帐号手机绑定，3新帐号未绑定手机）
            /// 身份证（0未设置，1已设置）
            /// 安全码(老账号的生日和密码提示问题)/安全问题（0老帐号安全码未设置，1老帐号安全码已设置，2新帐号安全问题已设置，3新帐号安全问题未设置）
            /// 密保绑定（1将军令，2帐号通，3密保卡，0无密保）
            /// 密保邮箱
            /// QQ号码
            try
            {
                
                string Html = strHtml;
                string searchResutl = Bases.SearchTxt("<td>关联手机</td>((?:.|\n)*?)</tr>", Html);

                if (searchResutl.Contains("已设置") ||
                    searchResutl.Contains("修改</a>") ||
                    searchResutl.Contains("解除</a>") ||
                    searchResutl.Contains("验证</a>") ||
                    searchResutl.Contains("更换</a>"))
                {
                    bindInfo += "1"; //老帐号手机已设置
                }

                else if (searchResutl.Contains("未设置"))
                {
                    bindInfo += "0"; //老帐号手机未设置
                }

                else
                {
                    searchResutl = Bases.SearchTxt("<td>安全手机</td>((?:.|\n)*?)</tr>", Html);

                    if (searchResutl.Contains("已设置") ||
                        searchResutl.Contains("修改</a>") ||
                        searchResutl.Contains("解除</a>") ||
                        searchResutl.Contains("验证</a>") ||
                        searchResutl.Contains("更换</a>") ||
                        searchResutl.Contains("冻结期</a>"))
                    {
                        bindInfo += "2"; //新帐号手机已设置
                    }

                    else if (searchResutl.Contains("未设置"))
                    {
                        bindInfo += "3"; //新帐号手机未设置
                    }

                    else
                    {
                        bindInfo += "-"; //验证手机是否设置未知结果
                    }
                }

                searchResutl = Bases.SearchTxt("<td>身份信息</td>((?:.|\n)*?)</tr>", Html);

                if (searchResutl.Contains("已设置") || searchResutl.Contains("修改</a>") || searchResutl.Contains("解除</a>"))
                {
                    bindInfo += "1"; //("身份信息已设置", true);
                }

                else if (searchResutl.Contains("未设置"))
                {
                    bindInfo += "0"; //("身份信息未设置", false);
                }

                else
                {
                    bindInfo += "3"; //("验证身份信息是否设置未知结果", true);
                }

                searchResutl = Bases.SearchTxt("<td>安全码</td>((?:.|\n)*?)</tr>", Html);

                //if (searchResutl.Contains("已设置") || searchResutl.Contains("修改</a>") || searchResutl.Contains("解除</a>"))
                //{
                //    bindInfo += "1"; //("网易老帐号安全码已设置", false);
                //}

                //else if (searchResutl.Contains("未设置"))
                //{
                //    bindInfo += "0"; //("网易老帐号安全码未设置", false);
                //}

                if (searchResutl.Contains("已设置") || searchResutl.Contains("修改</a>") || searchResutl.Contains("解除</a>") || searchResutl.Contains("未设置"))
                {
                    //bindInfo += "1"; //("网易老帐号安全码已设置", false);
                    searchResutl = Bases.SearchTxt("<td>生日和密码提示问题</td>((?:.|\n)*?)</tr>", Html);
                    if (searchResutl.Contains("已设置") || searchResutl.Contains("修改</a>") || searchResutl.Contains("解除</a>"))
                    {
                        bindInfo += "1"; //("网易老帐号安全码已设置", false);
                    }

                    else if (searchResutl.Contains("未设置"))
                    {
                        bindInfo += "0"; //("网易老帐号安全码未设置", false);
                    }
                }

                else
                {
                    searchResutl = Bases.SearchTxt("<td>安全问题</td>((?:.|\n)*?)</tr>", Html);

                    if (searchResutl.Contains("已设置") || searchResutl.Contains("修改</a>") || searchResutl.Contains("解除</a>"))
                    {
                        bindInfo += "2"; //("网易新帐号安全问题已设置", false);
                    }

                    else if (searchResutl.Contains("未设置"))
                    {
                        bindInfo += "3"; //("网易新帐号安全问题未设置", false);
                    }

                    else
                    {
                        bindInfo += "-"; //("验证安全问题是否设置未知结果:" + searchResutl, true);
                    }
                }
                //将军令1，帐号通2，密保卡3，无0

                //新增逻辑  因为密保信息需要手机验证，所以有密保绑定的一律在函数前转人工了。这里能运行到 一定是没密保绑定的
                bindInfo += "0";

                //GoUrl = "http://reg.163.com/mibao/controller/goIndex.jsp";
                //Html = RequestPage(GoUrl, "GET", string.Empty, "utf-8", cookie).Replace("\r\n", "");

                //if (Html.Contains("您尚未启用密码保护"))
                //{
                //    bindInfo += "0"; //("密码保护状态:您尚未启用密码保护", false);
                //}

                //else if (Html.Contains("您正在使用将军令"))
                //{
                //    string JUrl = "http://reg.163.com/otp/controller/list.jsp";
                //    Html = RequestPage(JUrl, "GET", string.Empty, "utf-8", cookie);

                //    if (Bases.NotNull(Html))
                //    {
                //        object[] arrCount = Bases.SearchMatchsTxts("序列号：<span class=\"f_w_b\">", Html);

                //        //WriteMessage("密码保护状态:你有" + arrCount.Length + "张将军令正在使用中", true);
                //        if (arrCount.Length > 0)
                //        {
                //            bindInfo += "1";
                //        }
                //    }
                //}

                //else if (Html.Contains("您正在使用帐号通"))
                //{
                //    bindInfo += "2";
                //}

                //else
                //{
                //    string strCardType = Bases.SearchTxt("张(.*?)正在使用中", Html);
                //    string strNum = Bases.SearchTxt("<span style=\"color:red;font-size:16px;font-weight:bold; margin: 0 5px 0 5px;\">(.*?)</span>", Html);
                //    //WriteMessage("密码保护状态:你有" + strNum + "张" + strCardType + "正在使用中", true);

                //    if (strCardType.Contains("密保卡") || !string.IsNullOrEmpty(strNum))
                //    {
                //        bindInfo += "3";
                //    }

                //    else
                //    {
                //        bindInfo += "-";
                //    }
                //}

                //GoUrl = "https://reg.163.com/chg/ShowBaomiInfo.jsp";
                //是否绑定密保邮箱
                //Html = RequestPage(GoUrl, "GET", string.Empty, "utf-8", cookie);
                searchResutl = Bases.SearchTxt("<td>保密邮箱</td>((?:.|\n)*?)</tr>", Html);
                if (searchResutl.Contains("修改"))
                {
                    bindInfo += "4";
                }
                else
                    bindInfo += "5";
                searchResutl = Bases.SearchTxt("<td>QQ号码</td>((?:.|\n)*?)</tr>", Html);
                if (searchResutl.Contains("修改"))
                {
                    bindInfo += "1";
                }
                else
                    bindInfo += "0";

                //if (!bindInfo.Contains("-"))
                //{
                //    return "1";
                //}
                return bindInfo;
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        //各种其他判断标准
        public Boolean isPassRule(string strHtml,out string Errmsg)
        {
            Errmsg="";
            try
            {
                //网易纯数字手机邮箱不交易
                string strUserName = Common.strUn;
                Boolean boolNotAllNumber = false;
                for (int i = 0; i < strUserName.Length; i++)
                {
                    if (!Char.IsNumber(strUserName, i))
                    {
                        boolNotAllNumber = true;
                        break;
                    }
                }
                if (!boolNotAllNumber)
                {
                    //AutoError(p, "纯数字账号不能交易");
                    Errmsg = "纯数字账号不能交易";
                    return false;
                }
                #region 新增的逻辑判断 调试时屏蔽
                //发现：您已经绑定了密保，查看详情
                //操作：由于无法查询到绑定了什么，统一转话务，请解除密保卡或将军令
                if (strHtml.Contains("您已经绑定了密保"))
                {
                    Errmsg = "您已经绑定了密保";
                    return false;
                }
                //isPassRule2包含以下规则验证
                ////核对是否有昵称帐号，不准用昵称帐号发布
                //Boolean isMainAccount = true;
                //Boolean isNickName = CheckIsNickName(p.CookieContainer, p.TOrder.SellerInfo, out isMainAccount);
                //if (MiBaoresult) { AutoError(p, "不准用昵称帐号发布"); }
                ////账号是可删除的登录邮件地址 那么取消发布
                //if (!isMainAccount) { AutoError(p, "登录账号是可删除的 不允许发布"); }
                #endregion
                return true;
            }
            catch (Exception ex)
            {
                return false;
            }
        }

        //各种其他判断标准
        public Boolean isPassRule2(string strHtml, out string Errmsg)
        {
            Errmsg = "";
            try
            {
                //核对是否有昵称帐号，不准用昵称帐号发布
                Boolean MiBaoresult = true;
                Boolean isMainAccount = true;
                Boolean isNickName = CheckIsNickName(strHtml, Common.strUn, out isMainAccount);
                if (isNickName)
                {
                    Errmsg = "不准用昵称帐号发布";
                    return false;
                }
                //账号是可删除的登录邮件地址 那么取消发布
                if (!isMainAccount)
                {
                    Errmsg = "登录账号是可删除的 不允许发布";
                    return false;
                }
                return true;
            }
            catch (Exception ex)
            {
                return false;
            }
        }


        /// <summary>
        /// 是否用昵称发布
        /// </summary>
        /// <param name="cookie"></param>
        /// <returns>1是绑定了 -是没绑定</returns>
        public Boolean CheckIsNickName(string strHTML, string strUserName, out Boolean isMainAccount)
        {
            Boolean result = false;
            isMainAccount = false;
            //判断昵称是否匹配
            string html = strHTML;
            int i = html.IndexOf("<li class=\"clear\">昵称：") + "<li class=\"clear\">昵称：".Length;
            int j = html.IndexOf("</li>", i);
            string strNickName = html.Substring(i, j - i).Trim();
            if (strNickName == strUserName)
            {
                result = true;
            }

            //判断是否是可删除的账号
            i = html.IndexOf("<a href=\"javascript:void(0)\" class=\"underline\" id=\"del_btn\">[删除]</a>");
            j = html.IndexOf("<!--弹出框 开始-->");
            if (i > 0 && j > 0)
            {
                i = html.IndexOf("<div class=\"f_left\">登录邮件地址：</div>");
                i = html.IndexOf("<b>") + 3;
                j = html.IndexOf("</b>", i);
                string strMainAccount = html.Substring(i, j - i);
                if (strMainAccount == strUserName)
                {
                    isMainAccount = true;
                }
            }
            else
            {
                isMainAccount = true;
            }
            return result;
        }

        //判断ShowBaomiInfo.jsp页显示身份信息未设置的 是否已经实名
        public Boolean isRealName_Authentication(string strHtml, out string Errmsg)
        {
            Errmsg = "";
            try
            {
                #region 新增的逻辑判断
                if (strHtml.Contains("您的资料通过了国家相关部门的实名审核，您的帐号未被纳入防沉迷系统。") || strHtml.Contains("您的实名认证申请尚在处理阶段"))
                {
                    Errmsg = "您已经实名认证了";
                    return true;
                }
                else
                {
                    Errmsg = "无法确认实名认证";
                    return false;
                }
                #endregion
            }
            catch (Exception ex)
            {
                return false;
            }
        }

        /// <summary>
        /// 根据图片名设置填充色
        /// </summary>
        /// <param name="strName"></param>
        public void chooseCorlr(string strName)
        {
            try
            {
                if (strName.ToLower().Contains("_red"))
                {
                    Common.cDiffArea = Common.cRed;
                }
                else if (strName.ToLower().Contains("_black"))
                {
                    Common.cDiffArea = Common.cBlack;
                }
                else if (strName.ToLower().Contains("_golden"))
                {
                    Common.cDiffArea = Common.cGolden;
                }
                else if (strName.ToLower().Contains("_green"))
                {
                    Common.cDiffArea = Common.cGreen;
                }
                else
                {
                    Common.cDiffArea = Common.cWhite;
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public void calcSafeQuestionSort(string strDocumentText)
        {
            try
            {
                int iQ1 = strDocumentText.IndexOf(Common.strQ1);
                int iQ2 = strDocumentText.IndexOf(Common.strQ2);
                int iQ3 = strDocumentText.IndexOf(Common.strQ3);

                int intTemp = 0;
                if (iQ3 < iQ1)
                {
                    ChangeDate(3,1);
                    intTemp = iQ1;
                    iQ1 = iQ3;
                    iQ3 = intTemp;
                }

                if (iQ2 < iQ1)
                {
                    ChangeDate(2, 1);
                    intTemp = iQ1;
                    iQ1 = iQ2;
                    iQ2 = intTemp;
                }

                if (iQ3 < iQ2)
                {
                    ChangeDate(3, 2);
                    intTemp = iQ2;
                    iQ2 = iQ3;
                    iQ3 = intTemp;
                }
            }
            catch (Exception ex)
            { 
            throw ex;
            }
        }

        public string[] calcSafeTxtBoxSort(string strDocumentText)
        {
            int iQ1 = strDocumentText.IndexOf("<input id=\"answer1");
            int iQ2 = strDocumentText.IndexOf("<input id=\"answer2");
            int iQ3 = strDocumentText.IndexOf("<input id=\"answer3");

            string[] strArr = new string[] { "answer1", "answer2", "answer3" };
            string strTemp=string.Empty;
            int intTemp = 0;

            if (iQ3 < iQ1)
            {
                strTemp = strArr[0];
                strArr[0] = strArr[2];
                strArr[2] = strTemp;

                intTemp = iQ1;
                iQ1 = iQ3;
                iQ3 = intTemp;
            }

            if (iQ2 < iQ1)
            {
                strTemp = strArr[0];
                strArr[0] = strArr[1];
                strArr[1] = strTemp;

                intTemp = iQ1;
                iQ1 = iQ2;
                iQ2 = intTemp;
            }

            if (iQ3 < iQ2)
            {
                strTemp = strArr[1];
                strArr[1] = strArr[2];
                strArr[2] = strTemp;

                intTemp = iQ2;
                iQ2 = iQ3;
                iQ3 = intTemp;
            }

            return strArr;
        }

        private void ChangeDate(int para1, int para2)
        {
            string firstQ = string.Empty;
            string firstA = string.Empty;
            string SecondQ = string.Empty;
            string SecondA = string.Empty;
            string strTemp = string.Empty;

            switch (para1)
            {
                case 1:
                    firstQ = Common.strQ1;
                    firstA = Common.strA1;
                    break;
                case 2:
                    firstQ = Common.strQ2;
                    firstA = Common.strA2;
                    break;
                case 3:
                    firstQ = Common.strQ3;
                    firstA = Common.strA3;
                    break;
            }

            switch (para2)
            {
                case 1:
                    SecondQ = Common.strQ1;
                    SecondA = Common.strA1;
                    break;
                case 2:
                    SecondQ = Common.strQ2;
                    SecondA = Common.strA2;
                    break;
                case 3:
                    SecondQ = Common.strQ3;
                    SecondA = Common.strA3;
                    break;
            }

            strTemp = firstQ;
            firstQ = SecondQ;
            SecondQ = strTemp;

            strTemp = firstA;
            firstA = SecondA;
            SecondA = strTemp;

            switch (para1)
            {
                case 1:
                    Common.strQ1 = firstQ;
                    Common.strA1 = firstA;
                    break;
                case 2:
                    Common.strQ2 = firstQ;
                    Common.strA2 = firstA;
                    break;
                case 3:
                    Common.strQ3 = firstQ;
                    Common.strA3 = firstA;
                    break;
            }

            switch (para2)
            {
                case 1:
                    Common.strQ1 = SecondQ;
                    Common.strA1 = SecondA;
                    break;
                case 2:
                    Common.strQ2 = SecondQ;
                    Common.strA2 = SecondA;
                    break;
                case 3:
                    Common.strQ3 = SecondQ;
                    Common.strA3 = SecondA;
                    break;
            }
        }


        //确定验证码图片位置 第一次出现的是小图 第二次才是大图  
        //返回 https://nisp-captcha.nosdn.127.net/1470911262124_1399738163这样的位置
        //返回 https://nos.netease.com/nisp-captcha/
        public string CheckPic(string strHtml)
        {
            string strPic =string.Empty;
            string strMark1 = "nos.netease.com/nisp-captcha/";
                string strMark2 = "width: 350px";
            try
            {
                int i = strHtml.IndexOf(strMark1);
                int j = strHtml.LastIndexOf("<",i);
                int k = strHtml.IndexOf(">",i);
                strPic = strHtml.Substring(j,k-j);

                if(!strPic.Contains(strMark2))//如果是小图 那么重新匹配
                {
                      i = strHtml.IndexOf(strMark1,i+1);
                     j = strHtml.LastIndexOf("<",i);
                     k = strHtml.IndexOf(">",i);
                    strPic = strHtml.Substring(j,k-j);
                }

                //锁定了位置 取字符串

                i = strHtml.LastIndexOf("http",i);
                j = strHtml.IndexOf("\"",i);
                strPic = strHtml.Substring(i, j-i);
                return strPic;
            }
            catch(Exception ex)
            {
                throw ex;
            }
        }

        public string ValidateBindInfo(string strHtml)
        {
            try
            {
                string strUserName = Common.strUn;
                Boolean boolNotAllNumber = false;
                for (int i = 0; i < strUserName.Length; i++)
                {
                    if (!Char.IsNumber(strUserName, i))
                    {
                        boolNotAllNumber = true;
                        break;
                    }
                }
                if (!boolNotAllNumber)
                {
                    return "纯数字账号不能交易";
                }
                #region 新增的逻辑判断 调试时屏蔽
                //发现：您已经绑定了密保，查看详情
                //操作：由于无法查询到绑定了什么，统一转话务，请解除密保卡或将军令
                if (!Common.isDebug)
                {
                    string MiBaoresult = CheckHasMibao(strHtml);
                    if (!string.IsNullOrEmpty(MiBaoresult)) { return MiBaoresult; }
                }
                //核对是否有昵称帐号，不准用昵称帐号发布
                //Boolean isMainAccount = true;
                //Boolean isNickName = p.WangYiMethod.CheckIsNickName(p.CookieContainer, p.TOrder.SellerInfo, out isMainAccount);
                //if (MiBaoresult) { AutoError(p, "不准用昵称帐号发布"); }
                //账号是可删除的登录邮件地址 那么取消发布
                //if (!isMainAccount) { AutoError(p, "登录账号是可删除的 不允许发布"); }
                return "";
                #endregion
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }


        /// <summary>
        /// 是否有绑定密保
        /// </summary>
        /// <param name="cookie"></param>
        /// <returns>1是绑定了 -是没绑定</returns>
        public string CheckHasMibao(string strHtml)
        {
            if (strHtml.Contains("您已经绑定了密保"))
            {
                return "您已经绑定了密保";
            }
            return "";
        }

    }
}
