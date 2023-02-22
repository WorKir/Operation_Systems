#include "dll.h"

using namespace std;

//Static connection

int main()
{
	string text = "first ababais ba";
	LPCTSTR path = "D:\\test.txt";
	//func(text, 1);
	//func2("D:\\test.txt", 5);
	ReadF(path, 5);
	cout << endl;
	Func(text, 1);
	//HMODULE hLib;
	//hLib = LoadLibrary("Dll1.dll");
}
