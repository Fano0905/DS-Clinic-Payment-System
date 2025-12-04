#include "service.h"
#include "infrastructure_clinic_port.h"
#include "infrastructure_patient_port.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <wchar.h>
#include <locale.h>
#include <stdarg.h>

#define RECEIPT_WIDTH 80

void free_service(Service_Provided *service);
bool hospital_ID_exists(List_Service_Provided *service_provided_list, const char *identifier);

// ---------------------------
// Generate a hospital identifier like "0001-DEPT-username"
// ---------------------------
char *generate_hospital_identifier(const char *username, const char *department, List_Service_Provided *service_provided_list) {
    if (username == NULL || department == NULL) return NULL;

    // Get last id (may be NULL)
    char *last_id = get_last_hospital_identifier_in_list(service_provided_list);
    int next_num = 1;
    if (last_id != NULL) {
        next_num = atoi(last_id) + 1;
        free(last_id);
    }

    // format number into 4-digit zero-padded
    char numbuf[16];
    if (next_num <= 9999 && next_num >= 0) {
        // Produce 4-digit zero-padded ID
        snprintf(numbuf, sizeof(numbuf), "%04d", next_num);
    } else {
        // Fallback for large numbers (no zero-padding)
        snprintf(numbuf, sizeof(numbuf), "%d", next_num);
    }

    // Take only the first 3 letters of department
    char dept_short[4];
    strncpy(dept_short, department, 3);
    dept_short[3] = '\0';

    // compute final size: num + "-" + dept_short + "-" + username + NUL
    size_t needed = strlen(numbuf) + 1 + strlen(dept_short) + 1 + strlen(username) + 1;
    char *uh_ID = malloc(needed);
    if (!uh_ID) return NULL;

    // Build the string
    snprintf(uh_ID, needed, "%s-%s-%s", numbuf, dept_short, username);
    return uh_ID;
}

float calculate_total_service_cost(const char *department_name, int number_days,
                                   bool hospital_transportation, bool exclusive_nurse_care,
                                   bool room, bool insurance_type, List_Department *department_list)
{
    if (department_name == NULL || number_days < 0) return -1.0f;

    Department *dept = get_department_by_name(department_list, department_name);
    if (dept == NULL) {
        fprintf(stderr, "Department not found: %s\n", department_name);
        return -1.0f;
    }

    float total = 0.0f;
    const float base_per_day = 30.0f;
    total = (float)number_days * base_per_day;
    float cost_service = 0.0f;

    
    if (exclusive_nurse_care)   cost_service += dept->exclusive_nurse_care;
    if (room)                   cost_service += dept->individual_room;
    cost_service *= number_days;
    if (hospital_transportation) cost_service += dept->ambulance_cost;
    total += cost_service;
    
    // Apply insurance discount: private => 90% discount (leave 10%), public => 25% discount
    if (insurance_type) {
        // private
        total = total * 0.10f; // equivalent to total -= total * 0.90f
    } else {
        // public
        total = total * 0.75f; // equivalent to total -= total * 0.25f
    }

    return total;
}

// ---------------------------
// Create a patient record
// ---------------------------
void set_new_patient_record(const char *first_name,
                           const char *last_name,
                           const char *username,
                           const char *password,
                           List_Patient **patient_list)
{
    if (!first_name || !last_name || !username || !password) {
        fprintf(stderr, "create_patient_record: invalid arguments\n");
        return;
    }

    if (!patient_list) {
        fprintf(stderr, "create_patient_record: patient_list pointer is NULL\n");
        return;
    }

    if (get_patient_by_username(*patient_list, username)){
        fprintf(stderr, "Username => %s already exists. Please choose another one\n", username);
        return;
    }

    Patient *patient = malloc(sizeof(Patient));
    if (!patient) {
        perror("malloc patient failed");
        return;
    }

    patient->fname = strdup(first_name);
    patient->lname = strdup(last_name);
    patient->username = strdup(username);
    patient->password = strdup(password);

    if (!patient->fname || !patient->lname || !patient->username || !patient->password) {
        free(patient->fname);
        free(patient->lname);
        free(patient->username);
        free(patient->password);
        free(patient);
        perror("strdup failed");
        return;
    }

    strupr(patient->fname);
    strupr(patient->lname);

    add_new_patient(patient_list, patient);
    if (get_patient_by_username(*patient_list, username) == NULL){
        perror("Failed to create new patient record\n");
        return;
    }
}

// ---------------------------
// Create a service record and optionally generate receipt.
// Ownership: this function allocates a Service_Provided struct and frees it before returning.
// ---------------------------
void set_new_service_record(const char *username, int number_days, const char *department,
                          bool hospital_transportation, bool exclusive_nurse_care,
                          bool room, const char *payment_status, bool insurance_type, List_Department *department_list, List_Service_Provided **service_provided_list)
{
    if (!username || !department || !payment_status) {
        fprintf(stderr, "Invalid args to create_service_record\n");
        return;
    }

    Service_Provided *service = malloc(sizeof(Service_Provided));
    if (!service) {
        perror("malloc");
        return;
    }
    memset(service, 0, sizeof(*service));

    // Generate UH ID
    char *uh_ID = generate_hospital_identifier(username, department, *service_provided_list);
    char *mock_ID = malloc(sizeof(char) * strlen(uh_ID));
    strcpy(mock_ID, uh_ID);
    if (!uh_ID) {
        fprintf(stderr, "Failed to generate UH ID\n");
        free(service);
        return;
    }

    // Calculate final cost
    float final_cost = calculate_total_service_cost(department, number_days,
                                                   hospital_transportation,
                                                   exclusive_nurse_care, room,
                                                   insurance_type, department_list);

    if (final_cost < 0.0f) {
        free(uh_ID);
        free(service);
        fprintf(stderr, "Failed to calculate final cost\n");
        return;
    }

    // Fill the service struct. We duplicate strings so we can free local memory safely.
    service->uh_ID = strdup(uh_ID);
    service->patient_username = strdup(username);
    service->department = strdup(department);
    service->number_days = number_days;
    service->hospital_transportation = hospital_transportation;
    service->exclusive_nurse_care = exclusive_nurse_care;
    service->room = room;
    service->payment_status = strdup(payment_status);
    strupr(service->payment_status);
    service->insurance_type = insurance_type;
    service->final_cost = final_cost;

    if (validate_service_provided(service_provided_list, service) == -1){
        fprintf(stderr, "Service [%s] already exists.\n", service->uh_ID);
        free_service(service);
        return;
    }
    if (validate_service_provided(service_provided_list, service) == -2){
        fprintf(stderr, "Error : Missing arguments in your object.\n");
        free_service(service);
        return;
    }

    // Save service (assume save_service copies data or stores pointer appropriately)
    add_new_service(service_provided_list, service);

    if (get_service_by_UID(*service_provided_list, mock_ID) == NULL){
        perror("Failed to proceed to checkout\n");
        return;
    }
}

void free_service(Service_Provided *service){
    if (!service){
        perror("Nothing to free.\n");
        return;
    }
    free(service->department);
    free(service->patient_username);
    free(service->payment_status);
    free(service->uh_ID);
    free(service);
}

int validate_service_provided(List_Service_Provided **service_provided_list, Service_Provided *service){

    if (hospital_ID_exists(*service_provided_list, service->uh_ID))
        return -1;
    if (!service->department ||!service->patient_username || !service->final_cost 
    || !service->payment_status || !service->number_days){
        return -2;
    }
    return 0;
}

List_Department *generate_list_departments(void){

    return auto_generate_departments();
}

List_Patient *generate_patient_list(void){
    return create_patient_list();
}

List_Service_Provided *generate_service_provided_list(void){
    return create_service_list();
}

void show_all_departments(List_Department *department_list)
{
    show_departments(department_list);
}

void show_all_departments_name(List_Department *department_list){
    show_departments_name(department_list);
}

void show_all_patients(List_Patient *patient_list){
    show_patients(patient_list);
}

void show_all_services_provided(List_Service_Provided *service_provided_list){
    show_services_provided(service_provided_list);
}

Patient *get_patient_by_uID(List_Patient *patient_list, const char *username)
{
    return find_patient_by_username(patient_list, username);
}

bool check_patient_exists(List_Patient *patient_list, const char *username)
{
    Patient *patient = get_patient_by_username(patient_list, username);

    if (patient != NULL) {
        return true;
    }
    return false;
}

void clear_department_list(List_Department **department_list){
    clear_all_departments(department_list);
}

void clear_patient_list(List_Patient **patient_list){
    clear_all_patients(patient_list);
}

void clear_service_provided_list(List_Service_Provided **service_provided_list){
    clear_all_services(service_provided_list);
}

Service_Provided *get_service_by_username_ID(List_Service_Provided *service_provided_list, const char *username){
    return get_service_by_username(service_provided_list, username);
}

// Helper: prints a formatted line inside │ ... │
void print_wline(const wchar_t *fmt, ...)
{
    wchar_t buffer[RECEIPT_WIDTH + 1];
    wchar_t content[RECEIPT_WIDTH + 1];

    va_list args;
    va_start(args, fmt);
    vswprintf(content, RECEIPT_WIDTH - 4, fmt, args);  // Leave room for borders
    va_end(args);

    // Build: │ content............... │
    swprintf(buffer, RECEIPT_WIDTH + 1, L"│ %-*ls │", RECEIPT_WIDTH - 4, content);
    wprintf(L"%ls\n", buffer);
}

void generate_receipt(const char *uh_ID,
                      List_Department *department_list,
                      Patient *patient,
                      List_Service_Provided *provided_service_list)
{
    setlocale(LC_ALL, ""); // REQUIRED for UTF-8 output with wchar

    if (!uh_ID) {
        fwprintf(stderr, L"generate_receipt: NULL uh_ID\n");
        return;
    }

    Service_Provided *svc = get_service_by_UID(provided_service_list, uh_ID);
    if (!svc) {
        fwprintf(stderr, L"Service with UH_ID \"%hs\" not found.\n", uh_ID);
        return;
    }

    Department *dept = get_department_by_name(department_list, svc->department);

    float initial_cost = svc->insurance_type ?
                         svc->final_cost / 0.10f :     // private
                         svc->final_cost / 0.75f ;     // public

    // Top border ┌───────────┐
    wprintf(L"┌");
    for (int i = 0; i < RECEIPT_WIDTH - 2; i++) wprintf(L"─");
    wprintf(L"┐\n");

    // Header
    print_wline(L"DS CLINIC");
    print_wline(L"Receipt: %hs", uh_ID);
    print_wline(L"");

    // Patient info
    print_wline(L"Patient Information:");
    print_wline(L"First Name: %hs", patient ? patient->fname : "UNKNOWN");
    print_wline(L"Last  Name: %hs", patient ? patient->lname : "UNKNOWN");
    print_wline(L"");

    // Department info
    print_wline(L"Department: %hs (%hs)",
                svc->department ? svc->department : "UNKNOWN",
                dept ? dept->code : "UNK");
    print_wline(L"Hospitalization Days: %d", svc->number_days);
    print_wline(L"");

    print_wline(L"Extra Services:");
    print_wline(L" - Ambulance: %ls   (%.2f €)",
                svc->hospital_transportation ? L"YES" : L"NO",
                dept ? dept->ambulance_cost : 0.0f);

    print_wline(L" - Nursing Care: %ls   (%.2f €)",
                svc->exclusive_nurse_care ? L"YES" : L"NO",
                dept ? dept->exclusive_nurse_care : 0.0f);

    print_wline(L" - Private Room: %ls   (%.2f €)",
                svc->room ? L"YES" : L"NO",
                dept ? dept->individual_room : 0.0f);

    print_wline(L"");
    print_wline(L"Insurance: %ls", svc->insurance_type ? L"Private" : L"Public");

    // Cost summary
    print_wline(L"");
    print_wline(L"Initial Cost: %.2f €", initial_cost);
    print_wline(L"Final Cost:   %.2f €", svc->final_cost);
    print_wline(L"");

    // Bottom border └───────────┘
    wprintf(L"└");
    for (int i = 0; i < RECEIPT_WIDTH - 2; i++) wprintf(L"─");
    wprintf(L"┘\n");
}