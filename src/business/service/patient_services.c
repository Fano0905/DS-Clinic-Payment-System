#include "service.h"
#include "infrastructure_patient_port.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

List_Payment_Method *generate_payment_method_list(void){

    return create_payment_method_list();
}

Payment_Method *get_payment_method_by_ID(List_Payment_Method *payment_method, int ID){
    return get_method_by_ID(payment_method, ID);
}

Payment_Method *get_method_by_username(List_Payment_Method *payment_method_list, const char *username) {
    return get_payment_method_by_username(payment_method_list, username);
}

int pay_services_provided(Service_Provided *service_provided,
                          List_Payment_Method *payment_method_list)
{
    char buffer[128];

    if (!service_provided) {
        fprintf(stderr, "Service is NULL\n");
        return EXIT_FAILURE;
    }

    if (!payment_method_list) {
        fprintf(stderr, "No payment methods registered\n");
        return EXIT_FAILURE;
    }

    show_all_payment_methods_for_patient(payment_method_list,
                                         service_provided->patient_username);

    printf("Enter the ID of the payment method you want to use: ");
    fflush(stdout);

    fgets(buffer, sizeof(buffer), stdin);
    chomp(buffer);

    int ID = atoi(buffer);

    Payment_Method *chosen_method = get_method_by_ID(payment_method_list, ID);

    if (!chosen_method) {
        fprintf(stderr, "Error : Unable to fetch the payment method.\n");
        return EXIT_FAILURE;
    }

    printf("Do you want to pay for this service? (YES/NO)\n");

    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        chomp(buffer);
        strupr(buffer);

        if (strcmp(buffer, "YES") == 0) {
            free(service_provided->payment_status);
            service_provided->payment_status = strdup("PAID");
            printf("Payment successful.\n");
            return EXIT_SUCCESS;
        }

        if (strcmp(buffer, "NO") == 0) {
            printf("Payment cancelled.\n");
            return EXIT_FAILURE;
        }

        printf("Please answer YES or NO:\n");
    }
}

void show_services_for_patient(List_Service_Provided *service_provided_list, const char *username){
    list_all_patient_services_provided(service_provided_list, username);
}

void show_all_payment_methods_for_patient(List_Payment_Method *payement_method_list, const char *username){
    list_all_user_payment_method(payement_method_list, username);
}

void set_new_payment_method(List_Payment_Method **payment_method_list, const char *username, const char *password, PaymentType type){

    if (!payment_method_list || !username || !password) {
        fprintf(stderr, "Error [PAYMENT_METHOD] : invalid arguments.\n");
        return;
    }

    Payment_Method *payment_method = malloc(sizeof(Payment_Method));

    if (!payment_method){
        perror("Error [PAYMENT METHOD] : failed to allocate memory for payment method.\n");
        return;
    }

    int id = generate_auto_payment_ID(payment_method_list);

    payment_method->ID = id;
    payment_method->username = strdup(username);
    payment_method->password = strdup(password);
    payment_method->payment = type;

    add_payment_method_to_list(payment_method_list, payment_method);
}

void clear_all_payment_method(List_Payment_Method **payment_method_list){
    delete_all_payment_methods(payment_method_list);
}
