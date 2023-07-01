#ifndef PROCESS_H
#define PROCESS_H
#include <stdio.h>
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

class Process
{
public:
     DWORD pid;
     HANDLE hProcess;
public:
     explicit Process(LPCWSTR lpName, BOOL bWindow = FALSE);
     ~Process();
     template<typename T>
     int read(LPCVOID addr, T* &data, size_t length)
     {
         return ReadProcessMemory(hProcess, addr, data, sizeof(T)*length, nullptr);
     }

     template<typename T>
     int write(LPVOID addr, T *data, size_t length)
     {
          return WriteProcessMemory(hProcess, addr, data, sizeof(T)*length, nullptr);
     }

     HMODULE getModuleAddr(LPCWSTR lpModuleName);

};



#endif // PROCESS_H
