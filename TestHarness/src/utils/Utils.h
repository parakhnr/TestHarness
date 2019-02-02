#pragma once
/////////////////////////////////////////////////////////////////////////////
// Utilities.h - Helper class for shared functional utilities across       //   
//               different packages.                                       //
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
* This module provides shared functional utilities across packages.
* TimeUtils:
* - Provide time related utilities.
*
* Public Interfaces:
* --------------------
* TimeUtils :
* getCurrentTime() - Returns current local time formatted as string.
*
* Maintainence History:
* ---------------------
* Version 1.0 - 22nd September 2018 
* - Initial release
*/
#include <time.h>
#include <iostream>
#include <sstream>


namespace TimeUtils {

	//----< Returns current local time as a formatted string>--------------------
	/*
	* Note : Time Format used - Wed Sep 19 20:20:20 2017
	*/
	inline std::string getCurrentTime() {
		char timeBuff[26];
		struct tm localTime;

		time_t currentTime = time(nullptr);
		localtime_s(&localTime, &currentTime);
		strftime(timeBuff, 26, "%c", &localTime);
		return std::string(timeBuff);
	}
}
