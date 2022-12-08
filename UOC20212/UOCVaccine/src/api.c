#include <stdio.h>
#include <assert.h>
#include "csv.h"
#include "api.h"

#include <string.h>
#include "person.h"
#include "vaccine.h"


#define FILE_READ_BUFFER_SIZE 2048

// Get the API version information
const char* api_version() {
    return "UOC PP 20212";
}

// Load data from a CSV file. If reset is true, remove previous data
tApiError api_loadData(tApiData* data, const char* filename, bool reset) {
    tApiError error;
    FILE *fin;    
    char buffer[FILE_READ_BUFFER_SIZE];
    tCSVEntry entry;
    
    // Check input data
    assert( data != NULL );
    assert(filename != NULL);
    
    // Reset current data    
    if (reset) {
        // Remove previous information
        error = api_freeData(data);
        if (error != E_SUCCESS) {
            return error;
        }
        
        // Initialize the data
        error = api_initData(data);
        if (error != E_SUCCESS) {
            return error;
        }
    }

    // Open the input file
    fin = fopen(filename, "r");
    if (fin == NULL) {
        return E_FILE_NOT_FOUND;
    }
    
    // Read file line by line
    while (fgets(buffer, FILE_READ_BUFFER_SIZE, fin)) {
        // Remove new line character     
        buffer[strcspn(buffer, "\n\r")] = '\0';
        
        csv_initEntry(&entry);
        csv_parseEntry(&entry, buffer, NULL);
        // Add this new entry to the api Data
        error = api_addDataEntry(data, entry);
        if (error != E_SUCCESS) {
            return error;
        }
        csv_freeEntry(&entry);
    }
    
    fclose(fin);
    
    return E_SUCCESS;
}

// Initialize the data structure
tApiError api_initData(tApiData* data) {            
    //////////////////////////////////
    // Ex PR1 2b
    /////////////////////////////////
    assert(data !=NULL);
	
	population_init(&data->population);
	vaccineList_init(&data->vaccines);
	vaccineLotData_init(&data->vaccineLots);
	
	return E_SUCCESS;	

}

// Add a new vaccines lot
tApiError api_addVaccineLot(tApiData* data, tCSVEntry entry) {
    //////////////////////////////////
    // Ex PR1 2c
    /////////////////////////////////
	tVaccineLot vaccineLot;
	tVaccine vaccine;
    
	//Pre-condition
	assert(data != NULL);
    
	//Check invalid entry type
	if(strcmp(csv_getType(&entry), "VACCINE_LOT") != 0){
		return E_INVALID_ENTRY_TYPE;
	}
    
	//Check invalid entry format
	if(strcmp(csv_getType(&entry), "VACCINE_LOT") == 0){
		if(csv_numFields(entry) != 7){
			return E_INVALID_ENTRY_FORMAT;
		}
  		//Gets vaccine lot from the csv entry  
		vaccineLot_parse(&vaccine, &vaccineLot, entry);

		//If the vaccine does not exist in the list of vaccines, it will be necessary to add it.
		if(vaccineList_find(data->vaccines, vaccine.name) == NULL){
			vaccineList_insert(&data->vaccines, vaccine);
		}
		vaccineLot.vaccine = vaccineList_find(data->vaccines, vaccine.name);
		vaccineLotData_add(&data->vaccineLots, vaccineLot);
		vaccine_free(&vaccine);
		vaccineLot_free(&vaccineLot);
		return E_SUCCESS;  
	}


}

// Get the number of persons registered on the application
int api_populationCount(tApiData data) {
    //////////////////////////////////
    // Ex PR1 2d
    /////////////////////////////////
    int pLen;
	pLen = population_len(data.population);
	return pLen;

}

// Get the number of vaccines registered on the application
int api_vaccineCount(tApiData data) {
    //////////////////////////////////
    // Ex PR1 2d
    /////////////////////////////////
    int vLen;
	vLen = vaccineList_len(data.vaccines);
	return vLen;

}

// Get the number of vaccine lots registered on the application
int api_vaccineLotsCount(tApiData data) {
    //////////////////////////////////
    // Ex PR1 2d
    /////////////////////////////////
    int lLen;
	lLen = vaccineLotData_len(data.vaccineLots);
	return lLen;

}


// Free all used memory
tApiError api_freeData(tApiData* data) {
    //////////////////////////////////
    // Ex PR1 2e
    /////////////////////////////////
	vaccineLotData_free(&data->vaccineLots);
	population_free(&data->population);
	vaccineList_free(&data->vaccines);
  
    return E_SUCCESS;

}


// Add a new entry
tApiError api_addDataEntry(tApiData* data, tCSVEntry entry) { 
    //////////////////////////////////
    // Ex PR1 2f
    /////////////////////////////////
	tPerson person;

	//Pre-condition
	assert(data != NULL);

	//Check the entry type
	if ((strcmp(csv_getType(&entry), "PERSON") != 0) && (strcmp(csv_getType(&entry), "VACCINE_LOT") != 0)){
		return E_INVALID_ENTRY_TYPE;
	}

	//Check the entry format
	if(csv_numFields(entry) != 7) {
		return E_INVALID_ENTRY_FORMAT;
	}

	//Add the vaccine lot to the api
	if (strcmp(csv_getType(&entry), "VACCINE_LOT") == 0) {
		return api_addVaccineLot(data, entry);
	}

	//Add the person to the api
	if (strcmp(csv_getType(&entry), "PERSON") == 0) {
		//Parse person from CSVEntry
		person_init(&person);
		person_parse(&person, entry);

		//Check duplicated person
		if(population_find(data->population, person.document) !=-1){
			return E_DUPLICATED_PERSON;
		}
		//Add person to population
		population_add(&data->population, person);

		return E_SUCCESS;
	}


}
// Get vaccine data
tApiError api_getVaccine(tApiData data, const char *name, tCSVEntry *entry) {
    //////////////////////////////////
    // Ex PR1 3a
    /////////////////////////////////
	char buffer[FILE_READ_BUFFER_SIZE]; 
    tVaccine *vaccine;
   
	//Check if there is a vaccine registered under the name provided
	if (vaccineList_find(data.vaccines, name)==NULL){
		return E_VACCINE_NOT_FOUND;
	} else{
		vaccine = vaccineList_find(data.vaccines, name);
 		
		//Parse vaccine from CSVEntry     
		csv_initEntry(entry);
		sprintf(buffer, "%s;%d;%d", vaccine->name, vaccine->required, vaccine->days);
		csv_parseEntry(entry, buffer, "VACCINE");
    
		return E_SUCCESS;  
	}

}

// Get vaccine lot data
tApiError api_getVaccineLot(tApiData data, const char* cp, const char* vaccine, tDateTime timestamp, tCSVEntry *entry) {
    //////////////////////////////////
    // Ex PR1 3b
    /////////////////////////////////
	int i = 0;
	char buffer [FILE_READ_BUFFER_SIZE];
	tVaccineLot lot;
  
		//Check if the vaccine_lot exists
		i = vaccineLotData_find (data.vaccineLots, cp, vaccine, timestamp);
		if (i == -1){
			return E_LOT_NOT_FOUND;
    
		} else {
			lot = (data.vaccineLots.elems[i]);

			csv_initEntry(entry);
			sprintf (buffer, "%02d/%02d/%04d;%02d:%02d;%s;%s;%d;%d;%d", 
						lot.timestamp.date.day,
						lot.timestamp.date.month,
						lot.timestamp.date.year,
						lot.timestamp.time.hour,
						lot.timestamp.time.minutes, 
						lot.cp, 
						lot.vaccine->name,
						lot.vaccine->required,
						lot.vaccine->days,
						lot.doses );
 			csv_parseEntry(entry, buffer, "VACCINE_LOT");
			return E_SUCCESS;

		}


}	


// Get registered vaccines
tApiError api_getVaccines(tApiData data, tCSVData *vaccines) {
    //////////////////////////////////
    // Ex PR1 3c
    /////////////////////////////////
	int i = 0;
	tCSVEntry entry;
	char buffer[FILE_READ_BUFFER_SIZE];
	tVaccineNode *vNode;
    tVaccineNode *pVaccine;
    
	//Init entry
	csv_initEntry(&entry);  

	// Traverse all data
	vNode = data.vaccines.first;
 
    while(i < data.vaccines.count && vNode != NULL) {
    
    pVaccine = vNode;
    sprintf(buffer, "%s;%d;%d", pVaccine->vaccine.name,
								pVaccine->vaccine.required,
								pVaccine->vaccine.days);
    
	csv_addStrEntry(vaccines, buffer, "VACCINE");
	vNode = vNode->next;
    csv_freeEntry(&entry);
    
    i++;
    }
  
	return E_SUCCESS;

}


// Get vaccine lots
tApiError api_getVaccineLots(tApiData data, tCSVData *lots) {
    //////////////////////////////////
    // Ex PR1 3d
    /////////////////////////////////
	char buffer[FILE_READ_BUFFER_SIZE];
	int i;
	tCSVEntry entry;

	//Init entry  
	csv_initEntry(&entry);

    // Traverse all data
	for (i=0; i < data.vaccineLots.count; i++){
		sprintf (buffer, "%02d/%02d/%04d;%02d:%02d;%s;%s;%d;%d;%d", data.vaccineLots.elems[i].timestamp.date.day,
																	data.vaccineLots.elems[i].timestamp.date.month,
																	data.vaccineLots.elems[i].timestamp.date.year, 
																	data.vaccineLots.elems[i].timestamp.time.hour,
																	data.vaccineLots.elems[i].timestamp.time.minutes,
																	data.vaccineLots.elems[i].cp,
																	data.vaccineLots.elems[i].vaccine->name,
																	data.vaccineLots.elems[i].vaccine->required,
																	data.vaccineLots.elems[i].vaccine->days,
																	data.vaccineLots.elems[i].doses);  
		csv_addStrEntry(lots, buffer, "VACCINE_LOT");   
  
	}
  
	return E_SUCCESS;

}
