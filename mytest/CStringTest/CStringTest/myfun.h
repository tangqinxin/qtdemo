#pragma once
#include<Windows.h>
#include<string>

std::wstring StringToWString(const std::string &s);
bool MakeMyDir(const std::string FileDir);