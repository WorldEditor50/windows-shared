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
    struct Hex {
        static char toInt4(char c);
        static WCHAR toInt4(WCHAR c);
        static unsigned char toInt8(const char* hex);
        static unsigned char toInt8(const WCHAR* hex);
        static unsigned short toInt16(const std::string &hex);
        static unsigned short toInt16(const std::wstring &hex);
        static unsigned int toInt32(const std::string &hex);
        static unsigned int toInt32(const std::wstring &hex);
    };


};

#endif // STRINGUTILS_H
