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
    public partial class Form1 : Form
    {
        Random random = new Random();

        Label firstClicked, secondClicked;

        public Form1()
        {
            InitializeComponent();
            AssignIcons();
        }

        public void LabelClick(object sender, EventArgs e)
        {
            if (firstClicked != null && secondClicked != null)
                return;

            Label clickedLabel = sender as Label;

            if (clickedLabel.ForeColor != Color.Green)
                return;

            if (clickedLabel == null || clickedLabel.ForeColor == Color.Cornsilk)
                return;

            if (firstClicked == null)
            {
                firstClicked = clickedLabel;
                clickedLabel.ForeColor = Color.Cornsilk;
                return;
            }
            secondClicked = clickedLabel;
            clickedLabel.ForeColor = Color.Cornsilk;

            CheckForWinner();

            if (firstClicked.Text == secondClicked.Text)
            {
                firstClicked.ForeColor = Color.DarkSeaGreen;
                secondClicked.ForeColor = Color.DarkSeaGreen;
                firstClicked = null;
                secondClicked = null;
            }
            else
            {
                timer1.Start();
                timer2.Start();
            }

            return;
        }

        private void CheckForWinner()
        {
            Label label;
            for (int i = 0; i < tableLayoutPanel1.Controls.Count; i++)
            {
                label = tableLayoutPanel1.Controls[i] as Label;
                if (label != null && label.ForeColor == label.BackColor)
                    return;
            }

            MessageBox.Show("You Won");
            Close();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            timer2.Stop();
            firstClicked.ForeColor = firstClicked.BackColor;
            secondClicked.ForeColor = secondClicked.BackColor;
            firstClicked = null;
            secondClicked = null;
            timer1.Stop();
        }

        private void label17_Click(object sender, EventArgs e)
        {
            SettingsForm sf = new SettingsForm(this);
            sf.Show();
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            if (firstClicked.ForeColor == Color.Cornsilk)
            {
                firstClicked.ForeColor = Color.Chocolate;
                secondClicked.ForeColor = Color.Cornsilk;
            }
            else
            {
                secondClicked.ForeColor = Color.LightSeaGreen;
                firstClicked.ForeColor = Color.Cornsilk;
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        public void AssignIcons()
        {
            List<string> icons = new List<string>()
            {
                "a", "b", ",", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "!", "@", "&",
                "n", "o", "u", "p", "r", "s", "t", "v", "x", "y", "z", "w", "#", "$", "%", "^"
            };

            Label label;
            int randomNumber;
            List<string> iconsUsed = new List<string>();

            for (int i = 0; i < tableLayoutPanel1.Controls.Count; i++)
            {
                if (tableLayoutPanel1.Controls[i] is Label)
                    label = (Label)tableLayoutPanel1.Controls[i];
                else
                    continue;

                if (i < tableLayoutPanel1.Controls.Count / 2)
                {
                    randomNumber = random.Next(0, icons.Count);
                    label.Text = icons[randomNumber];
                    iconsUsed.Add(icons[randomNumber]);
                    icons.RemoveAt(randomNumber);
                }
                else
                {
                    randomNumber = random.Next(0, iconsUsed.Count);
                    label.Text = iconsUsed[randomNumber];
                    iconsUsed.RemoveAt(randomNumber);
                }

            }
        }
    }
}
