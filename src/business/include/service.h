#if !defined(SERVICE_H)
#define SERVICE_H

#include "business.h"

int create_patient_record(const char *first_name, const char *last_name, const char *username, const char *password);
void list_departments(void);

#endif // SERVICE_H
