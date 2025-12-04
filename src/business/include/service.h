#if !defined(SERVICE_H)
#define SERVICE_H

#include "list_department.h"
#include "list_patient.h"
#include "list_service_provided.h"
#include "list_payment_method.h"
#include "chomp.h"
#include <stdio.h>

#define BUFFER_SIZE 1024

/*CLINIC SERVICES*/

void set_new_patient_record(const char *first_name,
                           const char *last_name,
                           const char *username,
                           const char *password,
                           List_Patient **patient_list);
void set_new_service_record(const char *username, int number_days, const char *department,
                          bool hospital_transportation, bool exclusive_nurse_care,
                          bool room, const char *payment_status, bool insurance_type, List_Department *department_list, List_Service_Provided **service_provided_list);
void show_all_patients(List_Patient *patient_list);
Patient *get_patient_by_uID(List_Patient *department_list, const char *username);
char *generate_hospital_identifier(const char *username, const char *department, List_Service_Provided *list_service_provided);
void generate_receipt(const char *uh_ID, List_Department *department_list, Patient *patient, List_Service_Provided *service_provided_list);
bool check_patient_exists(List_Patient *patient_list, const char *username);
List_Department *generate_list_departments(void);
void clear_department_list(List_Department **department_list);
void clear_patient_list(List_Patient **patient_list);
void clear_service_provided_list(List_Service_Provided **list);
void clear_payment_method_list(List_Payment_Method **payment_method_list);
List_Patient *generate_patient_list(void);
List_Service_Provided *generate_service_provided_list(void);

/* PATIENT SERVICES*/

Service_Provided *get_service_by_UID(List_Service_Provided *head, const char *uhID);
Service_Provided *get_service_by_username_ID(List_Service_Provided *head, const char *uhID);
Department *get_department_by_name(List_Department *dept, const char *name);
void show_services_for_patient(List_Service_Provided *service_provided_list, const char *username);
void show_all_services_provided(List_Service_Provided *service_provided_list);
void show_all_payment_methods_for_patient(List_Payment_Method *payement_method_list, const char *username);
void show_all_departments(List_Department *list);
void show_all_departments_name(List_Department *department_list);
void set_new_payment_method(List_Payment_Method **payment_method_list, const char *username, const char *password, PaymentType type);
List_Payment_Method *generate_payment_method_list(void);
int pay_services_provided(Service_Provided *service_provided, List_Payment_Method *payment_method_list);
Payment_Method *get_payment_method_by_ID(List_Payment_Method *payment_method, int ID);

#endif // SERVICE_H
