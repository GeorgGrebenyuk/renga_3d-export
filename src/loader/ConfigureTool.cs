using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using static loader.init_app;

namespace loader
{
    public partial class ConfigureTool : Form
    {
        public ConfigureTool()
        {
            InitializeComponent();
            textBox_entering_max_triangles.Text = init_app.max_triangles_count.ToString();

            radioButton1.Checked = true;
        }

        private void checkBox1_use_hidded_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void checkBox_unuse_categoties_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void checkBox_use_max_triangles_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void textBox_entering_max_triangles_TextChanged(object sender, EventArgs e)
        {

        }

        private void checkBox_use_coords_recacl_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/GeorgGrebenyuk/renga_3d-export");
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton1.Checked)
            {
                radioButton_grids.Enabled = false;
                checkBox_use_max_triangles.Enabled = false;
                radioButton_objects.Checked = true;
            }
            else
            {
                radioButton_objects.Checked = false;
                radioButton_grids.Enabled = true;
                checkBox_use_max_triangles.Enabled = true;
            }
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void groupBox2_Enter(object sender, EventArgs e)
        {

        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void groupBox3_Enter(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            
            //Считывание предустановленных параметров
            if (checkBox1_use_hidded.Checked) init_app.use_hidded_objects = true;
            else init_app.use_hidded_objects = false;
            if (checkBox_use_coords_recacl.Checked) init_app.use_recalc_coordinates = true;
            else init_app.use_recalc_coordinates = false;
            if (Convert.ToInt32(textBox_entering_max_triangles.Text) > 0) init_app.max_triangles_count = Convert.ToInt32(textBox_entering_max_triangles.Text);
            else init_app.max_triangles_count = -1;

            if (radioButton1.Checked) init_app.export_format = 0;
            else if (radioButton2.Checked) init_app.export_format = 1;

            if (radioButton_grids.Checked) init_app.export_mode = 0;
            else if (radioButton_objects.Checked) init_app.export_mode = 1;

            this.Close();
            //return to init_app class
        }

        private void label4_Click(object sender, EventArgs e)
        {

        }
    }
}
