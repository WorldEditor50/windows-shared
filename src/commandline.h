#ifndef COMMANDLINE_H
#define COMMANDLINE_H
#include <Windows.h>
#include <ShlObj.h>
#include <Psapi.h>
#include <string>

class CommandLine
{
public:
    static void exec(const std::string &cmd, bool visible);
    static void openUrl(const std::wstring &url, bool visible);
    static void execAsAdmin(const std::wstring &cmd);
    static void elevateCurrentProcess();
};

#endif // COMMANDLINE_H
