#include "infrastructure_patient_port.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* --------------------------------------------------------
   Add a payment method to the linked list
   -------------------------------------------------------- */
void add_payment_method_to_list(List_Payment_Method **head,
                                Payment_Method *method)
{
    if (!head || !method) return;

    List_Payment_Method *node = malloc(sizeof(List_Payment_Method));
    if (!node) {
        perror("malloc failed for List_Payment_Method");
        return;
    }

    node->payment_method = method;
    node->next_method = NULL;

    if (*head == NULL) {
        *head = node;
        return;
    }

    List_Payment_Method *curr = *head;

    while (curr->next_method != NULL)
        curr = curr->next_method;

    curr->next_method = node;
}


List_Payment_Method *create_payment_method_list(void)
{
    return NULL;
}


/* --------------------------------------------------------
   Find a payment method by username  
   -------------------------------------------------------- */
Payment_Method *find_payment_method_by_username(
        List_Payment_Method *list,
        const char *username)
{
    while (list != NULL) {
        if (strcmp(list->payment_method->username, username) == 0)
            return list->payment_method;
        
        list = list->next_method;
    }

    return NULL;
}


/* --------------------------------------------------------
   Print a single payment method
   -------------------------------------------------------- */
void print_payment_method(Payment_Method *method)
{
    if (!method) {
        printf("Nothing to print (method is NULL)\n");
        return;
    }

    printf("----- PAYMENT METHOD -----\n");

    printf("ID       : %d\n", method->ID);
    printf("Username       : %s\n", method->username);

    printf("Payment Type   : ");

    switch (method->payment) {
        case PAYPAL:     printf("PAYPAL\n"); break;
        case BANK_CARD:  printf("BANK_CARD\n"); break;
        case CHECK:      printf("CHECK\n"); break;
        default:         printf("UNKNOWN\n"); break;
    }

    printf("---------------------------\n");
}


/* --------------------------------------------------------
   List all payment methods  
   -------------------------------------------------------- */
void list_all_payment_methods(const List_Payment_Method *list)
{
    if (list == NULL) {
        printf("Payment method list is empty.\n");
        return;
    }

    while (list != NULL) {
        print_payment_method(list->payment_method);
        list = list->next_method;
    }
}


void list_all_user_payment_method(const List_Payment_Method *list, const char *username){

    if (list == NULL) {
        printf("List is empty.\n");
        return;
    }

    const List_Payment_Method *current = list;

    while (current != NULL) {
        if (strcmp(current->payment_method->username, username) == 0){
            print_payment_method(current->payment_method);
            current = current->next_method;
        }
    }
    
}

/* --------------------------------------------------------
   Delete a payment method by username  
   -------------------------------------------------------- */
void delete_payment_method(List_Payment_Method **head,
                           const char *username)
{
    if (!head || !*head) {
        printf("Payment method list is empty.\n");
        return;
    }

    List_Payment_Method *curr = *head;
    List_Payment_Method *prev = NULL;

    if (strcmp(curr->payment_method->username, username) == 0) {

        *head = curr->next_method;

        free(curr->payment_method->username);
        free(curr->payment_method->password);
        free(curr->payment_method);
        free(curr);

        printf("Payment method '%s' deleted.\n", username);
        return;
    }

    while (curr != NULL) {

        if (strcmp(curr->payment_method->username, username) == 0) {

            prev->next_method = curr->next_method;

            free(curr->payment_method->username);
            free(curr->payment_method->password);
            free(curr->payment_method);
            free(curr);

            printf("Payment method '%s' deleted.\n", username);
            return;
        }

        prev = curr;
        curr = curr->next_method;
    }

    printf("Payment method '%s' not found.\n", username);
}


/* --------------------------------------------------------
   Clear entire payment method list  
   -------------------------------------------------------- */
void clear_payment_method_list(List_Payment_Method **head)
{

    puts("------ Clearing payment_method list ------");

    if (!head || !*head) return;

    List_Payment_Method *curr = *head;

    while (curr) {
        List_Payment_Method *next = curr->next_method;

        free(curr->payment_method->username);
        free(curr->payment_method->password);
        free(curr->payment_method);

        free(curr);
        curr = next;
    }

    *head = NULL;

    printf("All payment methods freed from memory.\n");
}

int get_last_payment_ID(List_Payment_Method **head)
{
    if (*head == NULL)
        return 1;   // first ID

    int max_id = 0;
    List_Payment_Method *curr = *head;

    while (curr != NULL) {
        if (curr->payment_method->ID > max_id)
            max_id = curr->payment_method->ID;
        curr = curr->next_method;
    }

    return max_id + 1;
}

Payment_Method *get_method_by_ID(List_Payment_Method *list, int ID){
    
    if (!list) {
        perror("Error : failed to initialize the list.\n");
        return NULL;
    }

    List_Payment_Method *curr = list;

    while (curr != NULL)
    {
        if (curr->payment_method->ID == ID)
            return curr->payment_method;
        curr = curr->next_method;
    }
    printf("Error : failed to retrieve the payment [%d].\n", ID);
    return NULL;
}