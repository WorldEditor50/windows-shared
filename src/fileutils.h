#ifndef FILETOOLS_H
#define FILETOOLS_H
#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>

namespace FileUtils {
    int getAllFiles(const std::wstring& dir, const std::wstring& postfix, std::vector<std::wstring>& files);
    int getFiles(const std::wstring& dir, const std::wstring& postfix, std::vector<std::wstring>& files);
    void remove(const std::wstring& filePath);
    bool isFileExist(const std::wstring &filePath);
}

#endif // !FILETOOLS_H


