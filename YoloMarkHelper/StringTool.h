#pragma once

#include <string>
#include <vector>

#include <stdio.h>

namespace string_tool
{
	std::wstring CharToWide(const std::string & szBuf);
	std::string WideToChar(const std::wstring& szWBuf);

	template<typename T = std::wstring>
	std::vector<T> SplitStrByFlag(const T& str, const T& sep)
	{
		if (sep.empty())
			return {};
		std::vector<T> vec;
		size_t sep_size = sep.size();
		size_t pos1 = 0;
		size_t pos2 = str.find(sep);
		while (T::npos != pos2)
		{
			vec.emplace_back(str.substr(pos1, pos2 - pos1));
			pos1 = pos2 + sep_size;
			pos2 = str.find(sep, pos1);
		}
		vec.emplace_back(str.substr(pos1));
		return vec;
	}
}

