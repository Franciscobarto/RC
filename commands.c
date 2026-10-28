#include "commands.h"
#include "utils.h"      
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/stat.h>   

int handle_login(connection conn, struct addrinfo *res, user temp_user, int peerport, int *is_logged_in) {
    char buffer[MAX_BUFFER];
    ssize_t n;

    if (!verify_uid(get_id(&temp_user)) || !verify_password(get_password(&temp_user))) return 1;

    snprintf(buffer, sizeof(buffer), "LIN %s %s %d\n", get_id(&temp_user), get_password(&temp_user), peerport);
    if (sendto(conn.fd, buffer, strlen(buffer), 0, res->ai_addr, res->ai_addrlen) == -1) exit(1);

    conn.addrlen = sizeof(conn.addr);
    n = recvfrom(conn.fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&conn.addr, &conn.addrlen);
    if (n == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("didn't receive a server response. try again\n");
            return 1;
        } else {
            exit(1);
        }
    }

    buffer[n] = '\0';

    char response[4];

    if (sscanf(buffer, "%*s %s", response) == 1){
        if (strcmp(response, "OK") == 0) {
            *is_logged_in = 1;
            printf("successful login\n");
        } else if (strcmp(response, "NOK") == 0) {
            printf("incorrect login attempt\n");
        } else if (strcmp(response, "REG") == 0) {
            *is_logged_in = 1;
            printf("new user registered\n");
        }
    } else {
        printf("unexpected server response\n");
    }

    return 0;
}

int handle_logout(connection conn, struct addrinfo *res, user logged_user, int *is_logged_in) {
    char buffer[MAX_BUFFER];
    ssize_t n;

    snprintf(buffer, sizeof(buffer), "LOU %s %s\n", get_id(&logged_user), get_password(&logged_user));
    if(sendto(conn.fd, buffer, strlen(buffer), 0, res->ai_addr, res->ai_addrlen) == -1) exit(1);

    conn.addrlen = sizeof(conn.addr);
    n = recvfrom(conn.fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&conn.addr, &conn.addrlen);
    if (n == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("timeout waiting for server response\n");
            return 1;
        } else {
            exit(1);
        }
    }
    buffer[n] = '\0';

    char response[4];

    if(sscanf(buffer, "RLO %s", response) == 1){
        if(strcmp(response, "OK") == 0){
            *is_logged_in = 0;
            printf("successful logout\n");
        } else if(strcmp(response, "NLG") == 0){
            printf("user not logged in\n");
        } else if(strcmp(response, "UNR") == 0){
            printf("unknown user\n");
        } else if(strcmp(response, "WRP") == 0){
            printf("unsuccessful logout\n");
        }
    } else {
        printf("unexpected server response\n");
    }
    return 0; 
}

int handle_unregister(connection conn, struct addrinfo *res, user logged_user, int *is_logged_in) {
    char buffer[MAX_BUFFER];
    ssize_t n;

    snprintf(buffer, sizeof(buffer), "UNR %s %s\n", get_id(&logged_user), get_password(&logged_user));
    if(sendto(conn.fd, buffer, strlen(buffer), 0, res->ai_addr, res->ai_addrlen) == -1) exit(1);

    conn.addrlen = sizeof(conn.addr);
    n = recvfrom(conn.fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&conn.addr, &conn.addrlen);
    if(n == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("timeout waiting for server response\n");
            return 1;
        } else {
            exit(1);
        }
    }
    buffer[n] = '\0';

    char response[4];

    if(sscanf(buffer, "RUR %s", response) == 1){
        if(strcmp(response, "OK") == 0){
            *is_logged_in = 0;
            printf("successful unregister\n");
        } else if(strcmp(response, "NOK") == 0){
            printf("incorrect unregister attempt\n");
        } else if(strcmp(response, "UNR") == 0){
            printf("unknown user\n");
        } else if(strcmp(response, "WRP") == 0){
            printf("unsuccessful unregister\n");
        }
    } else {
        printf("unexpected server response\n");
    }
    return 0; 
}

int publish_resource(connection conn, struct addrinfo *res, user logged_user, const char *filename, const char *label){
    char buffer[MAX_BUFFER];
    ssize_t n;
    struct stat file_stat;

    if (!verify_filename(filename) || !verify_label(label)) return 1;

    if(stat(filename, &file_stat) == -1){
        printf("file does not exist in the local directory\n");
        return 0;
    }

    long f_size = file_stat.st_size;
    snprintf(buffer, sizeof(buffer), "PUB %s %s %s %ld %s\n", get_id(&logged_user), get_password(&logged_user), filename, f_size, label);
    if (sendto(conn.fd, buffer, strlen(buffer), 0, res->ai_addr, res->ai_addrlen) == -1) exit(1);

    conn.addrlen = sizeof(conn.addr);
    n = recvfrom(conn.fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&conn.addr, &conn.addrlen);
    if(n == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("timeout waiting for server response\n");
            return 1;
        } else {
            exit(1);
        }
    }
    buffer[n] = '\0';

    char response[4];

    if(sscanf(buffer, "RPB %s", response) == 1){
        if(strcmp(response, "OK") == 0){
            printf("successful publication\n");
        } else if(strcmp(response, "NLG") == 0){
            printf("user not logged in\n");
        } else if(strcmp(response, "UNR") == 0){
            printf("unknown user\n");
        } else if(strcmp(response, "WRP") == 0){
            printf("wrong password\n");
        } else if(strcmp(response, "NOK") == 0){               //basta colocar wrong password? nao sei
            printf("unsuccessful publish\n");
        }
    } else {
        printf("unexpected server response\n");
    }
    return 0; 
}

int remove_resource(connection conn, struct addrinfo *res, user logged_user, const char *filename){
    char buffer[MAX_BUFFER];
    ssize_t n;

    if (!verify_filename(filename)) return 1;

    snprintf(buffer, sizeof(buffer), "REM %s %s %s\n", get_id(&logged_user), get_password(&logged_user), filename);
    if (sendto(conn.fd, buffer, strlen(buffer), 0, res->ai_addr, res->ai_addrlen) == -1) exit(1);

    conn.addrlen = sizeof(conn.addr);
    n = recvfrom(conn.fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&conn.addr, &conn.addrlen);
    if(n == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("timeout waiting for server response\n");
            return 1;
        } else {
            exit(1);
        }
    }
    buffer[n] = '\0';

    char response[4];

    if(sscanf(buffer, "RRM %s", response) == 1){
        if(strcmp(response, "OK") == 0){
            printf("successful removal\n");
        } else if(strcmp(response, "NLG") == 0){
            printf("user not logged in\n");
        } else if(strcmp(response, "UNR") == 0){
            printf("unknown user\n");
        } else if(strcmp(response, "WRP") == 0){
            printf("wrong password\n");
        } else if(strcmp(response, "NOK") == 0){                                
            printf("resource not found\n");                      //no enunciado diz que essa é a mensagem mas devia ser tipo "nao foi publicado por este user" de acordo com outra parte do enunciado
        }
    } else {
        printf("unexpected server response\n");
    }
    return 0; 
}

int list_files(connection conn, struct addrinfo *res){
    char buffer[LIST_BUFFER];
    ssize_t n;

    snprintf(buffer, sizeof(buffer), "LST\n");

    if (sendto(conn.fd, buffer, strlen(buffer), 0, res->ai_addr, res->ai_addrlen) == -1) exit(1);

    conn.addrlen = sizeof(conn.addr);
    n = recvfrom(conn.fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&conn.addr, &conn.addrlen);
    if(n == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("timeout waiting for server response\n");
            return 1;
        } else {
            exit(1);
        }
    }
    buffer[n] = '\0';

    char response[4];

    if(sscanf(buffer, "RLS %s", response) == 1){
        if (strcmp(response, "NOK") == 0) {
            printf("no published resources\n");
        }
        else if (strcmp(response, "OK") == 0) {
            char *ptr = buffer + 6;
            char filename[MAX_FILENAME];
            int count = 0;
            printf("List of published resources:\n");
        
            int bytes_read;
            while (sscanf(ptr, "%s %n", filename, &bytes_read) == 1) {
                count++;
                printf("%d %s\n", count, filename);
                ptr += bytes_read;
            }

            printf("-----------------------------\n");
        }
    } else {
        printf("unexpected server response\n");
    }
    return 0;
}


int handle_versions(int tcp_fd, const char *filename) {
    char message[MAX_BUFFER];
    char buffer[MAX_BUFFER];
    char c;
    int n;

    if(!verify_filename(filename)){
        close(tcp_fd);
        return 1;
    }

    snprintf(message, sizeof(message), "VRS %s\n", filename);
    write(tcp_fd, message, strlen(message));

    n = read(tcp_fd, buffer, 7);
    if (n == -1) return 1; //maybe por um signal idk
    buffer[n] = '\0';

    char status[4];

    if(sscanf(buffer, "RVR %s", status) != 1){
        printf("unexpected server response\n");
        return 0;
    }

    if (strcmp(status, "OK") == 0) {
        printf("List of peers available:\n");
        PeerInfo peer;
        int count = 0;
        int buf_idx = 0; // reset buffer for peer data parsing

        while (1) {
            n = read(tcp_fd, &c, 1);
            if (n == -1) return 1;
            if (n == 0 || c == '\n') {
                break;
            }

            if (buf_idx < MAX_BUFFER - 1) {
                buffer[buf_idx++] = c;
                buffer[buf_idx] = '\0';
            }

            int chars_read = 0;
            if (sscanf(buffer, "%s %ld %s %s %s%n", 
                       peer.uid, &peer.fsize, peer.label, 
                       peer.pub_time, peer.availability, &chars_read) == 5) {
                
                if (strcmp(peer.availability, "AVL") == 0 || strcmp(peer.availability, "NAV") == 0) {
                    count++;
                    printf("%d.-----------------------------\n", count);
                    printf("Peer UID: %s\n", peer.uid);
                    printf("File Size: %ld bytes\n", peer.fsize);
                    printf("Label: %s\n", peer.label);
                    
                    int year, month, day, hour, min, sec;
                    sscanf(peer.pub_time, "%4d%2d%2d-%2d%2d%2d", &year, &month, &day, &hour, &min, &sec);
                    printf("Publication Time: %04d-%02d-%02d %02d:%02d:%02d\n", year, month, day, hour, min, sec);
                    
                    if (strcmp(peer.availability, "AVL") == 0) {
                        printf("Availability: Available\n");
                    } else {
                        printf("Availability: Not Available\n");
                    }
                    int remaining = buf_idx - chars_read;
                    if (remaining > 0) {
                        memmove(buffer, buffer + chars_read, remaining);
                        buf_idx = remaining;
                    } else {
                        buf_idx = 0;
                    }
                    buffer[buf_idx] = '\0';
                }
            }
        }
        printf("-----------------------------\n");
    } else if (strcmp(status, "NOK") == 0){
        printf("no peer available for such resource\n"); 
    } else if(strcmp(status, "ERR") == 0){
        printf("error");
    }
    return 0;

}