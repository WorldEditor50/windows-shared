#include "sharedmemory.h"

SharedMemory::SharedMemory():
    hMapFile(nullptr),
    ptr(nullptr)
{

}

int SharedMemory::mappingTo(const std::wstring &fileName_, std::size_t size_)
{
    if (hMapFile != nullptr || ptr != nullptr) {
        return 1;
    }

    totalsize = size_;
    fileName = fileName_;
    hMapFile = CreateFileMapping(
                  INVALID_HANDLE_VALUE,    // use paging file
                  NULL,                    // default security
                  PAGE_READWRITE,          // read/write access
                  0,                       // maximum object size (high-order DWORD)
                  size_,                   // maximum object size (low-order DWORD)
                  fileName_.c_str());      // name of mapping object

    if (hMapFile == nullptr) {
       return -1;
    }
    ptr = (LPTSTR) MapViewOfFile(hMapFile,   // handle to map object
                         FILE_MAP_ALL_ACCESS, // read/write permission
                         0,
                         0,
                         size_);

    if (ptr == nullptr) {
       CloseHandle(hMapFile);
       return -2;
    }
    return 0;
}

int SharedMemory::attach(const std::wstring &fileName_, std::size_t size_)
{
    if (hMapFile != nullptr || ptr != nullptr) {
        return 1;
    }
    totalsize = size_;
    fileName = fileName_;
    hMapFile = OpenFileMapping(
                    FILE_MAP_ALL_ACCESS,   // read/write access
                    FALSE,                 // do not inherit the name
                    fileName_.c_str());    // name of mapping object

    if (hMapFile == NULL) {
       return -1;
    }

    ptr = (LPTSTR) MapViewOfFile(hMapFile, // handle to map object
                FILE_MAP_ALL_ACCESS,  // read/write permission
                0,
                0,
                size_);

    if (ptr == NULL) {
       CloseHandle(hMapFile);
       return -2;
    }

    return 0;
}

void SharedMemory::release()
{
    if (ptr != nullptr) {
        UnmapViewOfFile(ptr);
    }
    if (hMapFile != nullptr) {
        CloseHandle(hMapFile);
    }
    return;
}
