#include <stdio.h>
#include <stdlib.h>
#include "test_suite.h"
#include "test.h"

int main(int argc, char **argv)
{
	tAppArguments parameters;
    tTestSuite testSuite;
    
    // Parse input arguments
    if (!parseArguments(&parameters, argc, argv)) {
        printf("ERROR: Invalid input arguments");
        // Wait user to press a key to ensure error is shown
        waitKey(parameters);
        // Exit with error
        exit(EXIT_FAILURE);
    }
    
    // Initialize the test suite
    testSuite_init(&testSuite);
    
    // Set the progress file
    testSuite_set_progress_file(&testSuite, parameters.progress_file);
    
    // Run all tests
    testSuite_run(&testSuite, parameters.in_file, parameters.readme_file);
    
    // Print test results
    testSuite_print(&testSuite);
    
    // Store test results
    if (parameters.out_file != NULL) {
        testSuite_export(&testSuite, parameters.out_file);
        printf("Restults stored in %s\n", parameters.out_file);
    }
    if (parameters.progress_file != NULL) {        
        printf("Progress stored in %s\n", parameters.progress_file);
    }
    
    // Remove test suite data
    testSuite_free(&testSuite);    
    
    // Wait user to press a key to ensure results are shown
    waitKey(parameters);
    
	exit(EXIT_SUCCESS);
}
