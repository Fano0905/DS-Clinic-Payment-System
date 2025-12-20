#include "src/business/include/service.h"
#include <assert.h>
#include <stdio.h>

void test_patient_create(){
    
    List_Patient *patient_list = create_patient_list();
    assert(patient_list != NULL);
    
    set_new_patient_record("John", "Doe", "JohnDoe", "password123", &patient_list);
    Patient* fetched_patient = get_patient_by_uID(patient_list, "JohnDoe");
    assert(fetched_patient != NULL);
    assert(strcmp(fetched_patient->fname, "JOHN") == 0);
    assert(strcmp(fetched_patient->lname, "DOE") == 0);
    assert(strcmp(fetched_patient->username, "JohnDoe") == 0);
    assert(strcmp(fetched_patient->password, "password123") == 0);

    clear_patient_list(&patient_list);
}

void test_proceed_to_checkout(){
    List_Patient *patient_list = create_patient_list();
    List_Department *department_list = generate_list_departments();
    List_Service_Provided *service_provided_list = generate_service_provided_list();
    assert(patient_list != NULL);
    assert(department_list != NULL);

    set_new_patient_record("John", "Doe", "JohnDoe", "password123", &patient_list);
    Patient* fetched_patient = get_patient_by_uID(patient_list, "JohnDoe");
    assert(fetched_patient != NULL);
    assert(strcmp(fetched_patient->fname, "JOHN") == 0);
    assert(strcmp(fetched_patient->lname, "DOE") == 0);
    assert(strcmp(fetched_patient->username, "JohnDoe") == 0);
    assert(strcmp(fetched_patient->password, "password123") == 0);
    
    // Simulate checkout process (details would depend on actual implementation)
    // For this test, we will just ensure that the function can be called without errors.
    set_new_service_record("JohnDoe", 3,"Cardiology", true, true, true, "Pending", "true", department_list, &service_provided_list);

    clear_patient_list(&patient_list);
}

void test_generate_service_provided_list(){
    List_Service_Provided *service_provided_list = generate_service_provided_list();
    assert(service_provided_list != NULL);
    // Further assertions can be added based on expected contents of the list
    clear_service_provided_list(&service_provided_list);
}

void test_calculate_total_service_cost(){
    // This function would test the cost calculation logic
    // Since the actual calculation function is not provided, this is a placeholder
    // Example:
    // float cost = calculate_total_service_cost(...);
    // assert(cost == expected_value);

    List_Department *department_list = generate_list_departments();
    assert(department_list != NULL);

    float expected_cost = 550.0f; // Placeholder expected value
    float claculated_cost = calculate_total_service_cost("CARDIOLOGY", 3, true, true, true, true, department_list); // Placeholder function call
    assert(claculated_cost == expected_cost);
    clear_department_list(&department_list);
}

void run_all_tests(){

    puts("--- Running Clinic Services Tests ---");
    test_patient_create();
    test_proceed_to_checkout();
    test_generate_service_provided_list();
    test_calculate_total_service_cost();
    printf("All tests passed successfully.\n");
}