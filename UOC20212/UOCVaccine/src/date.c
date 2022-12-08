#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "date.h"

// Parse a tDateTime from string information
void dateTime_parse(tDateTime* dateTime, const char* date, const char* time) {
    // Check output data
    assert(dateTime != NULL);
    
    // Check input date
    assert(date != NULL);
    assert(strlen(date) == 10);
    
    // Check input time
    assert(time != NULL);
    assert(strlen(time) == 5);
    
    // Parse the input date
    sscanf(date, "%d/%d/%d", &(dateTime->date.day), &(dateTime->date.month), &(dateTime->date.year));
    
    // Parse the input time
    sscanf(time, "%d:%d", &(dateTime->time.hour), &(dateTime->time.minutes));
}

// Compare two tDateTime structures and return -1 if dateTime1<dateTime2, 0 if equals and 1 if dateTime1>dateTime2.
int dateTime_cmp(tDateTime dateTime1, tDateTime dateTime2) {    
    // Checkl year
    if (dateTime1.date.year < dateTime2.date.year) {
        return -1;
    }
    if (dateTime1.date.year > dateTime2.date.year) {
        return 1;
    }
    // Check month
    if (dateTime1.date.month < dateTime2.date.month) {
        return -1;
    }
    if (dateTime1.date.month > dateTime2.date.month) {
        return 1;
    }  
    // Check day
    if (dateTime1.date.day < dateTime2.date.day) {
        return -1;
    }
    if (dateTime1.date.day > dateTime2.date.day) {
        return 1;
    }
    // Check hour
    if (dateTime1.time.hour < dateTime2.time.hour) {
        return -1;
    }
    if (dateTime1.time.hour > dateTime2.time.hour) {
        return 1;
    }
    // Check minutes
    if (dateTime1.time.minutes < dateTime2.time.minutes) {
        return -1;
    }
    if (dateTime1.time.minutes > dateTime2.time.minutes) {
        return 1;
    }
    
    return 0;
}

// Compare two tDateTime structures and return true if they contain the same value or false otherwise.
bool dateTime_equals(tDateTime dateTime1, tDateTime dateTime2) {
    return dateTime_cmp(dateTime1, dateTime2) == 0;
}
