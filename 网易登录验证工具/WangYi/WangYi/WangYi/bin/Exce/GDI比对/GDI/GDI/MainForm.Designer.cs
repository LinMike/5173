namespace GDI
{
    partial class MainForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.btn_LoadPic = new System.Windows.Forms.Button();
            this.Btn_Match = new System.Windows.Forms.Button();
            this.btn_Calc = new System.Windows.Forms.Button();
            this.wbMain = new System.Windows.Forms.WebBrowser();
            this.btn_LoadPicfromWB = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.txtPosition = new System.Windows.Forms.TextBox();
            this.btn_setMouse = new System.Windows.Forms.Button();
            this.btn_Search = new System.Windows.Forms.Button();
            this.btn_Start = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.txt_Left = new System.Windows.Forms.TextBox();
            this.txt_Right = new System.Windows.Forms.TextBox();
            this.Btn_Show = new System.Windows.Forms.Button();
            this.txt_Range = new System.Windows.Forms.TextBox();
            this.Btn_TestRange = new System.Windows.Forms.Button();
            this.lbl_PicName1 = new System.Windows.Forms.Label();
            this.button3 = new System.Windows.Forms.Button();
            this.Btn_TestRange2 = new System.Windows.Forms.Button();
            this.lbl_Msg = new System.Windows.Forms.Label();
            this.txtPosition350 = new System.Windows.Forms.TextBox();
            this.Btn_Login = new System.Windows.Forms.Button();
            this.timerAutoClose = new System.Windows.Forms.Timer(this.components);
            this.lbl_AppMsg = new System.Windows.Forms.Label();
            this.timerLoginWorkFlow = new System.Windows.Forms.Timer(this.components);
            this.btn_Exit = new System.Windows.Forms.Button();
            this.pictureBox_Bak = new System.Windows.Forms.PictureBox();
            this.btn_Temp = new System.Windows.Forms.Button();
            this.timerOption = new System.Windows.Forms.Timer(this.components);
            this.txtPosition226 = new System.Windows.Forms.TextBox();
            this.lblPosition350 = new System.Windows.Forms.Label();
            this.lblPosition226 = new System.Windows.Forms.Label();
            this.timerSafeQuestionWorkFlow = new System.Windows.Forms.Timer(this.components);
            this.lbl_isDebug = new System.Windows.Forms.Label();
            this.txt_OptionLog = new System.Windows.Forms.TextBox();
            this.lbl_AutoCloseMsg = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.lbl_Clipboard = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_Bak)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(12, 12);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(378, 105);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Location = new System.Drawing.Point(12, 133);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(378, 105);
            this.pictureBox2.TabIndex = 1;
            this.pictureBox2.TabStop = false;
            // 
            // btn_LoadPic
            // 
            this.btn_LoadPic.Location = new System.Drawing.Point(396, 21);
            this.btn_LoadPic.Name = "btn_LoadPic";
            this.btn_LoadPic.Size = new System.Drawing.Size(75, 23);
            this.btn_LoadPic.TabIndex = 2;
            this.btn_LoadPic.Text = "加载图片";
            this.btn_LoadPic.UseVisualStyleBackColor = true;
            this.btn_LoadPic.Click += new System.EventHandler(this.btn_LoadPic_Click);
            // 
            // Btn_Match
            // 
            this.Btn_Match.Location = new System.Drawing.Point(14, 391);
            this.Btn_Match.Name = "Btn_Match";
            this.Btn_Match.Size = new System.Drawing.Size(75, 23);
            this.Btn_Match.TabIndex = 3;
            this.Btn_Match.Text = "3填充差异";
            this.Btn_Match.UseVisualStyleBackColor = true;
            this.Btn_Match.Click += new System.EventHandler(this.Btn_Match_Click);
            // 
            // btn_Calc
            // 
            this.btn_Calc.Location = new System.Drawing.Point(16, 420);
            this.btn_Calc.Name = "btn_Calc";
            this.btn_Calc.Size = new System.Drawing.Size(73, 23);
            this.btn_Calc.TabIndex = 4;
            this.btn_Calc.Text = "4计算差异中心";
            this.btn_Calc.UseVisualStyleBackColor = true;
            this.btn_Calc.Click += new System.EventHandler(this.btn_Calc_Click);
            // 
            // wbMain
            // 
            this.wbMain.Location = new System.Drawing.Point(396, 50);
            this.wbMain.MinimumSize = new System.Drawing.Size(20, 20);
            this.wbMain.Name = "wbMain";
            this.wbMain.Size = new System.Drawing.Size(1188, 670);
            this.wbMain.TabIndex = 5;
            this.wbMain.DocumentCompleted += new System.Windows.Forms.WebBrowserDocumentCompletedEventHandler(this.wbMain_DocumentCompleted);
            // 
            // btn_LoadPicfromWB
            // 
            this.btn_LoadPicfromWB.Location = new System.Drawing.Point(12, 333);
            this.btn_LoadPicfromWB.Name = "btn_LoadPicfromWB";
            this.btn_LoadPicfromWB.Size = new System.Drawing.Size(75, 23);
            this.btn_LoadPicfromWB.TabIndex = 6;
            this.btn_LoadPicfromWB.Text = "1加载图片";
            this.btn_LoadPicfromWB.UseVisualStyleBackColor = true;
            this.btn_LoadPicfromWB.Click += new System.EventHandler(this.btn_LoadPicfromWB_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 264);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(125, 12);
            this.label1.TabIndex = 7;
            this.label1.Text = "暂时支出绿色车和浴室";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 285);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 12);
            this.label2.TabIndex = 8;
            this.label2.Text = "最终坐标";
            // 
            // txtPosition
            // 
            this.txtPosition.Location = new System.Drawing.Point(71, 279);
            this.txtPosition.Name = "txtPosition";
            this.txtPosition.Size = new System.Drawing.Size(31, 21);
            this.txtPosition.TabIndex = 9;
            this.txtPosition.Text = "0";
            // 
            // btn_setMouse
            // 
            this.btn_setMouse.Location = new System.Drawing.Point(14, 478);
            this.btn_setMouse.Name = "btn_setMouse";
            this.btn_setMouse.Size = new System.Drawing.Size(75, 23);
            this.btn_setMouse.TabIndex = 10;
            this.btn_setMouse.Text = "6设置鼠标";
            this.btn_setMouse.UseVisualStyleBackColor = true;
            this.btn_setMouse.Click += new System.EventHandler(this.btn_setMouse_Click);
            // 
            // btn_Search
            // 
            this.btn_Search.Location = new System.Drawing.Point(14, 362);
            this.btn_Search.Name = "btn_Search";
            this.btn_Search.Size = new System.Drawing.Size(75, 23);
            this.btn_Search.TabIndex = 11;
            this.btn_Search.Text = "2搜索原图";
            this.btn_Search.UseVisualStyleBackColor = true;
            this.btn_Search.Click += new System.EventHandler(this.btn_Search_Click);
            // 
            // btn_Start
            // 
            this.btn_Start.Location = new System.Drawing.Point(14, 536);
            this.btn_Start.Name = "btn_Start";
            this.btn_Start.Size = new System.Drawing.Size(75, 23);
            this.btn_Start.TabIndex = 12;
            this.btn_Start.Text = "8登陆结果";
            this.btn_Start.UseVisualStyleBackColor = true;
            this.btn_Start.Click += new System.EventHandler(this.btn_Start_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(16, 449);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(71, 23);
            this.button1.TabIndex = 13;
            this.button1.Text = "5账号密码";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(14, 697);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(104, 23);
            this.button2.TabIndex = 14;
            this.button2.Text = "收集到图库窗体";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // txt_Left
            // 
            this.txt_Left.Location = new System.Drawing.Point(108, 279);
            this.txt_Left.Name = "txt_Left";
            this.txt_Left.Size = new System.Drawing.Size(31, 21);
            this.txt_Left.TabIndex = 21;
            this.txt_Left.Text = "0";
            // 
            // txt_Right
            // 
            this.txt_Right.Location = new System.Drawing.Point(145, 279);
            this.txt_Right.Name = "txt_Right";
            this.txt_Right.Size = new System.Drawing.Size(31, 21);
            this.txt_Right.TabIndex = 22;
            this.txt_Right.Text = "0";
            // 
            // Btn_Show
            // 
            this.Btn_Show.Location = new System.Drawing.Point(182, 277);
            this.Btn_Show.Name = "Btn_Show";
            this.Btn_Show.Size = new System.Drawing.Size(47, 23);
            this.Btn_Show.TabIndex = 23;
            this.Btn_Show.Text = "show";
            this.Btn_Show.UseVisualStyleBackColor = true;
            this.Btn_Show.Click += new System.EventHandler(this.Btn_Show_Click);
            // 
            // txt_Range
            // 
            this.txt_Range.Location = new System.Drawing.Point(235, 279);
            this.txt_Range.Name = "txt_Range";
            this.txt_Range.Size = new System.Drawing.Size(36, 21);
            this.txt_Range.TabIndex = 24;
            this.txt_Range.Text = "0";
            // 
            // Btn_TestRange
            // 
            this.Btn_TestRange.Location = new System.Drawing.Point(277, 277);
            this.Btn_TestRange.Name = "Btn_TestRange";
            this.Btn_TestRange.Size = new System.Drawing.Size(46, 23);
            this.Btn_TestRange.TabIndex = 25;
            this.Btn_TestRange.Text = "校正";
            this.Btn_TestRange.UseVisualStyleBackColor = true;
            this.Btn_TestRange.Click += new System.EventHandler(this.Btn_TestRange_Click);
            // 
            // lbl_PicName1
            // 
            this.lbl_PicName1.AutoSize = true;
            this.lbl_PicName1.Location = new System.Drawing.Point(14, 118);
            this.lbl_PicName1.Name = "lbl_PicName1";
            this.lbl_PicName1.Size = new System.Drawing.Size(47, 12);
            this.lbl_PicName1.TabIndex = 26;
            this.lbl_PicName1.Text = "文件名:";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(477, 21);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 27;
            this.button3.Text = "保存图片";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // Btn_TestRange2
            // 
            this.Btn_TestRange2.Location = new System.Drawing.Point(329, 277);
            this.Btn_TestRange2.Name = "Btn_TestRange2";
            this.Btn_TestRange2.Size = new System.Drawing.Size(46, 23);
            this.Btn_TestRange2.TabIndex = 28;
            this.Btn_TestRange2.Text = "校正2";
            this.Btn_TestRange2.UseVisualStyleBackColor = true;
            this.Btn_TestRange2.Click += new System.EventHandler(this.Btn_TestRange2_Click);
            // 
            // lbl_Msg
            // 
            this.lbl_Msg.AutoSize = true;
            this.lbl_Msg.Location = new System.Drawing.Point(639, 29);
            this.lbl_Msg.Name = "lbl_Msg";
            this.lbl_Msg.Size = new System.Drawing.Size(119, 12);
            this.lbl_Msg.TabIndex = 29;
            this.lbl_Msg.Text = "timerAutoClose信息:";
            // 
            // txtPosition350
            // 
            this.txtPosition350.Location = new System.Drawing.Point(71, 306);
            this.txtPosition350.Name = "txtPosition350";
            this.txtPosition350.Size = new System.Drawing.Size(31, 21);
            this.txtPosition350.TabIndex = 30;
            this.txtPosition350.Text = "0";
            // 
            // Btn_Login
            // 
            this.Btn_Login.Location = new System.Drawing.Point(14, 507);
            this.Btn_Login.Name = "Btn_Login";
            this.Btn_Login.Size = new System.Drawing.Size(75, 23);
            this.Btn_Login.TabIndex = 31;
            this.Btn_Login.Text = "7登陆";
            this.Btn_Login.UseVisualStyleBackColor = true;
            this.Btn_Login.Click += new System.EventHandler(this.Btn_Login_Click);
            // 
            // timerAutoClose
            // 
            this.timerAutoClose.Interval = 2000;
            this.timerAutoClose.Tick += new System.EventHandler(this.timerAutoClose_Tick);
            // 
            // lbl_AppMsg
            // 
            this.lbl_AppMsg.AutoSize = true;
            this.lbl_AppMsg.Location = new System.Drawing.Point(639, 9);
            this.lbl_AppMsg.Name = "lbl_AppMsg";
            this.lbl_AppMsg.Size = new System.Drawing.Size(53, 12);
            this.lbl_AppMsg.TabIndex = 32;
            this.lbl_AppMsg.Text = "程序信息";
            // 
            // timerLoginWorkFlow
            // 
            this.timerLoginWorkFlow.Interval = 1000;
            this.timerLoginWorkFlow.Tick += new System.EventHandler(this.timerLoginWorkFlow_Tick);
            // 
            // btn_Exit
            // 
            this.btn_Exit.Location = new System.Drawing.Point(1422, 21);
            this.btn_Exit.Name = "btn_Exit";
            this.btn_Exit.Size = new System.Drawing.Size(75, 23);
            this.btn_Exit.TabIndex = 33;
            this.btn_Exit.Text = "立即退出";
            this.btn_Exit.UseVisualStyleBackColor = true;
            this.btn_Exit.Click += new System.EventHandler(this.btn_Exit_Click);
            // 
            // pictureBox_Bak
            // 
            this.pictureBox_Bak.Location = new System.Drawing.Point(12, 575);
            this.pictureBox_Bak.Name = "pictureBox_Bak";
            this.pictureBox_Bak.Size = new System.Drawing.Size(378, 105);
            this.pictureBox_Bak.TabIndex = 34;
            this.pictureBox_Bak.TabStop = false;
            // 
            // btn_Temp
            // 
            this.btn_Temp.Location = new System.Drawing.Point(558, 21);
            this.btn_Temp.Name = "btn_Temp";
            this.btn_Temp.Size = new System.Drawing.Size(75, 23);
            this.btn_Temp.TabIndex = 35;
            this.btn_Temp.Text = "临时";
            this.btn_Temp.UseVisualStyleBackColor = true;
            this.btn_Temp.Click += new System.EventHandler(this.btn_Temp_Click);
            // 
            // timerOption
            // 
            this.timerOption.Interval = 500;
            this.timerOption.Tick += new System.EventHandler(this.timerOption_Tick);
            // 
            // txtPosition226
            // 
            this.txtPosition226.Location = new System.Drawing.Point(161, 306);
            this.txtPosition226.Name = "txtPosition226";
            this.txtPosition226.Size = new System.Drawing.Size(31, 21);
            this.txtPosition226.TabIndex = 36;
            this.txtPosition226.Text = "0";
            // 
            // lblPosition350
            // 
            this.lblPosition350.AutoSize = true;
            this.lblPosition350.Location = new System.Drawing.Point(14, 309);
            this.lblPosition350.Name = "lblPosition350";
            this.lblPosition350.Size = new System.Drawing.Size(47, 12);
            this.lblPosition350.TabIndex = 37;
            this.lblPosition350.Text = "350坐标";
            // 
            // lblPosition226
            // 
            this.lblPosition226.AutoSize = true;
            this.lblPosition226.Location = new System.Drawing.Point(108, 309);
            this.lblPosition226.Name = "lblPosition226";
            this.lblPosition226.Size = new System.Drawing.Size(47, 12);
            this.lblPosition226.TabIndex = 38;
            this.lblPosition226.Text = "226坐标";
            // 
            // timerSafeQuestionWorkFlow
            // 
            this.timerSafeQuestionWorkFlow.Interval = 1000;
            this.timerSafeQuestionWorkFlow.Tick += new System.EventHandler(this.timerSafeQuestionWorkFlow_Tick);
            // 
            // lbl_isDebug
            // 
            this.lbl_isDebug.AutoSize = true;
            this.lbl_isDebug.Location = new System.Drawing.Point(1308, 26);
            this.lbl_isDebug.Name = "lbl_isDebug";
            this.lbl_isDebug.Size = new System.Drawing.Size(59, 12);
            this.lbl_isDebug.TabIndex = 39;
            this.lbl_isDebug.Text = "Debug状态";
            // 
            // txt_OptionLog
            // 
            this.txt_OptionLog.Location = new System.Drawing.Point(95, 333);
            this.txt_OptionLog.Multiline = true;
            this.txt_OptionLog.Name = "txt_OptionLog";
            this.txt_OptionLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txt_OptionLog.Size = new System.Drawing.Size(280, 236);
            this.txt_OptionLog.TabIndex = 40;
            // 
            // lbl_AutoCloseMsg
            // 
            this.lbl_AutoCloseMsg.AutoSize = true;
            this.lbl_AutoCloseMsg.Location = new System.Drawing.Point(1138, 26);
            this.lbl_AutoCloseMsg.Name = "lbl_AutoCloseMsg";
            this.lbl_AutoCloseMsg.Size = new System.Drawing.Size(89, 12);
            this.lbl_AutoCloseMsg.TabIndex = 41;
            this.lbl_AutoCloseMsg.Text = "倒计时关闭信息";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(1138, 9);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(71, 12);
            this.label3.TabIndex = 42;
            this.label3.Text = "剪切板监视:";
            // 
            // lbl_Clipboard
            // 
            this.lbl_Clipboard.AutoSize = true;
            this.lbl_Clipboard.Location = new System.Drawing.Point(1215, 9);
            this.lbl_Clipboard.Name = "lbl_Clipboard";
            this.lbl_Clipboard.Size = new System.Drawing.Size(83, 12);
            this.lbl_Clipboard.TabIndex = 43;
            this.lbl_Clipboard.Text = "lbl_Clipboard";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1596, 774);
            this.Controls.Add(this.lbl_Clipboard);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.lbl_AutoCloseMsg);
            this.Controls.Add(this.txt_OptionLog);
            this.Controls.Add(this.lbl_isDebug);
            this.Controls.Add(this.lblPosition226);
            this.Controls.Add(this.lblPosition350);
            this.Controls.Add(this.txtPosition226);
            this.Controls.Add(this.btn_Temp);
            this.Controls.Add(this.pictureBox_Bak);
            this.Controls.Add(this.btn_Exit);
            this.Controls.Add(this.lbl_AppMsg);
            this.Controls.Add(this.Btn_Login);
            this.Controls.Add(this.txtPosition350);
            this.Controls.Add(this.lbl_Msg);
            this.Controls.Add(this.Btn_TestRange2);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.lbl_PicName1);
            this.Controls.Add(this.Btn_TestRange);
            this.Controls.Add(this.txt_Range);
            this.Controls.Add(this.Btn_Show);
            this.Controls.Add(this.txt_Right);
            this.Controls.Add(this.txt_Left);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.btn_Start);
            this.Controls.Add(this.btn_Search);
            this.Controls.Add(this.btn_setMouse);
            this.Controls.Add(this.txtPosition);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btn_LoadPicfromWB);
            this.Controls.Add(this.wbMain);
            this.Controls.Add(this.btn_Calc);
            this.Controls.Add(this.Btn_Match);
            this.Controls.Add(this.btn_LoadPic);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.pictureBox1);
            this.Name = "MainForm";
            this.Text = "MainForm";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainForm_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_Bak)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btn_LoadPic;
        private System.Windows.Forms.Button Btn_Match;
        private System.Windows.Forms.Button btn_Calc;
        private System.Windows.Forms.Button btn_LoadPicfromWB;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtPosition;
        private System.Windows.Forms.Button btn_setMouse;
        private System.Windows.Forms.Button btn_Search;
        private System.Windows.Forms.Button btn_Start;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        public System.Windows.Forms.WebBrowser wbMain;
        public System.Windows.Forms.PictureBox pictureBox1;
        public System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.TextBox txt_Left;
        private System.Windows.Forms.TextBox txt_Right;
        private System.Windows.Forms.Button Btn_Show;
        private System.Windows.Forms.TextBox txt_Range;
        private System.Windows.Forms.Button Btn_TestRange;
        private System.Windows.Forms.Label lbl_PicName1;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button Btn_TestRange2;
        private System.Windows.Forms.Label lbl_Msg;
        private System.Windows.Forms.TextBox txtPosition350;
        private System.Windows.Forms.Button Btn_Login;
        private System.Windows.Forms.Timer timerAutoClose;
        private System.Windows.Forms.Label lbl_AppMsg;
        private System.Windows.Forms.Timer timerLoginWorkFlow;
        private System.Windows.Forms.Button btn_Exit;
        public System.Windows.Forms.PictureBox pictureBox_Bak;
        private System.Windows.Forms.Button btn_Temp;
        private System.Windows.Forms.Timer timerOption;
        private System.Windows.Forms.TextBox txtPosition226;
        private System.Windows.Forms.Label lblPosition350;
        private System.Windows.Forms.Label lblPosition226;
        private System.Windows.Forms.Timer timerSafeQuestionWorkFlow;
        private System.Windows.Forms.Label lbl_isDebug;
        private System.Windows.Forms.TextBox txt_OptionLog;
        private System.Windows.Forms.Label lbl_AutoCloseMsg;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label lbl_Clipboard;
    }
}

