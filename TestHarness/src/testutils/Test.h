#pragma once
/////////////////////////////////////////////////////////////////////////////
// TestUtils.h - Utility class for writing Tests and TestExecutors         //               
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
* This module provides the utility for creating and executing tests.
*
* This package provide classes:
* -----------------------------
* ITest - Interface for implementing tests.
* TestExecutive - A suite for executing multiple tests.
* TestExecutor - A test executor for individual test.
*
* Required Files:
* ---------------
* ILogger.h ITest.h
*
* Maintainence History:
* ---------------------
* Version 1.1 : 10th Oct 2018
* - Adding support for IHostedLogger in doTests().
* 
* Version 1.0 : 22nd September 2018
* - Initial release
*/


#include "ILogger.h"
#include "ITest.h"
#include <iostream>
#include <memory>
#include <vector>
#include <string>

///////////////////////////////////////////////////////////////////////////
// TestExecutive class
// - provide an ability to execute sequence of tests that implement the 
//   the ITest interface
// - provides an ability to add a test case to the container of test cases
// - provides an ability to clear the container ot test cases.

class TestExecutive {
public:
	virtual bool doTests(IHostedLogger* logger);
	void registerTest(ITest* iTest);
	void clearTests();
	virtual ~TestExecutive();
private:
	using Tests = std::vector<ITest*>;
	Tests tests_;
};

////////////////////////////////////////////////////////////////////////
// TestExecutor class
// - provides an interface for executing the test

class TestExecutor {
public:
	bool execute(ITest* iTest, const std::string& loggerPrefix, IHostedLogger* logger);
};


