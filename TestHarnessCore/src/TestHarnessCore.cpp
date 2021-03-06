#pragma once
#include "../../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include "../../CppCommWithFileXfer/Utilities/FileSystem/FileSystem.h"
#include "TestHarnessCore.h"
#include <iostream>
#include <thread>
#include <functional>

using namespace MsgPassingCommunication;


int main(){
	::SetConsoleTitle(L"Core");

	//create and start a comm
	EndPoint selfEndPoint(HOST_ADDRESS, HOST_PORT);
	TestHarnessCore core(selfEndPoint);
	//start the core
	core.start();

	//listen for the messages.
	std::function<void()> tproc = [&]() { core.readMessages(); };
	std::thread tget(tproc);
	tget.join();

	//shutdown the core
	core.shutdown();
	std::cin.get();
}

//------< read the messages from the comm and perform actions according to message command >------------
void TestHarnessCore::readMessages(){
	std::cout << "\n\nReading messages";
	Message msg;
	while (true){
		msg = comm.getMessage();
		if (msg.command() == "") continue;
		std::cout << "\nMessage received:\n" << msg.toString();
		if (msg.command() == "stop") {
			//stop the core from reading any further messages
			stop = true;
			std::cout << "\nStopped reading messages\n";
			return;
		} else if (msg.command() == "ready") {
			//one of the process is ready - enq to the readyQ
			readyQ_.enQ(msg.from());
		} else if (msg.command() == "testReq") {
			//received a test request - enq to the requestQ
			requestQ_.enQ(msg);
		} else if (msg.command() == "getFiles") {
			//reply with the list of files
			comm.postMessage(getFiles(msg));
		} else if (msg.command() == "getDirs") {
			//reply with the list of directories
			comm.postMessage(getDirs(msg));
		}
	}
}

//------< compose a message with list of all the directories in specified directory >--------------
Message TestHarnessCore::getDirs(Message& msg) {
	Message reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getDirs");
	std::string path = msg.value("path");
	if (path != "")
	{
		std::string searchPath = ".";
		if (path != ".")
			searchPath = searchPath + "\\" + path;
		std::vector<std::string> dirs = FileSystem::Directory::getDirectories(searchPath);
		size_t count = 0;
		for (auto item : dirs)
		{
			if (item != ".." && item != ".")
			{
				std::string countStr = Utilities::Converter<size_t>::toString(++count);
				reply.attribute("dir" + countStr, item);
			}
		}
	}
	else
	{
		std::cout << "\n  getDirs message did not define a path attribute";
	}
	return reply;
}

//------< compose a message with list of all the files in the specified directory >--------------
Message TestHarnessCore::getFiles(Message& msg) {
	Message reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getFiles");
	std::string path = msg.value("path");
	std::string pattern = msg.value("pattern");
	if (pattern == "") pattern = "*.*";
	if (path != "")
	{
		std::string searchPath = ".";
		if (path != ".")
			searchPath = searchPath + "\\" + path;
		std::vector<std::string> files = FileSystem::Directory::getFiles(searchPath, pattern);
		size_t count = 0;
		for (auto item : files)
		{
			std::string countStr = Utilities::Converter<size_t>::toString(++count);
			reply.attribute("file" + countStr, item);
		}
	}
	else
	{
		std::cout << "\n  getFiles message did not define a path attribute";
	}
	return reply;
}

//------< send message using comm >---------------------------------------------------------------------
void TestHarnessCore::sendMessage(Message & message){
	std::cout << "\nSending message:\n" << message.toString();
	comm.postMessage(message);
}

//------< start the core by launching the process and listening for the test requests >-----------------
void TestHarnessCore::start(){
	comm.setSaveFilePath("core");
	comm.setSendFilePath("core");
	comm.start();
	//spawn the processes
	for (int i = 0; i < 2; i++) {
		int port = 9005 + i;
		EndPoint endPoint("localhost", port);
		TestHarnessCore::spawnProcess(endPoint);
	}

	//listen for the test request messages
	std::thread listenForTestRequests([&]() {
		while (true) {
			//check are we good to continue
			if (shouldStop()) {
				std::cout << "\nStopped reading any requests\n";
				return;
			}

			Message msg = requestQ_.deQ();
			if (msg.command() == "testReq") {
				//wait for the process to be available
				EndPoint ready = readyQ_.deQ();
				msg.to(ready);
				sendMessage(msg);
			} 
		}
	});
	listenForTestRequests.detach();
}

//-------< spawn a new process and map it to endpoint for tracking >--------------------------------------
bool TestHarnessCore::spawnProcess(EndPoint endPoint){
	std::cout << "\n\nCreating a child process listening at : " << endPoint.toString();
	Process process;
	std::string name = endPoint.address + std::to_string(endPoint.port);
	process.title(name);
	process.application("TestHarness.exe");
	{
		//create the command line for the execution of the dll.
		std::ostringstream stream;
		stream << name << " " << HOST_ADDRESS << " " << HOST_PORT << " localhost " << endPoint.port;
		process.commandLine(stream.str());
	}
	//create the process
	if (!process.create()) {
		std::cout << "\nFailed to create the process\n";
		return false;
	}
	//add it to the map of endpoint against the process
	TestHarnessCore::processMap[endPoint.toString()] = process;
	//add process eixt callback handler
	process.setCallBackProcessing([]() { handleProcessExit(); });
	process.registerCallback();
	std::cout << "\nChild process created successfully\n";
	return true;
}

//-------< handle the process exit by re spawning the terminated child process >--------------------------
void TestHarnessCore::handleProcessExit(){
	std::cout << "\n\nOne of the child process has exited";
	for (auto it = processMap.begin(); it != processMap.end(); it++) {
		auto processHandle = it->second.processHandle();
		if (!processHandle) continue;
		DWORD exitCode;
		GetExitCodeProcess(processHandle, &exitCode);
		if (exitCode == 0 || exitCode == 259) {
			//process is all good
			continue;
		}
		//process is bad
		EndPoint endPoint = EndPoint::fromString(it->first);
		processMap.erase(it->first);
		std::cout << "\nBad process was running at :: " << endPoint.toString() << std::endl;
		//create a new process
		if (!spawnProcess(endPoint)) {
			return;
		}
		return;
	}
}

//------< shutdown the comm gracefully >---------------------------------------------------------------
void TestHarnessCore::shutdown(){
	//ask all the process to stop gracefully.
	for (auto it = processMap.begin(); it != processMap.end(); it++) {
		Message msg;
		msg.to(EndPoint::fromString(it->first));
		msg.from(EndPoint(HOST_ADDRESS, HOST_PORT));
		msg.command("stop");
		sendMessage(msg);
	}
	comm.stop();
}

//------< utility to check whether the comm should stop >------------------------------------------------
inline bool TestHarnessCore::shouldStop() {
	return stop;
}

//------< clear up the process map >---------------------------------------------------------------------
TestHarnessCore::~TestHarnessCore(){
	processMap.clear();
}

//-----< static variable initialization >----------------------
std::map<std::string, Process> TestHarnessCore::processMap;