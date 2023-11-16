#include "stringutils.h"


std::wstring StringUtils::toWString(const std::string &s)
{
    std::wstring ws;
    int len = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s.c_str(), -1, NULL, 0);
    if (len == 0) {
        return ws;
    }
    len += 1;
    std::unique_ptr<wchar_t[]> wstr(new wchar_t[len]);
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s.c_str(), -1, wstr.get(), len);
    ws = std::wstring(wstr.get());
    return ws;
}

std::string StringUtils::toString(const std::wstring &ws)
{
    std::string s;
    int len = WideCharToMultiByte(CP_ACP, 0, ws.c_str(), -1, NULL, 0, NULL, NULL);
    if (len == 0) {
        return s;
    }
    len += 1;
    std::unique_ptr<char[]> str(new char[len]);
    WideCharToMultiByte(CP_ACP, 0, ws.c_str(), -1, str.get(), len, NULL, NULL);
    s = std::string(str.get());
    return s;
}

std::string StringUtils::gbkToUtf8(const std::string &gbk)
{
    int len = MultiByteToWideChar(CP_ACP, 0, gbk.c_str(), -1, NULL, 0);
    std::unique_ptr<wchar_t[]> wstr(new wchar_t[len + 1]);
    memset(wstr.get(), 0, len + 1);
    MultiByteToWideChar(CP_ACP, 0, gbk.c_str(), -1, wstr.get(), len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr.get(), -1, NULL, 0, NULL, NULL);
    std::unique_ptr<char[]> utf8(new char[len + 1]);
    memset(utf8.get(), 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr.get(), -1, utf8.get(), len, NULL, NULL);
    return std::string(utf8.get());
}

std::string StringUtils::utf8ToGbk(const std::string &utf8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
    std::unique_ptr<wchar_t[]> wszGBK(new wchar_t[len + 1]);
    memset(wszGBK.get(), 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, wszGBK.get(), len);
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK.get(), -1, NULL, 0, NULL, NULL);
    std::unique_ptr<char[]> szGBK(new char[len + 1]);
    memset(szGBK.get(), 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wszGBK.get(), -1, szGBK.get(), len, NULL, NULL);
    return std::string(szGBK.get());
}

std::string StringUtils::asciiToUtf8(const std::string &ascii)
{
    int len = MultiByteToWideChar(CP_OEMCP, 0, ascii.c_str(), -1, NULL, 0);
    std::unique_ptr<wchar_t[]> wstr(new wchar_t[len + 1]);
    memset(wstr.get(), 0, len + 1);
    MultiByteToWideChar(CP_OEMCP, 0, ascii.c_str(), -1, wstr.get(), len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr.get(), -1, NULL, 0, NULL, NULL);
    std::unique_ptr<char[]> utf8(new char[len + 1]);
    memset(utf8.get(), 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr.get(), -1, utf8.get(), len, NULL, NULL);
    return std::string(utf8.get());
}

std::string StringUtils::utf8ToAscii(const std::string &utf8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
    std::unique_ptr<wchar_t[]> wszAscii(new wchar_t[len + 1]);
    memset(wszAscii.get(), 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, wszAscii.get(), len);
    len = WideCharToMultiByte(CP_OEMCP, 0, wszAscii.get(), -1, NULL, 0, NULL, NULL);
    std::unique_ptr<char[]> szAscii(new char[len + 1]);
    memset(szAscii.get(), 0, len + 1);
    WideCharToMultiByte(CP_OEMCP, 0, wszAscii.get(), -1, szAscii.get(), len, NULL, NULL);
    return std::string(szAscii.get());
}

std::string StringUtils::format(const char *pszFormat, ...)
{
    char buffer[MAX_PATH] = { 0 };

    va_list ap;
    va_start(ap, pszFormat);
    int nCount = ::vsprintf_s(buffer, _countof(buffer), pszFormat, ap);
    va_end(ap);

    if (nCount < 0) {
        return pszFormat;
    }

    return buffer;
}

std::wstring StringUtils::wformat(const wchar_t *pszFormat, ...)
{
    wchar_t buffer[MAX_PATH] = { 0 };

    va_list ap;
    va_start(ap, pszFormat);
    int nCount = ::vswprintf_s(buffer, _countof(buffer), pszFormat, ap);
    va_end(ap);

    if (nCount < 0) {
        return pszFormat;
    }

    return buffer;
}

char StringUtils::Hex::toInt4(char c)
{
    char x = 0;
    if (c >= 'A' && c <= 'Z') {
      x = c - 'A' + 10;
    } else if (c >= 'a' && c <= 'z') {
       x = c - 'a' + 10;
    } else if (c >= '0' && c <= '9') {
       x = c - '0';
    }
    return x;
}

WCHAR StringUtils::Hex::toInt4(WCHAR c)
{
    WCHAR x = 0;
    if (c >= 'A' && c <= 'Z') {
      x = c - 'A' + 10;
    } else if (c >= 'a' && c <= 'z') {
       x = c - 'a' + 10;
    } else if (c >= '0' && c <= '9') {
       x = c - '0';
    }
    return x;
}

unsigned char StringUtils::Hex::toInt8(const char* hex)
{
    unsigned char x0 = toInt4(hex[1]);
    unsigned char x1 = toInt4(hex[0]);
    return (x1<<4) + x0;
}
unsigned short StringUtils::Hex::toInt16(const std::string &hex)
{
    unsigned char x0 = toInt4(hex[3]);
    unsigned char x1 = toInt4(hex[2]);
    unsigned char x2 = toInt4(hex[1]);
    unsigned char x3 = toInt4(hex[0]);
    return (x3<<12) + (x2<<8) + (x1<<4) + x0;
}

unsigned short StringUtils::Hex::toInt16(const std::wstring &hex)
{
    unsigned char x0 = toInt4(hex[3]);
    unsigned char x1 = toInt4(hex[2]);
    unsigned char x2 = toInt4(hex[1]);
    unsigned char x3 = toInt4(hex[0]);
    return (x3<<12) + (x2<<8) + (x1<<4) + x0;
}

unsigned int StringUtils::Hex::toInt32(const std::string &hex)
{
    unsigned int x = 0;
    for (int i = 7; i >= 0; i--) {
        x |= (toInt4(hex[i])<<i*4);
    }
    return x;
}

unsigned int StringUtils::Hex::toInt32(const std::wstring &hex)
{
    unsigned int x = 0;
    for (int i = 7; i >= 0; i--) {
        x |= (toInt4(hex[i])<<i*4);
    }
    return x;
}
