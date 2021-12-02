#pragma once
#include <vector>
#include <string>
namespace tools {
	std::vector<std::wstring> GetFolderFiles(const std::wstring& path, const std::wstring& suffix);
}