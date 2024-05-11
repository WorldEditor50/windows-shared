#ifndef PIPE_H
#define PIPE_H
#include <Windows.h>
#include <string>
#include <cstring>

class Pipe
{
public:
    enum Type {
       READ = 0,
       WRITE = 1
    };
protected:
    HANDLE handle[2];
public:
    Pipe()
    {
        SECURITY_ATTRIBUTES saAttr;
        // Set the bInheritHandle flag so pipe handles are inherited.
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        saAttr.bInheritHandle = TRUE;
        saAttr.lpSecurityDescriptor = NULL;
        // Create a pipe for the child process's STDOUT.
        if (!CreatePipe(&handle[Pipe::READ], &handle[Pipe::WRITE], &saAttr, 0)) {

        }
        // Ensure the read handle to the pipe for STDOUT is not inherited.
        if (!SetHandleInformation(handle[Pipe::READ], HANDLE_FLAG_INHERIT, 0)) {

        }

    }

    DWORD write(const std::string &data)
    {
        DWORD dwWrote = 0;
        WriteFile(handle[Pipe::WRITE], data.c_str(), data.size(), &dwWrote, nullptr);
        return dwWrote;
    }

    std::string read(std::size_t totalsize)
    {
        char* ptr = new char[totalsize];
        memset(ptr, 0, totalsize);
        DWORD dwReadSize = 0;
        ReadFile(handle[Pipe::READ], ptr, totalsize, &dwReadSize, nullptr);
        return std::string(ptr, dwReadSize);
    }

};

#endif // PIPE_H
