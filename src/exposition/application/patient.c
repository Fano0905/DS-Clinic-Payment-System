#include "patient_ui.h"
#include "display.h"
#include "clinic_process_port.h"
#include "patient_process_port.h"
#include <unistd.h>

Patient *patient_login(List_Patient *list, Credentials credentials){
    
    Patient *patient = get_patient_by_username(list, credentials.username);
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

void patient(Patient *logged_user, List_Payment_Method **payment_method_list, List_Service_Provided **service_provided_list, char *buffer)
{
    printf("Patient management selected.\n");
    display_patient_menu();
    Service_Provided *service_to_pay;
    List_Department *department_list = generate_list_departments();

    if (strcmp(buffer, "0") == 0){
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
        printf("LOGGED USER => username : %s + password : %s\n", logged_user->username, logged_user->password);
        add_payment_method_info(payment_method_list, logged_user->username, logged_user->password);
    }
    if (strcmp(buffer, "3") == 0){
        clear_screen();
        printf("----- Proceeding to payment -----\n");
        list_services_for_patient(*service_provided_list, logged_user->username);
        service_to_pay = find_service_by_username_ID(*service_provided_list, logged_user->username);
        proceed_to_payment(service_to_pay, *payment_method_list);

        if (strcmp(service_to_pay->payment_status, "PAID") == 0){
            print_receipt(service_to_pay->uh_ID, department_list, logged_user, *service_provided_list);
        } else{
            perror("Payment failed. Please proceed again.\n");
            proceed_to_payment(service_to_pay, *payment_method_list);
        }
    }
    if (strcmp(buffer, "4") == 0)
        list_all_payment_methods_for_patient(*payment_method_list, logged_user->username);
}