#pragma once
#ifdef TEST_FILEHELPERS
#include <iostream>
#include "FileHelpers.h"

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cout << "Please enter the path\n";
		return 0;
	}

	std::string path = argv[1];
	FileHelpers::showDirContents(path, "\nShowing all the files for directory : " + *argv[1]);
	FileHelpers::showDirContents(path, "\nShowing only the cpp files for directory : " + *argv[1], ".*(.cpp)$");

	std::cout << "\nFetching all the list of .h files from directory: " + *argv[1];
	auto files = FileHelpers::listFiles(path, ".*(.h)$");
	for (auto file : files) {
		std::cout << file << std::endl;
	}

	FileHelpers::showFileLines(path + "TestHarness/src/Logger.h", 0, 10, "\nShowing file contents");

	return 0;
}
#endif