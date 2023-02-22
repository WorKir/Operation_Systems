#include <iostream>
#include <Windows.h>
#include <string>

using namespace std;

#define SIZE_BUFFER 4096
#define PATH_PIPE "\\\\.\\pipe\\PipeName" 
#define FILE_PATH "D:\\SomeFiles\\test.txt"

void ErrorMessage(const char *str)  // output detailed information about the error
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

void Write_File()
{
	LPCSTR str = "Этот текст в windows1251";
	DWORD iNumWrite = 0;
	OVERLAPPED olf = { 0 };
	HANDLE hFile = CreateFile(FILE_PATH, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!WriteFile(hFile, str, strlen(str), &iNumWrite, &olf))
	{
		CloseHandle(hFile);
		return;
	}
	CloseHandle(hFile);
}

int main()
{
	cout << "= = CHILD PROCESS 1 START = =" << endl;
	HANDLE hPipe;
	BOOL bWriteFile;
	char szInputBuffer[1023];  // Buffer In
	char szOutputBuffer[1023]; //Buffer Out
	DWORD dwszInputBuffer = sizeof(szInputBuffer); // "In" buffer size
	DWORD dwszOutputBuffer = sizeof(szOutputBuffer); //"Out" buffer size
	BOOL bFlushFileBuffer; // Writing buffered data to a file

	char szReadFileBuffer[1023]; //Read buffer
	DWORD dwReadBufferSize = sizeof(szReadFileBuffer); // Read buffer size
	DWORD dwNoBytesRead; //Unread bytes

	char szWriteFileBufferToProcess2[1023] = "No";  // Answer of the Process 2 
	string responseToProcess2 = szWriteFileBufferToProcess2; // Formation of the response of the process 2
	DWORD dwWriteBufferSize = sizeof(szWriteFileBufferToProcess2);  // The size of the record in the process 1 buffer
	DWORD dwNoBytesWrite; // Number of unwritten bytes

	while (true)
	{
		hPipe = CreateNamedPipe(PATH_PIPE, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, dwszOutputBuffer, dwszInputBuffer, 0, NULL);
		if (hPipe == INVALID_HANDLE_VALUE)
		{	
			ErrorMessage("NamedPipe creation failed");
		}
		
		bFlushFileBuffer = FlushFileBuffers(hPipe);
		if (!bFlushFileBuffer)
		{
			ErrorMessage("FlushFileBuffer failed");
		}

		cout << "Waiting for client..." << endl;

		BOOL bConnectNamedPipe = ConnectNamedPipe(hPipe, NULL); //Connect to pipe
		if(bConnectNamedPipe)
		{
			cout << "Client connected." << endl;
			cout << "Reading message from client..." << endl;
			BOOL bReadFile = ReadFile(hPipe, szReadFileBuffer, dwReadBufferSize, &dwNoBytesRead, NULL); // Read from pipe
			if (!bReadFile)
			{
				ErrorMessage("ReadFile Failed");
			}

			cout << "DATA READING FROM CLIENT -> " << szReadFileBuffer << endl; //Output of read information

			string request = szReadFileBuffer;
			if (request == "start")
			{
				bFlushFileBuffer = FlushFileBuffers(hPipe);
				if (!bFlushFileBuffer)
				{
					ErrorMessage("FlushFileBuffer failed");
				}

								Write_File();//Function

				responseToProcess2 = "Yes";
				DisconnectNamedPipe(hPipe); //Disconnection from the pipe
				CloseHandle(hPipe);
			}

			if (request == "Can i decode?")
			{
				bFlushFileBuffer = FlushFileBuffers(hPipe);
				if (!bFlushFileBuffer)
				{
					ErrorMessage("FlushFileBuffer failed ");
				}
				bWriteFile = WriteFile(hPipe, responseToProcess2.c_str(), dwWriteBufferSize, &dwNoBytesWrite, NULL);
				if (!bWriteFile)
				{
					ErrorMessage("Error write file to process 2");
				}
				cout << "DATA WRITING TO CLIENT -> " << responseToProcess2 << endl;
				cout << "End of session." << endl; cout << endl;
				DisconnectNamedPipe(hPipe);
				CloseHandle(hPipe);
			}
		}
	}
}
