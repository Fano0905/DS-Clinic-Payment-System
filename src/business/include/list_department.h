#ifndef LIST_DEPARTMENT_H
#define LIST_DEPARTMENT_H

#include "department.h"

typedef struct List_Department {
    Department *department;
    struct List_Department *next_dept;
} List_Department;

List_Department* create_department_list();

// Add a department to the list
void add_dept_to_list(List_Department **head, Department *dept);

// Print all departments
void list_all(const List_Department *head);

// Delete a department by name
void delete_dept(List_Department **head, const char *name);

// Find a department by name (returns pointer or NULL)
Department *get_department_by_name(List_Department *head, const char *name);

void clear_list(List_Department **head);

#endif // LIST_DEPARTMENT_H