#include "list_patient.h"

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

/**
 * Create an empty list.
 */
List_Patient *create_patient_list() {
    return NULL;
}

/**
 * Add a patient node at the end of the linked list.
 * The list behaves normally only if the head is NULL when empty.
 */
void save_new_patient(List_Patient **head, Patient *patient) {

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
 * Search a patient by username.
 * Returns NULL if not found.
 */
Patient *find_patient_by_username(List_Patient *head, const char *username) {

    List_Patient *current = head;

    while (current != NULL) {
        if (strcmp(current->patient->username, username) == 0) {
            return current->patient;
        }
        current = current->next_patient;
    }

    return NULL;
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

void list_all_patients(const List_Patient *list) {

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
void delete_all_patients(List_Patient **head){

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
