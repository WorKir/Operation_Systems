#include <iostream>
#include <Windows.h>
#include <string>

using namespace std;

#define MILISECONDS 3000 
#define PATH_PIPE "\\\\.\\pipe\\PipeName"
#define FILE_PATH "D:\\SomeFiles\\test.txt"
#define NEW_FILE_PATH "D:\\SomeFiles\\newtest.txt"

void ErrorMessage(const char *str)  //output detailed information about the error
{
	LPVOID msg;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(), //Returns the last error
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
		(LPTSTR)&msg,
		0,
		NULL
	);
	cout << str << ": " << msg << endl;
	LocalFree(msg);
}

HANDLE hEvent1, hEvent2;
void test(void* pParams)
{
	WaitForSingleObject(hEvent1, INFINITE);

	for (int i = 0; i < 50; i++)
	{
		cout << "******************Str " << i << endl;
	}

	SetEvent(hEvent2);
}

void Thread1(void* pParams)
{
	WaitForSingleObject(hEvent1, INFINITE);
	cout << "********************THREAD START*************************" << endl;
	DWORD part;
	HANDLE read_hand = CreateFile(FILE_PATH, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	cout << "********************FILE OPEN*************************" << endl;
	BOOL size = GetFileSize(read_hand, NULL);
	cout << "********************SIZE GETTING*************************: " << size << endl;
	TCHAR *str = new TCHAR[size];
	cout << "********************BUFFER CREATE*************************" << endl;
	cout << "Read File: " << ReadFile(read_hand, str, size, &part, NULL);
	cout << "********************FILE READ*************************" << endl;
	cout << "Read text: " << str << endl;
	cout << "Decode Start!" << endl;
	int result_u, result_c;
	result_u = MultiByteToWideChar(1251, 0, str, -1, 0, 0);
	if (!result_u) 
	{ 
		cout << "failt" << endl;
	}
	wchar_t *ures = new wchar_t[result_u];
	if (!MultiByteToWideChar(1251, 0, str, -1, ures, result_u)) {
		delete[] ures;
		cout << "failt" << endl;
	}
	result_c = WideCharToMultiByte(65001, 0, ures, -1, 0, 0, 0, 0);
	if (!result_c) {
		delete[] ures;
		cout << "failt" << endl;
	}
	char *cres = new char[result_c];
	if (!WideCharToMultiByte(65001, 0, ures, -1, cres, result_c, 0, 0)) {
		delete[] cres;
		cout << "failt" << endl;
	}
	delete[] ures;

	cout << "Decode End!" << endl;

	HANDLE hFile = CreateFile(NEW_FILE_PATH, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD num_buff = 0;
	cout << endl << "Write: " << WriteFile(hFile, cres, result_c, &num_buff, NULL) << endl;
	cout << num_buff << endl;
	CloseHandle(hFile);

	SetEvent(hEvent2);
}

int main()
{
	cout << "= = CHILD PROCESS 2 START = =" << endl;
	hEvent1 = CreateEvent(NULL, FALSE, TRUE, "event1"); //set signal state  
	hEvent2 = CreateEvent(NULL, FALSE, FALSE, "event2");
	unsigned long uThrID;
	WaitForSingleObject(hEvent1, INFINITE);

	while (true)
	{
		BOOL bWriteFile;
		DWORD dwNoBytesWrite;
		char szWriteFileBuffer[1023] = "Can i decode?";
		DWORD dwWriteFileBufferSize = sizeof(szWriteFileBuffer);
		HANDLE hPipe = CreateFile(PATH_PIPE, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hPipe)
		{
			ErrorMessage("Error:invalid handle value");
			break;
		}
		//Sleep(3000);
		bWriteFile = WriteFile(hPipe, szWriteFileBuffer, dwWriteFileBufferSize, &dwNoBytesWrite, NULL);
		if (!bWriteFile)
		{
			ErrorMessage("Error: write file");
			break;
		}

		DWORD dwNoBytesRead;
		char szReadFileBuffer[1023];
		DWORD dwReadFileBufferSize = sizeof(szReadFileBuffer);
		BOOL bReadFile = ReadFile(hPipe, szReadFileBuffer, dwReadFileBufferSize, &dwNoBytesRead, NULL);
		if (!bReadFile)
		{
			ErrorMessage("Error: read file");
		}

		cout << "Answer from server: " << szReadFileBuffer << endl;
		string response = szReadFileBuffer;
		if (response == "No")
		{
			Sleep(MILISECONDS);
			CloseHandle(hPipe);
			continue;
		}
		else
		{
			DisconnectNamedPipe(hPipe);
			CloseHandle(hPipe);
			SetEvent(hEvent1); //set signal state 
			HANDLE hThr = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread1, NULL, 0, &uThrID);
			Sleep(5000);
			break;
		}
	}
	cout << endl;
}
