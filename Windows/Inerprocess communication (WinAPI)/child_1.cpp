#include <iostream>
#include <Windows.h>

using namespace std;


void lock(LPCTSTR path)
{
	HANDLE hFile = CreateFile(path, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, NULL);

	cout << "pr1: File locked!" << LockFile(hFile, 0, 0, GetFileSize(hFile, NULL), GetFileSize(hFile, NULL)) << endl;
	Sleep(2000);
	cout << "pr1: File unlocked!" << UnlockFile(hFile, 0, 0, GetFileSize(hFile, NULL), GetFileSize(hFile, NULL)) << endl;

	CloseHandle(hFile);
}

int main()
{
	cout << "== == CHILD 1 PROCESS START == ==" << endl;

	TCHAR bufIn[MAX_PATH] = { 0 };
	TCHAR bufOut[MAX_PATH] = { 0 };
	GetEnvironmentVariable("szIn", bufIn, MAX_PATH);
	GetEnvironmentVariable("szOut", bufOut, MAX_PATH);

	LPCTSTR szIn = bufIn;
	LPCTSTR szOut = bufOut;
	WIN32_FIND_DATA ffd;
	HANDLE hFind;
	TCHAR FindF[MAX_PATH];
	TCHAR szInFileName[MAX_PATH];
	TCHAR szOutFileName[MAX_PATH];

	sprintf_s(FindF, "%s\\%s", szIn, "*.*");
	hFind = FindFirstFile(FindF, &ffd);
	do {   //We form a full path (source)  
		sprintf_s(szInFileName, "%s\\%s", szIn, ffd.cFileName);

		//We form a full path (result)
		sprintf_s(szOutFileName, "%s\\%s", szOut, ffd.cFileName);

		CopyFile(szInFileName, szOutFileName, TRUE);
	} while (FindNextFile(hFind, &ffd));

	FindClose(hFind);

	lock("D:\\CopyPlace\\The_Great_Poem.txt");

	cout << "== == CHILD 1 PROCESS END == ==" << endl;
}
