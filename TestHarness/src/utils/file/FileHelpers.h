#pragma once
/////////////////////////////////////////////////////////////////////////////
// FileHelpers.h - Helper class for file related operations                //               
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2017                             //
// Platform:    Dell Inspiron 7378, Core i5, Windows 10                    //
// Application: Graduate coursework, Fall - 2018                           //               
// Author:     Naman Parakh, Syracuse University                           //
//              nrparakh@syr.edu                                           //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* -------------------
* This module provides functionalities related to following file operations.
* 1. List all the files that match a specific pattern from the specified directory.
* 2. Display all the files that match a specific pattern from the specified directory.
* 3. Display the content of specific portion of a file. 
*
* Public Interfaces:
* --------------------
* FileHelpers :
* showDirContents(path, message, ostream)
* showDirContents(path, ostream)
* showFileLines(path, start, end, message, ostream)
*
* Required Files:
* ---------------
* Logger.h FileSystem.h FileSystem.cpp
*
* Maintainence History:
* ---------------------
* Version 1.1 - 10th October 2018
* - Modifying the function signature of ShowDirContents to accept regex for file names.
* - Addition of listFiles function to generalize the behaviour of recursing through directory 
*   to get file names.
* - Addition of relative path to the file name from the specified path.
*
* Version 1.0 - 22nd September 2018
* -Initial release
* 
*/
#include "FileSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <regex>


class FileHelpers
{
public:
	using Path = std::string;
	using Message = std::string;
	using Line = size_t;
	using File = std::string;
	using Files = std::vector<File>;

	static std::vector<std::string> listFiles(const Path& path, const std::string& fileNameRegex);
	static bool showDirContents(const Path& path, const Message& message, const std::string& fileNameRegex = ".", std::ostream& out = std::cout);
	static bool showFileLines(const Path& path, Line start, Line end, const Message& message, std::ostream& out = std::cout);

private:

	static std::string title(const std::string& title, const char underline = '=') {
		std::ostringstream out;
		out << "\n" << title;
		out << "\n" << std::string(title.size() + 2, underline);
		return out.str();
	}
};

//----<show directory contents with specific files that match the regex>-------------------------------------
inline std::vector<std::string> FileHelpers::listFiles(const Path& path, const std::string& fileNameRegex){
	std::vector<std::string> fileList;
	std::regex reg(fileNameRegex);

	if (!FileSystem::Directory::exists(path)) {
		std::cout << "\nDirectory not exist";
		return {};
	}

	Files files = FileSystem::Directory::getFiles(path);
	for (auto file : files) {
		std::string name = FileSystem::Path::getName(file);
		std::smatch match;
		std::cout << name << "\n";
		if (std::regex_search(name, match, reg)) {
			//match found add the path to the file name
			fileList.push_back(path + "/" + name);
		}
	}

	//get the files for the directories within the path
	auto directories = FileSystem::Directory::getDirectories(path);
	for (auto directory : directories) {
		if (directory.compare(".") == 0 || directory.compare("..") == 0) continue;
		auto recFileList = listFiles(path + "/" + directory, fileNameRegex);
		// append the nested result to current path
		if(recFileList.size() > 0) fileList.insert(fileList.end(), recFileList.begin(), recFileList.end()); 
	}
	return fileList;
}

//----<show directory contents where file names match specific regex along with a specified message>--------------------------------
inline bool FileHelpers::showDirContents(const Path& path, const Message& msg, const std::string& fileNameRegex, std::ostream& out) {
	if (msg.size() > 0)
		out << title(msg) << "\n";

	auto files = listFiles(path, fileNameRegex);
	for (auto file : files) {
		out << file << std::endl;
	}
	out << "\n";
	return files.size() > 0;
}


//-----<show lines of file from specified start and end lines>---------------------------------------------------------
inline bool FileHelpers::showFileLines(const Path& path, Line start, Line end, const Message& msg, std::ostream& out) {
	if (msg.size() > 0)
		out << title(msg);
	out << "\n  " << FileSystem::Path::getFullFileSpec(path);

	std::ifstream file(path);
	if (!file.good()) {
		out << "\n  can't open file";
		return false;
	}

	std::string line;
	size_t count = 0;
	while (std::getline(file, line))
	{
		if (++count < start) continue;
		if (count > end) break;
		out << "\n  " << std::setw(4) << count << " " << line;
	}
	out << "\n";
	return true;
}


