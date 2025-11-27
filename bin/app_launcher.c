void launch_app(){

    while (1) {

    char buffer[BUFFER_SIZE];
    Credentials credentials;

    //clear_screen();
    display_home_menu();
    display_prompt();

        // Read the line entered by the user
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            write(1, "\n", 1);
            break; // end of file (Ctrl+D)
        }

        // Remove the trailing '\n'
        buffer[strcspn(buffer, "\n")] = '\0';

        // If the user types "exit", quit
        if (strcmp(buffer, "0") == 0) {
            clear_screen();
            break;
        }
        if (strcmp(buffer, "1") == 0) {
            clear_screen();
            printf("Office management selected.\n");
            printf("Enter username: "); fflush(stdout);
            fgets(credentials.username, sizeof(credentials.username), stdin);
            printf("Enter password: "); fflush(stdout);
            fgets(credentials.password, sizeof(credentials.password), stdin);
            chomp(credentials.username);
            chomp(credentials.password);
            if (login(credentials) == 0) {
                printf("Login successful. Welcome to the admissions office.\n");
                clear_screen();
                while (1) {
                    display_clinic_menu();
                    display_prompt();
                    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
                        write(1, "\n", 1);
                        break; // end of file (Ctrl+D)
                    }
                    buffer[strcspn(buffer, "\n")] = '\0';
                    if (strcmp(buffer, "0") == 0) {
                        clear_screen();
                        break; // exit to main menu
                    }
                    office(buffer);
                }
            } else {
                printf("Login failed. Returning to main menu.\n");
            }
        }
        if (strcmp(buffer, "2") == 0){
            clear_screen();
            printf("Patient login selected.\n");
            printf("Enter username: "); fflush(stdout);
            fgets(credentials.username, sizeof(credentials.username), stdin);
            printf("Enter password: "); fflush(stdout);
            fgets(credentials.password, sizeof(credentials.password), stdin);
            chomp(credentials.username);
            chomp(credentials.password);
            if (patient_login(credentials) == 0) {
                printf("Patient login successful. Welcome to the patient portal.\n");
                clear_screen();
                while (1){
                    display_patient_menu();
                    display_prompt();
                    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
                        write(1, "\n", 1);
                        break; // end of file (Ctrl+D)
                    }
                    buffer[strcspn(buffer, "\n")] = '\0';
                    if (strcmp(buffer, "0") == 0) {
                        clear_screen();
                        break; // exit to main menu
                    }
                    patient();
                }
            } else {
                printf("Patient login failed. Returning to main menu.\n");
            }
        }
        if (strcmp(buffer, "1") != 0 && strcmp(buffer, "2") != 0 && strcmp(buffer, "0") != 0)
            printf("Invalid option. Please try again.\n");
        printf("Command inserted : [%s]\n", buffer);
    }
}