#include "clinic_process_port.h"
#include "patient_process_port.h"
#include "exposition.h"

#define BUFFER_SIZE 1024

int check_login(int option, Credentials credentials){

    if (option == 1){
        clear_screen();
        printf("Office management selected.\n");
        printf("Enter username: "); fflush(stdout);
        fgets(credentials.username, sizeof(credentials.username), stdin);
        printf("Enter password: "); fflush(stdout);
        fgets(credentials.password, sizeof(credentials.password), stdin);
        chomp(credentials.username);
        chomp(credentials.password);
        if (login(credentials) == 0)
            return option;
    }

    perror("Invalid option\n");
    return 84;
}

Patient *check_patient_login(List_Patient *patient_list, int option, Credentials credentials){
    
    if (option == 2){
        clear_screen();
        printf("Patient login selected.\n");
        printf("Enter username: "); fflush(stdout);
        fgets(credentials.username, sizeof(credentials.username), stdin);
        printf("Enter password: "); fflush(stdout);
        fgets(credentials.password, sizeof(credentials.password), stdin);
        chomp(credentials.username);
        chomp(credentials.password);
        Patient *logged_patient = patient_login(patient_list, credentials);
        if (logged_patient == NULL) {
            return NULL;
        }else
            return logged_patient;
    }
    perror("Invalid option\n");
    return NULL;
}

void run_application(void){

    List_Patient *patient_list = generate_patient_list();
    List_Service_Provided *service_provided_list = generate_service_provided_list();
    List_Payment_Method *payment_method_list = generate_empty_payment_method_list();
    Patient *logged_user;

    while (1){
    
        char buffer[BUFFER_SIZE];
        Credentials credentials;
        int option = 0;
        
        display_home_menu();
        display_prompt();
        // Read the line entered by the user
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            write(1, "\n", 1);
            break; // end of file (Ctrl+D)
        }

        // Remove the trailing '\n'
        buffer[strcspn(buffer, "\n")] = '\0';
        chomp(buffer);
        option = atoi(buffer);

        switch (option)
        {
        case 0:
            empty_patient_list(&patient_list);
            empty_services_provided_list(&service_provided_list);
            empty_payment_method_list(&payment_method_list);
            puts("Exiting...");
            clear_screen();
            return;
        case 1:
            clear_screen();
            printf("Office management selected.\n");
            if (option == check_login(option, credentials)){
                clear_screen();
                printf("Login successful. Welcome to the admissions office.\n");
                while (1) {
                    display_clinic_menu();
                    display_prompt();
                    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
                        write(1, "\n", 1);
                        break; // end of file (Ctrl+D)
                    }
                    buffer[strcspn(buffer, "\n")] = '\0';
                    if (strcmp(buffer, "0") == 0) {
                        clear_screen();
                        break; // exit to main menu
                    }
                    clinic_controller(&patient_list, &service_provided_list, buffer);
                }
            } else
                printf("Login failed. Returning to main menu.\n");
            break;
        case 2:
            clear_screen();
            printf("Patient login selected.\n");
            logged_user = check_patient_login(patient_list, option, credentials);
            if (logged_user != NULL){
                    clear_screen();
                    printf("Patient login successful. Welcome to the patient portal.\n");
                    while (1){
                        display_patient_menu();
                        display_prompt();
                        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
                            write(1, "\n", 1);
                            break; // end of file (Ctrl+D)
                        }
                        buffer[strcspn(buffer, "\n")] = '\0';
                        if (strcmp(buffer, "0") == 0) {
                            clear_screen();
                            break; // exit to main menu
                        }
                        patient_controller(logged_user, &payment_method_list, &service_provided_list, buffer);
                    }
            } else
                printf("Patient login failed. Returning to main menu.\n");
            break;

        default:
            if (strcmp(buffer, "1") != 0 && strcmp(buffer, "2") != 0 && strcmp(buffer, "0") != 0)
                printf("Invalid option. Please try again.\n");
            printf("Command inserted : [%s]\n", buffer);
            break;
        }
    }
}
