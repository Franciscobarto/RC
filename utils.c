#include "utils.h"

int verify_port(int port) {
    if (port < 1 || port > 65535) {
        printf("invalid port\n");
        return 0;
    }
    return 1;
}

int verify_uid(const char *UID) {
    if (strlen(UID) != 6) {
        printf("invalid uid\n");
        return 0;
    }
    for (int i = 0; i < 6; i++) {
        if (!isdigit(UID[i])) {
            printf("invalid uid\n");
            return 0;
        }
    }
    return 1;
}

int verify_password(const char *password) {
    if (strlen(password) != 8) {
        printf("invalid password\n");
        return 0;
    }
    for (int i = 0; i < 8; i++) {
        if (!isalnum(password[i])) {
            printf("invalid password\n");
            return 0;
        }
    }
    return 1;
}

int verify_filename(const char *filename){
    if(strlen(filename) >= 24){
        printf("invalid filename\n");
        return 0;
    }

    const char *dot = strrchr(filename, '.');
    if(dot == NULL || dot == filename){
        printf("invalid filename\n");
            return 0;
    }

    for(const char *p = filename; p < dot; p++){
        if(!isalnum(*p) && *p != '-' && *p != '_'){
            printf("invalid filename\n");
            return 0;
        }
    }

    if(strlen(dot + 1) != 3){
        printf("invalid filename\n");
        return 0;
    }

    for(int i = 1; i <= 3; i++){
        if(!isalnum(dot[i])){
            printf("invalid filename\n");
            return 0;
        }
    }
    return 1;
}

int verify_label(const char *label){
    if(strlen(label) > 20 || strlen(label) < 1){
        printf("invalid label\n");
        return 0;
    }

    for(const char *l = label; *l != '\0'; l++){
        if(!isalnum(*l) && *l != '-' && *l != '_'){
            printf("invalid label\n");
            return 0;
        }
    }
    return 1;
}