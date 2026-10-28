#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <string.h>
#include <netinet/in.h> 
#include <signal.h>     

#define MAX_CMD 11
#define MAX_UID 7
#define MAX_PASSWORD 9
#define MAX_BUFFER 128
#define LIST_BUFFER 2048
#define MAX_IP 30
#define MAX_FILENAME 24
#define MAX_LABEL 20
#define MAX_TIME 16
#define DSIP "193.136.138.142"
#define DSPORT "59000"

extern sig_atomic_t keep_running;

typedef struct {
    int fd;
    struct sockaddr_in addr;
    socklen_t addrlen;
} connection;

typedef struct {
    char uid[MAX_UID];
    char password[MAX_PASSWORD];
} user;

typedef struct {
    char uid[16];
    long fsize;
    char label[MAX_LABEL];
    char pub_time[MAX_TIME];
    char availability[4];
} PeerInfo;


char* get_id(user *x);
char* get_password(user *x);
void set_id(user *x, const char *id);
void set_password(user *x, const char *password);

#endif