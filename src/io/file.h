#pragma once
#include <vector>
#include <string>
#include <windows.h>
#include <shlobj.h>

class SearchFile
{
public:
	SearchFile();
	std::string getFilePath();

private:
	uint32_t pushNum; //読み込んだデータ数を保存
	std::vector<std::string> filePath;
	std::vector<std::string> getImageName(std::wstring &dirName);
	void getDirName(std::wstring &dirName);
};