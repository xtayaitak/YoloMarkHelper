#include "tools.h"
#include "filetool.h"
namespace tools {
    std::vector<std::wstring> GetFolderFiles(const std::wstring& path, const std::wstring& suffix) {
        std::vector<std::wstring> img_files;
        file_tools::GetFileNameList(img_files, path, suffix);
        return img_files;
    }
}
