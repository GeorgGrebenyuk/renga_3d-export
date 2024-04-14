using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

//using static loader.init_app;

namespace loader
{
    public partial class ConfigureTool : Form
    {
        public ConfigureTool()
        {
            InitializeComponent();
            textBox_entering_max_triangles.Text = init_app.max_triangles_count.ToString();

            radioButton1.Checked = true;
            checkBox_use_coords_recacl.Enabled = false;
            //radioButton_grids.Enabled = false;
            radioButton_objects.Checked = true;
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
                //checkBox_use_max_triangles.Enabled = false;
                radioButton_objects.Checked = true;
            }
            else
            {
                radioButton_objects.Checked = false;
                radioButton_grids.Enabled = true;
                //checkBox_use_max_triangles.Enabled = true;
            }
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton2.Checked)
            {
                radioButton_objects.Checked = true;
            }
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
            Stopwatch timer = new Stopwatch();
            timer.Start();

            //Считывание предустановленных параметров
            if (checkBox1_use_hidded.Checked) init_app.use_hidded_objects = true;
            else init_app.use_hidded_objects = false;
            if (checkBox_use_coords_recacl.Checked) init_app.use_recalc_coordinates = true;
            else init_app.use_recalc_coordinates = false;

            if (checkBox_use_max_triangles.Checked)
            {
                if (Convert.ToInt32(textBox_entering_max_triangles.Text) >= 1) init_app.max_triangles_count = Convert.ToInt32(textBox_entering_max_triangles.Text);
                else init_app.max_triangles_count = -1;
            }

            if (checkBox_turn_off_sub_geom.Checked) init_app.include_sub_geometry = false;

            if (radioButton1.Checked) init_app.export_format = 0;
            else if (radioButton2.Checked) init_app.export_format = 1;

            if (radioButton_grids.Checked) init_app.export_mode = 0;
            else if (radioButton_objects.Checked) init_app.export_mode = 1;

            //string log_path = "C:\\renga_3d_export_log.txt";
            //if (File.Exists(log_path))
            //{
            //    string file_log = File.ReadAllText(log_path);
            //    File.Delete(log_path);
            //    MessageBox.Show(file_log);
            //}

            timer.Stop();
            string message = $"Экспорт окончен! \n Время обработки = " + timer.Elapsed.TotalSeconds.ToString() + " сек.";
            MessageBox.Show(message);

            this.Close();
            //return to init_app class
        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void radioButton_grids_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_grids.Checked)
            {
                checkBox1_use_hidded.Enabled = false;
                textBox_entering_max_triangles.Enabled = false;
                checkBox_use_max_triangles.Enabled = false;
            }
            else
            {
                checkBox1_use_hidded.Enabled = true;
                checkBox_use_max_triangles.Enabled = true;
                textBox_entering_max_triangles.Enabled = true;
            }
        }

        private void checkBox_turn_off_sub_geom_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}
