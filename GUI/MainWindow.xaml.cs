///////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - GUI for Project3HelpWPF                      //
// ver 1.0                                                           //
// Naman Parakh, CSE687 - Object Oriented Design, Spring 2018        //
// Adapted from: MainWindow.xaml.cs by Jim Fawcett                   //
//               Syracuse university                                 //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF-based GUI for Project3HelpWPF demo.  It's 
 * responsibilities are to:
 * - Provide a display of directory contents of a remote ServerPrototype.
 * - It provides a subdirectory list and a filelist for the selected directory.
 * - You can navigate into subdirectories by double-clicking on subdirectory
 *   or the parent directory, indicated by the name "..".
 * - You can double click the filename that ends with .dll to begin testing. 
 * - Provide a display to view the results of the test request.
 * - You can double click on the result to view the log file for the results.
 * 
 * NOTE: For demo purposes auto click is performed as soon as the files
 * and results are loaded.
 * 
 *   
 * Required Files:
 * ---------------
 * Mainwindow.xaml, MainWindow.xaml.cs, PopupWindow.xaml, PopupWindow.xaml.cs
 * Translater.dll
 * 
 * Maintenance History:
 * --------------------
 * ver 1.2 05 Dec 2018
 * - adding functionalities for sending the test request and receving its result.
 * - adding GUI to display the results
 * ver 1/1 : 07 Aug 2018
 * - fixed bug in DirList_MouseDoubleClick by returning when selectedDir is null
 * ver 1.0 : 30 Mar 2018
 * - first release
 * - Several early prototypes were discussed in class. Those are all superceded
 *   by this package.
 */

// Translater has to be statically linked with CommLibWrapper
// - loader can't find Translater.dll dependent CommLibWrapper.dll
using System;
using System.IO;
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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Threading;
using MsgPassingCommunication;
using System.Text.RegularExpressions;

namespace WpfApp1
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            //initialize the arguments
            String[] args = Environment.GetCommandLineArgs();
            if (args.Length < 6)
            {
                throw new ArgumentNullException();
            }
            serverAddress = args[1];
            serverPort = Int32.Parse(args[2]);
            selfPort = Int32.Parse(args[3]);
            sendFilePath = args[4];
            saveFilePath = args[5];
            Title = "localhost::" + args[3];
        }

        private String serverAddress;
        private int serverPort;
        private int selfPort;
        private String sendFilePath;
        private String saveFilePath;
        private Stack<string> pathStack_ = new Stack<string>();
        private Translater translater;
        private CsEndPoint endPoint_;
        private Thread rcvThrd = null;
        private Dictionary<string, Action<CsMessage>> dispatcher_
          = new Dictionary<string, Action<CsMessage>>();

        //----< process incoming messages on child thread >----------------

        private void processMessages()
        {
            ThreadStart thrdProc = () =>
            {
                while (true)
                {
                    CsMessage msg = translater.getMessage();
                    string msgId = msg.value("command");
                    if (dispatcher_.ContainsKey(msgId))
                        dispatcher_[msgId].Invoke(msg);
                }
            };
            rcvThrd = new Thread(thrdProc);
            rcvThrd.IsBackground = true;
            rcvThrd.Start();
        }
        //----< function dispatched by child thread to main thread >-------

        private void clearDirs()
        {
            DirList.Items.Clear();
        }
        //----< function dispatched by child thread to main thread >-------

        private void addDir(string dir)
        {
            DirList.Items.Add(dir);
        }
        //----< function dispatched by child thread to main thread >-------

        private void insertParent()
        {
            DirList.Items.Insert(0, "..");
        }
        //----< function dispatched by child thread to main thread >-------

        private void clearFiles()
        {
            FileList.Items.Clear();
        }
        //----< function dispatched by child thread to main thread >-------

        private void addFile(string file)
        {
            FileList.Items.Add(file);

        }

        //-----< add result to the result list of the GUI >-----------------
        private void addResult(string testFileName, string resultFileName, string result)
        {
            ResultList.Items.Add("TestFile:: " + testFileName + ", " + "Result:: "+ result+ ", " + "Log:: " + resultFileName);
        }
        //----< add client processing for message with key >---------------

        private void addClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }

        //----< load getDirs processing into dispatcher dictionary >-------
        private void DispatcherLoadGetDirs()
        {
            Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
            {
                Action clrDirs = () =>
                {
                    clearDirs();
                };
                Dispatcher.Invoke(clrDirs, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("dir"))
                    {
                        Action<string> doDir = (string dir) =>
                        {
                            addDir(dir);
                        };
                        Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
                    }
                }
                Action insertUp = () =>
                {
                    insertParent();
                };
                Dispatcher.Invoke(insertUp, new Object[] { });
            };
            addClientProc("getDirs", getDirs);
        }

        //-------< load results processing into dispatcher dictionary >----------
        private void DispatcherRecieveTextMessages()
        {
            Action<CsMessage> getResults = (CsMessage rcvMsg) =>
            {
                if(rcvMsg.attributes.ContainsKey("test_file_name") && rcvMsg.attributes.ContainsKey("result_file_name"))
                {
                    Action<string, string, string> result = (string testFileName, string resultFileName, string res) =>
                    {
                        addResult(testFileName, resultFileName, res);
                    };
                    Dispatcher.Invoke(result, new Object[] { rcvMsg.attributes["test_file_name"],
                        rcvMsg.attributes["result_file_name"], rcvMsg.attributes["result"] });
                }
            };
            addClientProc("result", getResults);
        }

        //----< load getFiles processing into dispatcher dictionary >------
        private void DispatcherLoadGetFiles()
        {
            Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    clearFiles();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addFile(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("getFiles", getFiles);
        }
        //----< load all dispatcher processing >---------------------------

        private void loadDispatcher()
        {
            DispatcherLoadGetDirs();
            DispatcherLoadGetFiles();
            DispatcherRecieveTextMessages();
        }
        //----< start Comm, fill window display with dirs and files >------
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            setup();
            // start processing messages
            processMessages();
            // load dispatcher
            loadDispatcher();
            //set the initial dir
            pathStack_.Push(".");
            getDirsAndFiles();

        }

        //------< set up the translater and self endpoint >--------------------------------------------
        private void setup()
        {
            endPoint_ = new CsEndPoint();
            endPoint_.machineAddress = "localhost";
            endPoint_.port = selfPort;
            translater = new Translater();
            translater.listen(endPoint_);
            translater.setSaveFilePath(saveFilePath);
            translater.setSendFilePath(sendFilePath);
        }

        //------< get the files and dirs in the selected dir directory >------------------
        private void getDirsAndFiles()
        {
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = serverAddress;
            serverEndPoint.port = serverPort;
            PathTextBlock.Text = pathStack_.Peek();
            //compose the message with getFiles and getDirs command
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", pathStack_.Peek());
            translater.postMessage(msg);
            msg.remove("command");
            msg.add("pattern", "*.dll");
            msg.add("command", "getFiles");
            translater.postMessage(msg);
        }

        //----< strip off name of first part of path >---------------------
 
        private string removeFirstDir(string path)
        {
            string modifiedPath = path;
            int pos = path.IndexOf("/");
            modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);
            return modifiedPath;
        }

        //----< respond to mouse double-click on dir name >----------------
        private void DirList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            // build path for selected dir
            string selectedDir = (string)DirList.SelectedItem;
            if (selectedDir == null)
                return;
            string path;
            if (selectedDir == ".." || selectedDir == ".")
            {
                if (pathStack_.Count > 1)  // don't pop off "root directory"
                    pathStack_.Pop();
                else
                    return;
            }
            else
            {
                path = pathStack_.Peek() + "/" + selectedDir;
                pathStack_.Push(path);
            }

            getDirsAndFiles();
        }

        //------< respond to double click on results of the test requests >-----------
        private void ResultList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string selectedResult = (string) ResultList.SelectedItem;
            if (selectedResult == null) return;
            PopupWindow window = new PopupWindow();
            //set the title for the popup window
            window.Title = "Results for : " + Regex.Split(Regex.Split(selectedResult, ",")[2], "::")[1].Trim();
            try
            {
                //read the log file of results
                using (StreamReader sr = new StreamReader(saveFilePath + "/" + Regex.Split(Regex.Split(selectedResult, ",")[2], "::")[1].Trim()))
                {
                    window.Results.Text = sr.ReadToEnd();
                }
            }
            catch (Exception)
            {
                window.Results.Text = "Error reading file: " + Regex.Split(Regex.Split(selectedResult, ",")[2], "::")[1].Trim();
            }

            window.Show();

        }
        //----< handling double click on file list item >---------------------------------
        private void FileList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {

            string selectedFile = (string)FileList.SelectedItem;
            if (selectedFile == null || !selectedFile.EndsWith(".dll")) return;

            // build message to get dirs and post it
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = serverAddress;
            serverEndPoint.port = serverPort;
            //compose a message to test along with the file to be tested
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "testReq");
            msg.attributes.Add("sendingFile", pathStack_.Peek() + "/" + selectedFile);
            translater.postMessage(msg);
        }

        //----< used for demo : perform dummy click on the directory >-------------------------
        void clickDir()
        {
            MouseButtonEventArgs e = new MouseButtonEventArgs(null, 0, 0);
            DirList.SelectedIndex = 1;
            DirList_MouseDoubleClick(this, e);
        }

        //-----< used for demo : performs dummy click on the file list item >------------------
        void clickFile(int fileIndex)
        {
            MouseButtonEventArgs e = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, 0);
            FileList.SelectedIndex = fileIndex;
            FileList_MouseDoubleClick(this, e);
        }

        //-----< used for demo : performs dummy click on the result list item >-----------------
        void clickResult(int resultIndex)
        {
            MouseButtonEventArgs e = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, 0);
            ResultList.SelectedIndex = resultIndex;
            ResultList_MouseDoubleClick(this, e);
        }
    }
}
