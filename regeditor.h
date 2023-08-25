#ifndef REGEDITOR_H
#define REGEDITOR_H
#include <Windows.h>
#include <string>
#include <memory>
#include <vector>

#pragma comment(lib, "Advapi32.lib")

class RegEditor
{
public:
    RegEditor();
    /* create */
    static LONG createKey(HKEY hKey, const std::wstring& subKey);
    /* delete */
    static LONG deleteKey(HKEY hKey, const std::wstring& subKey);
    /* get */
    static LONG getDWORD(HKEY hKey, const std::wstring& subKey, const std::wstring& value, DWORD &data);
    static LONG getQWORD(HKEY hKey, const std::wstring& subKey, const std::wstring& value, unsigned long &data);
    static LONG getRegSZ(HKEY hKey, const std::wstring& subKey, const std::wstring& value, std::wstring &data);
    static LONG getRegExpandSZ(HKEY hKey, const std::wstring& subKey, const std::wstring& value, std::wstring &data);
    static LONG getRegMultiSZ(HKEY hKey, const std::wstring& subKey, const std::wstring& value, std::vector<std::wstring> &data);
    static LONG getBinary(HKEY hKey, const std::wstring& subKey, const std::wstring& value, std::vector<WCHAR> &data);
    static LONG enumValues(HKEY hKey, std::vector<std::pair<std::wstring, DWORD> > &values);
    /* set */
    static LONG setDWORD(HKEY hKey, const std::wstring &subKey, const std::wstring &value, DWORD data);
    static LONG setQWORD(HKEY hKey, const std::wstring &subKey, const std::wstring &value, unsigned long data);
    static LONG setRegSZ(HKEY hKey, const std::wstring &subKey, const std::wstring &value, const std::wstring &data);
    static LONG setRegExpandSZ(HKEY hKey, const std::wstring &subKey, const std::wstring &value, const std::wstring &data);
    static LONG setRegMultiSZ(HKEY hKey, const std::wstring& subKey, const std::wstring& value, std::vector<std::wstring> &data);
    static LONG setBinary(HKEY hKey, const std::wstring& subKey, const std::wstring& value, std::vector<WCHAR> &data);
};

#endif // REGEDITOR_H
