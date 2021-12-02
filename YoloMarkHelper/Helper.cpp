// Helper.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CmdParse.h"
#include "StringTool.h"

#include "CheckMark.h"
#include "ResizeImg.h"


int main(int argc, char* argv[])
{
    CCmdParse cmd;
    cmd.Parse(argc, argv);

    std::wstring command = cmd.GetOption(1);
    if (command == L"CheckMark") {
        CheckMark(string_tool::WideToChar(cmd.GetOption(2)), string_tool::WideToChar(cmd.GetOption(3)));
    }
    else if (command == L"CheckMark2") {
        CheckMark2(string_tool::WideToChar(cmd.GetOption(2)), string_tool::WideToChar(cmd.GetOption(3)));
    }
    else if (command == L"ResizeImg") {
        std::cout << "!!!! 本操作会将所有图片缩小一倍！！ 请慎重" << std::endl;
        system("pause");
        ResizeImg(cmd.GetOption(2), std::stof(cmd.GetOption(3)));
    }
    

    return 0;
}

