#include "crashdump.h"

BOOL CALLBACK miniDumpCallBack(PVOID pParam,
                               const PMINIDUMP_CALLBACK_INPUT pInput,
                               PMINIDUMP_CALLBACK_OUTPUT pOutput)
{
    if (pInput == 0 || pOutput == 0) {
        return FALSE;
    }
    switch (pInput->CallbackType) {
    case ModuleCallback:
        if (pOutput->ModuleWriteFlags&ModuleWriteDataSeg) {
            if (!CrashDump::instance().isDataSectionNeeded(pInput->Module.FullPath)) {
                pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);
            }
        }
    case IncludeModuleCallback:
    case IncludeThreadCallback:
    case ThreadCallback:
    case ThreadExCallback:
        return TRUE;
    default:
        break;
    }
    return FALSE;
}


void CrashDump::setDumpFile(const std::string &rootPath, const std::string &appName)
{
    if (rootPath.empty() || appName.empty()) {
        return;
    }
    char times[64] = {0};
    char days[64] = {0};

    SYSTEMTIME systime;
    GetLocalTime(&systime);
    sprintf(times, "%02d_%02d_%02d", systime.wHour, systime.wMinute, systime.wSecond);


    time_t currentTime;
    time(&currentTime);

    struct tm itm = *localtime(&currentTime);
    sprintf(days, "%04d%02d%02d", itm.tm_year + 1900, itm.tm_mon, itm.tm_mday);

    char szDumpFile[512];
    //dumpFile =
    return;
}

void CrashDump::registerDumpHandler(void *pInstallEvent)
{
    if (pInstallEvent != NULL) {
        event = pInstallEvent;
    }
    ::SetUnhandledExceptionFilter(unhandledExceptionFilterEx);
    return;
}

BOOL CrashDump::isDataSectionNeeded(const WCHAR *pModuleName)
{
    WCHAR szFileName[MAX_PATH];
    if (pModuleName == NULL) {
        return FALSE;
    }
    _wsplitpath(pModuleName, NULL, NULL, szFileName, NULL);
    if (_wcsicmp(szFileName, L"ntdll")) {
        return TRUE;
    }
    return FALSE;
}

BOOL CrashDump::createMiniDump(PEXCEPTION_POINTERS pExceptionPointers)
{
    if (pExceptionPointers == NULL) {
        return FALSE;
    }
    if (dumpFile.empty()) {
        return FALSE;
    }
    HANDLE hFile = CreateFile(dumpFile.c_str(),
                              GENERIC_READ|GENERIC_WRITE,
                              FILE_SHARE_WRITE,
                              NULL,
                              CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);
    if (hFile == NULL || hFile == INVALID_HANDLE_VALUE) {
        return FALSE;
    }
    /* exception */
    MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
    exceptionInfo.ThreadId = GetCurrentThreadId();
    exceptionInfo.ExceptionPointers = pExceptionPointers;
    exceptionInfo.ClientPointers = NULL;
    /* callback */
    MINIDUMP_CALLBACK_INFORMATION callbackInfo;
    callbackInfo.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)miniDumpCallBack;
    callbackInfo.CallbackParam = NULL;
    /* write dump */
    MiniDumpWriteDump(GetCurrentProcess(),
                      GetCurrentProcessId(),
                      hFile,
                      MiniDumpNormal,
                      &exceptionInfo,
                      NULL,
                      &callbackInfo);
    CloseHandle(hFile);
    return TRUE;
}

LONG CrashDump::unhandledExceptionFilterEx(PEXCEPTION_POINTERS pExceptionPointers)
{
    if (pExceptionPointers == NULL) {
        return EXCEPTION_CONTINUE_SEARCH;
    }

    std::wstring root = CrashDump::instance().root;
    int ret = _waccess(root.c_str(), 0);
    if (ret == -1) {
        CreateDirectory(root.c_str(), 0);
    }
    CrashDump::instance().createMiniDump(pExceptionPointers);
    if (CrashDump::instance().event) {
        CrashDump::instance().registerQuitHandler();
    }
    return EXCEPTION_CONTINUE_SEARCH;
}
