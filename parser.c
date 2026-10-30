#include "parser.h"
#include "commands.h"   
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void read_command(connection conn, struct addrinfo *res, int peerport, const char *ip, const char *port){
    char command[MAX_CMD];
    user logged_user;
    char extra[2];
    char buffer[MAX_BUFFER];
    char arg1[MAX_BUFFER];
    char arg2[MAX_BUFFER];
    int args;
    int is_logged_in = 0;

    while(keep_running){

        printf("command: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            if(!keep_running) break;
            break;
        }

        args = sscanf(buffer, "%s %s %s %2s", command, arg1, arg2, extra);

        if (strcmp(command, "login") == 0 && args == 3) {
            handle_login(conn, res, peerport, arg1, arg2, &logged_user, &is_logged_in);

        } else if (strcmp(command, "exit") == 0 && args == 1) {
            if(is_logged_in){
                printf("need to logout first\n");
                continue;
            } break;

        } else if (strcmp(command, "logout") == 0 && args == 1) {
            handle_logout(conn, res, logged_user, &is_logged_in);

        } else if (strcmp(command, "unregister") == 0 && args == 1) {
            handle_unregister(conn, res, logged_user, &is_logged_in);

        } else if (strcmp(command, "publish") == 0 && args == 3) {
            publish_resource(conn, res, logged_user, &is_logged_in, arg1, arg2);

        } else if (strcmp(command, "remove") == 0 && args == 2) {
            remove_resource(conn, res, logged_user, &is_logged_in, arg1);

        } else if (strcmp(command, "list") == 0 && args == 1) {
            list_files(conn, res);

        } else if (strcmp(command, "versions") == 0 && args == 2) {
            list_versions(ip, port, arg1);

        } else {
            printf("Invalid command or arguments\n");
        }
        printf("\n");
    }
}
