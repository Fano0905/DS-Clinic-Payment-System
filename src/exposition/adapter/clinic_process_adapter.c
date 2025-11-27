#include "clinic_process_port.h"

void add_patient_info(List_Patient **patient_list){

    char fname[256], lname[256], username[256], password[256];

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

    create_patient_record(fname, lname, username, password, patient_list);
    if (patient_list != NULL){
        printf("Successfully created %s [%s + %s] in our database\n", username, fname, lname);
    } else{
        printf("Failed to create [%s]\n", username);
    }
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
    nb_days = atoi(buffer);

    printf("Hospital transportation ? (1=yes, 0=no): ");
    fgets(buffer, sizeof(buffer), stdin);
    transport = atoi(buffer);

    printf("Exclusive nurse care ? (1=yes, 0=no): ");
    fgets(buffer, sizeof(buffer), stdin);
    extra_nurse_care = atoi(buffer);

    printf("Individual room ? (1=yes, 0=no): ");
    fgets(buffer, sizeof(buffer), stdin);
    individual_room = atoi(buffer);

    printf("Insurance type (1=private, 0=public): ");
    fgets(buffer, sizeof(buffer), stdin);
    insurance_type = atoi(buffer);

    create_service_record(patient_username, nb_days, department, transport, extra_nurse_care, individual_room, payment_status, insurance_type, department_list, service_provided_list);

}

void show_helper(List_Department *department_list){
    list_all_departments_name(department_list);
}

Patient *get_patient_by_username(List_Patient *patient_list, const char *username){
    return find_patient_by_uID(patient_list, username);
}

void list_departments(List_Department *department_list){
    list_all_departments(department_list);
}

void list_patients(List_Patient *patient_list){
    list_all_patients(patient_list);
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

void empty_department_list(List_Department **department_list){
    clear_all_departments(department_list);
}

void empty_patient_list(List_Patient **patient_list){
    clear_all_patients(patient_list);
}

void empty_services_provided_list(List_Service_Provided **service_provided_list){
    clear_all_services_provided(service_provided_list);
}