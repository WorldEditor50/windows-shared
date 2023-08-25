#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H
#include <Windows.h>
#include <string>


/*
    https://learn.microsoft.com/zh-cn/windows/win32/memory/creating-named-shared-memory?redirectedfrom=MSDN

*/
class SharedMemory
{
public:
    enum Code {
        CODE_OK = 0,

    };

private:
    std::wstring fileName;
    HANDLE hMapFile;
    LPCTSTR ptr;
    std::size_t totalsize;
public:
    SharedMemory();
    int mappingTo(const std::wstring &fileName_, std::size_t size_);
    int attach(const std::wstring &fileName_, std::size_t size_);
    void release();
    template<typename T>
    inline T* get()
    {
        return static_cast<T*>(ptr);
    }
    inline std::size_t size() {return totalsize;}

};

#endif // SHAREDMEMORY_H
