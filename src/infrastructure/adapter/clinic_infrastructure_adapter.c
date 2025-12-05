#include "infrastructure_clinic_port.h"
#include <ctype.h>

void show_departments(List_Department *list){
    list_all_departments(list);
}

void show_patients(const List_Patient *patient_list){
    list_all_patients(patient_list);
}

void show_services_provided(const List_Service_Provided *service_provided_list){
    list_all_services_provided(service_provided_list);
}

void show_patient_services_provided(const List_Service_Provided *service_provided_list, const char *username){
    list_all_patient_services_provided(service_provided_list, username);
}

List_Department *auto_generate_departments(void){
    return generate_list_department();
}

void show_departments_name(List_Department *department_list){
    list_all_departments_by_name(department_list);
}

void add_new_service(List_Service_Provided **service_provided_list, Service_Provided *service){
    save_new_service(service_provided_list, service);
}

void add_new_patient(List_Patient **patient_list, Patient *patient){
    save_new_patient(patient_list, patient);
}

char *get_last_hospital_identifier_in_list(List_Service_Provided *service_provided_list){
    return find_last_hospital_identifier_from_list(service_provided_list);
}

bool hospital_ID_exists(List_Service_Provided *service_provided_list, const char *identifier){
    return check_hospital_identifier_in_list(service_provided_list, identifier);
}

Service_Provided *get_service_by_UID(List_Service_Provided *head, const char *uhID){
    return find_service_by_UID(head, uhID);
}

Service_Provided *get_service_by_username(List_Service_Provided *head, const char *username){
    return find_service_by_username(head, username);
}

Patient *get_patient_by_username(List_Patient *head, const char *username){
    return find_patient_by_username(head, username);
}

void clear_all_departments(List_Department **department_list){
    delete_all_departments(department_list);
}

void clear_all_patients(List_Patient **patient_list){
    delete_all_patients(patient_list);
}

void clear_all_services(List_Service_Provided **service_provided_list){
    delete_all_services(service_provided_list);
}
