#pragma once




#include <vector>
#include <string>

namespace file_tools
{ 
	std::wstring GetFileByPathFile(const std::wstring& strPathFile);
	std::vector<std::string> ReadAsciiFileLines(const std::wstring& file_name);
	bool FileExist(const std::wstring& file_name);
	bool GetFileNameList(std::vector<std::wstring>& retFileNameList, const std::wstring& strFolder, const std::wstring& suffix);
}


