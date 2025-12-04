#ifndef INFRASTRUCTURE_CLINIC_PORT_H
#define INFRASTRUCTURE_CLINIC_PORT_H

#include "list_department.h"
#include "list_patient.h"
#include "list_service_provided.h"
#include "chomp.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

void add_new_patient(List_Patient **patient_list, Patient *patient);
void add_new_service(List_Service_Provided **service_provided_list, Service_Provided *service);
void show_departments(List_Department *department_list);
void show_patients(const List_Patient *patient_list);
void show_services_provided(const List_Service_Provided *service_provided_list);
void show_departments_name(List_Department *department_list);
bool hospital_ID_exists(List_Service_Provided *service_provided_list, const char *identifier);
char *get_last_hospital_identifier_in_list(List_Service_Provided *service_provided_list);
Patient *get_patient_by_username(List_Patient *head, const char *username);
Service_Provided *get_service_by_UID(List_Service_Provided *service_provided_list, const char *uhID);
Service_Provided *get_service_by_username(List_Service_Provided *service_provided_list, const char *username);
List_Department *auto_generate_departments(void);
void clear_all_departments(List_Department **department_list);
void clear_all_patients(List_Patient **patient_list);
void clear_all_services(List_Service_Provided **service_provided_list);

#endif // INFRASTRUCTURE_CLINIC_PORT_H