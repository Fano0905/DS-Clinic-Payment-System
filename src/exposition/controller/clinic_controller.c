#include "office.h"
#include "display.h"
#include "clinic_process_port.h"

int login(Credentials credentials){

    char awaited_username[128] = "admissions_office", awaited_password[128] = "ao123";
    
    if (strcmp(credentials.username, awaited_username) != 0 || strcmp(credentials.password, awaited_password) != 0) {
        printf("Invalid clinic credentials\n");
        return -1;
    }
    return 0;
}

void clinic_controller(List_Patient **patient_list, List_Service_Provided **service_provided_list, char *buffer)
{
    List_Department *department_list = auto_generate_list_departments();
    char patient_username[128];
    display_clinic_menu();
    if (strcmp(buffer, "0") == 0) {
        clear_screen();
        display_clinic_menu();
        empty_department_list(&department_list);
    }
    if (strcmp(buffer, "1") == 0) {
        clear_screen();
        printf("Create patient selected.\n");
        create_new_patient(patient_list);
    }
    if (strcmp(buffer, "2") == 0) {
        clear_screen();
        printf("Checkout patient selected.\n");
        printf("------ Proceeding to checkout ------\n");
        printf("Enter the username of the patient to search: "); 
        fflush(stdout);
        fgets(buffer, 128, stdin);
        chomp(buffer);
        if (fetch_patient_by_username(*patient_list, buffer)) {
            printf("\nPatient with username '%s' found. Proceeding to checkout...\n", buffer);
            strcpy(patient_username, buffer);
            proceed_to_checkout(buffer, department_list, service_provided_list, patient_username);
        } else {
            printf("\nPatient with username '%s' not found.\n", buffer);
        }
    }
    if (strcmp(buffer, "3") == 0) {
        clear_screen();
        printf("Show departments selected.\n");
        list_departments(department_list);
    }
    if (strcmp(buffer, "4") == 0){
        clear_screen();
        printf("----- Searching patient. -----\n");
        printf("Enter the username of the patient to search: "); fflush(stdout);
        fgets(buffer, 128, stdin);
        chomp(buffer);
        Patient *patient = fetch_patient_by_username(*patient_list, buffer);
        if (patient != NULL) { 
            printf("Patient found:\n");
            printf("First Name: %s\n", patient->fname);
            printf("Last Name: %s\n", patient->lname);
            printf("Username: %s\n", patient->username);
        } else {
            printf("Patient with username '%s' not found.\n", buffer);
        }
    }
    if (strcmp(buffer, "5") == 0)
        list_all_patients(*patient_list);
}