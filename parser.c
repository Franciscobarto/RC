#include "parser.h"
#include "commands.h"   
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int server_connect_TCP(const char *ip, const char *port);

void read_command(connection conn, struct addrinfo *res, int peerport, const char *ip, const char *port){
    char command[MAX_CMD];
    user temp_user;
    user logged_user;
    char extra[2];
    char buffer[MAX_BUFFER];
    int args;
    int is_logged_in = 0;
    char filename[MAX_FILENAME];
    char label[MAX_LABEL];
    int tcp_fd;

    while(keep_running){
        printf("command: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            if(!keep_running) break;
            break;
        }

        //args = sscanf(buffer, "%s %s %s %s", command, get_id(&temp_user), get_password(&temp_user), extra);
        args = sscanf(buffer, "%s", command);
        if (args == 1) {
            if (strcmp(command, "login") == 0 && sscanf(buffer, "%s %s %s %s", command, get_id(&temp_user), get_password(&temp_user), extra) == 3){
                if(is_logged_in) {
                    printf("already logged in\n");
                    continue;
                } handle_login(conn, res, temp_user, peerport, &is_logged_in);
                if(is_logged_in){
                    set_id(&logged_user, get_id(&temp_user));
                    set_password(&logged_user, get_password(&temp_user));
                }
            } else if (strcmp(command, "exit") == 0){
                if(is_logged_in){
                    printf("need to logout first\n");
                    continue;
                } break;
            } else if (strcmp(command, "logout") == 0){
                if (!is_logged_in) {
                    printf("user not logged in\n");
                    continue;
                } handle_logout(conn, res, logged_user, &is_logged_in);
            } else if (strcmp(command, "unregister") == 0){
                if (!is_logged_in) {
                    printf("user not logged in/registered\n");
                    continue;
                } handle_unregister(conn, res, logged_user, &is_logged_in);
            } else if (strcmp(command, "publish") == 0 && sscanf(buffer, "%s %s %s %s", command, filename, label, extra) == 3){
                if (!is_logged_in) {
                    printf("user not logged in\n");
                    continue;
                } publish_resource(conn, res, logged_user, filename, label);
            } else if (strcmp(command, "remove") == 0 && sscanf(buffer, "%s %s %s", command, filename, extra) == 2){
                if (!is_logged_in) {
                    printf("user not logged in\n");
                    continue;
                } remove_resource(conn, res, logged_user, filename);
            } else if (strcmp(command, "list") == 0) {
                list_files(conn, res);
            } else if (strcmp(command, "versions") == 0 && sscanf(buffer, "%s %s %s", command, filename, extra) == 2) {
                tcp_fd = server_connect_TCP(ip, port);
                handle_versions(tcp_fd, filename);
            } else {
                printf("Invalid command or arguments\n");
            }
        }
    }
}