#include <iostream>
#include <Windows.h>

using namespace std;

typedef int(*DllFunc)(string, int);
int main()
{
	string text = "first ababais ba";
	int multi;
	DWORD err;
	HINSTANCE hDll = LoadLibrary("Dll1.dll"); // the name of the connected library
	if (hDll != NULL)
	{
		printf("Library was loaded\n");
	}
	else
	{
		err = GetLastError();
		printf("Couldn't load dll. Error code %d\n", err);
		return 0;
	}
	// getting a pointer to a library function
	DllFunc Func = (DllFunc)GetProcAddress(hDll, "Func");
	if (Func!= NULL)
	{
		Func(text, 1);
	}
	// freeing the descriptor
	FreeLibrary(hDll);

	return 0;
}
