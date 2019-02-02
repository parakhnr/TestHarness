#pragma once
/////////////////////////////////////////////////////////////////////////////
// DllTestUtils.h - Helper class for Dll Test executions                   //               
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2017                             //
// Platform:    Dell Inspiron 7378, Core i5, Windows 10                    //
// Application: Graduate coursework, Fall - 2018                           //               
// Author:      Naman Parakh, Syracuse University                          //
//              nrparakh@syr.edu                                           //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* -------------------
* -This module provides utility classes for managing the test execution
* for the tests in the DLL file.
*
* Design Approach:
* ----------------
* -The loading and the execution part of the Tests within a single Dll is
*  is managed by class DllTestExector. This approach allows to manage the
*  DLL resources as well as the test execution in one single class.
*  Decoupling of both the features is not done, cause separating loader doesn't provide enough
*  abstraction, as the loading a function from the dll returns a address which
*  is supposed to be bound to a function or variable, so its hard to generalize
*  this binding due to lack of information about the function/variable.
* -We can a design a loader specific to loading of the ITests from the DLL and
*  the design an executor that loads the tests using the loader and perform
*  execution. (This is an alternative approach)
*
* Provided classes:
* ------------------
* DllTestExecutor : Provides loading and execution of the all the tests within
* a DLL.
* DllTestExecutive : Provides the functionality to load multiple DLL's
* within the specified directory and perform the tests on each of them.
*
* Required Files:
* ---------------
* Test.h
*
* Maintainence History:
* ---------------------
* Version 1.1 - 9th October 2018
* - Add functionalites to add all the dlls from a directory and adding a single dll
* 
* Version 1.0 - 10th October 2018
* -Initial release
*/
#include "Test.h"
#include <windows.h>

////////////////////////////////////////////////////////////////////////////////////
// DllTestExecutor class provides following functionalities.
// 1. Load all the test that implement ITest interface from the specified DLL.
// 2. Register and execute all the ITests within that DLL.
// 3. Unload the loaded the DLL to free up its resources.

class DllTestExecutor : public TestExecutive {
public:
	bool loadAndExtract(const std::string& dllPath);
	void unload();
	~DllTestExecutor();
private:
	HMODULE hmod;
};

/////////////////////////////////////////////////////////////////////////////////////
// DllTestExecutive class provides a "Test Suite" for implementing all the tests from
// the DLL within a specified directory.

class DllTestExecutive : public TestExecutive {
public:
	bool doTests(IHostedLogger* logger);
	void addDllsFromDir(std::string& dllDir);
	void addDll(std::string& dllPath);
private:
	std::vector<std::string> dllPaths;
};