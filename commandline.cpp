#include "commandline.h"

void CommandLine::exec(const std::string &cmd, bool visible)
{
    int flag = SW_HIDE;
    if (visible == true) {
        flag = SW_SHOW;
    }
    WinExec(cmd.c_str(), flag);
    return;
}

void CommandLine::openUrl(const std::wstring &url, bool visible)
{
    int flag = SW_HIDE;
    if (visible == true) {
        flag = SW_SHOW;
    }
    ShellExecute(NULL, TEXT("open"), url.c_str(), NULL, NULL, flag);
    return;
}

void CommandLine::execAsAdmin(const std::wstring &cmd)
{
    SHELLEXECUTEINFO info;
    info.fMask = NULL;
    info.lpVerb = L"runas";
    info.lpFile = L"cmd.exe";
    info.lpParameters = cmd.c_str();
    info.nShow = SW_HIDE;
    ShellExecuteEx(&info);
    return;
}

void CommandLine::elevateCurrentProcess()
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
