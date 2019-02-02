cd "bin\outputs\Debug\"
start TestHarnessCore.exe
start WpfApp1.exe localhost 9001 8082 . client1
start WpfApp1.exe localhost 9001 8083 . client2