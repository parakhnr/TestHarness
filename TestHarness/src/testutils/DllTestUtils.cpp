#pragma once
#include "DllTestUtils.h"
#include "../utils/file/FileHelpers.h"

//------<load and extract all the tests from the specified dll path>-----------------
bool DllTestExecutor::loadAndExtract(const std::string & dllPath){
	//load the dll
	hmod = LoadLibraryA(dllPath.c_str());
	if (!hmod) return false;

	//get the reference to getItests function which contains ITests
	using gIts = ITests * (*)();
	gIts tests = (gIts)GetProcAddress(hmod, "getITests");

	if (tests) {
		//execute the getITests function
		ITests* iTests = tests();
		if (!iTests) {
			//No tests found
			return false;
		}
		for (auto iTest : iTests->getTests()) {
			registerTest(iTest);
		}
		return true;
	}

	//getITests function not found.
	return false;
}

//------<free the loaded DLL resource>------------------------------------------------
void DllTestExecutor::unload(){
	if (hmod) {
		std::cout << "Unloading the DLL\n";
		FreeLibrary(hmod);
	}
}

//-------< destrucutor for the dllexecutor >------------------------------------------
DllTestExecutor::~DllTestExecutor(){
	unload();
}

//------<perform the tests for all the dlls>-----------------------------------------
bool DllTestExecutive::doTests(IHostedLogger * logger){
	bool result = true;
	std::cout << "\nDLL Test Executive executing\n";
	for (std::string dllPath : dllPaths) {
		std::cout << "\nLoading Dll - " << dllPath << std::endl;
		DllTestExecutor executor;
		if (!executor.loadAndExtract(dllPath)) {
			std::cout << "\nLoading dll " << dllPath << " failed\n";
			result = false;
			continue;
		}
		result |= executor.doTests(logger);
	}

	if (result) std::cout << "\nAll the tests contained by the Dlls passed\n";
	else std::cout << "\nAtleast one of the test case from one of the Dll failed\n";

	return result;
}

//------< add all the dlls to test from the specified directory >------------------------
void DllTestExecutive::addDllsFromDir(std::string & dllDir){
	std::cout << "\nFetching all the Dll's from " << dllDir << std::endl;
	dllPaths = FileHelpers::listFiles(dllDir, ".*(.dll)$");
	int count = 1;
	for (auto dllPath : dllPaths) {
		std::cout << count++ << ") " << dllPath << std::endl;
	}
	std::cout << "Done fetching all the Dll's from " << dllDir << std::endl;
	std::cout << std::endl;
}

//------< add a dll path to list of dlls to test >--------------------------------------
void DllTestExecutive::addDll(std::string & dllPath){
	dllPaths.push_back(dllPath);
}

#ifdef TEST_DLL_UTILS
#include "../Logger.h"
int main() {

	//creating Logger and HostedLogger instances
	Logger* logger = new Logger();
	logger->registerStream(&std::cout);
	HostedLogger* hostedLogger = new HostedLogger(logger);

	//load and execute the tests from dll using DLLTestExecutor
	std::cout << "\nTesting Test Executor begins\n";
	DllTestExecutor executor;
	executor.loadAndExtract("../bin/output/Debug/TestDLL.dll");
	//peform tests
	executor.doTests(hostedLogger);
	executor.unload();
	std::cout << "\nTesting Test Executor ends\n";

	std::cout << "\nTesting Test Executive begins\n";
	//DllTestExecutive loads all the DLLs from ".." relative path
	DllTestExecutive executive;
	executive.addDllsFromDir("..");
	//Running all the tests from the DLL
	executive.doTests(hostedLogger);

	std::cout << "\nTesting Test Executive ends\n";

	delete hostedLogger;
	delete logger;

	return 0;
}

#endif