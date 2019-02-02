#pragma once
/////////////////////////////////////////////////////////////////////////////
// ILogger.h - Helper class for providing Logging utilities                //               
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
* -This module provides interfaces for managing the logging functionalities.
* -The interfaces allow the Tests implemented in the DLL to log without
* an prior implementation about the logger.
* -This helps in allowing a centralized control over the logger wherein
* one entity can manage the lifecycle and implementation of the logger
* and provide a handler for others to log.
*
* Provided classes:
* ------------------
* IHostedLogger : Provides a interface to write a message (to a logger).
* Specifically designed so that the Tests within the Dll can log without
* being aware of the underlying implementation.
* ILogger : Provide a interface to logger functionalities.
*
*
* Maintainence History:
* ---------------------
* Version 1.0 - 10th October 2018
* -Initial release
*/

#include <iostream>
#include <mutex>

//////////////////////////////////////////////////////////////////////////
// IHostedLogger class
// Provide an interface to write a message, so that ITest can refer to 
// it for logging the messages, without worrying about its implementation

class IHostedLogger {
public:
	virtual void write(const std::string& message) = 0;
};

///////////////////////////////////////////////////////////////////////
// ILogger class
// Provide an interface for the logger to add stream, clear the streams
// and write messages.

class ILogger {
public:
	virtual void registerStream(std::ostream* stream) = 0;
	virtual void clear() = 0;
	virtual void write(const std::string& message) = 0;
};

