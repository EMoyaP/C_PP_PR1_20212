#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "test_pr1.h"
#include "api.h"

// Run all tests for PR1
bool run_pr1(tTestSuite* test_suite, const char* input) {
    bool ok = true;
    tTestSection* section = NULL;

    assert(test_suite != NULL);

    testSuite_addSection(test_suite, "PR1", "Tests for PR1 exercices");

    section = testSuite_getSection(test_suite, "PR1");
    assert(section != NULL);

    ok = run_pr1_ex1(section, input);
    ok = run_pr1_ex2(section, input) && ok;
    ok = run_pr1_ex3(section, input) && ok;

    return ok;
}

// Run all tests for Exercice 1 of PR1
bool run_pr1_ex1(tTestSection* test_section, const char* input) {
    bool passed = true, failed = false;
    const char* version;
    
    /////////////////////////////
    /////  PR1 EX1 TEST 1  //////
    /////////////////////////////
    failed = false;
    start_test(test_section, "PR1_EX1_1", "Read version information.");
    // Get the version
    version = api_version();
    if (strcmp(version, "UOC PP 20212") != 0) {
        failed = true;
        passed = false;
    }
    end_test(test_section, "PR1_EX1_1", !failed);
    
    return passed;
}

// Run all tests for Exercice 2 of PR1
bool run_pr1_ex2(tTestSection* test_section, const char* input) {
    tApiData data;
    tApiError error;
    tCSVEntry entry;    
    int nLots;
    int nVaccines;
    int nPeople;
    bool passed = true;
    bool failed = false;
    bool fail_all = false;
    
    /////////////////////////////
    /////  PR1 EX2 TEST 1  //////
    /////////////////////////////
    failed = false;
    start_test(test_section, "PR1_EX2_1", "Initialize the API data structure");
    // Initialize the data    
    error = api_initData(&data);
    if (error != E_SUCCESS) {
        failed = true;
        passed = false;
        fail_all = true;
    }
    end_test(test_section, "PR1_EX2_1", !failed);
    
    /////////////////////////////
    /////  PR1 EX2 TEST 2  //////
    /////////////////////////////
    failed = false;
    start_test(test_section, "PR1_EX2_2", "Add a valid lot");
    if (fail_all) {
        failed = true;
    } else {
        csv_initEntry(&entry);
        csv_parseEntry(&entry, "01/01/2022;13:45;08001;PFIZER;2;21;300", "VACCINE_LOT");
        error = api_addVaccineLot(&data, entry);
        if (error != E_SUCCESS) {
            failed = true;
            passed = false;
            fail_all = true;
        }
        csv_freeEntry(&entry);
    }
    end_test(test_section, "PR1_EX2_2", !failed);
           
    /////////////////////////////
    /////  PR1 EX2 TEST 3  //////
    /////////////////////////////
    failed = false;
    start_test(test_section, "PR1_EX2_3", "Check the number of vaccines");
    if (fail_all) {
        failed = true;
    } else {
        nVaccines = api_vaccineCount(data);        
        if (nVaccines != 1) {
            failed = true;
            passed = false;
            fail_all = true;
        }        
    }
    end_test(test_section, "PR1_EX2_3", !failed);
    
    /////////////////////////////
    /////  PR1 EX2 TEST 4  //////
    /////////////////////////////
    failed = false;
    start_test(test_section, "PR1_EX2_4", "Check the number of vaccine lots");
    if (fail_all) {
        failed = true;
    } else {        
        nLots = api_vaccineLotsCount(data);
        if (nLots != 1) {
            failed = true;
            passed = false;
            fail_all = true;
        }        
    }
    end_test(test_section, "PR1_EX2_4", !failed);
    
    /////////////////////////////
    /////  PR1 EX2 TEST 5  //////
    /////////////////////////////
    failed = false;
    start_test(test_section, "PR1_EX2_5", "Add a lot with invalid type");    
    if (fail_all) {
        failed = true;
    } else {
        csv_initEntry(&entry);
        csv_parseEntry(&entry, "01/01/2022;13:45;08001;PFIZER;2;21;300", "VACCINE_LOT_DATA");
        error = api_addVaccineLot(&data, entry);
        if (error != E_INVALID_ENTRY_TYPE) {
            failed = true;
            passed = false;
        }
        csv_freeEntry(&entry);
    }
    end_test(test_section, "PR1_EX2_5", !failed);
    
    /////////////////////////////
    /////  PR1 EX2 TEST 6  //////
    /////////////////////////////
    failed = false;
    start_test(test_section, "PR1_EX2_6", "Add a lot with invalid fields");    
    if (fail_all) {
        failed = true;
    } else {
        csv_initEntry(&entry);
        csv_parseEntry(&entry, "01/01/2022;13:45;08001;PFIZER;2;21;300;extra_field", "VACCINE_LOT");
        error = api_addVaccineLot(&data, entry);
        if (error != E_INVALID_ENTRY_FORMAT) {
            failed = true;
            passed = false;
        }
        csv_freeEntry(&entry);
    }
    end_test(test_section, "PR1_EX2_6", !failed);
    
    /////////////////////////////
    /////  PR1 EX2 TEST 7  //////
    /////////////////////////////
    failed = false;
    start_test(test_section, "PR1_EX2_7", "Free API data");    
    if (fail_all) {
        failed = true;
    } else {        
        error = api_freeData(&data);
        nVaccines = api_vaccineCount(data);
        nLots = api_vaccineLotsCount(data);
        nPeople = api_populationCount(data);
        if (error != E_SUCCESS || nVaccines != 0 || nLots != 0 || nPeople != 0) {
            failed = true;
            passed = false;
            fail_all = true;
        }        
    }
    end_test(test_section, "PR1_EX2_7", !failed);
    
    
    /////////////////////////////
    /////  PR1 EX2 TEST 8  //////
    /////////////////////////////
    failed = false;
    start_test(test_section, "PR1_EX2_8", "Load data from file");
    // Load basic data to the API
    if (fail_all) {
        failed = true;
    } else {
        error = api_loadData(&data, input, true);
        nVaccines = api_vaccineCount(data);
        nLots = api_vaccineLotsCount(data);
        nPeople = api_populationCount(data);
        if (error != E_SUCCESS || nVaccines != 2 || nLots != 5 || nPeople != 2) {
            failed = true;
            passed = false;
        }
    }
    end_test(test_section, "PR1_EX2_8", !failed);
    
    // Release all data
    api_freeData(&data);
    
    return passed;
}


// Run all tests for Exercice 3 of PR1
bool run_pr1_ex3(tTestSection* test_section, const char* input) {
    tApiData data;
    tApiError error;
    tCSVEntry entry;
    tCSVEntry refEntry;
    tCSVData report;
    tCSVData refReport;
    tDateTime timestamp;
    int nLots;
    int nVaccines;
    int nPeople;
    bool passed = true;
    bool failed = false;
    bool fail_all = false;
    
    
    // Initialize the data    
    error = api_initData(&data);
    if (error != E_SUCCESS) {        
        passed = false;        
        fail_all = true;
    }
    
    if (!fail_all) {
        error = api_loadData(&data, input, true);
        nVaccines = api_vaccineCount(data);
        nLots = api_vaccineLotsCount(data);
        nPeople = api_populationCount(data);
        if (error != E_SUCCESS || nVaccines != 2 || nLots != 5 || nPeople != 2) {            
            passed = false;
            fail_all = true;
        }
    }
    
    
    /////////////////////////////
    /////  PR1 EX3 TEST 1  //////
    /////////////////////////////
    failed = false;
    start_test(test_section, "PR1_EX3_1", "Request a valid vaccine");
    if (fail_all) {
        failed = true;
    } else {        
        csv_initEntry(&entry);
        csv_initEntry(&refEntry);
        csv_parseEntry(&refEntry, "PFIZER;2;21", "VACCINE");
        error = api_getVaccine(data, "PFIZER", &entry);
        if (error != E_SUCCESS || !csv_equalsEntry(entry, refEntry)) {
            failed = true;
            passed = false;            
        }
        csv_freeEntry(&entry);
        csv_freeEntry(&refEntry);
    }
    end_test(test_section, "PR1_EX3_1", !failed);
    
    /////////////////////////////
    /////  PR1 EX3 TEST 2  //////
    /////////////////////////////
    failed = false;
    start_test(test_section, "PR1_EX3_2", "Request a missing vaccine");
    if (fail_all) {
        failed = true;
    } else {
        csv_initEntry(&entry);
        error = api_getVaccine(data, "NO_VACC", &entry);
        if (error != E_VACCINE_NOT_FOUND) {
            failed = true;
            passed = false;            
        }
        csv_freeEntry(&entry);        
    }
    end_test(test_section, "PR1_EX3_2", !failed);
           
    /////////////////////////////
    /////  PR1 EX3 TEST 3  //////
    /////////////////////////////
    failed = false;
    start_test(test_section, "PR1_EX3_3", "Request a valid vaccine lot");
    if (fail_all) {
        failed = true;
    } else {        
        csv_initEntry(&entry);
        csv_initEntry(&refEntry);
        csv_parseEntry(&refEntry, "01/01/2022;13:45;08001;PFIZER;2;21;350", "VACCINE_LOT");
        dateTime_parse(&timestamp, "01/01/2022", "13:45");
        error = api_getVaccineLot(data, "08001", "PFIZER", timestamp, &entry);
        if (error != E_SUCCESS || !csv_equalsEntry(entry, refEntry)) {
            failed = true;
            passed = false;            
        }
        csv_freeEntry(&entry);
        csv_freeEntry(&refEntry);
    }
    end_test(test_section, "PR1_EX3_3", !failed);
    
    /////////////////////////////
    /////  PR1 EX3 TEST 4  //////
    /////////////////////////////
    failed = false;
    start_test(test_section, "PR1_EX3_4", "Request a missing vaccine lot");
    if (fail_all) {
        failed = true;
    } else {
        csv_initEntry(&entry);
        dateTime_parse(&timestamp, "01/01/2022", "13:45");
        error = api_getVaccineLot(data, "10001", "NO_VACC", timestamp, &entry);
        if (error != E_LOT_NOT_FOUND) {
            failed = true;
            passed = false;            
        }        
        csv_freeEntry(&entry);
    }
    end_test(test_section, "PR1_EX3_4", !failed);
    
    /////////////////////////////
    /////  PR1 EX3 TEST 5  //////
    /////////////////////////////
    failed = false;
    start_test(test_section, "PR1_EX3_5", "Get registered vaccines");
    if (fail_all) {
        failed = true;
    } else {
        csv_init(&report);
        csv_init(&refReport);
        csv_addStrEntry(&refReport, "MODERNA;1;0", "VACCINE");
        csv_addStrEntry(&refReport, "PFIZER;2;21", "VACCINE");        
        error = api_getVaccines(data, &report);
        if (error != E_SUCCESS || !csv_equals(report, refReport)) {
            failed = true;
            passed = false;            
        }  
        csv_free(&report);
        csv_free(&refReport);
    }
    end_test(test_section, "PR1_EX3_5", !failed);
    
    /////////////////////////////
    /////  PR1 EX3 TEST 6  //////
    /////////////////////////////
    failed = false;
    start_test(test_section, "PR1_EX3_6", "Get registered vaccine lots");
    if (fail_all) {
        failed = true;
    } else {
        csv_init(&report);
        csv_init(&refReport);
        csv_addStrEntry(&refReport, "01/01/2022;13:45;08001;PFIZER;2;21;350", "VACCINE_LOT");
        csv_addStrEntry(&refReport, "01/01/2022;13:45;08500;PFIZER;2;21;300", "VACCINE_LOT");
        csv_addStrEntry(&refReport, "02/01/2022;18:00;08500;MODERNA;1;0;100", "VACCINE_LOT");
        csv_addStrEntry(&refReport, "02/01/2022;15:45;08001;MODERNA;1;0;100", "VACCINE_LOT");
        csv_addStrEntry(&refReport, "03/01/2022;13:45;08500;PFIZER;2;21;70", "VACCINE_LOT");               
        error = api_getVaccineLots(data, &report);
        if (error != E_SUCCESS || !csv_equals(report, refReport)) {
            failed = true;
            passed = false;            
        }  
        csv_free(&report);
        csv_free(&refReport);
    }
    end_test(test_section, "PR1_EX3_6", !failed);
    
    // Release all data
    api_freeData(&data);
    
    return passed;
}
