#include "clinic_process_port.h"
#include <ctype.h>

bool is_all_alpha(const char *s){
    while (*s) {
        if (!isalpha((unsigned char)*s)) {
            return false;
        }
        s++;
    }
    return true;
}

int check_patient_input(char *fname, char *lname, char *username, char *password){

    if (fname == NULL || lname == NULL || username == NULL || password == NULL){
        return -1;
    }
    if (strlen(fname) == 0 || strlen(lname) == 0 || strlen(username) == 0 || strlen(password) == 0){
        return -1;
    }
    if (!is_all_alpha(username) || !is_all_alpha(fname) || !is_all_alpha(lname)){
        return -1;
    }
    return 0;

}

void create_new_patient(List_Patient **patient_list){

    char fname[256], lname[256], username[256], password[256], buffer[256];

    printf("Would you like to add a new patient to the database? (yes/no): ");
    fflush(stdin);
    fgets(buffer, sizeof(buffer), stdin);
    chomp(buffer);
    strupr(buffer);

    if (strcmp(buffer, "NO") == 0){
        printf("Aborting patient creation.\n");
        return;
    }
    if (strcmp(buffer, "YES") != 0){
        printf("Invalid input. Please try again.\n");
        create_new_patient(patient_list);
        return;
    }

    printf("Enter first name: ");
    fgets(fname, sizeof(fname), stdin);
    chomp(fname);

    printf("Enter last name: ");
    fgets(lname, sizeof(lname), stdin);
    chomp(lname);

    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    chomp(username);

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    chomp(password);

    if (check_patient_input(fname, lname, username, password) != 0){
        printf("Invalid input(s). Please try again.\n");
        create_new_patient(patient_list);
    }

    set_new_patient_record(fname, lname, username, password, patient_list);
}


bool isnum(const char *s) {
    if (!s || *s == '\0') return false;

    while (*s) {
        if (!isdigit(*s)) return false;
        s++;
    }
    return true;
}

int check_provided_service_input(int nb_days, char *payment_status, bool transport, bool extra_nurse_care, bool individual_room, bool insurance_type){

    printf("value of nb_days: %d\n", nb_days);
    printf("value of payment_status: %s\n", payment_status);
    printf("value of transport: %d\n", transport);
    printf("value of extra_nurse_care: %d\n", extra_nurse_care);
    printf("value of individual_room: %d\n", individual_room);
    printf("value of insurance_type: %d\n", insurance_type);

    if (nb_days <= 0){
        printf("nb_days check failed\n");
        return -1;
    }
    if (strcmp(payment_status, "Pending") != 0){
        printf("payment_status check failed\n");
        return -1;
    }
    if (transport != 1 && transport != 0){
        printf("transport check failed\n");
        return -1;
    }
    if (extra_nurse_care != 1 && extra_nurse_care != 0){
        printf("extra_nurse_care check failed\n");
        return -1;
    }
    if (individual_room != 1 && individual_room != 0){
        printf("individual_room check failed\n");
        return -1;
    }
    if (insurance_type != 1 && insurance_type != 0){
        printf("insurance_type check failed\n");
        return -1;
    }

    return 0;
}


void proceed_to_checkout(char *buffer, List_Department *department_list, List_Service_Provided **service_provided_list){
    
    if (!service_provided_list) {
        fprintf(stderr, "add_service_info: invalid list pointer\n");
        return;
    }

    char patient_username[128];
    char department[128];
    char payment_status[64] = "Pending";
    int nb_days = 0;
    bool transport;
    bool extra_nurse_care;
    bool individual_room;
    bool insurance_type;

    printf("Would you like to proceed to checkout a patient service? (yes/no): ");
    fgets(buffer, sizeof(buffer), stdin);
    chomp(buffer);
    strupr(buffer);
    if (strcmp(buffer, "NO") == 0){
        printf("Aborting service checkout.\n");
        return;
    }
    if (strcmp(buffer, "YES") != 0){
        printf("Invalid input. Please try again.\n");
        proceed_to_checkout(buffer, department_list, service_provided_list);
        return;
    }


    printf("Enter patient username: ");
    fgets(patient_username, sizeof(patient_username), stdin);
    chomp(patient_username);

    printf("Enter department ");
    show_helper(department_list);
    printf("=> ");
    fgets(department, sizeof(department), stdin);
    chomp(department);
    strupr(department);

    printf("Enter number of days spent at the hospital by the patient: ");
    fgets(buffer, sizeof(buffer), stdin);
    chomp(buffer);
    if (!isnum(buffer)) {
        printf("Invalid number of days. Please try again.\n");
        proceed_to_checkout(buffer, department_list, service_provided_list);
        return;
    }
    nb_days = atoi(buffer);

    printf("Hospital transportation ? (1=yes, 0=no): ");
    fgets(buffer, sizeof(buffer), stdin);
    chomp(buffer);
    transport = atoi(buffer);

    printf("Exclusive nurse care ? (1=yes, 0=no): ");
    fgets(buffer, sizeof(buffer), stdin);
    chomp(buffer);
    extra_nurse_care = atoi(buffer);

    printf("Individual room ? (1=yes, 0=no): ");
    fgets(buffer, sizeof(buffer), stdin);
    chomp(buffer);
    individual_room = atoi(buffer);

    printf("Insurance type (1=private, 0=public): ");
    fgets(buffer, sizeof(buffer), stdin);
    chomp(buffer);
    insurance_type = atoi(buffer);

    if (check_provided_service_input(nb_days, payment_status, transport, extra_nurse_care, individual_room, insurance_type) != 0){
        printf("Invalid input(s). Please try again.\n");
        proceed_to_checkout(buffer, department_list, service_provided_list);
        return;
    }

    set_new_service_record(patient_username, nb_days, department, transport, extra_nurse_care, individual_room, payment_status, insurance_type, department_list, service_provided_list);

}

void show_helper(List_Department *department_list){
    show_all_departments_name(department_list);
}

void list_departments(List_Department *department_list){
    show_all_departments(department_list);
}

void list_patients(List_Patient *patient_list){
    show_all_patients(patient_list);
}

bool check_patient_by_username(List_Patient *patient_list, const char *username){
    return check_patient_exists(patient_list, username);
}

List_Department *auto_generate_list_departments(){
    return generate_list_departments();
}

List_Patient *generate_empty_patient_list(){
    return generate_patient_list();
}

Patient *fetch_patient_by_username(List_Patient *patient_list, const char *username){
    return get_patient_by_uID(patient_list, username);
}

void empty_department_list(List_Department **department_list){
    clear_department_list(department_list);
}

void empty_patient_list(List_Patient **patient_list){
    clear_patient_list(patient_list);
}

void empty_services_provided_list(List_Service_Provided **service_provided_list){
    clear_service_provided_list(service_provided_list);
}