#pragma once
#define _WINSOCKAPI_
#include "testutils/Test.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Logger.h"
#include "utils/file/FileHelpers.h"
#include "testutils/DllTestUtils.h"
#include "TestHarness.h"
#include <stdio.h>

int main(int argc, char** argv){
	if (argc < 5) {
		std::cout << "You are missing the host name or port of core/child process.\n";
		std::cin.get();
		return 0;
	}

	std::string name = argv[0];
	::SetConsoleTitle(name.c_str());

	//get the core host details
	std::string coreHost = argv[1];
	int corePort = std::stoi(argv[2]);
	EndPoint coreEndPoint(coreHost, corePort);

	//get the host details for this process
	std::string selfHost = argv[3];
	int selfPort = std::stoi(argv[4]);
	EndPoint selfEndPoint(selfHost, selfPort);
	
	Logger logger;
	HostedLogger hostedLogger(&logger);
	TestHarness testHarness(selfEndPoint, coreEndPoint, name, &hostedLogger);
	testHarness.setSaveFilePath(name);
	testHarness.setSendFilePath(name);

	//start listening for the messages
	std::function<void()> tproc = [&]() { testHarness.start(); };
	std::thread tget(tproc);
	tget.join();

	return 0;
}

//------< load and execute the dll and when done notify the core with ready message and client with the results >--------
void TestHarness::processDLL(std::string fileName, EndPoint & clientEndPoint) {
	DllTestExecutive executive;
	std::string dllPath = name + "/" + fileName;
	executive.addDll(dllPath);

	//clear the logger for new output file
	const std::string logFileName = name + "\\" + fileName + "-log.txt";
	std::ofstream fStream(logFileName, std::ios::out);
	if (fStream.good()) hostedLogger->addStream(&fStream);
	hostedLogger->addStream(&std::cout);

	//perform the tests
	bool result = executive.doTests(hostedLogger);
	
	//clear the logger
	fStream.close();
	hostedLogger->clear();

	//notify the core with done and client with the result
	Message message;
	message.command("result");
	message.attributes()["result"] = result ? "Passed" : "Failed";
	message.file(fileName + "-log.txt");
	message.attributes()["test_file_name"] = fileName;
	message.attributes()["result_file_name"] = fileName + "-log.txt";
	
	message.to(clientEndPoint);
	notifyClient(message);
	notifyCore();
}

//----< constructor to initialize the endpoints of the core and self along with the name and hostedlogger >--------------------------------------
TestHarness::TestHarness(EndPoint & selfEndPoint, EndPoint & coreEndPoint, std::string& name, HostedLogger* hostedLogger) : selfEndPoint(selfEndPoint), 
coreEndPoint(coreEndPoint), name(name), hostedLogger(hostedLogger), comm(selfEndPoint) {}

//-----< starts the test harness by notifying the core with ready message and listening for other messages >-----------------------------------
void TestHarness::start(){
	comm.start();
	//notify the core with the ready message
	notifyCore();
	//wait for any messages from the core
	receiveMessage();
}

//-----< utility to send message using comm >----------------------------------------------------
void TestHarness::sendMessage(Message& message){
	std::cout << "\nSending message:\n" << message.toString();
	message.from(selfEndPoint);
	comm.postMessage(message);
}

//-----< utility to notify the core that its ready to serve the next request>---------------------
void TestHarness::notifyCore(){
	Message message;
	message.command("ready");
	message.to(coreEndPoint);
	sendMessage(message);
}

//-----< utility to notify the client with the desired message >----------------------------------
void TestHarness::notifyClient(Message & message){
	sendMessage(message);
}

//------< listen to the messages from the test harness core >--------------------------------------------------------------
void TestHarness::receiveMessage(){
	std::cout << "\nReading messages\n";
	while (1) {
		Message message = comm.getMessage();
		if (message.command() == "") continue;
		std::cout << "\nReceived message\n" << message.toString();
		const std::string command = message.command();
		if (command == "stop") {
			//stop reading any messages
			std::cout << "\nDone reading all the messages\n. Shutting down....";
			return;
		} else if (command == "testReq") {
			//process the test request
			const std::string clientHost = message.from().address;
			const int port = message.from().port;
			EndPoint clientEndpoint(clientHost, port);
			processDLL(message.file(), clientEndpoint);
		}
	}
}

