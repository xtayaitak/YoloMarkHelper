

#include "StringTool.h"
#include <Windows.h>

namespace string_tool
{
	std::wstring CharToWide(const std::string& szBuf)
	{
		auto nLenBytesRequire = ::MultiByteToWideChar(CP_ACP,
			0,
			szBuf.c_str(),
			strlen(szBuf.c_str()),
			NULL,
			0);
		if (nLenBytesRequire <= 0)
			return L"";
		std::vector<wchar_t> buffer(nLenBytesRequire);
		auto new_size = ::MultiByteToWideChar(CP_ACP,
			0,
			szBuf.c_str(),
			strlen(szBuf.c_str()),
			&buffer[0],
			buffer.size());
		return std::wstring(&buffer[0], new_size);
	}


	std::string WideToChar(const std::wstring& szWBuf)
	{
		BOOL bOk = FALSE;
		int nLen = ::WideCharToMultiByte(CP_ACP,
			0,
			szWBuf.c_str(),
			wcslen(szWBuf.c_str()),
			NULL,
			0,
			NULL,
			&bOk
		);
		if (nLen <= 0)
			return "";
		std::vector<char> dest_buffer(nLen);
		//char * szBuf = new char[nLen];
		int new_size = ::WideCharToMultiByte(CP_ACP,
			0,
			szWBuf.c_str(),
			wcslen(szWBuf.c_str()),
			&dest_buffer[0],
			dest_buffer.size(),
			NULL,
			&bOk);
		return std::string(&dest_buffer[0], new_size);
	}
}

