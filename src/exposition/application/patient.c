#include "patient_ui.h"
#include "display.h"
#include "clinic_process_port.h"
#include "patient_process_port.h"
#include <unistd.h>
#include <ctype.h>

Patient *patient_login(List_Patient *patient_list, Credentials credentials){
    
    Patient *patient = get_patient_by_username(patient_list, credentials.username);
    char *undefined = "Patient not found\n";
    char *invalid = ":\nInvalid credentials : password incorrect\n";

    if (patient == NULL) {
        write (1, undefined, strlen(undefined));
        return NULL;
    } else{
        if (strcmp(patient->password, credentials.password) != 0) {
            write(1, invalid, strlen(invalid));
            return NULL;
        }
    }
    return patient;
}

void pay_for_services(Patient *logged_user,
                      List_Payment_Method **payment_method_list,
                      List_Service_Provided **service_provided_list,
                      char *buffer,
                      Service_Provided **service_to_pay)
{
    char *command;
    while (*service_to_pay == NULL 
           || strcmp((*service_to_pay)->payment_status, "PAID") == 0) {

        list_services_for_patient(*service_provided_list, logged_user->username);

        printf("Enter the receipt ID of your service: ");
        fflush(stdout);

        fgets(buffer, BUFFER_SIZE, stdin);
        chomp(buffer);
        command = strdup(buffer);

        if (strcmp(strupr(command), "CANCEL") == 0) {
            puts("Payment cancelled.");
            free(command);
            break;
        }

        *service_to_pay = find_service_by_UID(*service_provided_list, buffer);

        if (*service_to_pay == NULL) {
            printf("Unable to find the following service [%s]\n", buffer);
            continue;
        }

        if (strcmp((*service_to_pay)->payment_status, "PAID") == 0) {
            printf("Service already paid.\n");
            continue;
        }
    }

    proceed_to_payment(*service_to_pay, *payment_method_list);
}


void patient(Patient *logged_user, List_Payment_Method **payment_method_list, 
    List_Service_Provided **service_provided_list, char *buffer)
{
    printf("Patient management selected.\n");
    display_patient_menu();
    List_Department *department_list = generate_list_departments();

    if (strcmp(buffer, "0") == 0){
        empty_department_list(&department_list);
        clear_screen();
        display_clinic_menu();
    }
    if (strcmp(buffer, "1") == 0){
        clear_screen();
        printf("----- Displaying patient services -----\n");
        list_services_for_patient(*service_provided_list, logged_user->username);
    }
    if (strcmp(buffer, "2") == 0){
        clear_screen();
        printf("----- Creating payment method for the patient-----\n");
        printf("LOGGED USER => username : [%s]\n", logged_user->username);
        add_payment_method_info(payment_method_list, logged_user->username, logged_user->password);
    }
    if (strcmp(buffer, "3") == 0)
    {
        clear_screen();
        printf("----- Proceeding to payment -----\n");

        Service_Provided *service_to_pay = NULL;

        pay_for_services(logged_user, payment_method_list, 
                        service_provided_list, buffer, &service_to_pay);

        if (service_to_pay && strcmp(service_to_pay->payment_status, "PAID") == 0) {
            print_receipt(service_to_pay->uh_ID, department_list,
                        logged_user, *service_provided_list);
        } else {
            printf("Payment failed. Please retry.\n");
        }
    }
}