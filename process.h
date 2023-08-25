#ifndef PROCESS_H
#define PROCESS_H
#include <stdio.h>
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <string>
#include <vector>
class Process
{
public:
    class Property
    {
    public:
        unsigned long pid;
        std::wstring name;
    public:
        Property(){}
        Property(unsigned long id, const std::wstring &procName)
            :pid(id),name(procName){}
    };
public:
     PROCESS_INFORMATION stProcessInfo;
     STARTUPINFO stStartUpInfo;
     std::wstring env;
     std::wstring dir;
public:
     Process(){}
     ~Process();
     template<typename T>
     int read(LPCVOID addr, T* &data, size_t length)
     {
         return ReadProcessMemory(stProcessInfo.hProcess, addr, data, sizeof(T)*length, nullptr);
     }

     template<typename T>
     int write(LPVOID addr, T *data, size_t length)
     {
          return WriteProcessMemory(stProcessInfo.hProcess, addr, data, sizeof(T)*length, nullptr);
     }

     HMODULE getModuleAddr(LPCWSTR lpModuleName);

     int start(std::wstring &fileName, const std::vector<std::wstring> &paramList);
     int startDetached(std::wstring &fileName, const std::vector<std::wstring> &paramList);
     int waitForFinished(int timeout);
     void stop();

     void find(LPCWSTR lpName, BOOL bWindow = FALSE);

     static std::wstring find(DWORD pid);
     static std::vector<DWORD> find(const std::wstring &processName);
     static int kill(DWORD pid);
     static void kill(const std::wstring &processName);
     static std::vector<Property> getAllProcess();
     static std::wstring getForegroundApp();
};



#endif // PROCESS_H
