#ifndef CRASHDUMP_H
#define CRASHDUMP_H
#include <Windows.h>
#include <DbgHelp.h>
#include <minidumpapiset.h>

#include <string>
#include <ctime>

#pragma comment(lib, "dbghelp.lib")

class CrashDump
{
public:
    std::wstring dumpFile;
    std::wstring root;
    void* event;

private:
    CrashDump(){}


public:
    ~CrashDump(){}
    static CrashDump& instance()
    {
        static CrashDump dump;
        return dump;
    }
    void setDumpFile(const std::string &rootPath, const std::string &appName);

    void registerDumpHandler(void* pInstallEvent=NULL);

    void registerQuitHandler()
    {
        return;
    }

    BOOL isDataSectionNeeded(const WCHAR *moduleName);

    BOOL createMiniDump(PEXCEPTION_POINTERS pExceptionPointers);

    static LONG unhandledExceptionFilterEx(PEXCEPTION_POINTERS pExceptionPointers);

};

#endif // CRASHDUMP_H
