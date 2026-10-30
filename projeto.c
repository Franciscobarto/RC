#include "estruturas.h"
#include "utils.h"
#include "parser.h"
#include "connections.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     
#include <signal.h>
#include <sys/socket.h>
#include <netdb.h>

sig_atomic_t keep_running = 1;

void sigint_handler(int sig){
    (void) sig;
    keep_running = 0;
}

int get_flags(int argc,char *argv[], int *peerport, char *ip, char *port){
    int opt;

    while ((opt = getopt(argc, argv, "m:n:p:")) != -1) {
        switch (opt) {
            case 'm':
                *peerport = atoi(optarg);
                break;
            case 'n':
                strcpy(ip, optarg);
                break;
            case 'p':
                strcpy(port, optarg);
                if (!verify_port(atoi(port))) {
                    printf("invalid dsport.\n");
                    return 1;
                }
                break;
            default:
                printf("invalid arguments\n");
                return 1;
        }
    }

    if (!verify_port(*peerport)) {
        printf("invalid peerport.\n");
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]){
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    signal(SIGPIPE, SIG_IGN);

    int peerport = 0;
    char ip[MAX_IP] = DSIP; 
    char port[6] = DSPORT;
    
    if (get_flags(argc, argv, &peerport, ip, port) != 0){
        return 1;
    }

    connection conn;
    struct addrinfo *res;

    server_connect_UDP(&conn, &res, ip, port);

    read_command(conn, res, peerport, ip, port);
    
    freeaddrinfo(res);
    close(conn.fd);
    return 0;
}