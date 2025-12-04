#include "list_service_provided.h"

List_Service_Provided *create_service_list(void) {
    return NULL;
}

void save_new_service(List_Service_Provided **list, Service_Provided *service)
{
    if (list == NULL || service == NULL) {
        fprintf(stderr, "save_service: NULL arguments.\n");
        free(service->uh_ID);
        free(service->patient_username);
        free(service->department);
        free(service->payment_status);
        free(service);
        return;
    }

    // ------------------------------
    // 1. Check if UH_ID already exists in the linked list
    // ------------------------------
    if (check_hospital_identifier_in_list(*list, service->uh_ID)) {
        fprintf(stderr,
                "save_service: UH ID '%s' already exists. Cannot save.\n",
                service->uh_ID);
        return;
    }

    // ------------------------------
    // 2. Allocate a new list node
    // ------------------------------
    List_Service_Provided *node =
        malloc(sizeof(List_Service_Provided));

    if (!node) {
        perror("save_service: malloc failed");
        return;
    }

    node->service = service;
    node->next_service = NULL;

    // ------------------------------
    // 3. Insert node at the end of the list
    // ------------------------------
    if (*list == NULL) {
        *list = node;
    } else {
        List_Service_Provided *curr = *list;
        while (curr->next_service != NULL)
            curr = curr->next_service;

        curr->next_service = node;
    }

    printf("Service saved successfully into linked list.\n");
}

Service_Provided *find_service_by_UID(List_Service_Provided *head, const char *uhID) {

    List_Service_Provided *current = head;

    while (current != NULL) {
        if (strcmp(current->service->uh_ID, uhID) == 0) {
            return current->service;
        }
        current = current->next_service;
    }

    perror("Service not found\n");
    return NULL;
}

Service_Provided *find_service_by_username(List_Service_Provided *head, const char *username){

    List_Service_Provided *current = head;

    while (current != NULL) {
        printf(current->service->uh_ID);
        if (strcmp(current->service->patient_username, username) == 0 && strcmp(current->service->payment_status, "PENDING") == 0) {
            return current->service;
        }
        current = current->next_service;
    }

    printf("Services for User [%s] not found\n", username);
    return NULL;
}

char *find_last_hospital_identifier_from_list(List_Service_Provided *list)
{
    if (list == NULL) {
        // No services → no previous ID
        return NULL;
    }

    // Traverse to the last node
    List_Service_Provided *current = list;
    while (current->next_service != NULL) {
        current = current->next_service;
    }

    // Now: current = last node
    if (current->service == NULL || current->service->uh_ID == NULL) {
        return NULL; // invalid data
    }

    const char *uh = current->service->uh_ID;

    // Extract first 4 chars of UH-ID
    size_t len = strlen(uh);

    char *last_id = NULL;

    if (len >= 4) {
        last_id = malloc(5);
        if (!last_id) return NULL;
        strncpy(last_id, uh, 4);
        last_id[4] = '\0';
    } 
    else {
        // fallback: copy whatever is available
        last_id = strdup(uh);
    }

    return last_id; // caller must free()
}

bool check_hospital_identifier_in_list(List_Service_Provided *list, const char *identifier)
{
    if (identifier == NULL) return false;
    if (strlen(identifier) < 4) return false;   // invalid ID

    char id_prefix[5];
    strncpy(id_prefix, identifier, 4);
    id_prefix[4] = '\0';

    List_Service_Provided *current = list;

    while (current != NULL) {

        if (current->service != NULL && current->service->uh_ID != NULL) {

            // Compare only the first 4 characters
            if (strncmp(current->service->uh_ID, id_prefix, 4) == 0) {
                return true;   // Found an existing ID (already used)
            }
        }

        current = current->next_service;
    }

    return false; // Not found
}

void list_all_services_provided(const List_Service_Provided *list) {

    if (list == NULL) {
        printf("List is empty.\n");
        return;
    }

    const List_Service_Provided *current = list;

    while (current != NULL) {

        printf("REFERENCE : [%s]\n \tFor : %s\n", current->service->uh_ID, current->service->patient_username);
        printf("------ DETAILS -----\n");
        printf("Hospitalization duration : %d days at %s.\n ", current->service->number_days, current->service->department);
        printf("----- EXTRA SERVICE TAKEN -----\n");
        printf("Transportation : %s\n", current->service->hospital_transportation ? "YES" : "NO");
        printf("Exclusive nurse care : %s\n", current->service->exclusive_nurse_care ? "YES" : "NO");
        printf("Individual room : %s\n", current->service->room ? "YES" : "NO");
        printf("INSURANCE TYPE: %s\n", current->service->insurance_type ? "PRIVATE" : "PUBLIC");

        printf("-------------[STATUS : %s]--------------\n", current->service->payment_status);

        current = current->next_service;
    }
}

void list_all_patient_services_provided(const List_Service_Provided *list, const char *username)
{
    
    if (list == NULL) {
        printf("You have nothing to pay for now.\n");
        return;
    }

    const List_Service_Provided *current = list;

    while (current != NULL) {

        if (strcmp(current->service->patient_username, username) == 0 && strcmp(current->service->payment_status, "PENDING") == 0) {
            
            printf("------- [DS CLINIC] ------\n");
            printf("REFERENCE : [%s]\n ", current->service->uh_ID);
            printf("\tFor : %s\n", current->service->patient_username);
            printf("------ DETAILS -----\n");
            printf("Hospitalization duration : %d days at %s.\n ", current->service->number_days, current->service->department);
            printf("----- EXTRA SERVICE TAKEN -----\n");
            printf("Transportation : %s\n", current->service->hospital_transportation ? "YES" : "NO");
            printf("Exclusive nurse care : %s\n", current->service->exclusive_nurse_care ? "YES" : "NO");
            printf("Individual room : %s\n", current->service->room ? "YES" : "NO");
            printf("INSURANCE TYPE: %s\n", current->service->insurance_type ? "PRIVATE" : "PUBLIC");

            printf("---------------------------\n");
        }
        current = current->next_service;
    }
}

void print_service(Service_Provided *service)
{
    if (service == NULL) {
        perror("Nothing to print. The service is NULL\n");
        return;
    }

    printf("----- SERVICE DETAILS -----\n");

    printf("UH-ID              : %s\n", service->uh_ID);
    printf("Patient Username   : %s\n", service->patient_username);
    printf("Department         : %s\n", service->department);
    
    printf("Number of Days     : %d\n", service->number_days);
    printf("Final Cost         : %.2f\n", service->final_cost);

    printf("----- EXTRA SERVICE TAKEN -----\n");

    printf("Hospital Transport : %s\n", 
            service->hospital_transportation ? "YES" : "NO");

    printf("Exclusive Nurse    : %s\n", 
            service->exclusive_nurse_care ? "YES" : "NO");

    printf("Room Included      : %s\n", 
            service->room ? "YES" : "NO");

    printf("Payment Status     : %s\n", service->payment_status);

    printf("Insurance Type     : %s\n",
            service->insurance_type ? "Private" : "Public");

    printf("---------------------------\n");
}

void delete_service(List_Service_Provided **head, const char *uh_ID)
{
    if (head == NULL || *head == NULL) {
        printf("Error: service list is empty\n");
        return;
    }

    List_Service_Provided *current = *head;
    List_Service_Provided *previous = NULL;

    // Case 1: first node matches
    if (strcmp(current->service->uh_ID, uh_ID) == 0) {

        *head = current->next_service;

        // Free everything inside Service_Provided
        free(current->service->uh_ID);
        free(current->service->patient_username);
        free(current->service->department);
        free(current->service->payment_status);

        free(current->service);  // free struct
        free(current);            // free list node

        printf("Successfully deleted service '%s' from memory.\n", uh_ID);
        return;
    }

    // Case 2: search rest of list
    while (current != NULL) {

        if (strcmp(current->service->uh_ID, uh_ID) == 0) {

            previous->next_service = current->next_service;

            free(current->service->uh_ID);
            free(current->service->patient_username);
            free(current->service->department);
            free(current->service->payment_status);

            free(current->service);
            free(current);

            printf("Successfully deleted service '%s' from memory.\n", uh_ID);
            return;
        }

        previous = current;
        current = current->next_service;
    }

    printf("Service with ID '%s' not found.\n", uh_ID);
}

void delete_all_services(List_Service_Provided **head)
{
    puts("------ Clearing Service_provided list ------");

    if (!head || !*head) {
        printf("Service list is already empty.\n");
        return;
    }

    List_Service_Provided *curr = *head;

    while (curr) {
        List_Service_Provided *next = curr->next_service;

        Service_Provided *svc = curr->service;

        if (svc) {
            free(svc->uh_ID);
            free(svc->patient_username);
            free(svc->department);
            free(svc->payment_status);
            free(svc);
        }

        free(curr);
        curr = next;
    }

    *head = NULL;
    printf("Successfully freed all services from memory.\n");
}