
/////////////////////////////////////////////////////////////////////////////
// TestRequirements.cpp - Tests for the all the requirements in project 3  //               
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
* This module provide the tests for all the requirements of the project 3.
*
* Required Files:
* ---------------
* ILogger.h FileUtilities.h ITest.h
*
* Maintainence History:
* ---------------------
* Version 1.0 : 8th November 2017
* - Initial release
*/

#include "../../TestHarness/src/testutils/ILogger.h"
#include "../../TestHarness/src/testutils/ITest.h"
#include <iostream>
#include <sstream>
#include "../../CppCommWithFileXfer/Utilities/FileUtilities/FileUtilities.h"

using namespace Utilities;

//////////////////////////////////////////////////////////////////////////////
// Requirement 1
// - Demonstrate Use of c++

class DLL_DECL Requirement1 : public ITest {
public:
	bool execute() {
		std::ostringstream stream;
		std::string msg = "Requirement 1 - Use of C++ files\n";
		Patterns patterns;
		patterns.push_back("*.cpp");
		patterns.push_back("*.h");
		showDirContents("../../../TestHarness/src", msg, patterns, stream);
		logger->write(stream.str());
		return true;
	}

	Author author() {
		return "Jim Moriarity";
	}

	Name name() {
		return "Requirement 1";
	}

	void setHostedLogger(IHostedLogger* hostedLogger) {
		logger = hostedLogger;
	}
private:
	IHostedLogger* logger;
};

//////////////////////////////////////////////////////////////////////////////
// Requirement2
// - Demonstrate use of WPF for user display

class DLL_DECL Requirement2 : public ITest {
public:
	bool execute() {
		std::string msg = "Requirement 2 - Use of WPF";
		std::ostringstream stream;
		Patterns patterns;
		showDirContents("../../../GUI/", msg, patterns, stream);
		logger->write(stream.str());
		return true;
	}

	Author author() {
		return "Jim Moriarity";
	}

	Name name() {
		return "Requirement 2";
	}

	void setHostedLogger(IHostedLogger* hostedLogger) {
		logger = hostedLogger;
	}
private:
	IHostedLogger* logger;
};


//////////////////////////////////////////////////////////////////////////////
// Requirement3
// -Demonstrate asynchronous client

class DLL_DECL Requirement3 : public ITest {
public:
	bool execute() {
	    std::string msg = "Requirement 3 - Demonstrate client doesn't wait after sending request to the server for receveing the reply";
		std::ostringstream stream;
		showFileLines("../../../GUI/MainWindow.xaml.cs", 202, 220, msg, stream);
		showFileLines("../../../GUI/MainWindow.xaml.cs", 359, 377, msg, stream);
		logger->write(stream.str());
		return true;
	}

	Author author() {
		return "Jim Moriarity";
	}

	Name name() {
		return "Requirement 3";
	}

	void setHostedLogger(IHostedLogger* hostedLogger) {
		logger = hostedLogger;
	}
private:
	IHostedLogger* logger;
};

//////////////////////////////////////////////////////////////////////////////
// Requirement4
// - Demonstrate display of GUI

class DLL_DECL Requirement4 : public ITest {
public:
	bool execute() {
		std::string msg = "Requirement 4 - Display the GUI";
		std::ostringstream stream;
		displayFileContents(msg, "../../../GUI/MainWindow.xaml", stream);
		displayFileContents(msg, "../../../GUI/PopupWindow.xaml", stream);
		logger->write(stream.str());
		return true;
	}

	Author author() {
		return "Jim Moriarity";
	}

	Name name() {
		return "Requirement 4";
	}

	void setHostedLogger(IHostedLogger* hostedLogger) {
		logger = hostedLogger;
	}
private:
	IHostedLogger* logger;
};


//////////////////////////////////////////////////////////////////////////////
// Requirement5
// - Demonstrate different messages as per requirement

class DLL_DECL Requirement5 : public ITest {
public:
	bool execute() {
		std::string msg = "Requirement 5 - Demonstration of messages as per requirement";
		std::ostringstream stream;
		stream << "\n Get directories and Get files message format";
		showFileLines("../../../GUI/MainWindow.xaml.cs", 289, 398, msg, stream);
		stream << "\n Test request message format";
		showFileLines("../../../GUI/MainWindow.xaml.cs", 371, 376, msg, stream);
		stream << "\n Test request result message format";
		showFileLines("../../../TestHarness/src/TestHarness.cpp", 67, 72, msg, stream);
		logger->write(stream.str());
		return true;
	}

	Author author() {
		return "Jim Moriarity";
	}

	Name name() {
		return "Requirement 5";
	}

	void setHostedLogger(IHostedLogger* hostedLogger) {
		logger = hostedLogger;
	}
private:
	IHostedLogger* logger;
};

//////////////////////////////////////////////////////////////////////////////
// Requirement6
// - Demonstrate sending test request from client to server
class DLL_DECL Requirement6 : public ITest {
public:
	bool execute() {
		std::string msg = "Requirement 6 -  Sending Test request from client to server";
		std::ostringstream stream;
		showFileLines("../../../GUI/MainWindow.xaml.cs", 359, 377, msg, stream);
		logger->write(stream.str());
		return true;
	}

	Author author() {
		return "Jim Moriarity";
	}

	Name name() {
		return "Requirement 6";
	}

	void setHostedLogger(IHostedLogger* hostedLogger) {
		logger = hostedLogger;
	}
private:
	IHostedLogger* logger;
};

//////////////////////////////////////////////////////////////////////////////
// Requirement7
// - Demonstrate executing tests as series of test requests in child process
class DLL_DECL Requirement7 : public ITest {
public:
	bool execute() {
		std::string msg = "Requirement 7 -  Executing tests as series of test requests in child process";
		std::ostringstream stream;
		stream << "\n Listening for test request and forwarding it to child";
		showFileLines("../../../TestHarnessCore/src/TestHarnessCore.cpp", 125, 157, msg, stream);
		stream << "\n Child processing the test request";
		showFileLines("../../../TestHarness/src/TestHarness.cpp", 112, 133, msg, stream);
		showFileLines("../../../TestHarness/src/TestHarness.cpp", 47, 77, msg, stream);

		logger->write(stream.str());
		return true;
	}

	Author author() {
		return "Jim Moriarity";
	}

	Name name() {
		return "Requirement 7";
	}

	void setHostedLogger(IHostedLogger* hostedLogger) {
		logger = hostedLogger;
	}
private:
	IHostedLogger* logger;
};


//////////////////////////////////////////////////////////////////////////////
// Requirement8
// - Demonstrate sending result messages post testing the dll
class DLL_DECL Requirement8 : public ITest {
public:
	bool execute() {
		std::string msg = "Requirement 8 - Sending result messages post testing the dll";
		std::ostringstream stream;
		showFileLines("../../../TestHarness/src/TestHarness.cpp", 47, 77, msg, stream);
		logger->write(stream.str());
		return true;
	}

	Author author() {
		return "Jim Moriarity";
	}

	Name name() {
		return "Requirement 8";
	}

	void setHostedLogger(IHostedLogger* hostedLogger) {
		logger = hostedLogger;
	}
private:
	IHostedLogger* logger;
};


//////////////////////////////////////////////////////////////////////////////
// Requirement9
// - Demonstrate concurrent running of two or more clients.
class DLL_DECL Requirement9 : public ITest {
public:
	bool execute() {
		std::string msg = "Requirement 9 - Concurrent running of two or more client";
		std::ostringstream stream;
		showFileLines("../../../TestHarness/src/TestHarness.cpp", 47, 77, msg, stream);
		logger->write(stream.str());
		return true;
	}

	Author author() {
		return "Jim Moriarity";
	}

	Name name() {
		return "Requirement 9";
	}

	void setHostedLogger(IHostedLogger* hostedLogger) {
		logger = hostedLogger;
	}
private:
	IHostedLogger* logger;
};


//////////////////////////////////////////////////////////////////////////////
// Requirement 10
// - Container for the all the tests to performed for the testing the project3
//   requirements

class DLL_DECL TestRequirements : public ITests {
public:
	std::vector<ITest*> getTests() {
		std::vector<ITest*> _tests;
		_tests.push_back(new Requirement1());
		_tests.push_back(new Requirement2());
		_tests.push_back(new Requirement3());
		_tests.push_back(new Requirement4());
		_tests.push_back(new Requirement5());
		_tests.push_back(new Requirement6());
		_tests.push_back(new Requirement7());
		_tests.push_back(new Requirement8());
		_tests.push_back(new Requirement9());
		return _tests;
	}
};

//------< utility functiton to get the reference to test container >-------------
DLL_DECL ITests* getITests() {
	return new TestRequirements();
}