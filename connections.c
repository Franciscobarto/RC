#include "connections.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int server_connect_UDP(connection *conn, struct addrinfo **res, const char *ip, const char *port) {
    struct addrinfo hints;
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0; 

    conn->fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (conn->fd ==-1) exit(1);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(ip, port, &hints, res) != 0) exit(1);

    if (setsockopt(conn->fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("setsockopt failed");
    }

    return 0;
}

int server_connect_TCP(const char *ip, const char *port){
    int fd, errcode;
    struct addrinfo hints, *res;
    ssize_t n;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) exit(1);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    errcode = getaddrinfo(ip, port, &hints, &res);
    if(errcode != 0) exit(1);

    n = connect(fd, res->ai_addr, res->ai_addrlen);
    if(n == -1) {
        freeaddrinfo(res);
        exit(1);
    }
    freeaddrinfo(res);
    return fd;
}