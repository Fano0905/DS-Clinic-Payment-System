#ifndef PATIENT_PROCESS_PORT_H
#define PATIENT_PROCESS_PORT_H

#include "list_department.h"
#include "list_patient.h"
#include "list_service_provided.h"
#include "list_payment_method.h"
#include "service.h"

List_Payment_Method *generate_empty_payment_method_list(void);
void show_payment_method_for_user(List_Payment_Method *payment_method_list, const char *username);
void create_new_payment_method(List_Payment_Method **payment_method_list, const char *username, const char *password);
void empty_payment_method_list(List_Payment_Method **payment_method_list);
int proceed_to_payment(Service_Provided *service_provided, List_Payment_Method *payment_method_list);
void print_receipt(const char *uh_ID, List_Department *department_list, Patient *patient, List_Service_Provided *service_provided_list);

#endif // PATIENT_PROCESS_PORT_H