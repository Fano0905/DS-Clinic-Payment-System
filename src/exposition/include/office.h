#if !defined(OFFICE_H)
#define OFFICE_H

#include "credential.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void chomp(char *s);
int login(Credentials credentials);
void office(char *buffer);


#endif // OFFICE_H
