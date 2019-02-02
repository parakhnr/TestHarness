#pragma once
#include "Test.h"
#include "../utils/Utils.h"
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <sstream>

//----< add a test item to container of tests>----------------------------------------------------
void TestExecutive::registerTest(ITest* pTest) {
	tests_.push_back(pTest);
}

//----< clear the container of tests>-------------------------------------------------------------
void TestExecutive::clearTests() {
	std::cout << "\nClearing up all the tests for test executive \n";
	tests_.clear();
}

TestExecutive::~TestExecutive(){
	clearTests();
}


//-----< perform the test execution over each of the registered test>-----------------------------
bool TestExecutive::doTests(IHostedLogger* logger)
{
	TestExecutor tester;
	{
		std::ostringstream out;
		out << "Executing Test Executive \n";
		logger->write(out.str());
	}
	bool result = true;
	for (auto test : tests_) {
		test->setHostedLogger(logger);
		std::ostringstream out;
		out << "\n" << TimeUtils::getCurrentTime() << " Author - " << test->author() << ", Test - " << test->name() << " : ";
		bool tResult = tester.execute(test, out.str(), logger);
		result |= tResult;
	}
	{
		std::ostringstream out;
		if (result) out << "\nAll the test Passed for Test executive \n";
		else out << "\nAtlease one of the test Failed for Test executive \n";
		logger->write(out.str());
	}
	return result;
}

//-----<Execute the single test with exception handling>-----------------------------------------
bool TestExecutor::execute(ITest* pTest, const std::string& loggerPrefix, IHostedLogger* logger) {
	bool result = false;
	{
		std::ostringstream out;
		out << loggerPrefix << "Executing - " << pTest->name();
		logger->write(out.str());
	}

	try {
		result = pTest->execute();
	}
	catch (std::exception &ex) {
		{
			std::ostringstream out;
			out << loggerPrefix << "Exception occured : " << ex.what() << "\n";
			logger->write(out.str());
		}
	}

	{
		std::ostringstream out;
		out << loggerPrefix;
		if (result) out << pTest->name() << " : Result " <<" - Passed\n";
		else out << pTest->name() <<  " : Result " <<  " Failed\n";
		logger->write(out.str());
	}

	return result;
}

#ifdef TEST_TEST_EXECUTIVE
#include "../Logger.h"


class DLL_DECL TestPass : public ITest {
public:
	bool execute() {
		return true;
	}
	Author author() {
		return "Picakchu";
	}
	virtual Name name() {
		return "Pass Test";
	}
	void setHostedLogger(IHostedLogger* logger) {}

};


int main() {

	Logger* logger = new Logger();
	logger->registerStream(&std::cout);
	HostedLogger* hostedLogger = new HostedLogger(logger);

	TestExecutive executive;
	TestPass* testPass = new TestPass();
	executive.registerTest(testPass);
	executive.doTests(hostedLogger);

	delete hostedLogger;
	delete logger;
	return 0;
}

#endif