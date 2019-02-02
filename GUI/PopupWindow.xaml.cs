///////////////////////////////////////////////////////////////////////
// PopupWindow.xaml.cs - A GUI window with a text box                //
// ver 1.0                                                           //
// Naman Parakh, CSE687 - Object Oriented Design, Spring 2018        //
// nrparakh@syr.edu                                                  //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * Provides a GUI window with a scrollable 
 * text box for displaying the results.
 *   
 * Required Files:
 * ---------------
 * PopupWindow.xaml
 * 
 * Maintenance History:
 * --------------------
 * ver 1.0 : 5th December 2018
 * - first release
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace WpfApp1
{
    /// <summary>
    /// Interaction logic for PopupWindow.xaml
    /// </summary>
    public partial class PopupWindow : Window
    {
        public PopupWindow()
        {
            InitializeComponent();
        }
    }
}
