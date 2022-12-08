#ifndef __PERSON_H__
#define __PERSON_H__
#include "csv.h"
#include "date.h"

typedef struct _tPerson {
    char* document;
    char* name;
    char* surname;
    char* cp;
    char* email;
    char* address;
    tDate birthday;
} tPerson;

typedef struct _tPopulation {
    tPerson* elems;
    int count;
} tPopulation;

// Initialize the population data
void population_init(tPopulation* data);

// Initialize a person structure
void person_init(tPerson* data);

// Remove the data from a person
void person_free(tPerson* data);

// Remove the data from all persons
void population_free(tPopulation* data);

// Parse input from CSVEntry
void person_parse(tPerson* data, tCSVEntry entry);

// Add a new person
void population_add(tPopulation* data, tPerson person);

// Remove a person
void population_del(tPopulation* data, const char *document);

// Return the position of a person with provided document. -1 if it does not exist
int population_find(tPopulation data, const char* document);

// Print the person data
void population_print(tPopulation data);

// Copy the data from the source to destination
void person_cpy(tPerson* destination, tPerson source);

// Return population lenght
int population_len(tPopulation data);

#endif
