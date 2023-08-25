#ifndef STRINGUTILS_H
#define STRINGUTILS_H
#include <Windows.h>
#include <string>
#include <memory>
#include <cstdio>


class StringUtils
{
public:
    static std::wstring toWString(const std::string &s);
    static std::string toString(const std::wstring &s);
    static std::string gbkToUtf8(const std::string &gbk);
    static std::string utf8ToGbk(const std::string &utf8);
    static std::string asciiToUtf8(const std::string &ascii);
    static std::string utf8ToAscii(const std::string &utf8);

    static std::string format(const char* pszFormat, ...);
    static std::wstring wformat(const wchar_t* pszFormat, ...);
};

#endif // STRINGUTILS_H
