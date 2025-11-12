#include "service.h"
#include <stdlib.h>
#include <stdio.h>

void list_departments(void)
{
    show_departments();
}

int create_patient_record(const char *first_name, const char *last_name, const char *username, const char *password)
{
    int result = create_patient(first_name, last_name, username, password);
    if (result == 0) {
        printf("Patient record created successfully.\n");
        return EXIT_SUCCESS;
    } else {
        printf("Failed to create patient record.\n");
        return EXIT_FAILURE;
    }
}