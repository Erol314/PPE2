using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lab_5
{
    public partial class SettingsForm : Form
    {
        Form1 form1 = null;

        public SettingsForm(Form1 form)
        {
            form1 = form;
            InitializeComponent();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (numericUpDown1.Value % 2 != 0)
                numericUpDown1.Value += 1;
            else if (numericUpDown1.Value == 0)
                numericUpDown1.Value = 2;

            if ((int)numericUpDown1.Value != form1.tableLayoutPanel1.RowCount)
            {
                form1.tableLayoutPanel1.SuspendLayout();
                form1.tableLayoutPanel1.Controls.Clear();
                form1.tableLayoutPanel1.ColumnStyles.Clear();
                form1.tableLayoutPanel1.RowStyles.Clear();

                form1.tableLayoutPanel1.RowCount = (int)numericUpDown1.Value;
                for (int x = 0; x < form1.tableLayoutPanel1.RowCount; x++)
                    form1.tableLayoutPanel1.RowStyles.Add(new RowStyle() { Height = 100 / (int)numericUpDown1.Value, SizeType = SizeType.Percent });
                form1.tableLayoutPanel1.ColumnCount = (int)numericUpDown1.Value;
                for (int x = 0; x < form1.tableLayoutPanel1.ColumnCount; x++)
                    form1.tableLayoutPanel1.ColumnStyles.Add(new ColumnStyle() { Width = 100 / (int)numericUpDown1.Value, SizeType = SizeType.Percent });

                Label[] labels = new Label[(int)numericUpDown1.Value * (int)numericUpDown1.Value];

                for (int i = 0; i < labels.Count(); i++)
                {
                    labels[i] = new Label();
                    labels[i].ForeColor = Color.Green;
                    labels[i].Text = "b";
                    labels[i].Dock = DockStyle.Fill;
                    labels[i].TextAlign = ContentAlignment.MiddleCenter;
                    labels[i].Font = new Font("Webdings", 288 / (int)numericUpDown1.Value);
                    labels[i].Click += new EventHandler(form1.LabelClick);
                    form1.tableLayoutPanel1.Controls.Add(labels[i]);
                }
                form1.tableLayoutPanel1.ResumeLayout();
                form1.AssignIcons();
            }
            form1.timer1.Interval = (int)numericUpDown2.Value;
            Close();
        }

        private void SettingsForm_Load(object sender, EventArgs e)
        {
            numericUpDown2.Value = form1.timer1.Interval;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
