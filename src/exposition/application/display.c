#include "exposition.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>

#define HOME_WIDTH 63
#define CLINIC_WIDTH 63
#define PATIENT_WIDTH  71 
// Function to display the prompt
void display_prompt(void)
{
    write(1, "$> ", 3);
}

static void print_center_line(const wchar_t *text)
{
    int text_len = wcslen(text);
    int padding = (HOME_WIDTH - 2 - text_len) / 2;

    if (padding < 0) padding = 0;

    wprintf(L"│");
    for (int i = 0; i < padding; i++) wprintf(L" ");
    wprintf(L"%ls", text);
    for (int i = 0; i < HOME_WIDTH - 2 - padding - text_len; i++)
        wprintf(L" ");
    wprintf(L"│\n");
}

void display_home_menu(void)
{
    setlocale(LC_ALL, "");

    // Top border
    wprintf(L"┌");
    for (int i = 0; i < HOME_WIDTH - 2; i++) wprintf(L"─");
    wprintf(L"┐\n");

    print_center_line(L"DS CLINIC");
    print_center_line(L"");
    print_center_line(L"WELCOME TO THE HOME MENU");
    print_center_line(L"");
    print_center_line(L"COMMANDS :");
    print_center_line(L"");
    print_center_line(L"Press 0 to EXIT");
    print_center_line(L"");
    print_center_line(L"Press 1 for OFFICE");
    print_center_line(L"");
    print_center_line(L"Press 2 for PATIENT");

    for (int i = 0; i < 12; i++)
        print_center_line(L"");

    // Bottom border
    wprintf(L"└");
    for (int i = 0; i < HOME_WIDTH - 2; i++) wprintf(L"─");
    wprintf(L"┘\n");
}

static void print_clinic_center_line(const wchar_t *text)
{
    int text_len = wcslen(text);
    int padding = (CLINIC_WIDTH - 2 - text_len) / 2;

    if (padding < 0) padding = 0;

    wprintf(L"│");
    for (int i = 0; i < padding; i++) wprintf(L" ");
    wprintf(L"%ls", text);
    for (int i = 0; i < CLINIC_WIDTH - 2 - padding - text_len; i++)
        wprintf(L" ");
    wprintf(L"│\n");
}

void display_clinic_menu(void)
{
    setlocale(LC_ALL, "");

    // Top border
    wprintf(L"┌");
    for (int i = 0; i < CLINIC_WIDTH - 2; i++) wprintf(L"─");
    wprintf(L"┐\n");

    print_clinic_center_line(L"DS CLINIC");
    print_clinic_center_line(L"");
    print_clinic_center_line(L"WELCOME TO THE CLINIC DASHBOARD");
    print_clinic_center_line(L"");
    print_clinic_center_line(L"COMMANDS :");
    print_clinic_center_line(L"");
    print_clinic_center_line(L"Press 0 to EXIT");
    print_clinic_center_line(L"");
    print_clinic_center_line(L"Press 1 for create_patient");
    print_clinic_center_line(L"");
    print_clinic_center_line(L"Press 2 for checkout_patient");
    print_clinic_center_line(L"");
    print_clinic_center_line(L"Press 3 to view all departments");
    print_clinic_center_line(L"");
    print_clinic_center_line(L"Press 4 to search a patient");
    print_clinic_center_line(L"");

    // Padding lines to match the txt file look
    for (int i = 0; i < 11; i++)
        print_clinic_center_line(L"");

    // Bottom border
    wprintf(L"└");
    for (int i = 0; i < CLINIC_WIDTH - 2; i++) wprintf(L"─");
    wprintf(L"┘\n");
}

static void print_center_patient(const wchar_t *text)
{
    int len = wcslen(text);
    int padding = (PATIENT_WIDTH - 2 - len) / 2;

    if (padding < 0) padding = 0;

    wprintf(L"│");
    for (int i = 0; i < padding; i++) wprintf(L" ");
    wprintf(L"%ls", text);

    for (int i = 0; i < PATIENT_WIDTH - 2 - padding - len; i++)
        wprintf(L" ");

    wprintf(L"│\n");
}

void display_patient_menu(void)
{
    setlocale(LC_ALL, "");

    wprintf(L"┌");
    for (int i = 0; i < PATIENT_WIDTH - 2; i++) wprintf(L"─");
    wprintf(L"┐\n");

    print_center_patient(L"DS CLINIC");
    print_center_patient(L"");
    print_center_patient(L"WELCOME TO THE PATIENT DASHBOARD");
    print_center_patient(L"");
    print_center_patient(L"COMMANDS :");
    print_center_patient(L"");
    print_center_patient(L"Press 0 to EXIT");
    print_center_patient(L"");
    print_center_patient(L"Press 1 for checking services");
    print_center_patient(L"");
    print_center_patient(L"Press 2 for add payment method");
    print_center_patient(L"");
    print_center_patient(L"Press 3 to pay for a service provided");

    for (int i = 0; i < 12; i++)
        print_center_patient(L"");

    // bordure basse
    wprintf(L"└");
    for (int i = 0; i < PATIENT_WIDTH - 2; i++) wprintf(L"─");
    wprintf(L"┘\n");
}

/* Portable screen clear implementation */
void clear_screen(void)
{
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    if (system("clear") == -1) {
        for (int i = 0; i < 50; ++i) putchar('\n');
    }
#endif
}