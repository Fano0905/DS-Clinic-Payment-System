#ifndef LIST_PATIENT_H
#define LIST_PATIENT_H

#include "patient.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef struct List_Patient
{
    Patient *patient;
    struct List_Patient *next_patient;
    
} List_Patient;

List_Patient *create_patient_list();
void save_new_patient(List_Patient **head, Patient *patient);
Patient *find_patient_by_username(List_Patient *head, const char *username);
void print_patient(Patient *patient);
void list_all_patients(const List_Patient *list);
void delete_patient(List_Patient **head, const char *username);
void delete_all_patients(List_Patient **head);

#endif //LIST_PATIENT_H