#if !defined(SERVICE_PROVIDED_H)
#define SERVICE_PROVIDED_H

#include <stdbool.h>

typedef struct ServiceProvided
{
    char *uh_ID;
    char *patient_username;
    float final_cost;
    int number_days;
    char *department;
    bool hospital_transportation;
    bool exclusive_nurse_care;
    bool room;
    char *payment_status;

} ServiceProvided;

#endif // SERVICE_PROVIDED_H
