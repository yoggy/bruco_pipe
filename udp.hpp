#ifndef ____UDP_HPP____
#define ____UDP_HPP____

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <string>

int open_udp(const char *host, int port);

#endif // #define ____UDP_HPP____
