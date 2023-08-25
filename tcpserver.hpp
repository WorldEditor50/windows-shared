#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP
#include <Windows.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

class Socket
{
public:
    SOCKET fd;
public:
    Socket(){}
    inline static SOCKET tcp()
    {
        return ::socket(AF_INET, SOCK_STREAM, 0);
    }

    inline static SOCKET udp()
    {
        return ::socket(AF_INET, SOCK_DGRAM, 0);
    }
    inline static SOCKET raw()
    {
        return ::socket(AF_INET, SOCK_RAW, 0);
    }


    int bindTo()
    {
        return 0;
    }

    int connect()
    {
        return 0;
    }

    int listen()
    {

        return 0;
    }

    int accept()
    {

        return 0;
    }
};

class TcpServer
{
public:

};


#endif // TCPSERVER_HPP
