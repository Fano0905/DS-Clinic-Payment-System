#include "infrastructure.h"
#include <stdbool.h>


int save_patient(const Patient *patient)
{
    FILE *file;
    char *data;
    size_t total_size;

    // Calculate total size needed: fname + lname + username + password + 4 commas + newline + null terminator
    total_size = strlen(patient->fname) + strlen(patient->lname) + strlen(patient->username) + strlen(patient->password) + 6;
    
    data = malloc(total_size);
    if (data == NULL) {
        printf("Error allocating memory\n");
        return EXIT_FAILURE;
    }

    // Properly concatenate strings
    snprintf(data, total_size, "%s,%s,%s,%s\n", patient->fname, patient->lname, patient->username, patient->password);

    // Open file in append mode
    file = fopen("database/patient.csv", "a");
    if (file == NULL) {
        printf("Error opening database/patient.csv\n");
        free(data);
        return EXIT_FAILURE;
    }

    // Write the data to the file
    fputs(data, file);

    // Close and cleanup
    fclose(file);
    free(data);
    return EXIT_SUCCESS;
}

void show_departments(void)
{
    FILE *file;
    char line[512];
    char *headers[32];
    size_t col_width[32] = {0};
    size_t ncols = 0;
    char **rows = NULL;
    size_t nrows = 0;

    file = fopen("database/department.csv", "r");
    if (file == NULL) {
        printf("Error opening database/department.csv\n");
        return;
    }

    /* Read header line */
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        printf("department.csv is empty\n");
        return;
    }

    /* Remove trailing newline */
    line[strcspn(line, "\r\n")] = '\0';

    /* Tokenize header to determine number of columns */
    {
        char *saveptr = NULL;
        char *token = strtok_r(line, ",", &saveptr);
        while (token != NULL && ncols < sizeof(headers)/sizeof(headers[0])) {
            headers[ncols] = strdup(token);
            col_width[ncols] = strlen(token);
            ncols++;
            token = strtok_r(NULL, ",", &saveptr);
        }
    }

    /* Read all rows into memory */
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\r\n")] = '\0'; // trim newline
        rows = realloc(rows, sizeof(char*) * (nrows + 1));
        rows[nrows] = strdup(line);
        nrows++;
    }

    /* Compute column widths from rows */
    for (size_t r = 0; r < nrows; ++r) {
        char *rowcopy = strdup(rows[r]);
        char *saveptr = NULL;
        char *token = strtok_r(rowcopy, ",", &saveptr);
        size_t c = 0;
        while (token != NULL && c < ncols) {
            size_t len = strlen(token);
            if (len > col_width[c]) col_width[c] = len;
            c++;
            token = strtok_r(NULL, ",", &saveptr);
        }
        free(rowcopy);
    }

    /* Print table header */
    printf("Available Departments:\n");

    /* print top border */
    printf("+");
    for (size_t c = 0; c < ncols; ++c) {
        for (size_t i = 0; i < col_width[c] + 2; ++i) putchar('-');
        printf("+");
    }
    printf("\n");

    /* print header row */
    printf("|");
    for (size_t c = 0; c < ncols; ++c) {
        printf(" %-*s |", (int)col_width[c], headers[c]);
    }
    printf("\n");

    /* print separator */
    printf("+");
    for (size_t c = 0; c < ncols; ++c) {
        for (size_t i = 0; i < col_width[c] + 2; ++i) putchar('-');
        printf("+");
    }
    printf("\n");

    /* print rows */
    for (size_t r = 0; r < nrows; ++r) {
        char *rowcopy = strdup(rows[r]);
        char *saveptr = NULL;
        char *token = strtok_r(rowcopy, ",", &saveptr);
        size_t c = 0;
        printf("|");
        while (token != NULL && c < ncols) {
            printf(" %-*s |", (int)col_width[c], token);
            c++;
            token = strtok_r(NULL, ",", &saveptr);
        }
        /* Fill remaining columns if any */
        while (c < ncols) {
            printf(" %*s |", (int)col_width[c], "");
            c++;
        }
        printf("\n");
        free(rowcopy);
    }

    /* print bottom border */
    printf("+");
    for (size_t c = 0; c < ncols; ++c) {
        for (size_t i = 0; i < col_width[c] + 2; ++i) putchar('-');
        printf("+");
    }
    printf("\n");

    /* cleanup */
    for (size_t c = 0; c < ncols; ++c) free(headers[c]);
    for (size_t r = 0; r < nrows; ++r) free(rows[r]);
    free(rows);
    fclose(file);
}

Patient *get_patient_by_username(const char *username)
{
    FILE *file;
    char line[512];

    file = fopen("database/patient.csv", "r");
    if (file == NULL) {
        printf("Error opening database/patient.csv\n");
        return NULL;
    }

    while (fgets(line, sizeof(line), file) != NULL) {

        char *saveptr = NULL;
        char *token = strtok_r(line, ",", &saveptr);
        char *fname = token;
        token = strtok_r(NULL, ",", &saveptr);
        char *lname = token;
        token = strtok_r(NULL, ",", &saveptr);
        char *user = token;
        token = strtok_r(NULL, ",", &saveptr);
        char *pass = token;

        if (strcmp(user, username) == 0) {
            Patient *patient = malloc(sizeof(Patient));
            if (patient != NULL) {
                strncpy(patient->fname, fname, sizeof(patient->fname));
                strncpy(patient->lname, lname, sizeof(patient->lname));
                strncpy(patient->username, user, sizeof(patient->username));
                chomp(pass);
                strncpy(patient->password, pass, sizeof(patient->password));
            }
            fclose(file);
            return patient;
        }
    }

    fclose(file);
    return NULL; // not found
}

char *get_last_hospital_identifier(void)
{
    FILE *file;
    char line[512];
    char *last_id = NULL;

    file = fopen("database/service_provided.csv", "r");
    if (file == NULL) {
        printf("Error opening database/service_provided.csv\n");
        return NULL;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char *saveptr = NULL;
        char *token = strtok_r(line, ",", &saveptr);
        if (token != NULL) {
            if (last_id != NULL) free(last_id);
            /* Extract only the first 4 characters */
            if (strlen(token) >= 4) {
                last_id = malloc(5);  /* 4 chars + null terminator */
                if (last_id != NULL) {
                    strncpy(last_id, token, 4);
                    last_id[4] = '\0';
                }
            } else {
                last_id = strdup(token);  /* fallback: if less than 4 chars, copy all */
            }
        }
    }

    fclose(file);
    return last_id; // may be NULL if file is empty
}

bool check_hospital_identifier(char *identifier)
{
    FILE *file;
    char line[512];

    if (identifier == NULL) return -1;
    /* We only care about the first 4 characters of the identifier */
    if (strlen(identifier) < 4) return -1;

    file = fopen("database/service_provided.csv", "r");
    if (file == NULL) {
        printf("Error opening database/service_provided.csv\n");
        return EXIT_FAILURE;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        /* trim newline */
        line[strcspn(line, "\r\n")] = '\0';
        char *saveptr = NULL;
        char *token = strtok_r(line, ",", &saveptr);
        if (token == NULL) continue;
        /* token is the uh_ID field */
        /* compare only the first 4 characters (the significant part) */
        if (strncmp(token, identifier, 4) == 0) {
            fclose(file);
            return true; /* identifier already used */
        }
    }

    fclose(file);
    return false; /* not found */
}

int save_service(Service_Provided *service)
{
    if (service == NULL) {
        fprintf(stderr, "save_service: NULL service pointer.\n");
        return EXIT_FAILURE;
    }

    size_t total_size =
        strlen(service->uh_ID) +
        strlen(service->patient_username) +
        strlen(service->department) +
        strlen(service->payment_status) +
        100; // extra space for commas, numbers, newline, etc.

    char *data = malloc(total_size);
    if (data == NULL) {
        fprintf(stderr, "save_service: Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    // -----------------------------------------
    // 2. Check if UH_ID already exists (BEFORE opening file)
    // -----------------------------------------
    if (check_hospital_identifier(service->uh_ID)) {
        fprintf(stderr,
                "save_service: UH ID '%s' already exists. Cannot save.\n",
                service->uh_ID);
        free(data);
        return EXIT_FAILURE;
    }

    snprintf(data, total_size,
             "%s,%s,%.2f,%d,%s,%d,%d,%d,%s,%d\n",
             service->uh_ID,
             service->patient_username,
             service->final_cost,
             service->number_days,
             service->department,
             service->hospital_transportation ? 1 : 0,
             service->exclusive_nurse_care ? 1 : 0,
             service->room ? 1 : 0,
             service->payment_status,
             service->insurance_type ? 1 : 0);


    // -----------------------------------------
    // 5. Open CSV for append
    // -----------------------------------------
    FILE *file = fopen("database/service_provided.csv", "a");
    if (file == NULL) {
        perror("save_service: Failed to open database/service_provided.csv");
        free(data);
        return EXIT_FAILURE;
    }

    // -----------------------------------------
    // 6. Write line to file
    // -----------------------------------------
    if (fputs(data, file) == EOF) {
        perror("save_service: Failed to write to file");
        fclose(file);
        free(data);
        return EXIT_FAILURE;
    }

    // -----------------------------------------
    // 7. Cleanup
    // -----------------------------------------
    fclose(file);
    free(data);

    return EXIT_SUCCESS;
}

Service_Provided *get_service_by_uID(const char *uh_ID)
{
    FILE *file;
    char line[512];

    file = fopen("database/service_provided.csv", "r");
    if (file == NULL) {
        printf("Error opening database/service_provided.csv\n");
        return NULL;
    }

    while (fgets(line, sizeof(line), file) != NULL) {

        char *saveptr = NULL;
        char *token = strtok_r(line, ",", &saveptr);
        char *record_uh_ID = token;

        if (strcmp(record_uh_ID, uh_ID) == 0) {
            Service_Provided *service = malloc(sizeof(Service_Provided));
            if (service != NULL) {
                service->uh_ID = strdup(record_uh_ID);
                token = strtok_r(NULL, ",", &saveptr);
                service->patient_username = strdup(token);
                token = strtok_r(NULL, ",", &saveptr);
                service->final_cost = atof(token);
                token = strtok_r(NULL, ",", &saveptr);
                service->number_days = atoi(token);
                token = strtok_r(NULL, ",", &saveptr);
                service->department = strdup(token);
                token = strtok_r(NULL, ",", &saveptr);
                service->hospital_transportation = atoi(token) != 0;
                token = strtok_r(NULL, ",", &saveptr);
                service->exclusive_nurse_care = atoi(token) != 0;
                token = strtok_r(NULL, ",", &saveptr);
                service->room = atoi(token) != 0;
                token = strtok_r(NULL, ",", &saveptr);
                service->payment_status = strdup(token);
                token = strtok_r(NULL, ",", &saveptr);
                service->insurance_type = atoi(token) != 0;
            }
            fclose(file);
            return service;
        }
    }

    fclose(file);
    return NULL; // not found
}

Department *get_department_by_name(const char *dept_name)
{
    FILE *file;
    char line[512];

    file = fopen("database/department.csv", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening database/department.csv\n");
        return NULL;
    }

    while (fgets(line, sizeof(line), file) != NULL) {

        char *saveptr = NULL;

        // Parse CSV fields
        char *name = strtok_r(line, ",", &saveptr);
        char *code = strtok_r(NULL, ",", &saveptr);
        char *ambulance_cost = strtok_r(NULL, ",", &saveptr);
        char *exclusive_nurse_care = strtok_r(NULL, ",", &saveptr);
        char *individual_room = strtok_r(NULL, ",", &saveptr);

        if (!name || !code || !ambulance_cost || !exclusive_nurse_care || !individual_room)
            continue; // malformed line

        // Check if this is the department we want
        if (strcmp(name, dept_name) == 0) {

            Department *department = malloc(sizeof(Department));
            if (!department) {
                fclose(file);
                return NULL;
            }

            // strdup allocates memory for strings (caller must free)
            department->name = strdup(name);
            department->code = strdup(code);

            if (!department->name || !department->code) {
                // Cleanup on failure
                free(department->name);
                free(department->code);
                free(department);
                fclose(file);
                return NULL;
            }

            // Convert costs from strings to integers
            department->ambulance_cost = atoi(ambulance_cost);
            department->exclusive_nurse_care = atoi(exclusive_nurse_care);
            department->individual_room = atoi(individual_room);

            fclose(file);
            return department;
        }
    }

    fclose(file);
    return NULL; // Department not found
}
