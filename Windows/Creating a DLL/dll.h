#pragma once
# define WIN32_LEAN_AND_MEAN
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <string>


using namespace std;

//#define EXPORT __declspec (dllexport)

extern "C" __declspec(dllexport)
int Func(string text, int space_count);

extern "C" __declspec(dllexport)
int ReadF(LPCTSTR path, int size);
