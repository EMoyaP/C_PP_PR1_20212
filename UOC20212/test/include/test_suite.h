#ifndef __TEST_SUITE__H
#define __TEST_SUITE__H
#include <stdbool.h>
#include <stdbool.h>
#include <stdio.h>

// #define PRINT_TEST_PROGRESS

// Size of the buffer used to read files data
#define BUFFER_SIZE 2048

// Arguments for a test program
typedef struct _AppArguments {
    char* app_name;
    char* out_file;    
    char* in_file;
    char* readme_file;
    char* progress_file;
    bool wait_on_exit;
} tAppArguments;

// Status of a test
typedef enum {
    TEST_RUNNING,
    TEST_NOT_IMPLEMENTED,
    TEST_PASSED,
    TEST_FAILED
} tTestResult;

// A single test
typedef struct {
    // Code of the test
    char* code;
    // Description of the test
    char* description;
    // Result of the test
    tTestResult result;
} tTest;

// Grup of tests
typedef struct {
    // Code of the test section
    char* code;
    // Title of the section
    char* title;
    // Number of tests
    int numTests;    
    // Array of tests
    tTest* tests;
    // Progress file
    char* progress_file;
} tTestSection;

// Test suit learner data
typedef struct {
    // Email
    char* email;
    // Username
    char* username;
    // First name
    char* first_name;   
    // Last name
    char* last_name;
    // Environment
    char* environment;
    // Whether the learner data file exists or not
    bool file_exists;
} tLearnerData;

// Test suit with multiple sections
typedef struct {
    // Learner data
    tLearnerData learner;
    // Number of sections
    int numSections;    
    // Array of sections
    tTestSection* sections;
    // Progress file
    char* progress_file;
} tTestSuite;


// Wait user key
void waitKey(tAppArguments parameters);

// Display help text
void help(const char* app_name);

// Parse application arguments
bool parseArguments(tAppArguments* arguments, int argc, char **argv);


// Initialize a test Suite
void testSuite_init(tTestSuite* object);

// Set output progress file
void testSuite_set_progress_file(tTestSuite* object, char* progress_file);

// Load learner data
bool testSuite_load_learner(tTestSuite* object, const char* file);

// Remove a test Suite
void testSuite_free(tTestSuite* object);

// Add a test Section
void testSuite_addSection(tTestSuite* object, const char* code, const char* title);

// Add a test
void testSuite_addTest(tTestSuite* object, const char* section_code, const char* code, const char* description, tTestResult result);

// Update a test result
void testSuite_updateTest(tTestSuite* object, const char* section_code, const char* test_code, tTestResult result);

// Get a pointer to a section
tTestSection* testSuite_getSection(tTestSuite* object, const char* section_code);

// Get a pointer to a test
tTest* testSuite_getTest(tTestSuite* object, const char* section_code, const char* test_code);

// Get test statistics
void testSuite_getStats(tTestSuite* object, int* total, int* passed, int* failed, int* not_implemented);

// Print test suite
void testSuite_print(tTestSuite* object);

// Export a test suite
void testSuite_export(tTestSuite* object, const char* output);



// Initialize a test Section
void testSection_init(tTestSection* object, const char* code, const char* title);

// Remove a test Section
void testSection_free(tTestSection* object);

// Add a test to the Section
void testSection_addTest(tTestSection* object, const char* code, const char* description, tTestResult result);

// Update a test result
void testSection_updateTest(tTestSection* object, const char* test_code, tTestResult result);

// Get a pointer to a test
tTest* testSection_getTest(tTestSection* object, const char* test_code);

// Get test statistics
void testSection_getStats(tTestSection* object, int* total, int* passed, int* failed, int* not_implemented);

// Print test section
void testSection_print(tTestSection* object);

// Export a test section
void testSection_export(tTestSection* object, FILE* fout);



// Initialize a test
void test_init(tTest* object, const char* code, const char* description, tTestResult result);

// Remove a test
void test_free(tTest* object);

// Update a test result
void test_updateTest(tTest* object, tTestResult result);

// Print test
void test_print(tTest* object);

// Export a test
void test_export(tTest* object, FILE* fout);


// Start a test
void start_test(tTestSection* section, const char* code, const char* description);

// Finish a test
void end_test(tTestSection* section, const char* code, bool passed);

// Set output progress file
void _save_progress(tTestSection* section, const char* test_code, const char* test_result);


#endif // __TEST_SUITE__H