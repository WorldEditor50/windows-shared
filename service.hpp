#ifndef SERVICE_HPP
#define SERVICE_HPP
#include <Windows.h>
#include <stdio.h>
#include <stdio.h>
#include <string>

class Service
{
public:
    static BOOL create(const std::wstring &filePath,
                       const std::wstring &serviceName,
                       const std::wstring &displayName)
    {
        SC_HANDLE schSCManager;
        // Open a handle to the SC Manager database.
        schSCManager = OpenSCManager(NULL,                    // local machine
                                     NULL,                    // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS);  // full access rights

        if (schSCManager == NULL) {
           printf("OpenSCManager failed (%d)/n", GetLastError());
           return FALSE;
        }
        SC_HANDLE schService = CreateService(schSCManager,              // SCManager database
                                             serviceName.c_str(),       // name of service
                                             displayName.c_str(),       // service name to display
                                             SERVICE_ALL_ACCESS,        // desired access
                                             SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, // service type
                                             SERVICE_AUTO_START,        // start type
                                             SERVICE_ERROR_NORMAL,      // error control type
                                             filePath.c_str(),          // path to service's binary
                                             NULL,                      // no load ordering group
                                             NULL,                      // no tag identifier
                                             NULL,                      // no dependencies
                                             NULL,                      // LocalSystem account
                                             NULL);                     // no password

         if (schService == NULL) {
             printf("CreateService failed (%d)/n", GetLastError());
             return FALSE;
         }

         if (!StartService(schService, NULL, NULL)){
             CloseServiceHandle(schService);
             printf("StartService failed (%d)/n", GetLastError());
             return FALSE;
         }
         CloseServiceHandle(schService);
         return TRUE;
    }

    BOOL remove(const std::wstring &serviceName)
    {
        SC_HANDLE schSCManager;
        // Open a handle to the SC Manager database.
        schSCManager = OpenSCManager(NULL,                    // local machine
                                     NULL,                    // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS);  // full access rights

        if (schSCManager == NULL) {
            printf("OpenSCManager failed (%d)/n", GetLastError());
            return FALSE;
        }
        SC_HANDLE schService = OpenService(schSCManager,            // SCManager database
                                           serviceName.c_str(),     // name of service
                                           DELETE);                 // only need DELETE access

         if (schService == NULL) {
             printf("OpenService failed (%d)/n", GetLastError());
             return FALSE;
         }

         if (!DeleteService(schService)) {
             printf("DeleteService failed (%d)/n", GetLastError());
             return FALSE;
         }
         CloseServiceHandle(schService);
         return TRUE;
     }


};


#endif // SERVICE_HPP
