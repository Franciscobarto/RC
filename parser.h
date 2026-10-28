#ifndef PARSER_H
#define PARSER_H

#include "estruturas.h" 
#include <netdb.h>

void read_command(connection conn, struct addrinfo *res, int peerport, const char *ip, const char *port);

#endif
