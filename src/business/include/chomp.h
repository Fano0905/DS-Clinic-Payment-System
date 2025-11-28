#if !defined(CHOMP_H)
#define CHOMP_H

#include <stdlib.h>
#include <string.h>

/* Function to remove trailing newline characters from a string */
void chomp(char *s);
int validate_service_provided(List_Service_Provided **service_provided_list, Service_Provided *service_provided);
int validate_department(List_Department **department_list, Department *department);

#endif // CHOMP_H
