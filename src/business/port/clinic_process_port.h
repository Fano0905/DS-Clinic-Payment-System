#ifndef CLINIC_PROCESS_PORT_H
#define CLINIC_PROCESS_PORT_H

#include "service.h"

void add_patient_info(List_Patient **list);
void proceed_to_checkout(char *buffer, List_Department *department_list, List_Service_Provided **service_provided_list);
Patient *get_patient_by_username(List_Patient *patient_list, const char *username);
void list_departments(List_Department *list);
void list_patients(List_Patient *list);
List_Department *auto_generate_list_departments();
List_Patient *generate_empty_patient_list();
void empty_department_list(List_Department **list);
void empty_patient_list(List_Patient **list);
void empty_services_provided_list(List_Service_Provided **list);
void show_helper(List_Department *department_list);

#endif // CLINIC_PROCESS_PORT_H