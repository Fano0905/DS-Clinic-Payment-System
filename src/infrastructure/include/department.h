#if !defined(DEPARTMENT_H)
#define DEPARTMENT_H

typedef struct Department
{
    char *department;
    char *code;
    int ambulance_cost;
    int exclusive_nurse_care;
    int individual_room;

} Department;

#endif // DEPARTMENT_H
