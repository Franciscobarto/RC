#ifndef COMMANDS_H
#define COMMANDS_H

#include "estruturas.h"
#include <netdb.h> 

int handle_login(connection conn, struct addrinfo *res, int peerport, const char *id, const char *password, user *logged_user, int *is_logged_in);
int handle_logout(connection conn, struct addrinfo *res, user logged_user, int *is_logged_in);
int handle_unregister(connection conn, struct addrinfo *res, user logged_user, int *is_logged_in);
int publish_resource(connection conn, struct addrinfo *res, user logged_user, int *is_logged_in, const char *filename, const char *label);
int remove_resource(connection conn, struct addrinfo *res, user logged_user, int *is_logged_in, const char *filename);
int list_files(connection conn, struct addrinfo *res);
int list_versions(const char *ip, const char *port, const char *filename);
int read_list_versions(int tcp_fd);


#endif