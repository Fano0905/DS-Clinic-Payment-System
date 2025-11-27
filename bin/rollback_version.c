void add_service_info(List_Service_Provided **list)
{
    if (!list) {
        fprintf(stderr, "add_service_info: invalid list pointer\n");
        return;
    }

    Service_Provided *service = malloc(sizeof(Service_Provided));
    if (!service) {
        perror("malloc service failed");
        return;
    }

        // For simplicity, we will use fixed values for other parameters.
    printf("Enter number of hospitalization days: ");
    fflush(stdout);
    fgets(buffer, 128, stdin);
    chomp(buffer);
    int number_days = atoi(buffer);
    printf("Enter department name: ");
    fflush(stdout);
    fgets(buffer, 128, stdin);
    chomp(buffer);
    char department[128];
    strncpy(department, buffer, sizeof(department) - 1);
    department[sizeof(department) - 1] = '\0';
    printf("Does the patient require hospital transportation? (1 for yes, 0 for no): ");
    fflush(stdout);
    fgets(buffer, 128, stdin);
    chomp(buffer);
    bool hospital_transportation = (atoi(buffer) != 0);
    printf("Does the patient require exclusive nurse care? (1 for yes, 0 for no): ");
    fflush(stdout);
    fgets(buffer, 128, stdin);
    chomp(buffer);
    bool exclusive_nurse_care = (atoi(buffer) != 0);
    printf("Does the patient require a private room? (1 for yes, 0 for no): ");
    fflush(stdout);
    fgets(buffer, 128, stdin);
    chomp(buffer);
    bool room = (atoi(buffer) != 0);
    printf("Enter payment status (e.g., 'Paid', 'Pending'): ");
    fflush(stdout);
    fgets(buffer, 128, stdin);
    chomp(buffer);
    char payment_status[128];
    strncpy(payment_status, buffer, sizeof(payment_status) - 1);
    payment_status[sizeof(payment_status) - 1] = '\0';
    printf("Enter insurance type (1 for Private, 0 for Public): ");
    fflush(stdout);
    fgets(buffer, 128, stdin);
    chomp(buffer);
    bool insurance_type = (atoi(buffer) != 0);
}