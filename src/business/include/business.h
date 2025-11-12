#if !defined(BUSINESS_H)
#define BUSINESS_H

#include "department.h"
#include "patient.h"
#include "service_provided.h"

void show_departments(void);
int create_patient(const char *fname, const char *lname, const char *username, const char *password);

#endif // BUSINESS_H
