#ifndef NETWORK_H
#define NETWORK_H



#ifdef WIN32
#include <winsock2.h>
#include <stdio.h>

#elif __linux__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else

#error not available on your platform

#endif

static void initNetwork(void)
{
#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (err < 0)
    {
        printf("WSAStartup failed !\n");
        exit(EXIT_FAILURE);
    }
#endif
}

static void cleanNetwork(void)
{
#ifdef WIN32
    WSACleanup();
#endif
}
#endif
