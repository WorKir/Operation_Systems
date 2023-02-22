#include <iostream>
#include <Windows.h>

using namespace std;

void ErrorMessage(const char *str)  //Out detail information about error
{

	LPVOID msg;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(), //Return the last error
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
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
	DWORD excode, excode2;

	string ApplicationName("C:\\Users\\kir\\source\\repos\\Winapi_lab_3_1\\Debug\\Child_1.exe");
	LPSTR lpAppName = &ApplicationName[0];
	string CommandLine("Child_1.exe");
	LPSTR lpCmdLine = &CommandLine[0];

	string ApplicationName2("C:\\Users\\kir\\source\\repos\\Winapi_lab_3_1\\Debug\\Child_2.exe");
	LPSTR lpAppName2 = &ApplicationName2[0];
	string CommandLine2("Child_2.exe");
	LPSTR lpCmdLine2 = &CommandLine2[0];

	cout << "== == PARENT PROCESS START == ==" << endl;

	CreateDirectoryA("D:\\FILE11", NULL); //NULL -- Security attr  
	CreateDirectoryA("D:\\FILE11\\FILE12", NULL);
	CreateDirectoryA("D:\\FILE21", NULL);
	CreateDirectoryA("D:\\FILE21\\FILE22", NULL);
	CreateDirectoryA("D:\\FILE21\\FILE22\\FILE23", NULL);

	TCHAR Path[] = "szIn=D:\\SomeFiles\0szOut=D:\\CopyPlace\0";

	if (!CreateProcess(lpAppName,	 // The name of the executable module
		lpCmdLine,	//Command line
		NULL,	// SECURITY_ATTRIBURES of the process
		NULL,	// SECURITY_ATTRIBURES of thread
		FALSE,	// Успадкування об'єктів
		NULL,	// Creation flags that affect exactly how a new process is created
		(LPVOID)Path,	// Environment variables and if NULL, the child process inherits strings of environment variables from the parent process
		NULL,	// The current directory, and if NULL, the new process's working directory will be the same as the application's
		&si,	// Startup information is a pointer to the STARTUPINFO structure
		&pi		// Process information is a pointer to the PROCESS_INFO structure
	)) {
		ErrorMessage("CreateProcess");
		return -1;
	}

	if (!CreateProcess(lpAppName2, lpCmdLine2, NULL, NULL, TRUE, NULL, (LPVOID)"USERPROFILE=D:\\FILE11", NULL, &si2, &pi2))
	{
		ErrorMessage("CreateProcess");
		return -1;
	}

	string ApplicationName3("C:\\Users\\kir\\source\\repos\\Winapi_lab_3_1\\Debug\\Child_3.exe");
	LPSTR lpAppName3 = &ApplicationName3[0];
	string CommandLine3("Child_3.exe");
	LPSTR lpCmdLine3 = &CommandLine3[0];

	DWORD excode3;
	STARTUPINFO si3 = { 0 };
	PROCESS_INFORMATION pi3 = { 0 };
	SECURITY_ATTRIBUTES sa3 = { 0 };

	//Channel to intercept the stdOut of child process #3
	char buf[1024];           // Output buffer
	HANDLE newstdout, read_stdout;
	SECURITY_DESCRIPTOR sd; // Channel security framework

	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, true, NULL, false);
	sa3.lpSecurityDescriptor = &sd;
	sa3.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa3.bInheritHandle = true;

	if (!CreatePipe(&read_stdout, &newstdout, &sa3, NULL)) // create pipe for stdout

	{
		ErrorMessage("CreatePipe");
		return 0;
	}

	GetStartupInfo(&si3);

	/*
		The dwFlags parameter tells the CreateProcess function
		exactly how to create a process.

		STARTF_USESTDHANDLES controls hStd* fields.
		STARTF_USESHOWWINDOW controls the wShowWindow field.
	 */

	si3.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si3.wShowWindow = SW_HIDE;
	si3.hStdOutput = newstdout;
	si3.hStdError = newstdout;   //Replace handle for child process
	if (!CreateProcess(lpAppName3, lpCmdLine3, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si3, &pi3))
	{
		ErrorMessage("CreateProcess");
		CloseHandle(newstdout);
		CloseHandle(read_stdout);
		return -1;
	}

	unsigned long exit = 0;  // process completion code
	unsigned long bread;   // number of bytes read
	unsigned long avail;   // number of bytes available

	memset(buf, 0, sizeof(buf)); // Reset the buffer

	for (;;)      // the main cycle of the program
	{
		GetExitCodeProcess(pi3.hProcess, &exit); // until the child process is closed
		if (exit != STILL_ACTIVE)
			break;

		PeekNamedPipe(read_stdout, // Pipe identifier
			buf, // Buffer for read data
			1023, // Size of the read buffer
			&bread, // The address of the variable in which the number of bytes read will be written
			&avail,// The address of the variable to which the total number of bytes available in the channel will be written
			NULL // The address of the variable in which the number of unread bytes in this message will be written
		);

		// We check whether there is data to read in stdout
		if (bread != 0)
		{
			memset(buf, 0, sizeof(buf));
			if (avail > 1023)
			{
				while (bread >= 1023)
				{
					ReadFile(read_stdout, buf, 1023, &bread, NULL);  // read from the stdout pipe
					cout << buf;
					memset(buf, 0, sizeof(buf));
				}
			}
			else 
			{
				ReadFile(read_stdout, buf, 1023, &bread, NULL);
				cout << buf;
			}
		}
	}


	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	GetExitCodeProcess(pi.hProcess, &excode);

	WaitForSingleObject(pi2.hProcess, INFINITE);
	CloseHandle(pi2.hThread);
	CloseHandle(pi2.hProcess);
	GetExitCodeProcess(pi2.hProcess, &excode2);

	WaitForSingleObject(pi3.hProcess, INFINITE);
	CloseHandle(pi3.hThread);
	CloseHandle(pi3.hProcess);
	CloseHandle(newstdout);
	CloseHandle(read_stdout);
	GetExitCodeProcess(pi3.hProcess, &excode3);

	if (excode != STILL_ACTIVE) { cout << "Procces 1 done" << endl; }
	else { cout << "Procces 1 stiil running" << endl; }
	if (excode2 != STILL_ACTIVE) { cout << "Procces 2 done" << endl; }
	else { cout << "Procces 2 stiil running" << endl; }
	if (excode3 != STILL_ACTIVE) { cout << "Procces 3 done" << endl; }
	else { cout << "Procces 3 stiil running" << endl; }

	return 0;
}
