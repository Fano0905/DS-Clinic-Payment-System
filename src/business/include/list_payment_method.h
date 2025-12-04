#ifndef LIST_PAYMENT_METHOD_H
#define LIST_PAYMENT_METHOD_H

#include "payment_method.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct List_Payment_Method
{
    Payment_Method *payment_method;
    struct List_Payment_Method *next_method;
} List_Payment_Method;


List_Payment_Method *create_payment_method_list(void);
void save_new_payment_method(List_Payment_Method **head,
                                Payment_Method *method);
Payment_Method *find_payment_method_by_username(
        List_Payment_Method *list,
        const char *username);
Payment_Method *find_method_by_ID(List_Payment_Method *list, int ID);
int find_last_payment_ID(List_Payment_Method **head);
void print_payment_method(Payment_Method *method);
void list_all_payment_methods(const List_Payment_Method *list);
void list_all_user_payment_method(const List_Payment_Method *list, const char *username);
void delete_payment_method_by_username(
        List_Payment_Method **head,
        const char *username);
void delete_all_payment_methods(List_Payment_Method **head);

#endif // LIST_PAYMENT_METHOD_H