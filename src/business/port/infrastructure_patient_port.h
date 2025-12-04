#ifndef INFRASTRUCTURE_PATIENT_PORT_H
#define INFRASTRUCTURE_PATIENT_PORT_H

#include "list_payment_method.h"
#include "list_department.h"

void add_payment_method_to_list(List_Payment_Method **payment_method_list,
                                Payment_Method *method);

List_Payment_Method *create_payment_method_list(void);

Payment_Method *get_payment_method_by_username(
        List_Payment_Method *payment_method_list,
        const char *username);
Payment_Method *get_method_by_ID(List_Payment_Method *payment_method_list, int ID);
Department *get_department_by_name(List_Department *dept, const char *name);
int generate_auto_payment_ID(List_Payment_Method **payment_method_list);
void print_payment_method(Payment_Method *method);
void list_all_payment_methods(const List_Payment_Method *payment_method_list);
void list_all_user_payment_method(const List_Payment_Method *payment_method_list, const char *username);
void clear_payment_method_list(List_Payment_Method **payment_method_list);

#endif //INFRASTRUCTURE_PATIENT_PORT_H