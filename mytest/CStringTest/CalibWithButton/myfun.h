#pragma once
#include<Windows.h>
#include<string>

// 这个文件在qt中被我命名为创建convertstring.h

std::wstring StringToWString(const std::string &s);

// 在当前目录下创建名为FileDir文件夹
bool MakeMyDir(const std::string FileDir);