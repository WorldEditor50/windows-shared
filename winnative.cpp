#include "winnative.h"

std::wstring WindowsNative::stringToWString(const std::string &s)
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

std::string WindowsNative::wstringToString(const std::wstring &ws)
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

std::string WindowsNative::gbkToUtf8(const std::string &gbk)
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

std::string WindowsNative::utf8ToGbk(const std::string &utf8)
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

std::string WindowsNative::asciiToUtf8(const std::string &ascii)
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

std::string WindowsNative::utf8ToAscii(const std::string &utf8)
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

std::string WindowsNative::findProcess(unsigned long pid)
{
    std::string procName;
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return procName;
    }
    BOOL ret = Process32First(hProcessSnap, &pe32);
    while (ret)  {
        if (pe32.th32ProcessID == pid) {
            procName = wstringToString(pe32.szExeFile);
            break;
        }
        ret = Process32Next(hProcessSnap, &pe32);
    }
    CloseHandle(hProcessSnap);
    return procName;
}

unsigned long WindowsNative::findProcess(const std::string &procName)
{
    DWORD pid = 0;
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return pid;
    }
    BOOL ret = Process32First(hProcessSnap, &pe32);
    while (ret)  {
        if (wstringToString(pe32.szExeFile) == procName) {
            pid = pe32.th32ProcessID;
            break;
        }
        ret = Process32Next(hProcessSnap, &pe32);
    }
    CloseHandle(hProcessSnap);
    return pid;
}

std::vector<WindowsNative::Proc> WindowsNative::getAllProcess()
{
    std::vector<WindowsNative::Proc> proc;
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return proc;
    }
    BOOL ret = Process32First(hProcessSnap, &pe32);
    while (ret)  {
        proc.push_back(Proc(pe32.th32ProcessID,wstringToString(pe32.szExeFile)));
        ret = Process32Next(hProcessSnap, &pe32);
    }
    CloseHandle(hProcessSnap);
    return proc;
}

std::string WindowsNative::getForegroundApp()
{
    std::string appName;
    /* get foreground window */
    HWND hwnd = GetForegroundWindow();
    if (hwnd == NULL) {
        return appName;
    }
    /* get process's name */
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    return findProcess(pid);
}

std::string WindowsNative::getForegroundUwpApp()
{
    std::string appName;

    return appName;
}

int WindowsNative::raise(const std::string &appName)
{
    return 0;
}

void WindowsNative::exec(const std::string &cmd, bool visible)
{
    int flag = SW_HIDE;
    if (visible == true) {
        flag = SW_SHOW;
    }
    WinExec(cmd.c_str(), flag);
    return;
}

void WindowsNative::openUrl(const std::string &url, bool visible)
{
    int flag = SW_HIDE;
    if (visible == true) {
        flag = SW_SHOW;
    }
    ShellExecute(NULL, TEXT("open"), stringToWString(url).c_str(), NULL, NULL, flag);
    return;
}

void WindowsNative::execAsAdmin(const std::string &cmd)
{
    SHELLEXECUTEINFO info;
    info.fMask = NULL;
    info.lpVerb = L"runas";
    info.lpFile = L"cmd.exe";
    info.lpParameters = stringToWString(cmd).c_str();
    info.nShow = SW_HIDE;
    ShellExecuteEx(&info);
    return;
}

void WindowsNative::elevateCurrentProcess()
{
    if (IsUserAnAdmin()) {
        return;
    }
    HANDLE hProc = GetCurrentProcess();
    WCHAR appPath[MAX_PATH];
    DWORD ret = GetModuleFileName(NULL, appPath, MAX_PATH);
    if (ret != 0) {
        return;
    }
    SHELLEXECUTEINFO info;
    info.cbSize = sizeof (SHELLEXECUTEINFO);
    info.fMask = NULL;
    info.hProcess = hProc;
    info.lpVerb = L"runas";
    info.lpFile = appPath;
    info.lpParameters = NULL;
    info.nShow = SW_HIDE;
    ShellExecuteEx(&info);
    return;
}
