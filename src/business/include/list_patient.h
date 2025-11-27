#ifndef LIST_PATIENT_H
#define LIST_PATIENT_H

#include "patient.h"

typedef struct List_Patient
{
    Patient *patient;
    struct List_Patient *next_patient;
    
} List_Patient;

List_Patient *create_patient_list();
void delete_patient(List_Patient **head, const char *username);
void print_patient(Patient *patient);
void add_patient_info(List_Patient **list);

#endif //LIST_PATIENT_H