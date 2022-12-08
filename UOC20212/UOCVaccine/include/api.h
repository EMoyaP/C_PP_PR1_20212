#ifndef __UOCVACCINE_API__H
#define __UOCVACCINE_API__H
#include <stdbool.h>
#include "error.h"
#include "csv.h"

#include "person.h"
#include "vaccine.h"


// Type that stores all the application data
typedef struct _ApiData {
    ////////////////////////////////
    // PR1 EX2a
    ////////////////////////////////
	tPopulation population;
	tVaccineList vaccines;
	tVaccineLotData vaccineLots;   
} tApiData;

// Get the API version information
const char* api_version();

// Load data from a CSV file. If reset is true, remove previous data
tApiError api_loadData(tApiData* data, const char* filename, bool reset);

// Add a new entry
tApiError api_addDataEntry(tApiData* data, tCSVEntry entry);

// Free all used memory
tApiError api_freeData(tApiData* data);

// Initialize the data structure
tApiError api_initData(tApiData* data);

// Add a new vaccines lot
tApiError api_addVaccineLot(tApiData* data, tCSVEntry entry);

// Get the number of persons registered on the application
int api_populationCount(tApiData data);

// Get the number of vaccines registered on the application
int api_vaccineCount(tApiData data);

// Get the number of vaccine lots registered on the application
int api_vaccineLotsCount(tApiData data);

// Get vaccine data
tApiError api_getVaccine(tApiData data, const char *name, tCSVEntry *entry);

// Get vaccine lot data
tApiError api_getVaccineLot(tApiData data, const char* cp, const char* vaccine, tDateTime timestamp, tCSVEntry *entry);

// Get registered vaccines
tApiError api_getVaccines(tApiData data, tCSVData *vaccines);

// Get vaccine lots
tApiError api_getVaccineLots(tApiData data, tCSVData *lots);

#endif // __UOCVACCINE_API__H