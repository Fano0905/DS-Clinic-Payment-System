#ifndef INFRASTRUCTURE_CLINIC_PORT_H
#define INFRASTRUCTURE_CLINIC_PORT_H

#include "list_department.h"
#include "list_patient.h"
#include "list_service_provided.h"
#include "chomp.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

void save_patient(List_Patient **patient_list, Patient *patient);
void save_service(List_Service_Provided **service_provided_list, Service_Provided *service);
Patient *find_patient_by_username(List_Patient *head, const char *username);
void clear_patient_list(List_Patient **patient_list);
void show_departments(List_Department *department_list);
void show_departments_name(List_Department *department_list);
void show_patients(const List_Patient *patient_list);
char *get_last_hospital_identifier_from_list(List_Service_Provided *service_provided_list);
bool check_hospital_identifier_in_list(List_Service_Provided *service_provided_list, const char *identifier);
Service_Provided *get_service_by_uID(List_Service_Provided *service_provided_list, const char *uhID);
Service_Provided *get_service_by_username(List_Service_Provided *service_provided_list, const char *username);
List_Department *generate_list_department(void);

#endif // INFRASTRUCTURE_CLINIC_PORT_H