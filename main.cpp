#include <iostream>
#include "winnative.h"

int main()
{
#if 0
    auto proc = WindowsNative::Process::getAllProcess();
    for (auto &x : proc) {
        std::cout<<x.name<<std::endl;
    }

    std::cout<<WindowsNative::Window::getForegroundApp()<<std::endl;
#endif
    WindowsNative::exec("ipconfig", true);
    system("Pause");
    return 0;
}
