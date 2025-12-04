#include "infrastructure_patient_port.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Department *get_department_by_name(List_Department *dept, const char *name){
    return find_department_by_name(dept, name);
}

Payment_Method *get_method_by_ID(List_Payment_Method *payment_method, int ID){
    return find_method_by_ID(payment_method, ID);
}

int generate_auto_payment_ID(List_Payment_Method **payment_method_list){
    return find_last_payment_ID(payment_method_list);
}

void add_payment_method_to_list(List_Payment_Method **payment_method_list,
                                Payment_Method *method){
                                    save_new_payment_method(payment_method_list, method);
                                }

void clear_payment_method_list(List_Payment_Method **payment_method_list){
    delete_all_payment_methods(payment_method_list);
}

Payment_Method *get_payment_method_by_username(
        List_Payment_Method *payment_method_list,
        const char *username){
    return find_payment_method_by_username(payment_method_list, username);
}
