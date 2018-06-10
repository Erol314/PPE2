using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;

namespace Lab_6
{
    public partial class Form1 : Form
    {

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text != "")
                listBox1.Items.Add(textBox1.Text);

            System.IO.StreamWriter SaveFile = new System.IO.StreamWriter("tasks.txt");
            foreach (var item in listBox1.Items)
            {
                SaveFile.WriteLine(item);
            }
            SaveFile.Close();
        }

        private void DeleteItem(object sender, MouseEventArgs e)
        {
            if (MessageBox.Show("Delete this item ??", "DElete?", MessageBoxButtons.YesNo) == DialogResult.Yes)
                listBox1.Items.RemoveAt(listBox1.SelectedIndex);
        }

        private void Form1Load(object sender, EventArgs e)
        {
            if (File.Exists("tasks.txt"))
            {
                string line;
                System.IO.StreamReader file = new System.IO.StreamReader("tasks.txt");
                while ((line = file.ReadLine()) != null)
                {
                    listBox1.Items.Add(line);
                }
                file.Close();
            }
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
