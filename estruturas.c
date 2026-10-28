#include "estruturas.h"
#include <netinet/in.h> 

char* get_id(user *x) {
    return x->uid;
}
char* get_password(user *x) {
    return x->password;
}
void set_id(user *x, const char *id) {
    strcpy(x->uid, id);
}
void set_password(user *x, const char *password) {
    strcpy(x->password, password);
}