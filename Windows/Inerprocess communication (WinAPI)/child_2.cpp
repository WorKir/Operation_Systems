#include <iostream>
#include <Windows.h>

using namespace std;

void Knock_knock(LPCTSTR path)
{
	DWORD part;
	HANDLE read_hand = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	BOOL size = GetFileSize(read_hand, NULL);
	if (size > -1)
	{
		TCHAR *buff = new TCHAR[size];
		ReadFile(read_hand, buff, size, &part, NULL);
		cout << buff << endl;
	}
	else
	{
		cout << "pr2: File lock:((" << endl;
	}

	/*
	//I can't remember what is mean. It was commented. I may have been debugging code with this

	Sleep(3000);
	read_hand = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	size = GetFileSize(read_hand, NULL);
	TCHAR *buff = new TCHAR[size];
	buff[size] = { 0 };
	ReadFile(read_hand, buff, size, &part, NULL);
	cout << buff << endl;
	*/
}

int main()
{
	cout << "== == CHILD 2 PROCESS START == ==" << endl;

	TCHAR buf[MAX_PATH] = { 0 };
	GetEnvironmentVariable("USERPROFILE", buf, MAX_PATH);
	cout << "pr2: UserProfile: " << buf << endl;

	LPCTSTR path = "D:\\CopyPlace\\MyBinary.bin";
	HANDLE hFile = CreateFile(path, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	TCHAR buff[] = "121351846515313541532135415321354542465465132154268541878798617896541236548";
	DWORD num_buff = 0;
	cout << endl << "Write: " << WriteFile(hFile, buff, sizeof(buff), &num_buff, NULL) << endl;
	CloseHandle(hFile);

	
	Knock_knock("D:\\CopyPlace\\The_Great_Poem.txt");

	cout << "== == CHILD 2 PROCESS END == ==" << endl;
