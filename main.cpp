#include <iostream>
#include <chrono>
#include "commandline.h"
#include "crashdump.hpp"
#include "clock.hpp"
#include "ctimer.hpp"
#include "usbdiscovery.hpp"

void test_crashdump()
{
    CrashDump crashdump;
    struct Test {
        int *p;
    };
    Test t;
    delete t.p;
    return;
}

void test_clock()
{
    LARGE_INTEGER t1 = Clock::now();
    Sleep(1000);
    LARGE_INTEGER t2 = Clock::now();

    printf("Time in s:%f\n", Clock::duration(t2, t1));
    std::cout<<"timestamp:"<<Clock::getSecondTimestamp()<<std::endl;
    return;
}

int onTimeout()
{
    std::cout<<"time out"<<std::endl;
    return 0;
}
void test_ctimer()
{
    CTimer timer;
    timer.startAfter(5, onTimeout);
    Sleep(6000);
    return;
}

void test_hexstring2int16()
{
    std::wstring hex2 = L"2F8A";
    unsigned short x2 = UsbDiscovery::hexString2Int16(hex2);
    wprintf(L"x = %X\n", x2);

    std::wstring name = L"VID_2F8A&PID_0012";
    unsigned short vid = 0;
    unsigned short pid = 0;
    UsbDiscovery::parse(name, vid, pid);
    wprintf(L"vid = %X, pid=%X\n", vid, pid);
    return;
}

void attachedNotify(unsigned short vid, unsigned short pid)
{
    std::cout<<"arrived device: vid="<<vid<<",pid="<<pid<<std::endl;
    return;
}

void detachedNotify(unsigned short vid, unsigned short pid)
{
    std::cout<<"left device: vid="<<vid<<",pid="<<pid<<std::endl;
    return;
}
void test_usb_discovery()
{
    UsbDiscovery discovery;
    bool ret = discovery.registerDevice(std::vector<UsbDiscovery::Device>{UsbDiscovery::Device(
                                                               0x1BCF,
                                                               0x0007,
                                                               GUID({0x65E8773D, 0x8F56, 0x11D0, {0xA3, 0xB9, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96}}),
                                                               &attachedNotify,
                                                               &detachedNotify
                                                               )});

    if (ret == true) {
        //getchar();
    }
    while (1) {
        Sleep(3000);
    }
    return;
}

int main()
{
    test_usb_discovery();
    return 0;
}
