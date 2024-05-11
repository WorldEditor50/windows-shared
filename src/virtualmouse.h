#ifndef VIRTUALMOUSE_H
#define VIRTUALMOUSE_H
#include <Windows.h>

class VirtualMouse
{
public:
    static void sendKey(int key);
    static void leftDown(long gx, long gy, double x, double y);
    static void leftUp(long gx, long gy, double x, double y);
    static void rightDown(long gx, long gy, double x, double y);
    static void rightUp(long gx, long gy, double x, double y);
    static void leftClick(long gx, long gy, double x, double y);
    static void doubleLeftClick(long gx, long gy, double x, double y);
    static void rightClick(long gx, long gy, double x, double y);
    static void doubleRightClick(long gx, long gy, double x, double y);
    static void moveTo(long gx, long gy, double x, double y);
};

#endif // VIRTUALMOUSE_H
