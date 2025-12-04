#if !defined(PATIENT_UI_H)
#define PATIENT_UI_H

#include "credential.h"
#include "list_payment_method.h"
#include "service.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Patient *patient_login(List_Patient *list, Credentials credentials);
void patient_controller(Patient *logged_user, List_Payment_Method **payment_method_list, 
    List_Service_Provided **service_provided_list, char *buffer);
#endif // PATIENT_UI_H