#include "process.h"

Process::~Process()
{
    stop();
}

HMODULE Process::getModuleAddr(LPCWSTR lpModuleName)
{
     HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
     MODULEENTRY32 me32;
     HMODULE hModule = 0;

    // Take a snapshot of all modules in the specified process.
    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, stProcessInfo.dwProcessId);
    if (hModuleSnap == INVALID_HANDLE_VALUE) {
        //printf("Error: CreateToolhelp32Snapshot (of modules) ");
        return NULL;
    }

    // Set the size of the structure before using it.
    me32.dwSize = sizeof(MODULEENTRY32);

    // Retrieve information about the first module,
    // and exit if unsuccessful
    if (!Module32First(hModuleSnap, &me32)) {
        //printf("Error: Module32First ");  // show cause of failure
        CloseHandle(hModuleSnap);     // clean the snapshot object
        return NULL;
    }

    // Now walk the module list of the process,
    // and display information about each module
    do {
        if (!wcscmp(lpModuleName, me32.szModule)) {
            hModule = (HMODULE)me32.modBaseAddr;
            CloseHandle(hModuleSnap);
            return hModule;
        }

    } while (Module32Next(hModuleSnap, &me32));

    CloseHandle(hModuleSnap);
    return NULL;
}

int Process::start(std::wstring &fileName, const std::vector<std::wstring> &paramList)
{
    ::memset(&stStartUpInfo, 0 ,sizeof(stStartUpInfo));
    stStartUpInfo.cb = sizeof(stStartUpInfo);

    ::memset(&stProcessInfo, 0 ,sizeof(stProcessInfo));
    std::wstring params = L" ";//与执行文件名分开
    for (std::size_t i = 0; i < paramList.size(); i++) {
        params += paramList[i] + L" ";
    }
    bool bRet = ::CreateProcess(fileName.c_str(),
                                (WCHAR*)params.c_str(),
                                NULL,
                                NULL,
                                FALSE,
                                CREATE_NEW_CONSOLE,
                                NULL,
                                NULL,
                                &stStartUpInfo,
                                &stProcessInfo);

    if (bRet) {

    } else {
        //如果创建进程失败，查看错误码
        DWORD dwErrCode = GetLastError();
        printf_s("ErrCode : %d\n",dwErrCode);
    }
    return 0;
}

int Process::startDetached(std::wstring &fileName, const std::vector<std::wstring> &paramList)
{
    ::memset(&stStartUpInfo, 0 ,sizeof(stStartUpInfo));
    stStartUpInfo.cb = sizeof(stStartUpInfo);

    ::memset(&stProcessInfo, 0 ,sizeof(stProcessInfo));
    std::wstring params = L" ";//与执行文件名分开
    for (std::size_t i = 0; i < paramList.size(); i++) {
        params += paramList[i] + L" ";
    }
    BOOL bRet = ::CreateProcess(fileName.c_str(),
                                (WCHAR*)params.c_str(),
                                NULL,
                                NULL,
                                FALSE,
                                DETACHED_PROCESS,
                                NULL,
                                NULL,
                                &stStartUpInfo,
                                &stProcessInfo);

    if (bRet == FALSE) {
        //如果创建进程失败，查看错误码
        DWORD dwErrCode = GetLastError();
        printf_s("ErrCode : %d\n",dwErrCode);
    }
    return 0;
}

int Process::waitForFinished(int timeout)
{
    return WaitForSingleObject(stProcessInfo.hProcess, timeout);
}

void Process::stop()
{
    if (stProcessInfo.hProcess != NULL) {
        ::CloseHandle(stProcessInfo.hProcess);
    }
    if (stProcessInfo.hThread != NULL) {
        ::CloseHandle(stProcessInfo.hThread);
    }

    stProcessInfo.hProcess = NULL;
    stProcessInfo.hThread = NULL;
    stProcessInfo.dwProcessId = 0;
    stProcessInfo.dwThreadId = 0;
    return;
}

void Process::find(LPCWSTR lpName, BOOL bWindow)
{
    if (bWindow == true){
        HWND hWnd = FindWindowW(NULL, lpName);
        if(hWnd == NULL){
            return;
        }
        GetWindowThreadProcessId(hWnd, &stProcessInfo.dwProcessId);
        stProcessInfo.hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, stProcessInfo.dwProcessId);
    } else {
        HANDLE hProcessSnap;
        PROCESSENTRY32 pe32;

       // Take a snapshot of all processes in the system.
        hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hProcessSnap == INVALID_HANDLE_VALUE)
        {
            //printf("Error: CreateToolhelp32Snapshot (of processes) ");
            return;
        }

       // Set the size of the structure before using it.
        pe32.dwSize = sizeof(PROCESSENTRY32);

       // Retrieve information about the first process,
        // and exit if unsuccessful
        if (!Process32First(hProcessSnap, &pe32))
        {
            //printf("Error: Process32First "); // show cause of failure
            CloseHandle(hProcessSnap);          // clean the snapshot object
            return;
        }

       // Now walk the snapshot of processes, and
        // display information about each process in turn
        int namelen = 200;
        char name[201] = { 0 };
        do {
            if (!wcscmp(pe32.szExeFile, lpName)) {
                stProcessInfo.dwProcessId = pe32.th32ProcessID;
                stProcessInfo.hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, stProcessInfo.dwProcessId);
                break;
            }

       } while (Process32Next(hProcessSnap, &pe32));

       CloseHandle(hProcessSnap);
    }
    return;
}



std::wstring Process::find(DWORD pid)
{
    std::wstring procName;
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return procName;
    }
    BOOL ret = Process32First(hProcessSnap, &pe32);
    while (ret)  {
        if (pe32.th32ProcessID == pid) {
            procName = std::wstring(pe32.szExeFile);
            break;
        }
        ret = Process32Next(hProcessSnap, &pe32);
    }
    CloseHandle(hProcessSnap);
    return procName;
}

std::vector<DWORD> Process::find(const std::wstring &procName)
{
    std::vector<DWORD> pids;
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return pids;
    }
    BOOL ret = Process32First(hProcessSnap, &pe32);
    while (ret)  {
        if (std::wstring(pe32.szExeFile) == procName) {
            pids.push_back(pe32.th32ProcessID);
        }
        ret = Process32Next(hProcessSnap, &pe32);
    }
    CloseHandle(hProcessSnap);
    return pids;
}

int Process::kill(DWORD pid)
{
    HANDLE handle = ::OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (handle == nullptr) {
        return -1;
    }

    BOOL ret = TerminateProcess(handle, 0);
    if (ret != 0) {
        return -2;
    }
    ::WaitForSingleObject(handle, 1000);
    CloseHandle(handle);
    return 0;
}

void Process::kill(const std::wstring &processName)
{
    std::vector<DWORD> pids = find(processName);
    for (std::size_t i = 0; i < pids.size(); i++) {
        kill(pids[i]);
    }
    return;
}

std::vector<Process::Property> Process::getAllProcess()
{
    std::vector<Process::Property> proc;
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return proc;
    }
    BOOL ret = Process32First(hProcessSnap, &pe32);
    while (ret)  {
        proc.push_back(Process::Property(pe32.th32ProcessID, pe32.szExeFile));
        ret = Process32Next(hProcessSnap, &pe32);
    }
    CloseHandle(hProcessSnap);
    return proc;
}

std::wstring Process::getForegroundApp()
{
    std::wstring appName;
    /* get foreground window */
    HWND hwnd = GetForegroundWindow();
    if (hwnd == NULL) {
        return appName;
    }
    /* get process's name */
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    return find(pid);
}

