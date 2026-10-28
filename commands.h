#ifndef COMMANDS_H
#define COMMANDS_H

#include "estruturas.h"
#include <netdb.h> 

int handle_login(connection conn, struct addrinfo *res, user temp_user, int peerport, int *is_logged_in);
int handle_logout(connection conn, struct addrinfo *res, user logged_user, int *is_logged_in);
int handle_unregister(connection conn, struct addrinfo *res, user logged_user, int *is_logged_in);
int publish_resource(connection conn, struct addrinfo *res, user logged_user, const char *filename, const char *label);
int remove_resource(connection conn, struct addrinfo *res, user logged_user, const char *filename);
int list_files(connection conn, struct addrinfo *res);
int handle_versions(int tcp_fd, const char *filename);


#endif