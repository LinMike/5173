namespace GDI
{
    partial class CollectPic
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.btn_Save = new System.Windows.Forms.Button();
            this.wbMain = new System.Windows.Forms.WebBrowser();
            this.btn_LoadPic = new System.Windows.Forms.Button();
            this.btn_Search = new System.Windows.Forms.Button();
            this.lbl_PicName2 = new System.Windows.Forms.Label();
            this.lbl_PicName1 = new System.Windows.Forms.Label();
            this.txt_Range = new System.Windows.Forms.TextBox();
            this.lbl_Msg1 = new System.Windows.Forms.Label();
            this.btn_SaveNewLeft = new System.Windows.Forms.Button();
            this.Btn_TestRange = new System.Windows.Forms.Button();
            this.Btn_Match = new System.Windows.Forms.Button();
            this.btn_Calc = new System.Windows.Forms.Button();
            this.btn_SavePicBox = new System.Windows.Forms.Button();
            this.btn_LoadPicBox = new System.Windows.Forms.Button();
            this.btn_SaveNewRight = new System.Windows.Forms.Button();
            this.txtPosition = new System.Windows.Forms.TextBox();
            this.Btn_Show = new System.Windows.Forms.Button();
            this.txt_Left = new System.Windows.Forms.TextBox();
            this.txt_Right = new System.Windows.Forms.TextBox();
            this.lbl_Msg = new System.Windows.Forms.Label();
            this.btn_newPic = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(12, 0);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(330, 105);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Location = new System.Drawing.Point(12, 129);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(330, 105);
            this.pictureBox2.TabIndex = 1;
            this.pictureBox2.TabStop = false;
            // 
            // btn_Save
            // 
            this.btn_Save.Location = new System.Drawing.Point(12, 370);
            this.btn_Save.Name = "btn_Save";
            this.btn_Save.Size = new System.Drawing.Size(75, 23);
            this.btn_Save.TabIndex = 2;
            this.btn_Save.Text = "保存到图库";
            this.btn_Save.UseVisualStyleBackColor = true;
            this.btn_Save.Click += new System.EventHandler(this.btn_Save_Click);
            // 
            // wbMain
            // 
            this.wbMain.Location = new System.Drawing.Point(663, 0);
            this.wbMain.MinimumSize = new System.Drawing.Size(20, 20);
            this.wbMain.Name = "wbMain";
            this.wbMain.Size = new System.Drawing.Size(417, 438);
            this.wbMain.TabIndex = 4;
            // 
            // btn_LoadPic
            // 
            this.btn_LoadPic.Location = new System.Drawing.Point(12, 261);
            this.btn_LoadPic.Name = "btn_LoadPic";
            this.btn_LoadPic.Size = new System.Drawing.Size(75, 23);
            this.btn_LoadPic.TabIndex = 5;
            this.btn_LoadPic.Text = "加载图";
            this.btn_LoadPic.UseVisualStyleBackColor = true;
            this.btn_LoadPic.Click += new System.EventHandler(this.btn_LoadPic_Click);
            // 
            // btn_Search
            // 
            this.btn_Search.Location = new System.Drawing.Point(12, 287);
            this.btn_Search.Name = "btn_Search";
            this.btn_Search.Size = new System.Drawing.Size(75, 23);
            this.btn_Search.TabIndex = 6;
            this.btn_Search.Text = "搜索原图";
            this.btn_Search.UseVisualStyleBackColor = true;
            this.btn_Search.Click += new System.EventHandler(this.btn_Search_Click);
            // 
            // lbl_PicName2
            // 
            this.lbl_PicName2.AutoSize = true;
            this.lbl_PicName2.Location = new System.Drawing.Point(12, 245);
            this.lbl_PicName2.Name = "lbl_PicName2";
            this.lbl_PicName2.Size = new System.Drawing.Size(47, 12);
            this.lbl_PicName2.TabIndex = 7;
            this.lbl_PicName2.Text = "文件名:";
            // 
            // lbl_PicName1
            // 
            this.lbl_PicName1.AutoSize = true;
            this.lbl_PicName1.Location = new System.Drawing.Point(10, 114);
            this.lbl_PicName1.Name = "lbl_PicName1";
            this.lbl_PicName1.Size = new System.Drawing.Size(47, 12);
            this.lbl_PicName1.TabIndex = 8;
            this.lbl_PicName1.Text = "文件名:";
            // 
            // txt_Range
            // 
            this.txt_Range.Location = new System.Drawing.Point(75, 423);
            this.txt_Range.Name = "txt_Range";
            this.txt_Range.Size = new System.Drawing.Size(100, 21);
            this.txt_Range.TabIndex = 9;
            this.txt_Range.Text = "0";
            // 
            // lbl_Msg1
            // 
            this.lbl_Msg1.AutoSize = true;
            this.lbl_Msg1.Location = new System.Drawing.Point(10, 426);
            this.lbl_Msg1.Name = "lbl_Msg1";
            this.lbl_Msg1.Size = new System.Drawing.Size(59, 12);
            this.lbl_Msg1.TabIndex = 10;
            this.lbl_Msg1.Text = "覆盖范围:";
            // 
            // btn_SaveNewLeft
            // 
            this.btn_SaveNewLeft.Location = new System.Drawing.Point(124, 456);
            this.btn_SaveNewLeft.Name = "btn_SaveNewLeft";
            this.btn_SaveNewLeft.Size = new System.Drawing.Size(75, 23);
            this.btn_SaveNewLeft.TabIndex = 11;
            this.btn_SaveNewLeft.Text = "左覆盖保存";
            this.btn_SaveNewLeft.UseVisualStyleBackColor = true;
            this.btn_SaveNewLeft.Click += new System.EventHandler(this.btn_SaveNewLeft_Click);
            // 
            // Btn_TestRange
            // 
            this.Btn_TestRange.Location = new System.Drawing.Point(183, 421);
            this.Btn_TestRange.Name = "Btn_TestRange";
            this.Btn_TestRange.Size = new System.Drawing.Size(75, 23);
            this.Btn_TestRange.TabIndex = 12;
            this.Btn_TestRange.Text = "校正";
            this.Btn_TestRange.UseVisualStyleBackColor = true;
            this.Btn_TestRange.Click += new System.EventHandler(this.Btn_TestRange_Click);
            // 
            // Btn_Match
            // 
            this.Btn_Match.Location = new System.Drawing.Point(12, 316);
            this.Btn_Match.Name = "Btn_Match";
            this.Btn_Match.Size = new System.Drawing.Size(75, 23);
            this.Btn_Match.TabIndex = 13;
            this.Btn_Match.Text = "填充差异";
            this.Btn_Match.UseVisualStyleBackColor = true;
            this.Btn_Match.Click += new System.EventHandler(this.Btn_Match_Click);
            // 
            // btn_Calc
            // 
            this.btn_Calc.Location = new System.Drawing.Point(12, 345);
            this.btn_Calc.Name = "btn_Calc";
            this.btn_Calc.Size = new System.Drawing.Size(75, 23);
            this.btn_Calc.TabIndex = 14;
            this.btn_Calc.Text = "计算差异";
            this.btn_Calc.UseVisualStyleBackColor = true;
            this.btn_Calc.Click += new System.EventHandler(this.btn_Calc_Click);
            // 
            // btn_SavePicBox
            // 
            this.btn_SavePicBox.Location = new System.Drawing.Point(12, 494);
            this.btn_SavePicBox.Name = "btn_SavePicBox";
            this.btn_SavePicBox.Size = new System.Drawing.Size(75, 23);
            this.btn_SavePicBox.TabIndex = 15;
            this.btn_SavePicBox.Text = "保存PICBOX";
            this.btn_SavePicBox.UseVisualStyleBackColor = true;
            this.btn_SavePicBox.Click += new System.EventHandler(this.btn_SavePicBox_Click);
            // 
            // btn_LoadPicBox
            // 
            this.btn_LoadPicBox.Location = new System.Drawing.Point(102, 494);
            this.btn_LoadPicBox.Name = "btn_LoadPicBox";
            this.btn_LoadPicBox.Size = new System.Drawing.Size(75, 23);
            this.btn_LoadPicBox.TabIndex = 16;
            this.btn_LoadPicBox.Text = "载入PICBOX";
            this.btn_LoadPicBox.UseVisualStyleBackColor = true;
            this.btn_LoadPicBox.Click += new System.EventHandler(this.btn_LoadPicBox_Click);
            // 
            // btn_SaveNewRight
            // 
            this.btn_SaveNewRight.Location = new System.Drawing.Point(220, 456);
            this.btn_SaveNewRight.Name = "btn_SaveNewRight";
            this.btn_SaveNewRight.Size = new System.Drawing.Size(75, 23);
            this.btn_SaveNewRight.TabIndex = 17;
            this.btn_SaveNewRight.Text = "右覆盖保存";
            this.btn_SaveNewRight.UseVisualStyleBackColor = true;
            this.btn_SaveNewRight.Click += new System.EventHandler(this.btn_SaveNewRight_Click);
            // 
            // txtPosition
            // 
            this.txtPosition.Location = new System.Drawing.Point(264, 240);
            this.txtPosition.Name = "txtPosition";
            this.txtPosition.Size = new System.Drawing.Size(31, 21);
            this.txtPosition.TabIndex = 18;
            this.txtPosition.Text = "0";
            // 
            // Btn_Show
            // 
            this.Btn_Show.Location = new System.Drawing.Point(322, 261);
            this.Btn_Show.Name = "Btn_Show";
            this.Btn_Show.Size = new System.Drawing.Size(47, 23);
            this.Btn_Show.TabIndex = 19;
            this.Btn_Show.Text = "show";
            this.Btn_Show.UseVisualStyleBackColor = true;
            this.Btn_Show.Click += new System.EventHandler(this.Btn_Show_Click);
            // 
            // txt_Left
            // 
            this.txt_Left.Location = new System.Drawing.Point(301, 240);
            this.txt_Left.Name = "txt_Left";
            this.txt_Left.Size = new System.Drawing.Size(31, 21);
            this.txt_Left.TabIndex = 20;
            this.txt_Left.Text = "0";
            // 
            // txt_Right
            // 
            this.txt_Right.Location = new System.Drawing.Point(338, 242);
            this.txt_Right.Name = "txt_Right";
            this.txt_Right.Size = new System.Drawing.Size(31, 21);
            this.txt_Right.TabIndex = 21;
            this.txt_Right.Text = "0";
            // 
            // lbl_Msg
            // 
            this.lbl_Msg.AutoSize = true;
            this.lbl_Msg.Location = new System.Drawing.Point(12, 399);
            this.lbl_Msg.Name = "lbl_Msg";
            this.lbl_Msg.Size = new System.Drawing.Size(29, 12);
            this.lbl_Msg.TabIndex = 22;
            this.lbl_Msg.Text = "信息";
            // 
            // btn_newPic
            // 
            this.btn_newPic.Location = new System.Drawing.Point(200, 494);
            this.btn_newPic.Name = "btn_newPic";
            this.btn_newPic.Size = new System.Drawing.Size(75, 23);
            this.btn_newPic.TabIndex = 23;
            this.btn_newPic.Text = "刷新页面";
            this.btn_newPic.UseVisualStyleBackColor = true;
            this.btn_newPic.Click += new System.EventHandler(this.btn_newPic_Click);
            // 
            // CollectPic
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1186, 543);
            this.Controls.Add(this.btn_newPic);
            this.Controls.Add(this.lbl_Msg);
            this.Controls.Add(this.txt_Right);
            this.Controls.Add(this.txt_Left);
            this.Controls.Add(this.Btn_Show);
            this.Controls.Add(this.txtPosition);
            this.Controls.Add(this.btn_SaveNewRight);
            this.Controls.Add(this.btn_LoadPicBox);
            this.Controls.Add(this.btn_SavePicBox);
            this.Controls.Add(this.btn_Calc);
            this.Controls.Add(this.Btn_Match);
            this.Controls.Add(this.Btn_TestRange);
            this.Controls.Add(this.btn_SaveNewLeft);
            this.Controls.Add(this.lbl_Msg1);
            this.Controls.Add(this.txt_Range);
            this.Controls.Add(this.lbl_PicName1);
            this.Controls.Add(this.lbl_PicName2);
            this.Controls.Add(this.btn_Search);
            this.Controls.Add(this.btn_LoadPic);
            this.Controls.Add(this.wbMain);
            this.Controls.Add(this.btn_Save);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.pictureBox1);
            this.Name = "CollectPic";
            this.Text = "CollectPic";
            this.Load += new System.EventHandler(this.CollectPic_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.Button btn_Save;
        private System.Windows.Forms.WebBrowser wbMain;
        private System.Windows.Forms.Button btn_LoadPic;
        private System.Windows.Forms.Button btn_Search;
        private System.Windows.Forms.Label lbl_PicName2;
        private System.Windows.Forms.Label lbl_PicName1;
        private System.Windows.Forms.TextBox txt_Range;
        private System.Windows.Forms.Label lbl_Msg1;
        private System.Windows.Forms.Button btn_SaveNewLeft;
        private System.Windows.Forms.Button Btn_TestRange;
        private System.Windows.Forms.Button Btn_Match;
        private System.Windows.Forms.Button btn_Calc;
        private System.Windows.Forms.Button btn_SavePicBox;
        private System.Windows.Forms.Button btn_LoadPicBox;
        private System.Windows.Forms.Button btn_SaveNewRight;
        private System.Windows.Forms.TextBox txtPosition;
        private System.Windows.Forms.Button Btn_Show;
        private System.Windows.Forms.TextBox txt_Left;
        private System.Windows.Forms.TextBox txt_Right;
        private System.Windows.Forms.Label lbl_Msg;
        private System.Windows.Forms.Button btn_newPic;
    }
}