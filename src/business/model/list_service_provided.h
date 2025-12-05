#ifndef LIST_SERVICE_PROVIDED_H
#define LIST_SERVICE_PROVIDED_H

#include "service_provided.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct List_Service_Provided
{
    Service_Provided *service;
    struct List_Service_Provided *next_service;
} List_Service_Provided;

List_Service_Provided *create_service_list(void);
void save_new_service(List_Service_Provided **head, Service_Provided *service);
char *find_last_hospital_identifier_from_list(List_Service_Provided *list);
bool check_hospital_identifier_in_list(List_Service_Provided *list, const char *identifier);
Service_Provided *find_service_by_UID(List_Service_Provided *head, const char *uhID);
Service_Provided *find_service_by_username(List_Service_Provided *head, const char *username);
void print_service(Service_Provided *service);
void list_all_services_provided(const List_Service_Provided *list);
void list_all_patient_services_provided(const List_Service_Provided *list, const char *username);
void delete_service(List_Service_Provided **head, const char *uh_ID);
void delete_all_services(List_Service_Provided **head);

#endif // LIST_SERVICE_PROVIDED_H