#include "infrastructure_clinic_port.h"
#include <ctype.h>

void print_border(size_t ncols, size_t col_width[]);

void print_border(size_t ncols, size_t col_width[]) {
    printf("+");
    for (size_t c = 0; c < ncols; c++) {
        for (size_t i = 0; i < col_width[c] + 2; i++)
            putchar('-');
        printf("+");
    }
    printf("\n");
}

List_Department *create_department_list(){

    return NULL;
}

void show_departments_name(List_Department *list){

    if (!list) {
        puts("No departments found.");
        return;
    }

    List_Department *curr = list;

    printf("(");
    while (curr != NULL)
    {
        printf("%s", curr->department->name);
        if (curr->next_dept != NULL)
            printf(",");
        curr = curr->next_dept;
    }
    printf(")\n");
}

void show_departments(List_Department *list)
{
    if (!list) {
        puts("No departments found.\n");
        return;
    }

    /* Define headers */
    const char *headers[] = {
        "Name",
        "Code",
        "Ambulance Cost",
        "Nurse Care Cost",
        "Room Cost"
    };
    const size_t ncols = 5;

    /* Column width initialization */
    size_t col_width[5] = {
        strlen(headers[0]),
        strlen(headers[1]),
        strlen(headers[2]),
        strlen(headers[3]),
        strlen(headers[4])
    };

    /* First pass: compute column widths from list content */
    for (List_Department *cur = list; cur != NULL; cur = cur->next_dept) {
        Department *d = cur->department;

        size_t values[5];
        values[0] = strlen(d->name);
        values[1] = strlen(d->code);

        char buf[64];
        snprintf(buf, sizeof(buf), "%d", d->ambulance_cost);
        values[2] = strlen(buf);

        snprintf(buf, sizeof(buf), "%d", d->exclusive_nurse_care);
        values[3] = strlen(buf);

        snprintf(buf, sizeof(buf), "%d", d->individual_room);
        values[4] = strlen(buf);

        for (size_t i = 0; i < ncols; i++)
            if (values[i] > col_width[i])
                col_width[i] = values[i];
    }

    puts("Available Departments:\n");
    print_border(ncols, col_width);

    /* Print header row */
    printf("|");
    for (size_t c = 0; c < ncols; c++)
        printf(" %-*s |", (int)col_width[c], headers[c]);
    printf("\n");

    print_border(ncols, col_width);

    /* Print each department entry */
    for (List_Department *cur = list; cur != NULL; cur = cur->next_dept) {
        Department *d = cur->department;

        printf("| %-*s | %-*s | %-*d | %-*d | %-*d |\n",
            (int)col_width[0], d->name,
            (int)col_width[1], d->code,
            (int)col_width[2], d->ambulance_cost,
            (int)col_width[3], d->exclusive_nurse_care,
            (int)col_width[4], d->individual_room
        );
    }

    print_border(ncols, col_width);
}

Department *create_department(char *name, int ambulance_cost, int exclusive_nurse_care, int individual_room_cost){

    Department *dept = malloc(sizeof(Department));

        if (!dept) { 
            perror("malloc failed"); 
            return NULL; 
        }

    char *code = malloc(sizeof(char) * 4);

    if (code == NULL){
        perror("failed to allocate memory for input values.\n");
        return NULL;
    }
    dept->ambulance_cost = ambulance_cost;
    dept->exclusive_nurse_care = exclusive_nurse_care;
    dept->individual_room = individual_room_cost;
    dept->name = strdup(name);
    strncpy(code, name, 3);
    strupr(code);
    code[3] = '\0';
    dept->code = strdup(code);

    return dept;
}

/**
 * Add a department node at the end of the linked list.
 * The list behaves normally only if the head is NULL when empty.
 */
void add_dept_to_list(List_Department **head, Department *dept) {

    // Allocate a new list node
    List_Department *newNode = malloc(sizeof(List_Department));
    if (!newNode) {
        perror("malloc failed for List_Department");
        return;
    }

    newNode->department = dept;
    newNode->next_dept = NULL;

    // Case 1: list is empty
    if (*head == NULL) {
        *head = newNode;
        return;
    }

    // Case 2: append to end of list
    List_Department *temp = *head;
    while (temp->next_dept != NULL) {
        temp = temp->next_dept;
    }

    temp->next_dept = newNode;
}

/**
 * Print the content of the entire list.
 */
void list_all(const List_Department *list) {

    if (list == NULL) {
        printf("List is empty.\n");
        return;
    }

    const List_Department *current = list;

    while (current != NULL) {

        printf("Department name : %s [%s]\n",
               current->department->name,
               current->department->code);

        printf("Individual room cost : %d\n",
               current->department->individual_room);

        printf("Ambulance transportation cost : %d\n",
               current->department->ambulance_cost);

        printf("Exclusive nurse care cost : %d\n",
               current->department->exclusive_nurse_care);

        printf("---------------------------\n");

        current = current->next_dept;
    }
}

/**
 * Search a department by name.
 * Returns NULL if not found.
 */
Department *get_department_by_name(List_Department *head, const char *name) {

    List_Department *current = head;

    while (current != NULL) {
        if (strcmp(current->department->name, name) == 0) {
            return current->department;
        }
        current = current->next_dept;
    }

    return NULL;
}

/**
 * Delete a department by name from the list.
 */
void delete_dept(List_Department **head, const char *name) {

    if (head == NULL || *head == NULL) {
        printf("Error: list is empty\n");
        return;
    }

    List_Department *current = *head;
    List_Department *previous = NULL;

    // Case 1: first node must be deleted
    if (strcmp(current->department->name, name) == 0) {

        *head = current->next_dept;

        free(current->department->name);
        free(current->department->code);
        free(current->department);
        free(current);

        printf("Successfully freed Department in memory\n");
        return;
    }

    // Case 2: search in the rest of the list
    while (current != NULL) {

        if (strcmp(current->department->name, name) == 0) {

            previous->next_dept = current->next_dept;

            free(current->department->name);
            free(current->department);
            free(current);

            printf("Successfully freed Department in memory\n");
            return;
        }

        previous = current;
        current = current->next_dept;
    }

    printf("Department '%s' not found.\n", name);
}

/**
 * Free the entire list and all associated memory.
 */
void clear_list(List_Department **head) {

    puts("------ Clearing Department list ------");

    if (head == NULL){
        puts("The list is already empty.\n");
        return;
    }

    List_Department *curr = *head;

    while (curr) {

        List_Department *next = curr->next_dept;

        // Free allocated department fields
        free(curr->department->name);
        free(curr->department);

        // Free the list node itself
        free(curr);

        curr = next;
    }

    *head = NULL;
    printf("Successfully freed object in memory\n");
    return;
}

List_Department *generate_list_department(void)
{
    FILE *file = fopen("database/department.csv", "r");
    if (!file) {
        printf("Error: unable to open database/department.csv\n");
        return NULL;
    }

    char line[512];

    /* Read header line (and ignore it) */
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        printf("department.csv is empty\n");
        return NULL;
    }

    List_Department *head = NULL;
    List_Department *tail = NULL;

    /* Read all data lines */
    while (fgets(line, sizeof(line), file) != NULL) {

        line[strcspn(line, "\r\n")] = '\0'; // remove newline

        char *saveptr = NULL;
        char *token = strtok_r(line, ",", &saveptr);

        if (!token) continue;  // skip empty lines

        /* Create a new department */
        Department *dept = malloc(sizeof(Department));
        if (!dept) {
            perror("malloc");
            fclose(file);
            return head;   // return what was already loaded
        }

        /* COLUMN 1: name */
        dept->name = strdup(token);
        strupr(dept->name);

        /* COLUMN 2: code */
        token = strtok_r(NULL, ",", &saveptr);
        dept->code = strdup(token ? token : "");

        /* COLUMN 3: ambulance_cost */
        token = strtok_r(NULL, ",", &saveptr);
        dept->ambulance_cost = token ? atoi(token) : 0;

        /* COLUMN 4: exclusive_nurse_care */
        token = strtok_r(NULL, ",", &saveptr);
        dept->exclusive_nurse_care = token ? atoi(token) : 0;

        /* COLUMN 5: individual_room */
        token = strtok_r(NULL, ",", &saveptr);
        dept->individual_room = token ? atoi(token) : 0;

        /* Create new list node */
        List_Department *node = malloc(sizeof(List_Department));
        node->department = dept;
        node->next_dept = NULL;

        /* Append to linked list */
        if (!head) {
            head = node;
            tail = node;
        } else {
            tail->next_dept = node;
            tail = node;
        }
    }

    fclose(file);
    return head;
}

void print_patient_border(size_t *col_width, size_t ncols);


/**
 * Add a patient node at the end of the linked list.
 * The list behaves normally only if the head is NULL when empty.
 */
void save_patient(List_Patient **head, Patient *patient) {

    // Allocate a new list node
    List_Patient *newNode = malloc(sizeof(List_Patient));
    if (!newNode) {
        perror("malloc failed for List_Patient");
        return;
    }

    newNode->patient = patient;
    newNode->next_patient = NULL;

    // Case 1: list is empty
    if (*head == NULL) {
        *head = newNode;
        return;
    }

    // Case 2: append to end of list
    List_Patient *temp = *head;
    while (temp->next_patient != NULL) {
        temp = temp->next_patient;
    }

    temp->next_patient = newNode;

}

/**
 * Create an empty list.
 */
List_Patient *create_patient_list() {
    return NULL;
}

/**
 * Print the content of the entire list.
 */
void print_patient_border(size_t *col_width, size_t ncols) {
    printf("+");
    for (size_t c = 0; c < ncols; ++c) {
        for (size_t i = 0; i < col_width[c] + 2; ++i)
            putchar('-');
        printf("+");
    }
    printf("\n");
}

void show_patients(const List_Patient *list) {

    if (list == NULL) {
        printf("List is empty.\n");
        return;
    }

    // Column names
    const char *headers[] = {
        "First Name",
        "Last Name",
        "Username",
        "Password"
    };
    const size_t ncols = 4;

    size_t col_width[4] = {0};

    // Initialize column widths from headers
    for (size_t i = 0; i < ncols; i++) {
        col_width[i] = strlen(headers[i]);
    }

    // First pass: compute max width for each column
    const List_Patient *cur = list;
    while (cur != NULL) {
        if (cur->patient->fname && strlen(cur->patient->fname) > col_width[0])
            col_width[0] = strlen(cur->patient->fname);

        if (cur->patient->lname && strlen(cur->patient->lname) > col_width[1])
            col_width[1] = strlen(cur->patient->lname);

        if (cur->patient->username && strlen(cur->patient->username) > col_width[2])
            col_width[2] = strlen(cur->patient->username);

        if (cur->patient->password && strlen(cur->patient->password) > col_width[3])
            col_width[3] = strlen(cur->patient->password);

        cur = cur->next_patient;
    }

    // Print header
    printf("Registered Patients:\n");

    print_patient_border(col_width, ncols);

    // Header row
    printf("|");
    for (size_t c = 0; c < ncols; ++c) {
        printf(" %-*s |", (int)col_width[c], headers[c]);
    }
    printf("\n");

    print_patient_border(col_width, ncols);

    // Print each patient
    cur = list;
    while (cur != NULL) {
        printf("| %-*s | %-*s | %-*s | %-*s |\n",
               (int)col_width[0], cur->patient->fname,
               (int)col_width[1], cur->patient->lname,
               (int)col_width[2], cur->patient->username,
               (int)col_width[3], cur->patient->password);

        cur = cur->next_patient;
    }

    print_patient_border(col_width, ncols);
}

/**
 * Search a patient by username.
 * Returns NULL if not found.
 */
Patient *find_patient_by_username(List_Patient *head, const char *username) {

    List_Patient *current = head;

    while (current != NULL) {
        printf(current->patient->username);
        if (strcmp(current->patient->username, username) == 0) {
            return current->patient;
        }
        current = current->next_patient;
    }

    return NULL;
}

/**
 * Delete a patient by username from the list.
 */
void delete_patient(List_Patient **head, const char *username) {

    if (head == NULL || *head == NULL) {
        printf("Error: list is empty\n");
        return;
    }

    List_Patient *current = *head;
    List_Patient *previous = NULL;

    // Case 1: first node must be deleted
    if (strcmp(current->patient->username, username) == 0) {

        *head = current->next_patient;

        free(current->patient->username);
        free(current->patient->fname);
        free(current->patient->lname);
        free(current->patient->password);
        free(current->patient);
        free(current);

        printf("Successfully freed Patient in memory\n");
        return;
    }

    // Case 2: search in the rest of the list
    while (current != NULL) {

        if (strcmp(current->patient->username, username) == 0) {

            previous->next_patient = current->next_patient;

            free(current->patient->username);
            free(current->patient->fname);
            free(current->patient->lname);
            free(current->patient->password);
            free(current->patient);
            free(current);

            printf("Successfully freed Patient in memory\n");
            return;
        }

        previous = current;
        current = current->next_patient;
    }

    printf("Patient '%s' not found.\n", username);
}

/**
* Free the entire list and all associated memory.
*/
void clear_patient_list(List_Patient **head) {

    puts("------ Clearing patient list ------");

    if (head == NULL){
        puts("The list is already empty.\n");
        return;
    }

    List_Patient *curr = *head;

    while (curr) {

        List_Patient *next = curr->next_patient;

        // Free allocated patient fields
        free(curr->patient->fname);
        free(curr->patient->lname);
        free(curr->patient->username);
        free(curr->patient->password);
        free(curr->patient);

        // Free the list node itself
        free(curr);

        curr = next;
    }

    *head = NULL;
    printf("Successfully freed object in memory\n");
    return;
}

void print_patient(Patient *patient){

    if (patient == NULL){
        perror("Nothing to print. The patient is NULL\n");
        return;
    }

    printf("IDENTITY: Name => %s + %s\n " , patient->fname, patient->lname);
    printf ("CREDENTIAL : [Username : %s, Password : %s]\n", patient->username, patient->password);
    return;
}

void add_service_to_list(List_Service_Provided **head, Service_Provided *service) {

    // Allocate a new list node
    List_Service_Provided *newNode = malloc(sizeof(List_Service_Provided));
    if (!newNode) {
        perror("malloc failed for List_Service_Provided");
        return;
    }

    newNode->service = service;
    newNode->next_service = NULL;

    // Case 1: list is empty
    if (*head == NULL) {
        *head = newNode;
        return;
    }

    // Case 2: append to end of list
    List_Service_Provided *temp = *head;
    while (temp->next_service != NULL) {
        temp = temp->next_service;
    }

    temp->next_service = newNode;
}

List_Service_Provided *create_service_list(void) {
    return NULL;
}

Service_Provided *get_service_by_uID(List_Service_Provided *head, const char *uhID) {

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

Service_Provided *get_service_by_username(List_Service_Provided *head, const char *username){

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

void list_all_services(const List_Service_Provided *list) {

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

void list_patient_services(const List_Service_Provided *list, const char *username)
{
    
    if (list == NULL) {
        printf("List is empty.\n");
        return;
    }

    const List_Service_Provided *current = list;

    while (current != NULL) {

        if (strcmp(current->service->patient_username, username) == 0){
            
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
void clear_service_list(List_Service_Provided **head)
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

char *get_last_hospital_identifier_from_list(List_Service_Provided *list)
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


void save_service(List_Service_Provided **list, Service_Provided *service)
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
