#include "exposition.h"

// Function to display the prompt
void display_prompt(void)
{
    write(1, "$> ", 3);
}

void display_home_menu(void)
{
    FILE *file;
    char line[256];
    int terminal_width = 80;
    int line_len = 0;
    int padding = 0;
    
    file = fopen("assets/home_screen.txt", "r");
    if (file == NULL) {
        printf("Error opening home_screen.txt\n");
        return;
    }
    
    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove trailing newline if present
        line_len = strlen(line);
        if (line[line_len - 1] == '\n') {
            line[line_len - 1] = '\0';
            line_len--;
        }
        
        // Calculate padding for centering
        padding = (terminal_width - line_len) / 2;
        if (padding < 0) padding = 0;
        
        // Print centered line
        printf("%*s%s\n", padding, "", line);
    }
    
    fclose(file);
}

void display_clinic_menu(void)
{
    FILE *file;
    char line[256];
    
    file = fopen("assets/office_screen.txt", "r");
    if (file == NULL) {
        printf("Error opening office_screen.txt\n");
        return;
    }
    
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }
    
    fclose(file);
}

void display_patient_menu(void)
{
    FILE *file;
    char line[256];
    
    file = fopen("assets/patient_screen.txt", "r");
    if (file == NULL) {
        printf("Error opening patient_screen.txt\n");
        return;
    }
    
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }
    
    fclose(file);
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