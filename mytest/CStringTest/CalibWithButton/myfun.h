#pragma once
#include<Windows.h>
#include<string>

// ����ļ���qt�б�������Ϊ����convertstring.h

std::wstring StringToWString(const std::string &s);

// �ڵ�ǰĿ¼�´�����ΪFileDir�ļ���
bool MakeMyDir(const std::string FileDir);