#pragma once
#ifndef EASYINJECTORAPI_H
#define	EASYINJECTORAPI_H

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <tlhelp32.h>

using namespace std;

typedef NTSTATUS(NTAPI* NtCreateThreadExFunc)(
	OUT PHANDLE hThread,
	IN ACCESS_MASK DesiredAccess,
	IN PVOID ObjectAttributes,
	IN HANDLE ProcessHandle,
	IN PVOID lpStartAddress,
	IN PVOID lpParameter,
	IN ULONG Flags,
	IN SIZE_T StackZeroBits,
	IN SIZE_T SizeOfStackCommit,
	IN SIZE_T SizeOfStackReserve,
	OUT PVOID lpBytesBuffer
	);

class EasyInjectorAPI {
public:
	EasyInjectorAPI(DWORD processID);
	EasyInjectorAPI(HANDLE hProcess);
	~EasyInjectorAPI();

	virtual bool inject(string& dllPath);

	virtual bool injectNt(string& dllPath);
	virtual bool injectManualMap(string& dllPath);

	bool checkDllFile(const char* dllPath);

private:
	HANDLE hProcess;
	DWORD processId;

	bool isProcessAvilable() {
		if (!hProcess) {
			return false;
		}

		return true;
	}

	LPVOID WriteDllPath(string& dllPath);
}; 

#endif // !EASYINJECTORAPI_H