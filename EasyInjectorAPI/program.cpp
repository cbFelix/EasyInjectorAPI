#include <iostream>

#include "EasyInjectorAPI.h"

using namespace std;

int main() {
	DWORD pid = -1;
	string dllPath = "";

	EasyInjectorAPI inj(pid);
	if (!inj.injectNt(dllPath)) {
		cerr << "Failed to inject dll" << endl;

		return -1;
	}

	cout << "Injection succesfull" << endl;

	return 0;
}