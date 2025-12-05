#ifndef LIST_DEPARTMENT_H
#define LIST_DEPARTMENT_H

#include "department.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef struct List_Department {
    Department *department;
    struct List_Department *next_dept;
} List_Department;

List_Department* create_department_list();
List_Department *generate_list_department(void);
Department *find_department_by_name(List_Department *head, const char *name);
Department *create_department(char *name, int ambulance_cost, int exclusive_nurse_care, int individual_room_cost);
void add_dept_to_list(List_Department **head, Department *dept);
void list_all_departments(const List_Department *head);
void list_all_departments_by_name(List_Department *list);
void delete_dept(List_Department **head, const char *name);
void delete_all_departments(List_Department **head);
void print_border(size_t ncols, size_t col_width[]);

#endif // LIST_DEPARTMENT_H