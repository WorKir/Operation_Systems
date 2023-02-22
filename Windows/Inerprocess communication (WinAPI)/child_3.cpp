#include <iostream>
#include <Windows.h>

using namespace std;

int SearchPattern(LPCTSTR path, LPCTSTR pattern)
{
	DWORD part;
	HANDLE read_hand = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	BOOL size = GetFileSize(read_hand, NULL);
	TCHAR *buff = new TCHAR[size];
	buff[size] = { 0 };
	ReadFile(read_hand, buff, size, &part, NULL);

	int i, j, lenPattern; // Cycle counters and pattern length
	int n = 1; // Number of pattern repetitions
	for (lenPattern = 0; pattern[lenPattern]; lenPattern++); // Find length of pattern

	for (i = 0; i <= size - lenPattern; i++) // As long as there is a search option
	{
		for (j = 0; j < lenPattern && buff[i + j] == pattern[j]; j++); // Character-by-character match checking
		if (j == lenPattern)
			if (n - 1) n--;
			else
			{
				//cout << buff << endl; // File contents
				return i;
			}
	}
	return 0;
}

void Search(LPCTSTR Dir, LPCTSTR Mask, LPCTSTR pattern)
{
	TCHAR buf[MAX_PATH] = { 0 };
	sprintf_s(buf, "%s\\%s", Dir, Mask);
	WIN32_FIND_DATA FindFileData;
	HANDLE hFile;
	hFile = FindFirstFile(buf, &FindFileData);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			sprintf_s(buf, "%s\\%s", Dir, FindFileData.cFileName);
			if (SearchPattern(buf, pattern))
				cout << buf << endl;

			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (strcmp(FindFileData.cFileName, "..") != 0 && strcmp(FindFileData.cFileName, ".") != 0)
				{
					//MessageBox(0,"You have entered a new folder","Info",MB_OK);
					Search(buf, Mask, pattern);
				}
			}
		} while (FindNextFile(hFile, &FindFileData));
		FindClose(hFile);
	}
}

int main()
{
	cout << "== == CHILD 3 PROCESS START == ==" << endl;
	Search("D:\\SomeFiles", "*.txt", "keep"); //(Path, extension, template)

	cout << "== == CHILD 3 PROCESS END == ==" << endl;
}
