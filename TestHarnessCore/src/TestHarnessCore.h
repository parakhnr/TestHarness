/////////////////////////////////////////////////////////////////////////////
// TestHarnessCore.h - A core module for the test harnesss, with a purpose //
//                     of acting as load distributor and manager for       //
//                     handling the the requests from the client.          //
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
* This module act as the core module for the test harness where it acts as
* a central place for the client to communicate for carrying out the requests for
* tests. The module manages the load by creating several child processes using
* Windows process creation and distributes the load to them accordingly.
* Apart from just distributing the load it also tracks the status of the process
* and spawns new processess if the any of them die. This module uses Windows Process
* for creation and uses Win32 Sockets for inter process communication. BlockingQueue
* is used to listen for the messages from both the client and the child process.
* 
*
* Operations performed:
* ---------------------
* - Listen for messages from the client and child processes.
* - Redirect the test request from the client to the child.
* - Spawn new process if one of them die.
*
* Required Projects:
* ------------------
*  - Message          constructs and parses HTTP style messages.
*  - MsgPassingComm   Sends and Retrieves messages.
*  - Sockets          Used by Comm to send and receive strings and bytes.
*  - Process          Used to create process
* 
* Required Files:
* ---------------
* Comm.h Message.h BlockingQueue.h
*
* Maintainence History:
* ---------------------
* Version 1.1 - 5th December 2018
* - Adding support for getFiles and getDirs message command.
* - TestHarnessCore now composes an instance of Comm.
* Version 1.0 - 7th November 2018
* -Initial release
*/
#pragma once
#include "../../CppCommWithFileXfer/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../../CppCommWithFileXfer/Message/Message.h"
#include "../../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include <vector>
#include <map>
#include "../../Process/Process.h"

static const std::string HOST_ADDRESS = "localhost";
static const int HOST_PORT = 9001;

using namespace MsgPassingCommunication;

class TestHarnessCore {
public:
	void readMessages();
	void sendMessage(Message& message);
	void start();
	static bool spawnProcess(EndPoint endPoint);
	static void handleProcessExit();
	void shutdown();
	bool shouldStop();
	static Message getDirs(Message& message);
	static Message getFiles(Message& message);
	TestHarnessCore(EndPoint& endPoint) : comm(endPoint, "core"){}
	~TestHarnessCore();
private:
	static std::map<std::string, Process> processMap;
	Comm comm;
	BlockingQueue<EndPoint> readyQ_;
	BlockingQueue<Message> requestQ_;
	bool stop = false;
};