#include <iostream>

#include "EasyInjectorAPI.h"

using namespace std;

int main() {
	DWORD pid = -1;		// target process id
	string dllPath = "";	// full path to dll

	EasyInjectorAPI inj(pid); // Get the handle of process with target process id
	if (!inj.inject(dllPath)) {	// Default injection method with remote thread. You can use inj.injectNt(dllPath) to use NtCreateThreadEx() to inject the dll.
		cerr << "Failed to inject dll" << endl;

		return -1;
	}

	cout << "Injection succesfull." << endl;


	return 0;
}