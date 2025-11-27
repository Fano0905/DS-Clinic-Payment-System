#include "patient_process_port.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>

void list_payment_method_for_user(List_Payment_Method *list, const char *username){
    list_all_payment_methods_for_patient(list, username);
}

List_Payment_Method *generate_empty_payment_method_list(void){
    return generate_payment_method_list();
}

void empty_payment_method_list(List_Payment_Method **payment_method_list){
    clear_all_payment_method(payment_method_list);
}

void add_payment_method_info(List_Payment_Method **payment_method_list, const char *username, const char *password)
{

    char type_input[4];
    PaymentType type = INVALID;

    while (type == INVALID) {

        printf("Enter the type of payment (1) PAYPAL, (2) BANK CARD, (3) CHECK: ");
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
    create_payment_method(payment_method_list, username, password, type);
}

int proceed_to_payment(Service_Provided *service_provided, List_Payment_Method *payment_method_list){

    return pay_services_provided(service_provided, payment_method_list);
}

void print_receipt(const char *uh_ID, List_Department *department_list, Patient *patient, List_Service_Provided *provided_service_list){

    generate_receipt(uh_ID, department_list, patient, provided_service_list);
}