#ifndef __TEST__H
#define __TEST__H
#include "test_suite.h"

// Write data to file
void save_data(const char* filename, const char* data);

// Run all available tests
void testSuite_run(tTestSuite* test_suite, const char* input, const char* readme);

// Check if README.txt is available and have the correct information
//void readReadme(tTestSuite* test_suite);

#endif // __TEST__H