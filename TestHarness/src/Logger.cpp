#pragma once
#include "Logger.h"

//.........< add the stream to collection of streams >----------------------
void Logger::registerStream(std::ostream * stream) {
	_streams.push_back(stream);
}

//.........< clearing all the registered streams >--------------------------
void Logger::clear() {
	std::cout << "\nClearing all the registered streams to logger\n";
	_streams.clear();
}

//.........< write the message to registered streams >----------------------
void Logger::write(const std::string & message) {
	_mtx.lock();
	for (auto stream : _streams) {
		if (stream) *stream << message;
		else std::cout << "Something went wrong with the stream " << std::endl;
	}
	_mtx.unlock();
}


#ifdef TEST_LOGGER
#include "Logger.h"
#include <iostream>
int main() {

	std::cout << "\nTesting Logger begins\n";
	Logger logger;
	logger.registerStream(&std::cout);
	logger.write("\nWriting to console\n");
	logger.clear();
	std::cout << "\nTesting Logger ends\n";


	std::cout << "\nTesting HostedLogger begins\n";
	HostedLogger hostedLogger(&logger);
	logger.registerStream(&std::cout);
	hostedLogger.write("\nWriting to console again\n");

	logger.clear();

	return 0;
}


#endif
