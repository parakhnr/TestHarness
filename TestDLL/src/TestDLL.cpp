///////////////////////////////////////////////////////////////////////
// TestDLL.cpp - Provides tests as part of dll                       //
// ver 1.2                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Naman Parakh, Syracuse University, CST 4-187         //
//              nrparakh@syr.edu                                     //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides classes:
* - ProcessCmdLineTest  tests the processcmdLine class
* - ConverterTest       tests the converter class
* - BoxTest             tests the box class
* - PersistFactoryTest  tests the persisit factory class
* - TestDLLTests        container of all the above tests
*
* Maintenance History:
* --------------------
* ver 1.0 : 18th September 2018
* - first release
*/

#pragma once
#include "TestDLL.h"
#include "../../TestHarness/src/testutils/ILogger.h"
#include "../../TestHarness/src/testutils/ITest.h"
#include <iostream>
#include <cstring>

using namespace CodeUtilities;

//////////////////////////////////////////////////////////////////////////////
// ConverterTest class
// - Test for the converter class in Codeutilities.h
// - Converts a integer to string and compare to the actual string value
// - Convert the string back to integer and compare to the actual integer
//	 value
// Test : Input = 10 ; Desired output = "10";

class DLL_DECL ConverterTest : public ITest {
public:
	bool execute() {
		const int magic = 10;
		std::string magicString = Converter<int>::toString(magic);
		return !magicString.compare("10") && Converter<int>::toValue(magicString) == magic;
	}

	Author author() {
		return "Jim Moriarity";
	}

	Name name() {
		return "Converter Test";
	}

	void setHostedLogger(IHostedLogger* logger) {}
};

///////////////////////////////////////////////////////////////////////////////
// PersistFactoryTest class
// - Test for the PersistFactory class in Codeutilities.h
// - Test whether the wrapped type of the class preserves the actual 
//   type and the operator operations.
// Test : Input : 3.1415297; Desired output : 6.2830594;

class DLL_DECL PersistFactoryTest : public ITest {
public:
	bool execute() {
		const double pi = 3.1415297;
		PersistFactory<Box<double>> value = Box<double>(pi);
		value *= 2;
		return value == 2 * pi;
	}

	Author author() {
		return "Dr. John Watson";
	}

	Name name() {
		return "PresistFactory Test";
	}

	void setHostedLogger(IHostedLogger* logger) {}
};

//////////////////////////////////////////////////////////////////////////////////
// BoxTest class
// - Test for the BoxTest class in Codeutilities.h
// - Test whether the wrapped type the class preserves the actual type
//   by comparing to the original value
// Test : Input : 3.1415297; Desired output : 3.1415297;
//
class DLL_DECL BoxTest : public ITest {
public:
	bool execute() {
		const double pi = 3.1415297;
		Box<double> boxedDouble = pi;
		return boxedDouble == pi;
	}

	Author author() {
		return "Sherlock Holmes";
	}

	Name name() {
		return "Box Test";
	}

	void setHostedLogger(IHostedLogger* logger) {}
};

//////////////////////////////////////////////////////////////////////////////////
// ProcessCommandLineTest class
// - Test for the ProcessCmdLine class in Codeutilities.h
// - Test whether the class shows the correct path which is passed as
//   argv[2] in argv.
//  Test : Input - char* argv[]; Desired output : all options processed from argv
//
// Note : Since the argv is supplied as a command line argument, the 
// test case to test against the acutal output varies and hence the  
// current output may or may not match.
//
class DLL_DECL ProcessCommandLineTest : public ITest {
private:

	IHostedLogger* logger;
public:
	bool execute() {
		const int SIZE = 10;
		int argc = 2;
		char ** argv = new char*[argc];
		argv[0] = new char[SIZE];
		argv[1] = new char[SIZE];

		strcpy_s(argv[0], SIZE, "Hello");
		strcpy_s(argv[1], SIZE, "abcd");

		std::ostringstream out;
		ProcessCmdLine cmdLine{ argc, argv, out };

		logger->write("\nBefore adding option : " + out.str());
		cmdLine.option(97);
		cmdLine.showOptions();
		logger->write("\nAfter adding option : " + out.str());

		delete argv[0];
		delete argv[1];
		delete[] argv;
		return out.str().compare("//a ");
	}

	Author author() {
		return "Arthur Conan Doyle";
	}

	Name name() {
		return "Process command line Test";
	}

	void setHostedLogger(IHostedLogger* l) {
		logger = l;
	}
};

//////////////////////////////////////////////////////////////////////////////////
// TestDLLTests class
// - Contains a container that holds all the tests to be performed 

class DLL_DECL TestDllTests : public ITests {
public:
	std::vector<ITest*> getTests() {
		std::vector<ITest*> _tests;
		_tests.push_back(new BoxTest());
		_tests.push_back(new PersistFactoryTest());
		_tests.push_back(new ConverterTest());
		_tests.push_back(new ProcessCommandLineTest());
		return _tests;
	}
};

//-----< returns reference to the class that holds the tests container >---------------------------
DLL_DECL ITests* getITests() {
	return new TestDllTests();
}