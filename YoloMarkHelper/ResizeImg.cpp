#include "ResizeImg.h"
#include "tools.h"
#include "StringTool.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

void ResizeImg(const std::wstring& path, float scale)
{
	auto files =  tools::GetFolderFiles(path, L"*.jpg");
	for (const auto& path_file : files) {
		cv::Mat src = cv::imread(string_tool::WideToChar(path_file));
		cv::Mat dst;
		cv::resize(src,dst, cv::Size(), scale, scale);
		cv::imwrite(string_tool::WideToChar(path_file), dst);
	}
}
