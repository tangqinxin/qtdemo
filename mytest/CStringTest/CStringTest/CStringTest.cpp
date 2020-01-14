#include<atlstr.h>
#include<string>
#include"myfun.h"

int main() {
	CString s = "1.bmp";
	LPCTSTR lps = s.GetBuffer(0);
	std::wstring s2 = L"2.bmp";
	LPCTSTR lps2 = s2.c_str();

	std::string s3 = "3.bmp";
	std::wstring ws3=StringToWString(s3);
	LPCTSTR lps3 = ws3.c_str();

	std::string s4 = "D:\\StructureLine Project\\data\\4.bmp";//保存路径的格式
	std::wstring ws4 = StringToWString(s4);
	LPCTSTR lps4 = ws4.c_str();

	HANDLE hFile = ::CreateFile(lps4,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);


	std::string DirectoryName = ".\\MyDirctory";//建立文件夹
	std::wstring wDirectoryName = StringToWString(DirectoryName);
	LPCTSTR lpwdir = wDirectoryName.c_str();
	CreateDirectory(lpwdir, NULL);
	std::string s5 = DirectoryName+"\\"+"5.bmp";//保存路径的格式
	std::wstring ws5 = StringToWString(s5);
	LPCTSTR lps5 = ws5.c_str();

	HANDLE hFile2 = ::CreateFile(lps5,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	std::string DirctoryName2 = ".\\dir2";//建立文件夹2
	bool dirok=MakeMyDir(DirctoryName2);
	std::string s6 = DirctoryName2 + "\\" + "6.bmp";//保存路径的格式
	std::wstring ws6 = StringToWString(s6);
	LPCTSTR lps6 = ws6.c_str();

	HANDLE hFile3 = ::CreateFile(lps6,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	return 0;
}

