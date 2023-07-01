#include "process.h"

Process::~Process()
{
     if (hProcess != nullptr){
         CloseHandle(hProcess);
     }
}

Process::Process(LPCWSTR lpName, BOOL bWindow)
{
     if (bWindow == true){
         HWND hWnd = FindWindowW(NULL, lpName);
         if(hWnd == NULL){
             return;
         }
         GetWindowThreadProcessId(hWnd, &(pid));
         hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
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
                 pid = pe32.th32ProcessID;
                 hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
                 break;
             }

        } while (Process32Next(hProcessSnap, &pe32));

        CloseHandle(hProcessSnap);
     }
}


HMODULE Process::getModuleAddr(LPCWSTR lpModuleName)
{
     HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
     MODULEENTRY32 me32;
     HMODULE hModule = 0;

    // Take a snapshot of all modules in the specified process.
    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
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
