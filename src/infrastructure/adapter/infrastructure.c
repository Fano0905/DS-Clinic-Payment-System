#include "infrastructure.h"

int create_patient(const char *fname, const char *lname, const char *username, const char *password)
{
    FILE *file;
    char *data;
    size_t total_size;

    // Calculate total size needed: fname + lname + username + password + 4 commas + newline + null terminator
    total_size = strlen(fname) + strlen(lname) + strlen(username) + strlen(password) + 6;
    
    data = malloc(total_size);
    if (data == NULL) {
        printf("Error allocating memory\n");
        return EXIT_FAILURE;
    }

    // Properly concatenate strings
    snprintf(data, total_size, "%s,%s,%s,%s\n", fname, lname, username, password);

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