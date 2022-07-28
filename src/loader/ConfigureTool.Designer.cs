
namespace loader
{
    partial class ConfigureTool
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ConfigureTool));
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radioButton_objects = new System.Windows.Forms.RadioButton();
            this.radioButton_grids = new System.Windows.Forms.RadioButton();
            this.label2 = new System.Windows.Forms.Label();
            this.checkBox_use_coords_recacl = new System.Windows.Forms.CheckBox();
            this.textBox_entering_max_triangles = new System.Windows.Forms.TextBox();
            this.checkBox_use_max_triangles = new System.Windows.Forms.CheckBox();
            this.checkBox1_use_hidded = new System.Windows.Forms.CheckBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.button1 = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.linkLabel1 = new System.Windows.Forms.LinkLabel();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(0, 13);
            this.label1.TabIndex = 0;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.radioButton_objects);
            this.groupBox1.Controls.Add(this.radioButton_grids);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.checkBox_use_coords_recacl);
            this.groupBox1.Controls.Add(this.textBox_entering_max_triangles);
            this.groupBox1.Controls.Add(this.checkBox_use_max_triangles);
            this.groupBox1.Controls.Add(this.checkBox1_use_hidded);
            this.groupBox1.Location = new System.Drawing.Point(12, 13);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(367, 133);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Параметры выборки объектов:";
            this.groupBox1.Enter += new System.EventHandler(this.groupBox1_Enter);
            // 
            // radioButton_objects
            // 
            this.radioButton_objects.AutoSize = true;
            this.radioButton_objects.Location = new System.Drawing.Point(155, 101);
            this.radioButton_objects.Name = "radioButton_objects";
            this.radioButton_objects.Size = new System.Drawing.Size(194, 17);
            this.radioButton_objects.TabIndex = 8;
            this.radioButton_objects.TabStop = true;
            this.radioButton_objects.Text = "Объект с вложенной геометрией";
            this.radioButton_objects.UseVisualStyleBackColor = true;
            // 
            // radioButton_grids
            // 
            this.radioButton_grids.AutoSize = true;
            this.radioButton_grids.Location = new System.Drawing.Point(4, 101);
            this.radioButton_grids.Name = "radioButton_grids";
            this.radioButton_grids.Size = new System.Drawing.Size(136, 17);
            this.radioButton_grids.TabIndex = 7;
            this.radioButton_grids.TabStop = true;
            this.radioButton_grids.Text = "Сетки с материалами";
            this.radioButton_grids.UseVisualStyleBackColor = true;
            this.radioButton_grids.CheckedChanged += new System.EventHandler(this.radioButton_grids_CheckedChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(4, 85);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(198, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "Выберите режим экспорта объектов:";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // checkBox_use_coords_recacl
            // 
            this.checkBox_use_coords_recacl.AutoSize = true;
            this.checkBox_use_coords_recacl.Location = new System.Drawing.Point(6, 65);
            this.checkBox_use_coords_recacl.Name = "checkBox_use_coords_recacl";
            this.checkBox_use_coords_recacl.Size = new System.Drawing.Size(188, 17);
            this.checkBox_use_coords_recacl.TabIndex = 5;
            this.checkBox_use_coords_recacl.Text = "Применять пересчет координат";
            this.checkBox_use_coords_recacl.UseVisualStyleBackColor = true;
            this.checkBox_use_coords_recacl.CheckedChanged += new System.EventHandler(this.checkBox_use_coords_recacl_CheckedChanged);
            // 
            // textBox_entering_max_triangles
            // 
            this.textBox_entering_max_triangles.Location = new System.Drawing.Point(295, 42);
            this.textBox_entering_max_triangles.Name = "textBox_entering_max_triangles";
            this.textBox_entering_max_triangles.Size = new System.Drawing.Size(67, 20);
            this.textBox_entering_max_triangles.TabIndex = 4;
            this.textBox_entering_max_triangles.TextChanged += new System.EventHandler(this.textBox_entering_max_triangles_TextChanged);
            // 
            // checkBox_use_max_triangles
            // 
            this.checkBox_use_max_triangles.AutoSize = true;
            this.checkBox_use_max_triangles.Location = new System.Drawing.Point(6, 42);
            this.checkBox_use_max_triangles.Name = "checkBox_use_max_triangles";
            this.checkBox_use_max_triangles.Size = new System.Drawing.Size(283, 17);
            this.checkBox_use_max_triangles.TabIndex = 3;
            this.checkBox_use_max_triangles.Text = "Не учитывать объекты с числом полигонов более:";
            this.checkBox_use_max_triangles.UseVisualStyleBackColor = true;
            this.checkBox_use_max_triangles.CheckedChanged += new System.EventHandler(this.checkBox_use_max_triangles_CheckedChanged);
            // 
            // checkBox1_use_hidded
            // 
            this.checkBox1_use_hidded.AutoSize = true;
            this.checkBox1_use_hidded.Location = new System.Drawing.Point(6, 19);
            this.checkBox1_use_hidded.Name = "checkBox1_use_hidded";
            this.checkBox1_use_hidded.Size = new System.Drawing.Size(179, 17);
            this.checkBox1_use_hidded.TabIndex = 1;
            this.checkBox1_use_hidded.Text = "Учитывать скрытые объекты;";
            this.checkBox1_use_hidded.UseVisualStyleBackColor = true;
            this.checkBox1_use_hidded.CheckedChanged += new System.EventHandler(this.checkBox1_use_hidded_CheckedChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.radioButton2);
            this.groupBox2.Controls.Add(this.radioButton1);
            this.groupBox2.Location = new System.Drawing.Point(11, 152);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(367, 67);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Формат экспорта";
            this.groupBox2.Enter += new System.EventHandler(this.groupBox2_Enter);
            // 
            // radioButton2
            // 
            this.radioButton2.AutoSize = true;
            this.radioButton2.Location = new System.Drawing.Point(10, 42);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(125, 17);
            this.radioButton2.TabIndex = 1;
            this.radioButton2.TabStop = true;
            this.radioButton2.Text = "В fbx (binary, 2019.5)";
            this.radioButton2.UseVisualStyleBackColor = true;
            this.radioButton2.CheckedChanged += new System.EventHandler(this.radioButton2_CheckedChanged);
            // 
            // radioButton1
            // 
            this.radioButton1.AutoSize = true;
            this.radioButton1.Location = new System.Drawing.Point(10, 19);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(167, 17);
            this.radioButton1.TabIndex = 0;
            this.radioButton1.TabStop = true;
            this.radioButton1.Text = "В nwc (Autodesk Navisworks)";
            this.radioButton1.UseVisualStyleBackColor = true;
            this.radioButton1.CheckedChanged += new System.EventHandler(this.radioButton1_CheckedChanged);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(286, 257);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 3;
            this.button1.Text = "Запуск";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(16, 262);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(230, 13);
            this.label4.TabIndex = 2;
            this.label4.Text = "Нажмите для запуска процедуры экспорта:";
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.linkLabel1);
            this.groupBox3.Controls.Add(this.label3);
            this.groupBox3.Location = new System.Drawing.Point(11, 292);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(367, 111);
            this.groupBox3.TabIndex = 3;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "О приложении";
            this.groupBox3.Enter += new System.EventHandler(this.groupBox3_Enter);
            // 
            // linkLabel1
            // 
            this.linkLabel1.AutoSize = true;
            this.linkLabel1.Location = new System.Drawing.Point(10, 85);
            this.linkLabel1.Name = "linkLabel1";
            this.linkLabel1.Size = new System.Drawing.Size(263, 13);
            this.linkLabel1.TabIndex = 1;
            this.linkLabel1.TabStop = true;
            this.linkLabel1.Text = "https://github.com/GeorgGrebenyuk/renga_3d-export";
            this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabel1_LinkClicked);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(10, 20);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(357, 65);
            this.label3.TabIndex = 0;
            this.label3.Text = resources.GetString("label3.Text");
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // ConfigureTool
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ClientSize = new System.Drawing.Size(390, 410);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ConfigureTool";
            this.Text = "Настройка параметров 3d-экспорта";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox textBox_entering_max_triangles;
        private System.Windows.Forms.CheckBox checkBox_use_max_triangles;
        private System.Windows.Forms.CheckBox checkBox1_use_hidded;
        private System.Windows.Forms.CheckBox checkBox_use_coords_recacl;
        private System.Windows.Forms.RadioButton radioButton_objects;
        private System.Windows.Forms.RadioButton radioButton_grids;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.RadioButton radioButton2;
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.LinkLabel linkLabel1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label4;
    }
}