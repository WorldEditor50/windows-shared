#ifndef INIHELPER_HPP
#define INIHELPER_HPP
#include <Windows.h>
#include <string>


class IniHelper
{
protected:
    std::wstring iniFile;
public:
    explicit IniHelper(const std::wstring &filePath):iniFile(filePath){}
    bool write(const std::wstring &section, const std::wstring &keyValue)
    {
       return WritePrivateProfileSection(section.c_str(), keyValue.c_str(), iniFile.c_str());
    }

    int readInt(const std::wstring &section, const std::wstring &key, int defaultValue)
    {
        int value = defaultValue;
        value = GetPrivateProfileInt(section.c_str(), key.c_str(), defaultValue, iniFile.c_str());
        return value;
    }

    std::wstring readString(const std::wstring &section,
                            const std::wstring &key,
                            const std::wstring &defaultValue,
                            std::size_t totalsize=1024)
    {
        std::wstring value(totalsize, 0);
        GetPrivateProfileString(section.c_str(),
                                key.c_str(),
                                defaultValue.c_str(),
                                (WCHAR*)value.c_str(),
                                totalsize,
                                iniFile.c_str());
        return value;
    }
};

#endif // INIHELPER_HPP
