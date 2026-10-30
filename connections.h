#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include <sys/socket.h>
#include <netdb.h>
#include "estruturas.h"

int server_connect_TCP(const char *ip, const char *port);
int server_connect_UDP(connection *conn, struct addrinfo **res, const char *ip, const char *port);

#endif 