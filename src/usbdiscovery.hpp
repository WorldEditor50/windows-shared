#ifndef DISCOVERY_HPP
#define DISCOVERY_HPP
#include <Windows.h>
#include <Dbt.h>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <iostream>

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Ole32.lib")

class UsbDiscovery
{
public:
    using FnAttachedNotify = std::function<void(unsigned short vid, unsigned short pid)>;
    using FnDetachedNotify = std::function<void(unsigned short vid, unsigned short pid)>;

    class Device
    {
    public:
        unsigned short vendorID;
        unsigned short productID;
        GUID uuid;
        HDEVNOTIFY handle;
        FnAttachedNotify attachedNotify;
        FnDetachedNotify detachedNotify;
    public:
        Device():vendorID(0),productID(0),uuid(GUID_NULL),handle(nullptr){}
        Device(const Device &r):
            vendorID(r.vendorID),productID(r.productID),uuid(r.uuid),handle(r.handle),
            attachedNotify(r.attachedNotify),detachedNotify(r.detachedNotify){}

        Device(unsigned short vid,
               unsigned short pid,
               const GUID &guid,
               const FnAttachedNotify &attachedNotify_,
               const FnDetachedNotify &detachedNotify_):
            vendorID(vid),productID(pid),uuid(guid),handle(nullptr),
            attachedNotify(attachedNotify_),detachedNotify(detachedNotify_){}
    };

private:
    HWND hwnd;
    std::wstring windowName;
    std::map<std::string, Device> deviceMap;
private:

    static LRESULT CALLBACK handleWindowMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        if (message == WM_DEVICECHANGE) {
            DEV_BROADCAST_HDR *broadcast = reinterpret_cast<DEV_BROADCAST_HDR *>(lParam);
            if (broadcast == nullptr) {
                std::cout<<"failed at broadcast"<<std::endl;
                return ::DefWindowProcW(hwnd, message, wParam, lParam);
            }
            UsbDiscovery *discovery = reinterpret_cast<UsbDiscovery*>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA));
            if (discovery == nullptr) {
                std::cout<<"failed at discovery"<<std::endl;
                return ::DefWindowProcW(hwnd, message, wParam, lParam);
            }

            /* parse device */
            DEV_BROADCAST_DEVICEINTERFACE_W *devInterface = reinterpret_cast<DEV_BROADCAST_DEVICEINTERFACE_W*>(broadcast);

            std::string uuid = guid2String(devInterface->dbcc_classguid);
            if (discovery->deviceMap.find(uuid) == discovery->deviceMap.end()) {
                std::cout<<"failed at find uuid"<<std::endl;
                return ::DefWindowProcW(hwnd, message, wParam, lParam);
            }
            Device& dev = discovery->deviceMap[uuid];
            /* parse vid, pid */
            unsigned short vid = 0;
            unsigned short pid = 0;
            int ret = parse(devInterface->dbcc_name, vid, pid);
            if (ret != 0) {
                return ::DefWindowProcW(hwnd, message, wParam, lParam);
            }

            switch (wParam) {
            case DBT_DEVICEARRIVAL:
                dev.attachedNotify(vid, pid);
                break;
            case DBT_DEVICEREMOVECOMPLETE:
                dev.detachedNotify(vid, pid);
                break;
            default:
                break;
            }
        }

        return ::DefWindowProcW(hwnd, message, wParam, lParam);
    }

    bool createMessageWindow()
    {
        HINSTANCE hi = ::GetModuleHandleW(nullptr);

        WNDCLASSW wc;
        memset(&wc, 0, sizeof(WNDCLASSW));
        wc.lpfnWndProc = UsbDiscovery::handleWindowMessage;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hi;
        wc.lpszClassName = windowName.c_str();
        ::RegisterClassW(&wc);

        hwnd = ::CreateWindowW(wc.lpszClassName, // classname
                               wc.lpszClassName, // window name
                               0,  // style
                               0,  // x
                               0,  // y
                               0,  // width
                               0,  // height
                               0,  // parent
                               0,  // menu handle
                               hi, // application
                               0); // windows creation data.
        if (!hwnd) {
            return false;
        }
        DEV_BROADCAST_DEVICEINTERFACE_W notificationFilter;
        memset(&notificationFilter, 0, sizeof(DEV_BROADCAST_DEVICEINTERFACE_W));
        notificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE_W);
        notificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
        for (auto it = deviceMap.begin(); it != deviceMap.end(); it++) {
            Device &dev = it->second;
            notificationFilter.dbcc_classguid = dev.uuid;
            HDEVNOTIFY handle = ::RegisterDeviceNotificationW(hwnd,
                                                              &notificationFilter,
                                                              DEVICE_NOTIFY_WINDOW_HANDLE);
            if (handle == nullptr) {
                std::cout<<"failed at register device"<<std::endl;
                continue;
            }
            dev.handle = handle;
        }
        ::SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)this);
        return true;
    }

    void destroyMessageWindow()
    {
        if (!hwnd) {
            return;
        }
        ::DestroyWindow(hwnd);
        hwnd = nullptr;

        for (auto it = deviceMap.begin(); it != deviceMap.end(); it++) {
            ::UnregisterDeviceNotification(it->second.handle);
        }
        ::UnregisterClassW(windowName.c_str(), ::GetModuleHandleW(nullptr));
        return;
    }
public:
    UsbDiscovery():windowName(L"USB_DISCOVERY_WINDOW"){}
    ~UsbDiscovery()
    {
        destroyMessageWindow();
    }
    static WCHAR hex2Int4(WCHAR c)
    {
        WCHAR x = 0;
        if (c >= 'A' && c <= 'Z') {
          x = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'z') {
           x = c - 'a' + 10;
        } else if (c >= '0' && c <= '9') {
           x = c - '0';
        }
        return x;
    }

    static unsigned short hexString2Int16(const std::wstring &hex)
    {
        WCHAR x0 = hex2Int4(hex[3]);
        WCHAR x1 = hex2Int4(hex[2]);
        WCHAR x2 = hex2Int4(hex[1]);
        WCHAR x3 = hex2Int4(hex[0]);
        return (x3<<12) + (x2<<8) + (x1<<4) + x0;
    }

    static std::string guid2String(const GUID &guid)
    {
        char buffer[64] = {0};
        int maxsize = 64;
        //StringFromGUID2(guid, buffer, maxsize);

        sprintf(buffer,
                "%08X-%04X-%04X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X",
                guid.Data1, guid.Data2, guid.Data3,
                guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
                guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
        return std::string(buffer);
    }

    static int parse(const std::wstring &devName, unsigned short &vid, unsigned short &pid)
    {
        int index = devName.find(L"VID_");
        if (index < 0) {
            return -1;
        }
        std::wstring vidstr = devName.substr(index + 4, 4);
        index = devName.find(L"PID_");
        if (index < 0) {
            return -2;
        }
        std::wstring pidstr = devName.substr(index + 4, 4);
        vid = hexString2Int16(vidstr);
        pid = hexString2Int16(pidstr);
        wprintf(L"dev:%s\n", devName.c_str());
        return 0;
    }

    bool registerDevice(const std::vector<Device> &devices)
    {
        for (const Device& dev:devices) {
            std::cout<<"register,size="<<devices.size()<<","<<guid2String(dev.uuid)<<std::endl;
            deviceMap.insert(std::pair<std::string, Device>(guid2String(dev.uuid), dev));
        }
        return createMessageWindow();
    }

};


#endif // DISCOVERY_HPP
