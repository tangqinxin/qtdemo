#include"myfun.h"

std::wstring StringToWString(const std::string &s)
{
	std::wstring wsTmp(s.begin(), s.end());
	return wsTmp;
}

bool MakeMyDir(const std::string FileDir) {
	std::wstring wDirectoryName = StringToWString(FileDir);
	LPCTSTR lpwdir = wDirectoryName.c_str();
	return CreateDirectory(lpwdir, NULL);//һ��ʼ�޷�ʶ���ԭ����.h�ļ���û�м���windows.h
}