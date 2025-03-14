#include "EasyInjectorAPI.h"

EasyInjectorAPI::EasyInjectorAPI(DWORD processID) {
	processId = processID;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
	if (!hProcess) {
		DWORD error = GetLastError();
		throw runtime_error("Failed to open process");
		cout << "Error: " << error;
	}
}
EasyInjectorAPI::~EasyInjectorAPI() {
	if (isProcessAvilable()) {
		CloseHandle(hProcess);
	}
}

bool EasyInjectorAPI::checkDllFile(const char* dllPath) {
	ifstream iFile(dllPath);
	if (!iFile) {
		return false;
	}

	return true;
}

LPVOID EasyInjectorAPI::WriteDllPath(string& dllPath) {
	const char* filename = dllPath.c_str();

	if (!checkDllFile(filename)) {
		throw runtime_error("Failed to open DLL file.");
	}

	size_t nDllPath = strlen(filename) + 1;
	LPVOID allocMem = VirtualAllocEx(hProcess, NULL, nDllPath, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!allocMem) {
		CloseHandle(hProcess);

		throw runtime_error("Failed to allocate memory in process.");
	}

	if (!WriteProcessMemory(hProcess, allocMem, filename, nDllPath, NULL)) {
		VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);

		throw runtime_error("Failed to write path to dll in process.");
	}

	return allocMem;
}

bool EasyInjectorAPI::inject(string &dllPath) {
	if (!isProcessAvilable()) {
		throw runtime_error("The process is no longer available.");
	}

	LPVOID allocMem = WriteDllPath(dllPath);
	if (!allocMem) {
		throw runtime_error("Failed to write path to dll in process.");
	}

	LPVOID lpLoadLibraryAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	HANDLE hDLLThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpLoadLibraryAddr, allocMem, NULL, NULL);
	if (!hDLLThread) {
		DWORD error = GetLastError();
		cerr << "Failed to create remote thread in process. Error: " << error << endl;

		VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return false;
	}

	WaitForSingleObject(hDLLThread, INFINITE);
	
	VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);

	return true;
}

bool EasyInjectorAPI::injectNt(string& dllPath) {
	if (!isProcessAvilable()) {
		throw runtime_error("The process is no longer available.");
	}

	LPVOID allocMem = WriteDllPath(dllPath);

	LPVOID lpLoadLibraryAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	NtCreateThreadExFunc NtCreateThreadEx = (NtCreateThreadExFunc)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtCreateThreadEx");
	HANDLE hDLLThread = NULL;
	NTSTATUS status = NtCreateThreadEx(&hDLLThread, THREAD_ALL_ACCESS, NULL, hProcess, (LPTHREAD_START_ROUTINE)lpLoadLibraryAddr, allocMem, 0x4, 0, 0, 0, 0);
	if (!hDLLThread) {
		DWORD error = GetLastError();
		cerr << "Failed to create remote hidden thread in process. Error: " << error << endl;

		VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return false;
	}

	WaitForSingleObject(hDLLThread, INFINITE);

	VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);

	return true;
}

bool EasyInjectorAPI::injectManualMap(string& dllPath) {
	return true;
}