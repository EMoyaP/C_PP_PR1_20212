#ifndef __TEST_PR1_H__
#define __TEST_PR1_H__

#include <stdbool.h>
#include "test_suite.h"

// Run all tests for PR1
bool run_pr1(tTestSuite* test_suite, const char* input);

// Run tests for PR1 exercice 1
bool run_pr1_ex1(tTestSection* test_section, const char* input);

// Run tests for PR1 exercice 2
bool run_pr1_ex2(tTestSection* test_section, const char* input);

// Run tests for PR1 exercice 3
bool run_pr1_ex3(tTestSection* test_section, const char* input);


#endif // __TEST_PR1_H__