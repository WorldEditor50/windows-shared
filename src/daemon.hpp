#ifndef DAEMON_HPP
#define DAEMON_HPP
#include <Windows.h>
#include <ShlObj.h>
#include <TlHelp32.h>
#include <string>
#include <vector>

#define DAEMON_CHECK_INTERVAL 3000 // 3 seconds

class Daemon
{
public:
    class Application
    {
    public:
        std::wstring name;
        std::wstring op;
        std::wstring filePath;
        std::wstring params;
        std::wstring dir;
        int displayFlag;
    public:
        Application(){}
        void run()
        {
            SHELLEXECUTEINFO info = {0};
            info.cbSize = sizeof(SHELLEXECUTEINFO);
            info.fMask = SEE_MASK_NOCLOSEPROCESS;
            info.hwnd = NULL;
            if (op.empty()) {
                info.lpVerb = L"open";
            } else {
                info.lpVerb = op.c_str();
            }
            info.lpFile = filePath.c_str();
            if (params.empty()) {
                info.lpParameters = NULL;
            } else {
                info.lpParameters = params.c_str();
            }
            if (dir.empty()) {
                info.lpDirectory = NULL;
            } else {
                info.lpDirectory = dir.c_str();
            }
            info.nShow = displayFlag;
            info.hInstApp = NULL;
            BOOL ret = ShellExecuteEx(&info);
            if (ret == TRUE) {
                WaitForSingleObject(info.hProcess, 3000);
            }
            return;
        }
    };
private:
    std::vector<Application> monitorList;
private:
    Daemon(){}
public:
    inline static Daemon& instance()
    {
        static Daemon daemon;
        return daemon;
    }
    ~Daemon(){}

    static DWORD getProcessID(const std::wstring &name)
    {
        PROCESSENTRY32 pe;
        DWORD id = 0;
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (!Process32First(hSnapshot, &pe)) {
            return 0;
        }

        do
        {
            pe.dwSize = sizeof(PROCESSENTRY32);
            if (Process32Next(hSnapshot, &pe) == FALSE) {
                break;
            }
            if (wcscmp(pe.szExeFile, name.c_str()) == 0) {
                id = pe.th32ProcessID;
                break;
            }

        } while (1);

        CloseHandle(hSnapshot);
        return id;
    }

    void registerApplication(const Application &app)
    {
        for (std::size_t i = 0; i < monitorList.size(); i++) {
            if (monitorList[i].name == app.name) {
                return;
            }
        }
        monitorList.push_back(app);
        return;
    }

    void run()
    {
        /* 检查守护进程是否已启动 */
        DWORD pid = 0;
        pid = getProcessID(L"daemon.exe");
        if (pid == 0) {
            return;
        }
        while (1) {
            /* 检查测试程序是否正在运行 */
            for (std::size_t i = 0; i < monitorList.size(); i++) {
                DWORD pid = 0;
                pid = getProcessID(monitorList[i].name.c_str());
                if (pid != 0) {
                    continue;
                }
                monitorList[i].run();
            }
            Sleep(DAEMON_CHECK_INTERVAL);
        }
        return;
    }
};

#endif // DAEMON_HPP
