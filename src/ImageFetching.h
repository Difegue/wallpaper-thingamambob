#ifndef IMGFTC
#define IMGFTC

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
using namespace std;

std::wstring s2ws(const std::string& s);
std::string ws2s(const std::wstring& s);
vector<wstring> FetchDirectories(string text);

#endif