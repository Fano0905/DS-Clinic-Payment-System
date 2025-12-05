#include "list_department.h"

List_Department *create_department_list(void){

    return NULL;
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

/**
 * Search a department by name.
 * Returns NULL if not found.
 */
Department *find_department_by_name(List_Department *head, const char *name) {

    List_Department *current = head;

    while (current != NULL) {
        if (strcmp(current->department->name, name) == 0) {
            return current->department;
        }
        current = current->next_dept;
    }

    return NULL;
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

void list_all_departments(const List_Department *list)
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

void list_all_departments_by_name(List_Department *list){

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
void delete_all_departments(List_Department **head) {

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

void print_border(size_t ncols, size_t col_width[]) {
    printf("+");
    for (size_t c = 0; c < ncols; c++) {
        for (size_t i = 0; i < col_width[c] + 2; i++)
            putchar('-');
        printf("+");
    }
    printf("\n");
}