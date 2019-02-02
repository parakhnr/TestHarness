/////////////////////////////////////////////////////////////////////////////
// Logger.h - Concrete implementation of Logger functionalities            //               
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2017                             //
// Platform:    Dell Inspiron 7378, Core i5, Windows 10                    //
// Application: Graduate coursework, Fall - 2018                           //               
// Author:      Naman Parakh, Syracuse University                          //
//              nrparakh@syr.edu                                           //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations and Design approach:
* --------------------------------------
* - This module provides implementation of IHostedLogger and ILogger interfaces.
* - Logger provides the functionality to register a stream, clear registered
*   streams and write to the streams.
* - "A single instance" of Logger gurantees thread safe usage with use of mutex.
* - HostedLogger is an implementation of IHostedLogger which provides a
*   abstract mechanism for allowing the Tests in Dll or others to log without
*   prior understanding of the implementation.
* -  To accomplish this HostedLogger holds an reference to the Logger which it
*   it uses to write.
*
*
* Provided classes:
* ------------------
* Logger : Provides implementation of ILogger interface
* HostedLogger : Provides implementation of IHostedLogger interface.
*
* Required Files:
* ---------------
* ILogger.h
*
* Maintainence History:
* ---------------------
* Version 1.2 - 8th November 2018.
* - Added functionality to clear and add streams to hostedlogger class.
* Version 1.1 - 10th October 2018
* - Logger now implements ILogger.
* - Write operation on logger instance uses mutex for thread safe writing.
* - Addition of HostedLogger to provide implementation of IHostedLogger
*   for the test executives to use.
* Version 1.0 - 22nd September 2018
* -Initial release
*/


#pragma once
#include "testutils/ILogger.h"
#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <fstream>

////////////////////////////////////////////////////////////////////////////////////
// Logger class provides concrete implementation for the ILogger interface.
// This instance of Logger can be used to write logs to various streams.

class Logger : public ILogger {
public:
	void registerStream(std::ostream* stream);
	void clear();
	void write(const std::string& message);
private:
	std::vector<std::ostream*> _streams;
	std::mutex _mtx;
};

////////////////////////////////////////////////////////////////////////////////////
// HostedLogger class provides concrete implementation for the IHostedLogger interface.
// Instance of HostedLogger allows the tests inside the dlls to log.

class HostedLogger : public IHostedLogger {
public:
	HostedLogger(Logger* logger) : logger(logger) {}
	~HostedLogger() {
		if (logger) logger->clear();
	}
	//---<write a message to the logger >-----------------------------
	void write(const std::string& message) {
		if (logger) logger->write(message);
	}
	//---< clear all the registered streams for the logger >----------
	void clear() {
		if (logger) logger->clear();
	}
	//---< adds a stream to logger >----------------------------------
	void addStream(std::ostream* stream) {
		if (logger) logger->registerStream(stream);
	}
private:
	Logger *logger;
};

