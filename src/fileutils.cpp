#include "fileutils.h"

int FileUtils::getAllFiles(const std::wstring &dir, const std::wstring& postfix, std::vector<std::wstring> &filePaths)
{
    std::wstring dir_;
    if (postfix.empty()) {
        dir_ = dir + L"\\*.*";
    }
    else {
        dir_ = dir + L"\\" + postfix;
    }
    LARGE_INTEGER size;
    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW(dir_.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to find first file!\n";
        return -1;
    }
    do {
        std::wstring fileName(findData.cFileName);
        // 是否是文件夹，并且名称不为"."或".."
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
            fileName != L"." && fileName != L"..") {
            // 将dirNew设置为搜索到的目录，并进行下一轮搜索
            dir_ = dir + L"\\" + fileName;
            getAllFiles(dir_, postfix, filePaths);
        } else {
            std::wstring path = dir + L"\\" + fileName;
            filePaths.push_back(path);
        }
    } while (FindNextFileW(hFind, &findData));
    FindClose(hFind);
    return 0;
}

int FileUtils::getFiles(const std::wstring &dir, const std::wstring &postfix, std::vector<std::wstring> & filePaths)
{
    WIN32_FIND_DATAW findData;
    std::wstring dir_;
    if (postfix.empty()) {
        dir_ = dir + L"\\*.*";
    }
    else {
        dir_ = dir + L"\\" + postfix;
    }
    HANDLE hFind = FindFirstFileW(dir_.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to find first file!\n";
        return -1;
    }
    LARGE_INTEGER size;
    do {
        std::wstring fileName(findData.cFileName);
        // 忽略"."和".."两个结果
        if (fileName == L"." || fileName == L"..") {
            continue;
        }
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            continue;
        }

        /* 文件大小 */
        size.LowPart = findData.nFileSizeLow;
        size.HighPart = findData.nFileSizeHigh;
        
        std::wstring path = dir + L"\\" + fileName;
        filePaths.push_back(path);
    } while (FindNextFileW(hFind, &findData));

    return 0;
}

void FileUtils::remove(const std::wstring& filePath)
{
    if (filePath.empty()) {
        return;
    }
    DeleteFileW(filePath.c_str());
    return;
}

bool FileUtils::isFileExist(const std::wstring &filePath)
{

    return false;
}
