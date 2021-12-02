
#include "FileTool.h"
#include "StringTool.h"
#include <windows.h>
#include <fstream>
#include "Shlwapi.h"

#pragma comment(lib,"Shlwapi.lib")

namespace file_tools
{
	std::wstring GetFileByPathFile(const std::wstring& strPathFile)
	{
		std::wstring strLocalFullPath = strPathFile;
		WCHAR szDir[256], szDrive[20], szName[256], szExt[60];
		if (_wsplitpath_s(strLocalFullPath.c_str(),
			szDrive, 20,
			szDir, 256,
			szName, 256,
			szExt, 60
		) == 0)
			return  std::wstring(szName) + szExt;//为了得到文件夹
		else
			return L"";
	}

	std::vector<std::string> ReadAsciiFileLines(const std::wstring& file_name)
	{
		std::ifstream in_file(file_name);
		std::vector<std::string> ret_lines;
		if (in_file.is_open())
		{
			std::string line;
			while (std::getline(in_file, line))
				ret_lines.push_back(line);
			in_file.close();
		}
		return ret_lines;
	}

	bool FileExist(const std::wstring& file_name)
	{
		return PathFileExistsW(file_name.c_str()) == TRUE;
	}
	bool GetFileNameList(std::vector<std::wstring>& retFileNameList, const std::wstring& strFolder, const std::wstring& suffix)
	{
		WIN32_FIND_DATAW ffd;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		std::wstring find_dir;
		find_dir = strFolder;
		if (find_dir[find_dir.length() - 1] != '\\' || find_dir[find_dir.length() - 1] != '/') {
			find_dir.push_back('\\');
		}
		find_dir += suffix;
		hFind = FindFirstFileW(find_dir.c_str(), &ffd);

		if (INVALID_HANDLE_VALUE == hFind)
		{
			//::MessageBoxA(NULL,"hFind is INVALID_HANDLE_VALUE",NULL,MB_OK);
			return false;;
		}

		BOOL bRet = false;
		do
		{
			if (wcscmp(ffd.cFileName, L".") == 0 || wcscmp(ffd.cFileName, L"..") == 0)
				continue;
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				GetFileNameList(retFileNameList, strFolder + ffd.cFileName + L"\\", suffix);
			}
			else
			{
				std::wstring strFileName = ffd.cFileName;
				retFileNameList.push_back((strFolder + ffd.cFileName));
			}
		} while (FindNextFileW(hFind, &ffd) != 0);
		FindClose(hFind);
		return true;
	}
}

