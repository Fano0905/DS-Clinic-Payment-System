#include "service.h"
#include "infrastructure_patient_port.h"
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define RECEIPT_WIDTH 80

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

    Service_Provided *svc = find_service_by_UID(provided_service_list, uh_ID);
    if (!svc) {
        fwprintf(stderr, L"Service with UH_ID \"%hs\" not found.\n", uh_ID);
        return;
    }

    Department *dept = find_department_by_name(department_list, svc->department);

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

int pay_services_provided(Service_Provided *service_provided,
                          List_Payment_Method *payment_method_list)
{
    char buffer[128];

    if (!service_provided) {
        fprintf(stderr, "Service is NULL\n");
        return EXIT_FAILURE;
    }

    if (!payment_method_list) {
        fprintf(stderr, "No payment methods registered\n");
        return EXIT_FAILURE;
    }

    list_all_payment_methods_for_patient(payment_method_list,
                                         service_provided->patient_username);

    printf("Enter the ID of the payment method you want to use: ");
    fflush(stdout);

    fgets(buffer, sizeof(buffer), stdin);
    chomp(buffer);

    int ID = atoi(buffer);

    Payment_Method *chosen_method = find_method_by_ID(payment_method_list, ID);

    if (!chosen_method) {
        fprintf(stderr, "Error : Unable to fetch the payment method.\n");
        return EXIT_FAILURE;
    }

    printf("Do you want to pay for this service? (YES/NO)\n");

    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        chomp(buffer);
        strupr(buffer);

        if (strcmp(buffer, "YES") == 0) {
            free(service_provided->payment_status);
            service_provided->payment_status = strdup("PAID");
            printf("Payment successful.\n");
            return EXIT_SUCCESS;
        }

        if (strcmp(buffer, "NO") == 0) {
            printf("Payment cancelled.\n");
            return EXIT_FAILURE;
        }

        printf("Please answer YES or NO:\n");
    }
}

Service_Provided *find_service_by_UID(List_Service_Provided *head, const char *uhID){
    return get_service_by_uID(head, uhID);
}

Department *find_department_by_name(List_Department *dept, const char *name){
    return get_department_by_name(dept, name);
}

void list_services_for_patient(List_Service_Provided *service_provided_list, const char *username){
    list_patient_services(service_provided_list, username);
}

void list_all_payment_methods_for_patient(List_Payment_Method *payement_method_list, const char *username){
    list_all_user_payment_method(payement_method_list, username);
}

void create_payment_method(List_Payment_Method **payment_method_list, const char *username, const char *password, PaymentType type){

    if (!payment_method_list || !username || !password) {
        fprintf(stderr, "Error [PAYMENT_METHOD] : invalid arguments.\n");
        return;
    }

    Payment_Method *payment_method = malloc(sizeof(Payment_Method));

    if (!payment_method){
        perror("Error [PAYMENT METHOD] : failed to allocate memory for payment method.\n");
        return;
    }

    int id = generate_auto_payment_ID(payment_method_list);

    payment_method->ID = id;
    payment_method->username = strdup(username);
    payment_method->password = strdup(password);
    payment_method->payment = type;

    add_payment_method_to_list(payment_method_list, payment_method);
}

List_Payment_Method *generate_payment_method_list(void){

    return create_payment_method_list();
}

void clear_all_payment_method(List_Payment_Method **payment_method_list){

    clear_payment_method_list(payment_method_list);
}

int generate_auto_payment_ID(List_Payment_Method **payment_method_list){
    return get_last_payment_ID(payment_method_list);
}

Payment_Method *find_method_by_ID(List_Payment_Method *payment_method, int ID){

    return get_method_by_ID(payment_method, ID);
}