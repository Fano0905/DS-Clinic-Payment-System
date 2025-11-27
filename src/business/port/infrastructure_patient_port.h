#ifndef INFRASTRUCTURE_PATIENT_PORT_H
#define INFRASTRUCTURE_PATIENT_PORT_H

#include "list_payment_method.h"

void add_payment_method_to_list(List_Payment_Method **head,
                                Payment_Method *method);

List_Payment_Method *create_payment_method_list(void);

Payment_Method *find_payment_method_by_username(
        List_Payment_Method *list,
        const char *username);
void print_payment_method(Payment_Method *method);
void list_all_payment_methods(const List_Payment_Method *list);
void delete_payment_method(List_Payment_Method **head,
                           const char *username);
void clear_payment_method_list(List_Payment_Method **head);
void list_all_user_payment_method(const List_Payment_Method *list, const char *username);
int get_last_payment_ID(List_Payment_Method **head);
Payment_Method *get_method_by_ID(List_Payment_Method *list, int ID);

#endif //INFRASTRUCTURE_PATIENT_PORT_H