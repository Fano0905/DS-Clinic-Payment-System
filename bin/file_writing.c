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