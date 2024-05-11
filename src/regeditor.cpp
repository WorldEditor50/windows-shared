#include "regeditor.h"

RegEditor::RegEditor()
{

}

LONG RegEditor::createKey(HKEY hKey, const std::wstring &subKey)
{
    HKEY hNewKey = nullptr;
    DWORD disposition = 0;
    LONG ret = RegCreateKeyExW(hKey, subKey.c_str(), 0, nullptr,
                              REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, nullptr, &hNewKey, &disposition);
    if (ret != ERROR_SUCCESS) {
        return ret;
    }
    if (hNewKey != nullptr) {
        RegCloseKey(hNewKey);
    }
    return ERROR_SUCCESS;
}

LONG RegEditor::deleteKey(HKEY hKey, const std::wstring &subKey)
{
    return RegDeleteTreeW(hKey, subKey.c_str());
}

LONG RegEditor::getDWORD(HKEY hKey,
                        const std::wstring& subKey,
                        const std::wstring& value,
                        DWORD &data)
{
    DWORD datasize = sizeof (DWORD);
    return RegGetValueW(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_DWORD,
                       nullptr, &data, &datasize);
}

LONG RegEditor::getQWORD(HKEY hKey, const std::wstring &subKey, const std::wstring &value, unsigned long &data)
{
    DWORD datasize = sizeof (unsigned long);
    return RegGetValueW(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_QWORD,
                       nullptr, &data, &datasize);
}


LONG RegEditor::getRegSZ(HKEY hKey, const std::wstring &subKey, const std::wstring &value, std::wstring &data)
{
    DWORD datasize = 0;
    /* get length */
    LONG ret = RegGetValueW(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_SZ,
                       nullptr, nullptr, &datasize);
    if (ret != ERROR_SUCCESS) {
        return ret;
    }
    /* get data */
    data.resize(datasize);
    ret = RegGetValueW(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_SZ,
                       nullptr, &data[0], &datasize);
    if (ret != ERROR_SUCCESS) {
        return ret;
    }
    return ERROR_SUCCESS;
}

LONG RegEditor::getRegExpandSZ(HKEY hKey, const std::wstring &subKey, const std::wstring &value, std::wstring &data)
{
    DWORD datasize = 0;
    /* get length */
    LONG ret = RegGetValueW(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_EXPAND_SZ,
                       nullptr, nullptr, &datasize);
    if (ret != ERROR_SUCCESS) {
        return ret;
    }
    /* get data */
    data.resize(datasize);
    ret = RegGetValueW(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_EXPAND_SZ,
                       nullptr, &data[0], &datasize);
    if (ret != ERROR_SUCCESS) {
        return ret;
    }
    return ERROR_SUCCESS;
}

LONG RegEditor::getRegMultiSZ(HKEY hKey, const std::wstring &subKey, const std::wstring &value, std::vector<std::wstring> &data)
{
    DWORD datasize = 0;
    /* get length */
    LONG ret = RegGetValueW(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_MULTI_SZ,
                       nullptr, nullptr, &datasize);
    if (ret != ERROR_SUCCESS) {
        return ret;
    }
    /* get data */
    std::vector<WCHAR> data_(datasize);
    ret = RegGetValueW(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_MULTI_SZ,
                       nullptr, &data[0], &datasize);
    if (ret != ERROR_SUCCESS) {
        return ret;
    }

    /* parse */
    const WCHAR* currStringPtr = &data_[0];
    while (*currStringPtr != L'\0') {
      // Current string is NUL-terminated, so get its length with wcslen
      const size_t currStringLength = wcslen(currStringPtr);
      // Add current string to result vector
      data.push_back(std::wstring{ currStringPtr, currStringLength });
      // Move to the next string
      currStringPtr += currStringLength + 1;
    }
    return ERROR_SUCCESS;
}

LONG RegEditor::getBinary(HKEY hKey, const std::wstring &subKey, const std::wstring &value, std::vector<WCHAR> &data)
{
    DWORD datasize = 0;
    /* get length */
    LONG ret = RegGetValueW(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_BINARY,
                       nullptr, nullptr, &datasize);
    if (ret != ERROR_SUCCESS) {
        return ret;
    }
    /* get data */
    data.resize(datasize);
    ret = RegGetValueW(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_BINARY,
                       nullptr, &data[0], &datasize);
    if (ret != ERROR_SUCCESS) {
        return ret;
    }
    return ERROR_SUCCESS;
}

LONG RegEditor::enumValues(HKEY hKey, std::vector<std::pair<std::wstring, DWORD> > &values)
{
    DWORD valueCount = 0;
    DWORD maxValueNameLen = 0;
    LONG ret = RegQueryInfoKeyW(hKey, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                               &valueCount, &maxValueNameLen, nullptr, nullptr, nullptr);
    if (ret != ERROR_SUCCESS) {
        return ret;
    }
    maxValueNameLen++;
    auto nameBuffer = std::make_unique<WCHAR[]>(maxValueNameLen);
    for (DWORD index = 0; index < valueCount; index++) {
        DWORD valueNameLen = maxValueNameLen;
        DWORD valueType{};
        ret = ::RegEnumValueW(hKey, index, nameBuffer.get(), &valueNameLen,
                             nullptr, &valueType, nullptr,  nullptr);
        if (ret != ERROR_SUCCESS) {
            continue;
        }
        values.push_back(std::make_pair(
          std::wstring{ nameBuffer.get(), valueNameLen },
          valueType
        ));
    }
    return ERROR_SUCCESS;
}

LONG RegEditor::setDWORD(HKEY hKey, const std::wstring &subKey, const std::wstring &value, DWORD data)
{
    DWORD datasize = sizeof (DWORD);
    return RegSetKeyValueW(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_DWORD, &data, datasize);
}

LONG RegEditor::setQWORD(HKEY hKey, const std::wstring &subKey, const std::wstring &value, unsigned long data)
{
    DWORD datasize = sizeof (unsigned long);
    return RegSetKeyValueW(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_QWORD, &data, datasize);
}

LONG RegEditor::setRegSZ(HKEY hKey, const std::wstring &subKey, const std::wstring &value, const std::wstring &data)
{
    DWORD datasize = data.size();
    return RegSetKeyValueW(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_SZ, &data[0], datasize);
}

LONG RegEditor::setRegExpandSZ(HKEY hKey, const std::wstring &subKey, const std::wstring &value, const std::wstring &data)
{
    DWORD datasize = data.size();
    return RegSetKeyValueW(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_EXPAND_SZ, &data[0], datasize);
}

LONG RegEditor::setRegMultiSZ(HKEY hKey, const std::wstring &subKey, const std::wstring &value, std::vector<std::wstring> &data)
{
    std::vector<WCHAR> data_;
    std::size_t pos = 0;
    for (std::size_t i = 0; i < data.size(); i++) {
        std::copy(data[i].begin(), data[i].end(), data_.begin() + pos);
        pos += data.size();
    }
    DWORD datasize = data_.size();
    return RegSetKeyValueW(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_MULTI_SZ,
                          &data_[0], datasize);
}

LONG RegEditor::setBinary(HKEY hKey, const std::wstring &subKey, const std::wstring &value, std::vector<WCHAR> &data)
{
    DWORD datasize = data.size();
    return RegSetKeyValueW(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_EXPAND_SZ, &data[0], datasize);
}
