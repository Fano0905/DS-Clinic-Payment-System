#ifndef LIST_SERVICE_PROVIDED_H
#define LIST_SERVICE_PROVIDED_H

#include "service_provided.h"

typedef struct List_Service_Provided
{
    Service_Provided *service;
    struct List_Service_Provided *next_service;
} List_Service_Provided;

void add_service_to_list(List_Service_Provided **head, Service_Provided *service);
List_Service_Provided *create_service_list(void);
Service_Provided *get_service_by_uID(List_Service_Provided *head, const char *uhID);
void list_all_services(const List_Service_Provided *list);
void delete_service(List_Service_Provided **head, const char *uh_ID);
void clear_service_list(List_Service_Provided **head);
void add_service_info(List_Service_Provided **list);
void list_patient_services(const List_Service_Provided *list, const char *username);


#endif // LIST_SERVICE_PROVIDED_H