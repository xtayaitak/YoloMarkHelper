#include "CheckMark.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "FileTool.h"
#include "StringTool.h"
#include "tools.h"

#include <iostream>


std::wstring GetFileNameWithoutSuffix(const std::wstring& path_file) {
    std::wstring file_name = file_tools::GetFileByPathFile(path_file);
    auto pos = file_name.rfind('.');
    return file_name.substr(0, pos);
}


class ObjClass {
public:
    void LoadByFile(const std::wstring& file_name) {
        auto lines = file_tools::ReadAsciiFileLines(file_name);
        for (auto& line : lines) {
            names_.push_back(line);
        }
    }
    const std::string GetNameByIndex(int index) const {
        return names_.at(index);
    }
private:
    std::vector<std::string> names_;
};

struct MarkPositionInfo {
    int obj_index;
    float x_center;
    float y_center;
    float width;
    float height;
};
MarkPositionInfo ParseLineToMarkInfo(const std::string& line)
{
    //1 0.703571 0.539437 0.075714 0.188732
    MarkPositionInfo info;
    int flag_index = 0;
    size_t start_pos = 0;
    for (size_t i = 0; i < line.length(); i++) {
        if (line.at(i) == ' ') {
            if (flag_index == 0) {
                info.obj_index = std::stoi(line.substr(start_pos, i));
                start_pos = i + 1;
            }
            else if (flag_index == 1) {
                info.x_center = std::stof(line.substr(start_pos, i));
                start_pos = i + 1;
            }
            else if (flag_index == 2) {
                info.y_center = std::stof(line.substr(start_pos, i));
                start_pos = i + 1;
            }
            else if (flag_index == 3) {
                info.width = std::stof(line.substr(start_pos, i));
                start_pos = i + 1;

                info.height = std::stof(line.substr(start_pos));
                break;
            }
            flag_index++;
        }
    }
    return info;
}


cv::Rect MarkToRect(const cv::Mat& img, const MarkPositionInfo& mark) {
    float absolute_x_center = mark.x_center * img.cols;
    float absolute_y_center = mark.y_center * img.rows;
    float absolute_width = mark.width * img.cols;
    float absolute_height = mark.height * img.rows;

    float left_corner_x = absolute_x_center - absolute_width / 2.0f;
    float left_corner_y = absolute_y_center - absolute_height / 2.0f;

    return cv::Rect((int)left_corner_x, (int)left_corner_y, (int)absolute_width, (int)absolute_height);
}

bool PointInRect(const cv::Point& pt,const cv::Rect & rect)
{
    return pt.x >= rect.x && 
        pt.x <= rect.x + rect.width && 
        pt.y >= rect.y && 
        pt.y <= rect.y + rect.height;
}

bool PointInRects(const cv::Point& pt, const std::vector<cv::Rect>& rects) {
    for (auto& rect : rects) {
        if (PointInRect(pt, rect)) {
            return true;
        }
    }
    return false;
}

void DrawBoundary(const std::string& img_path, const std::vector<MarkPositionInfo>& marks) {
    std::cout << "path:" << img_path << std::endl;
    auto img = cv::imread(img_path);

    CV_Assert(img.depth() == CV_8U);
    CV_Assert(img.channels() == 3);

    std::vector<cv::Rect> rects;
    for (auto& mark : marks) {
        cv::Rect rc = MarkToRect(img, mark);
        cv::rectangle(img, rc, cv::Scalar(0, 0, 255));
        rects.push_back(rc);
    }


    for (int row = 0; row < img.rows; row++) {
        for (int col = 0; col < img.cols; col++) {
            if (!PointInRects(cv::Point(col, row), rects)) {
                img.at<cv::Vec3b>(row, col) = { 0,0,0 };
            }
        }
    }

    cv::putText(img, img_path, cv::Point(0, 20), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255));
    cv::imshow("Main", img);
    cv::waitKey(0);
}




void CheckMark(const std::string& path, const std::string& obj_name)
{
    ObjClass objclass;
    objclass.LoadByFile(string_tool::CharToWide((path + "classes.txt")));
    std::vector<std::wstring> files = tools::GetFolderFiles(string_tool::CharToWide(path), L"*.jpg");
    for (const auto& file_path : files) {
        const std::wstring text_file = string_tool::CharToWide(path) + GetFileNameWithoutSuffix(file_path) + L".txt";
        std::vector<MarkPositionInfo> marks;
        if (file_tools::FileExist(text_file)) {
            auto lines = file_tools::ReadAsciiFileLines(text_file);
            for (const auto& line : lines) {
                std::string class_index = line.substr(0, line.find(' '));
                auto name = objclass.GetNameByIndex(std::stoi(class_index));
                if (obj_name == name) {
                    marks.push_back(ParseLineToMarkInfo(line));
                }
            }
        }

        if (marks.size()) {
            DrawBoundary(string_tool::WideToChar(file_path), marks);
        }
    }
}


std::vector<cv::Mat> GetSubImages(const std::string& img_path, const std::vector<MarkPositionInfo>& marks)
{
    std::cout << "path:" << img_path << std::endl;
    auto img = cv::imread(img_path);

    CV_Assert(img.depth() == CV_8U);
    CV_Assert(img.channels() == 3);

    std::vector<cv::Rect> rects;
    for (auto& mark : marks) {
        cv::Rect rc = MarkToRect(img, mark);
        cv::rectangle(img, rc, cv::Scalar(0, 0, 255));
        rects.push_back(rc);
    }

    std::vector<cv::Mat> result;
    for (auto& rect : rects) {
        cv::Mat dst = img(rect);
        result.push_back(dst);
    }
    return result;
}


void CheckMark2(const std::string& path, const std::string& obj_name) 
{
    ObjClass objclass;
    objclass.LoadByFile(string_tool::CharToWide((path + "classes.txt")));
    std::vector<std::wstring> files = tools::GetFolderFiles(string_tool::CharToWide(path), L"*.jpg");


    std::vector<cv::Mat> all_small_images;
    for (const auto& file_path : files) {
        const std::wstring text_file = string_tool::CharToWide(path) + GetFileNameWithoutSuffix(file_path) + L".txt";
        std::vector<MarkPositionInfo> marks;
        if (file_tools::FileExist(text_file)) {
            auto lines = file_tools::ReadAsciiFileLines(text_file);
            for (const auto& line : lines) {
                std::string class_index = line.substr(0, line.find(' '));
                auto name = objclass.GetNameByIndex(std::stoi(class_index));
                if (obj_name == name) {
                    marks.push_back(ParseLineToMarkInfo(line));
                }
            }
        }

        if (marks.size()) {
            auto sub_images = GetSubImages(string_tool::WideToChar(file_path), marks);
            all_small_images.insert(all_small_images.end(), sub_images.begin(), sub_images.end());
        }
    }

    const int SMALL_WIDTH = 60;
    const int SMALL_HEIGHT = 90;
    
    std::vector<cv::Mat> all_lines;
    std::vector<cv::Mat> line_mat_list;
    for (size_t i = 0; i < all_small_images.size(); i++) {

        cv::Mat small_img = all_small_images.at(i);
        cv::Mat small_img_resized;
        cv::resize(small_img, small_img_resized, cv::Size(SMALL_WIDTH, SMALL_HEIGHT));
        line_mat_list.push_back(small_img_resized);

        if (line_mat_list.size() == 10) {
            cv::Mat line_img;
            cv::hconcat(line_mat_list, line_img);
            all_lines.push_back(line_img);
            line_mat_list.clear();
        }
    }



    cv::Mat result_image(0, all_lines[0].cols, all_lines[0].type());

    //the last row maybe not equal to 10 * SMALL_WIDTH ,add empty mat.
    if (all_lines[all_lines.size() - 1].cols < SMALL_WIDTH * 10) {
        cv::Mat temp = cv::Mat::zeros(SMALL_HEIGHT, SMALL_WIDTH * 10 - all_lines[all_lines.size() - 1].cols, all_lines[all_lines.size() - 1].type());
        cv::hconcat(all_lines[all_lines.size() - 1],temp, all_lines[all_lines.size() - 1]);
    }
    cv::vconcat(all_lines, result_image);
    cv::imshow("Main", result_image);
    cv::waitKey(0);
}
