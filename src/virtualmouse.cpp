#include "virtualmouse.h"
#include <iostream>
#pragma comment(lib, "User32.lib")
void VirtualMouse::sendKey(int key)
{
    INPUT input;
    memset(&input, 0, sizeof (INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = key;
    SendInput(1, &input, sizeof(INPUT));
    return;
}

void VirtualMouse::leftDown(long gx, long gy, double x, double y)
{
    moveTo(gx, gy, x, y);
    sendKey(MOUSEEVENTF_LEFTDOWN);
    return;
}

void VirtualMouse::leftUp(long gx, long gy, double x, double y)
{
    moveTo(gx, gy, x, y);
    sendKey(MOUSEEVENTF_LEFTUP);
    return;
}

void VirtualMouse::rightDown(long gx, long gy, double x, double y)
{
    moveTo(gx, gy, x, y);
    sendKey(MOUSEEVENTF_RIGHTDOWN);
    return;
}

void VirtualMouse::rightUp(long gx, long gy, double x, double y)
{
    moveTo(gx, gy, x, y);
    sendKey(MOUSEEVENTF_RIGHTUP);
    return;
}

void VirtualMouse::leftClick(long gx, long gy, double x, double y)
{
    moveTo(gx, gy, x, y);
    INPUT input[2] = {};
    input[0].type = INPUT_MOUSE;
    input[1].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(ARRAYSIZE(input), input, sizeof(INPUT));
    return;
}

void VirtualMouse::doubleLeftClick(long gx, long gy, double x, double y)
{
    moveTo(gx, gy, x, y);
    INPUT input[2] = {};
    input[0].type = INPUT_MOUSE;
    input[1].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    for (int i = 0; i < 2; i++) {
        SendInput(ARRAYSIZE(input), input, sizeof(INPUT));
    }
    return;
}

void VirtualMouse::rightClick(long gx, long gy, double x, double y)
{
    moveTo(gx, gy, x, y);
    INPUT input[2] = {};
    input[0].type = INPUT_MOUSE;
    input[1].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    input[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    SendInput(ARRAYSIZE(input), input, sizeof(INPUT));
    return;
}

void VirtualMouse::doubleRightClick(long gx, long gy, double x, double y)
{
    moveTo(gx, gy, x, y);
    INPUT input[2] = {};
    input[0].type = INPUT_MOUSE;
    input[1].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    input[1].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    for (int i = 0; i < 2; i++) {
        SendInput(ARRAYSIZE(input), input, sizeof(INPUT));
    }
    return;
}

void VirtualMouse::moveTo(long gx, long gy, double x, double y)
{
    HMONITOR hMonitor = MonitorFromPoint({gx, gy}, MONITOR_DEFAULTTONEAREST);
    MONITORINFO monitorInfo = { sizeof(MONITORINFO) };
    GetMonitorInfo(hMonitor, &monitorInfo);
    LONG screenWidth = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
    LONG screenHeight = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    double targetX = monitorInfo.rcMonitor.left + x;
    double targetY = monitorInfo.rcMonitor.top + y;
    input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    input.mi.dx = targetX/screenWidth*65535;
    input.mi.dy = targetY/screenHeight*65535;
    SendInput(1, &input, sizeof(INPUT));
    return;
}

