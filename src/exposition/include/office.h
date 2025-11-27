#if !defined(OFFICE_H)
#define OFFICE_H

#include "credential.h"
#include "list_patient.h"
#include "list_service_provided.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int login(Credentials credentials);
void office(List_Patient **patient_list, List_Service_Provided **service_provided_list, char *buffer);


#endif // OFFICE_H
