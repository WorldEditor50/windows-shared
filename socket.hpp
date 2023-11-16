#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <Windows.h>
#include <WinSock2.h>
#include <string>
#include <memory>

#pragma comment(lib, "ws2_32.lib")

class Socket
{
public:
    enum Proto {
        PROTO_TCP = 0,
        PROTO_UDP,
        PROTO_RAW
    };
    class WSA
    {
    public:
        WSADATA data;
    public:
        WSA()
        {
             int ret = WSAStartup(MAKEWORD(2, 2), &data);
             if (ret != 0) {
                 printf("WSAStartup failed: %d\n", ret);
             }
        }
        ~WSA()
        {
            WSACleanup();
        }
    };
public:
    SOCKET fd;
    static WSA wsa;
public:

    Socket():fd(INVALID_SOCKET){}

    ~Socket()
    {
        close();
    }

    inline static SOCKET tcp()
    {
        return ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }

    inline static SOCKET udp()
    {
        return ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }
    inline static SOCKET raw()
    {
        return ::socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    }


    int bind(int port)
    {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.S_un.S_addr = INADDR_ANY;
        if(::bind(fd, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR) {
            printf("bind error !");
            return -1;
        }
        return 0;
    }

    int connect(const std::string &saddr, int port)
    {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.S_un.S_addr = inet_addr(saddr.c_str());
        if (::connect(fd, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
            printf("connect error !");
            return -1;
        }
        return 0;
    }

    int listen(int connectNum)
    {
        int ret = ::listen(fd, connectNum);
        if (ret == SOCKET_ERROR) {
            printf("Listen failed with error: %ld\n", WSAGetLastError());
            return -1;
        }
        return 0;
    }

    Socket accept()
    {

        Socket socket;
        socket.fd = ::accept(fd, NULL, NULL);
        if (socket.fd == INVALID_SOCKET) {
            printf("accept failed: %d\n", WSAGetLastError());
        }
        return socket;
    }

    int shutdown()
    {
        int ret = ::shutdown(fd, SD_SEND);
        if (ret == SOCKET_ERROR) {
            printf("shutdown failed: %d\n", WSAGetLastError());
            return -1;
        }
        return 0;
    }

    void close()
    {
        if (fd != INVALID_SOCKET) {
            closesocket(fd);
        }
        return;
    }


    int send(const char* data, int datasize)
    {
        if (datasize < 1500) {
            return ::send(fd, data, datasize, 0);
        }
        int pos = 0;
        while (pos < datasize) {
            int len = ::send(fd, data + pos, 1500, 0);
            if (len < 0) {
                return -1;
            }
            pos += len;
        }
        return pos;
    }

    int recv(char* &data, int datasize)
    {
        return ::recv(fd, data, datasize, 0);
    }

    int send(const std::string &data)
    {
        return Socket::send(data.c_str(), data.size());
    }

    int sendto(const std::string &addr, const char* data, int datasize)
    {

        return 0;
    }

    int recvfrom(const std::string &addr, char* data, int datasize)
    {

        return 0;
    }
};
#endif // SOCKET_HPP
