#ifndef __VACCINE__H
#define __VACCINE__H

#include "csv.h"
#include "date.h"

// Vaccine data
typedef struct _tVaccine {
    char *name;
    int required;
    int days;
} tVaccine;

// Node of a list of vaccines
typedef struct _tVaccineNode {    
    tVaccine vaccine;
    struct _tVaccineNode *next;
} tVaccineNode;

// List of vaccines
typedef struct _tVaccineList {    
    tVaccineNode* first;
    int count;
} tVaccineList;

// Vaccine lot data
typedef struct _tVaccineLot {
    tVaccine* vaccine;
    tDateTime timestamp;
    char *cp;
    int doses;
} tVaccineLot;

// Table of lots
typedef struct _tVaccineLotData {    
    tVaccineLot* elems;
    int count;
} tVaccineLotData;



// Initialize vaccine structure
void vaccine_init(tVaccine* vaccine, const char* name, int required, int days);

// Release vaccine data
void vaccine_free(tVaccine* vaccine);

// Copy the data of a vaccine from the source to destination
void vaccine_cpy(tVaccine* destination, tVaccine source);



// Release vaccine lot data
void vaccineLot_init(tVaccineLot* lot, tVaccine* vaccine, const char* cp, tDateTime timestamp, int doses);

// Release vaccine lot data
void vaccineLot_free(tVaccineLot* lot);

// Copy the data of a vaccine lot from the source to destination
void vaccineLot_cpy(tVaccineLot* destination, tVaccineLot source);

// Parse input from CSVEntry
void vaccineLot_parse(tVaccine* vaccine, tVaccineLot* lot, tCSVEntry entry);



// Initialize the vaccine's list
void vaccineList_init(tVaccineList* list);

// Remove all elements
void vaccineList_free(tVaccineList* list);

// Get the number of vaccines
int vaccineList_len(tVaccineList list);

// Find a vaccine in the list of vaccines
tVaccine* vaccineList_find(tVaccineList list, const char* name);

// Add a new vaccine
void vaccineList_insert(tVaccineList* list, tVaccine vaccine);

// Remove a vaccine
void vaccineList_del(tVaccineList* list, const char* vaccine);



// Initialize the vaccine lots data
void vaccineLotData_init(tVaccineLotData* data);

// Remove all elements
void vaccineLotData_free(tVaccineLotData* data);

// Get the number of lots
int vaccineLotData_len(tVaccineLotData data);

// Add a new vaccine lot
void vaccineLotData_add(tVaccineLotData* data, tVaccineLot lot);

// Remove vaccines from a lot
void vaccineLotData_del(tVaccineLotData* data, const char* cp, const char* vaccine, tDateTime timestamp, int doses);

// Return the position of a vaccine lot entry with provided information. -1 if it does not exist
int vaccineLotData_find(tVaccineLotData data, const char* cp, const char* vaccine, tDateTime timestamp);


#endif // __VACCINE__H
