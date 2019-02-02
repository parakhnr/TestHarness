#pragma once
/////////////////////////////////////////////////////////////////////////////
// ITest.h - Helper interfaces to implement and load tests                 //               
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
* - This module provides interfaces for managing the Tests.
* - ITest interface provide an abstraction for the tests, thus the executor
*   can be generalized to accept the Tests that implement this interface.
*   This helps in loading the tests from Dll effectively, as we can simply
*   refer to the classes that implement ITest as an instance of ITest. 
* - The ITests interface provide an easy way to load a collection of 
*   tests.
*
* Provided classes:
* ------------------
* IHostedLogger : Provides a interface to write a message (to a logger).
* Specifically designed so that the Tests within the Dll can log without
* being aware of the underlying implementation.
* ILogger : Provide a interface to logger functionalities.
*
* Required packages:
* ------------------
* ILogger.h 
*
* Maintainence History:
* ---------------------
* Version 1.1 - 10th October 2018
* - Allowing functions to be exported in the DLL using declspec decorator.
* - Adding a means to pass IHostedLogger to log.
* Version 1.0 - 22nd September 2018
* -Initial release
*/

#include <iostream>
#include <vector>
#include "ILogger.h"

#ifdef IN_DLL 
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

/////////////////////////////////////////////////////////////////////
// ITest interface
// - provides an interface for execution of the test

class DLL_DECL ITest {
public:
	using Author = std::string;
	using Name = std::string;

	virtual bool execute() = 0;
	virtual Author author() = 0;
	virtual Name name() = 0;
	virtual void setHostedLogger(IHostedLogger* logger) = 0;
};

///////////////////////////////////////////////////////////////////
//ITests interface
// -Provide an interface to a collection of ITests

class DLL_DECL ITests {
public:
	virtual std::vector<ITest*> getTests() = 0;
};

extern "C" {
	////////////////////////////////////////////////////////////////
	// getITests function
	// - An interface to provide access to list of tests from a DLL
	DLL_DECL ITests* getITests();
};