#include "office.h"
#include "service.h"
#include "display.h"
#include <stdlib.h>

// Function to remove trailing newline characters from a string
void chomp(char *s) {
    size_t len = strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r')) {
        s[--len] = '\0';
    }
}


int login(Credentials credentials){

    FILE *f = fopen("assets/auth.txt", "r");
    char line[256], username[128] = "", password[128] = "";

    if (f == NULL) {
        printf("Error opening auth.txt\n");
        return -1;
    }

    while (fgets(line, sizeof(line), f)) {

        char *p = strchr(line, ':'); if (!p) continue; p++;
        if (strncmp(line, "username", 8) == 0) sscanf(p, " %127[^\n]", username);
        if (strncmp(line, "password", 8) == 0) sscanf(p, " %127[^\n]", password);

    }

    if (strcmp(credentials.username, username) != 0 || strcmp(credentials.password, password) != 0) {
        printf("Invalid credentials\n");
        fclose(f);
        return -1;
    }

    fclose(f);
    return 0;
}

int add_patient_info(){

    Patient patient = {0};

    printf("Enter the first name: ");
    fflush(stdout);
    fgets(patient.fname, sizeof(patient.fname), stdin);
    chomp(patient.fname);
    strupr(patient.fname);
    printf("Enter the last name: ");
    fflush(stdout);
    fgets(patient.lname, sizeof(patient.lname), stdin);
    chomp(patient.lname);
    strupr(patient.lname);
    printf("Enter the username: ");
    fflush(stdout);
    fgets(patient.username, sizeof(patient.username), stdin);
    chomp(patient.username);
    printf("Enter the password: ");
    fflush(stdout);
    fgets(patient.password, sizeof(patient.password), stdin);
    chomp(patient.password);

    int result = create_patient_record(patient.fname, patient.lname, patient.username, patient.password);
    return result;
}

void office(char *buffer)
{
    display_clinic_menu();
    if (strcmp(buffer, "0") == 0) {
        clear_screen();
        display_clinic_menu();
    }
    if (strcmp(buffer, "1") == 0) {
        clear_screen();
        printf("Create patient selected.\n");
        add_patient_info();
    }
    if (strcmp(buffer, "2") == 0) {
        clear_screen();
        printf("Checkout patient selected.\n");
    }
}