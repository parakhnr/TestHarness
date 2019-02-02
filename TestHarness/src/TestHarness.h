/////////////////////////////////////////////////////////////////////////////
// TestHarness.h - Represents a child process for the testharness          //
//                     core to carry out the processing of the test        //
//                     requests.                                           //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2017                             //
// Platform:    Dell Inspiron 7378, Core i5, Windows 10                    //
// Application: Graduate coursework, Fall - 2018                           //               
// Author:      Naman Parakh, Syracuse University                          //
//              nrparakh@syr.edu                                           //
/////////////////////////////////////////////////////////////////////////////

/*
* Module Description:
* ------------------
* This module demonstrates the use of processing a dll that comprise of all
* the tests cases and returning back the results to the client. The module
* receives the test request from the TestHarnesCore, which comprise of the client
* address the request is arising from along with the dll to be processed.
* It then process this dll by executing all the tests that implement the ITest interface
* within the dll and returns back the result to the client as log file. The module
* uses Comm for communication both with the client and the core. 
*
* Operations performed:
* ---------------------
* - Wait for the request from TestHarnessCore to perform testing on a dll at a given path.
* - Load and extract the tests that implement ITest within the dll.
* - Execute the above tests and return the result back to the client as a log file.
*
* Required Projects:
* ------------------
*  - Message          constructs and parses HTTP style messages.
*  - MsgPassingComm   Sends and Retrieves messages.
*  - Sockets          Used by Comm to send and receive strings and bytes.
*
* Required Files:
* ---------------
* Comm.h Message.h Logger.h
*
* Maintainence History:
* ---------------------
* Version 1.2 - 5th December 2018
* - Modify the message content when replying to client.
* - TestHarness now composes a Comm instance
* Version 1.1 - 10th November 2018
* - Adding support for communication over to comm to process a test dll
* Version 1.0 - 7th November 2018
* -Initial release
*/
#pragma once
#include "../../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include "../../CppCommWithFileXfer/Message/Message.h"
#include "Logger.h"

using namespace MsgPassingCommunication;

class TestHarness {
private:
	EndPoint selfEndPoint;
	EndPoint coreEndPoint;
	std::string name;
	HostedLogger* hostedLogger;
	Comm comm;
public:
	TestHarness(EndPoint& selfEndPoint, EndPoint& coreEndPoint, 
		std::string& name, HostedLogger* hostedLogger);
	void start();
	void setSendFilePath(std::string& filePath) { comm.setSendFilePath(filePath); }
	void setSaveFilePath(std::string& filePath) { comm.setSaveFilePath(filePath); }
	void sendMessage(Message& message);
	void notifyCore();
	void notifyClient(Message& message);
	void processDLL(std::string path, EndPoint& endPoint);
	void receiveMessage();
	~TestHarness() {}
	std::string getName() {
		return name;
	}
};