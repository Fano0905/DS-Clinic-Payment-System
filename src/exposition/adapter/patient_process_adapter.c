#include "patient_process_port.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>

void show_payment_method_for_user(List_Payment_Method *list, const char *username){
    show_all_payment_methods_for_patient(list, username);
}

List_Payment_Method *generate_empty_payment_method_list(void){
    return generate_payment_method_list();
}

void empty_payment_method_list(List_Payment_Method **payment_method_list){
    clear_payment_method_list(payment_method_list);
}

void create_new_payment_method(List_Payment_Method **payment_method_list, const char *username, const char *password)
{

    char type_input[4];
    PaymentType type = INVALID;

    while (type == INVALID) {

        printf("Select payment method type:\n");
        printf("1. PAYPAL\n");
        printf("2. BANK CARD\n");
        printf("3. CHECK\n");
        printf("> ");
        fgets(type_input, sizeof(type_input), stdin);
        chomp(type_input);
        
        if (strcmp(type_input, "1") == 0)
            type = PAYPAL;
        if (strcmp(type_input, "2") == 0)
            type = BANK_CARD;
        if (strcmp(type_input, "3") == 0)
            type = CHECK;
        if (type == INVALID)
            printf("INVALID TYPE CHOSEN. PLEASE TRY AGAIN.\n");
    }
    set_new_payment_method(payment_method_list, username, password, type);
}

void list_all_services_for_user(List_Service_Provided *service_provided_list, const char *username){
    show_services_for_patient(service_provided_list, username);
}

int proceed_to_payment(Service_Provided *service_provided, List_Payment_Method *payment_method_list){

    return pay_services_provided(service_provided, payment_method_list);
}

void print_receipt(const char *uh_ID, List_Department *department_list, Patient *patient, List_Service_Provided *provided_service_list){
    generate_receipt(uh_ID, department_list, patient, provided_service_list);
}