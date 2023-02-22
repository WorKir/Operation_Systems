#include <iostream>
#include <Windows.h>

using namespace std;

#define PATH_PIPE "\\\\.\\pipe\\PipeName"

void ErrorMessage(const char *str)  //output detailed information about the error
{

	LPVOID msg;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(), // Returns the last error
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
		(LPTSTR)&msg,
		0,
		NULL
	);
	cout << str << ": " << msg << endl;
	LocalFree(msg);
}

int main()
{
	STARTUPINFO si = { 0 }, si2 = { 0 };
	PROCESS_INFORMATION pi = { 0 }, pi2 = { 0 };

	string ApplicationName("C:\\Users\\kir\\source\\repos\\WinAPI_4_2\\Debug\\Child_1.exe");
	LPSTR lpAppName = &ApplicationName[0];
	string CommandLine("Child_1.exe");
	LPSTR lpCmdLine = &CommandLine[0];


	string ApplicationName2("C:\\Users\\kir\\source\\repos\\WinAPI_4_2\\Debug\\Child_2.exe");
	LPSTR lpAppName2 = &ApplicationName2[0];
	string CommandLine2("Child_2.exe");
	LPSTR lpCmdLine2 = &CommandLine2[0];

	//Creating the first child process
	if (!CreateProcess(lpAppName, lpCmdLine, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		ErrorMessage("CreateProcess");
	}

	//creating a second child process
	if (!CreateProcess(lpAppName2, lpCmdLine2, NULL, NULL, TRUE, NULL, NULL, NULL, &si2, &pi2))
	{
		ErrorMessage("CreateProcess2");
	}

	Sleep(500);
	// Connecting to the server (Process 1)
	HANDLE hPipe = CreateFile(PATH_PIPE, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hPipe == INVALID_HANDLE_VALUE) { ErrorMessage("ConnectToPipe"); }

	DWORD dwNoBytesWrite;
	char szWriteFileBuffer[1023] = "start"; //Forming a connection request
	DWORD dwWriteFileBufferSize = sizeof(szWriteFileBuffer);

	BOOL bWriteFile = WriteFile(hPipe, szWriteFileBuffer, dwWriteFileBufferSize, &dwNoBytesWrite, NULL);
	if (!bWriteFile) { ErrorMessage("WriteToPipe"); }

	cout << "/n##############wait object...####################" << endl;
	WaitForSingleObject(hPipe, INFINITE);
	cout << "/n###################pipe end####################" << endl;
	WaitForSingleObject(pi.hProcess, INFINITE);
	cout << "/n#################Process 1 end###########" << endl;
	WaitForSingleObject(pi2.hProcess, INFINITE);
	cout << "/n######################3Process 2 end##############" << endl;
	Sleep(6000);
	cout << "GOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOAL" << endl;
	CloseHandle(hPipe);
}
