#ifndef IMGFTC
#define IMGFTC

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")

std::wstring s2ws(const std::string& s);
std::string ws2s(const std::wstring& s);
std::vector<std::string> FetchDirectories(std::string text);
char* string2CA(std::string& s);
bool compFloat (float A, float B, float EPSILON);

#endif