#if !defined(BUSINESS_H)
#define BUSINESS_H

#include "list_department.h"
#include "list_patient.h"
#include "list_service_provided.h"
#include "list_payment_method.h"
#include "chomp.h"
#include <stdio.h>

#define BUFFER_SIZE 1024

int check_hospital_identifier(char *identifier);
float calculate_final_cost(const char *department, int number_days,
                           bool hospital_transportation,
                           bool exclusive_nurse_care,
                           bool room);

#endif // BUSINESS_H
