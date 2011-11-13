#ifndef _XPSOCKET_H_
#define _XPSOCKET_H_

#ifdef WIN32

#include <windows.h>
#include <winsock2.h>
extern const char *LOCAL_TX_IP;
extern char AltLocalIP[48];
#define EXTRACT_TERMID8(addr) (addr.sin_addr.S_un.S_un_b.s_b4)
#define EXTRACT_GROUPID8(addr) (addr.sin_addr.S_un.S_un_b.s_b3)
#define EXTRACT_TERMID16(addr) ((addr.sin_addr.S_un.S_un_b.s_b3 << 8) | addr.sin_addr.S_un.S_un_b.s_b4)
#define ADDR_AS_NUM S_un.S_addr
typedef int SocketSizeType;

#else // (not) WIN32

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define EXTRACT_TERMID8(addr) ((addr.sin_addr.s_addr) >> 24)
#define EXTRACT_GROUPID8(addr) (((addr.sin_addr.s_addr) >> 16) & 0xFF)
#define EXTRACT_TERMID16(addr) (ntohs((addr.sin_addr.s_addr) >> 16))
#define EXTRACT_TERMID32(addr) (ntohl(addr.sin_addr.s_addr))
#define ADDR_AS_NUM s_addr
#define SOCKET int
extern const char * LOCAL_TX_IP;
typedef socklen_t SocketSizeType;

#endif // WIN32

int xpsocket_init();
void xpsocket_cleanup();
int xpsocket_serve();
int xpsocket_send(char* buffer, int size);

#endif
