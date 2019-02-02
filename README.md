# Test Harness - Object Oriented Design using C++ (CSE 687 Syracuse University)
## Name: Naman Parakh
## Professor: Jim Fawcett

The project is designed to allow multiple users to test the dlls which contain the tests. The project comprise of following modules which were  developed as part of the project whereas the rest of the modules were provided by the professor:

* GUI : A C# based windows application that allows users to choose the dll they want to test and display the results of each of the test in separate windows.

* TestHarnessCore: Act as core module for the project. The functionalities of this module are:
  * Creating a process pool by spawning and managing the lifecycle of TestHarness process.
  * Handle the requests from the client and forward it to available process from the process pool.

* TestHarness: Process the Test request from the TestHarnessCore by loading the required dll and performing tests. Return the result back to the client and free itself to accept any other requests.

* TestDLL: A dummy project to create the test dll for testing.

* TestRequirements: Demonstrate the requirements of the project. Also used to demonstrate as testing dll.

## Build and execute instructions:
* Download Visual studio 2017.
* Ensure you have .Net package installed. (Required for GUI project)
* Load the TestHarness.sln into visual studio. 
* Provide the name of the GUI project, hostname of the client, port number of the server, port number of the client, the base directory path where you want to load the dlls from, name of the client as command line arguments (Check demo command line in run.bat).
* Run the project.
* Building and executing using Terminal:
  * Ensure you have devenv.exe(a visual studio component) on your path.
  * Execute compile.bat
  * Execute run.bat