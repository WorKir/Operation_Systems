“MyDll1.cpp”

#include "pch.h"
#include "dll.h"

extern "C" __declspec(dllexport)
int Func(string text, int word_num)
{
	string temp;
	int space_count = 0;
	int i = 0;

	for (int i = 0; i < text.size(); i++)
	{
		if (space_count == word_num + 1)
		{
			if (text[i] != ' ')
				temp += text[i];
		}
		if (text[i] == ' ') { space_count++; }
	}

	space_count = 0;
	while (i < text.size())
	{
		if (space_count == word_num)
		{
			if (temp.find_first_of(text[i], 0) != string::npos)
			{
				text.erase(i, 1);
				continue;
			}
		}
		if (text[i] == ' ') { space_count++; }
		i++;
	}

	cout << text << endl;

	return 0;
}

extern "C" __declspec(dllexport)
int ReadF(LPCTSTR path, int symbnum)
{
	setlocale(LC_ALL, "RU");
	DWORD part;
	HANDLE read_hand = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	BOOL size = GetFileSize(read_hand, NULL);
	if (symbnum <= size)
	{
		TCHAR *buff = new TCHAR[symbnum];

		ReadFile(read_hand, buff, symbnum, &part, NULL);
		cout << "File: " << endl;
		for (int i = 0; i < symbnum; i++)
		{
			cout << buff[i];
		}
		//cout << endl << buff << endl;
	}
	else if(size == -1)
	{
		cout << "File is no find" << endl;
	}
	else
	{
		cout << "Number of symbol is very big" << endl;
	}

	return 0;
}
