#ifndef UTILS_H
#define UTILS_H

#include "estruturas.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

int verify_port(int port);
int verify_uid(const char *UID);
int verify_password(const char *password) ;
int verify_filename(const char *filename);
int verify_label(const char *label);

#endif