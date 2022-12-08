#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "vaccine.h"

// Initialize vaccine structure
void vaccine_init(tVaccine* vaccine, const char* name, int required, int days) {
    assert(vaccine != NULL);
    assert(name != NULL);
    
    // Allocate memory for the name
    vaccine->name = (char*) malloc(strlen(name) + 1);
    assert(vaccine->name != NULL);
    
    // Set the data
    strcpy(vaccine->name, name);
    vaccine->required = required;
    vaccine->days = days;
}

// Release vaccine data
void vaccine_free(tVaccine* vaccine) {
    assert(vaccine != NULL);
    
    // Release used memory
    if (vaccine->name != NULL) {
        free(vaccine->name);
        vaccine->name = NULL;
    }
}

// Copy the data of a vaccine from the source to destination
void vaccine_cpy(tVaccine* destination, tVaccine source) {
    assert(destination != NULL);
    
    // Set the data
    vaccine_init(destination, source.name, source.required, source.days);
}


// Release vaccine lot data
void vaccineLot_init(tVaccineLot* lot, tVaccine* vaccine, const char* cp, tDateTime timestamp, int doses) {
    assert(lot != NULL);
    assert(cp != NULL);
    
    // Allocate memory for the cp
    lot->cp = (char*) malloc(strlen(cp) + 1);
    assert(lot->cp != NULL);
    
    // Set the data
    strcpy(lot->cp, cp);
    lot->vaccine = vaccine;
    lot->timestamp = timestamp;
    lot->doses = doses;
}

// Release vaccine lot data
void vaccineLot_free(tVaccineLot* lot) {
    assert(lot != NULL);
    
    // Release used memory
    if (lot->cp != NULL) {
        free(lot->cp);
        lot->cp = NULL;
    }
}

// Copy the data of a vaccine lot from the source to destination
void vaccineLot_cpy(tVaccineLot* destination, tVaccineLot source) {
    assert(destination != NULL);
    
    // Set the data
    vaccineLot_init(destination, source.vaccine, source.cp, source.timestamp, source.doses);
}

// Parse input from CSVEntry
void vaccineLot_parse(tVaccine* vaccine, tVaccineLot* lot, tCSVEntry entry) {
    char date[11];
    char time[6];
    char buffer[512];
    tDateTime timestamp;
    int doses;
    int required;
    int days;
    
    // Check input data
    assert(vaccine != NULL);
    assert(lot != NULL);
    assert(csv_numFields(entry) == 7);
        
    // Get Lot data
    csv_getAsString(entry, 0, date, 11);    
    csv_getAsString(entry, 1, time, 6);
    dateTime_parse(&timestamp, date, time);
    csv_getAsString(entry, 2, buffer, 511);
    doses = csv_getAsInteger(entry, 6);
    
    // Initialize the lot structure
    vaccineLot_init(lot, NULL, buffer, timestamp, doses);
                
    // Get vaccine data    
    csv_getAsString(entry, 3, buffer, 511);
    required = csv_getAsInteger(entry, 4);
    days = csv_getAsInteger(entry, 5);
    
    // Initialize the vaccine data
    vaccine_init(vaccine, buffer, required, days);
}


// Initialize the vaccine's list
void vaccineList_init(tVaccineList* list) {
    assert(list != NULL);
    
    list->first = NULL;
    list->count = 0;
}

// Remove all elements
void vaccineList_free(tVaccineList* list) {
    tVaccineNode *pNode = NULL;
    tVaccineNode *pAux = NULL;
    
    assert(list != NULL);
    
    pNode = list->first;
    while(pNode != NULL) {
        // Store the position of the current node
        pAux = pNode;
        
        // Move to the next node in the list
        pNode = pNode->next;
        
        // Remove previous node
        vaccine_free(&(pAux->vaccine));
        free(pAux);
    }
    
    // Initialize to an empty list
    vaccineList_init(list);
}

// Get the number of vaccines
int vaccineList_len(tVaccineList list) {
    return list.count;
}

// Find a vaccine in the list of vaccines
tVaccine* vaccineList_find(tVaccineList list, const char* name) {
    tVaccineNode *pNode = NULL;
    tVaccineNode *pVaccine = NULL;
        
    // Point the first element
    pNode = list.first;
    
    while(pNode != NULL && pVaccine == NULL) {
        // Compare current node with given name
        if (strcmp(pNode->vaccine.name, name) == 0) {
            pVaccine = pNode;
        }
        pNode = pNode->next;
    }
    
    return &(pVaccine->vaccine);
}

// Add a new vaccine
void vaccineList_insert(tVaccineList* list, tVaccine vaccine) {
    tVaccineNode *pNode = NULL;
    tVaccineNode *pPrev = NULL;
    
    assert(list != NULL);
    
    // If the list is empty add the node as first position
    if (list->count == 0) {
        list->first = (tVaccineNode*) malloc(sizeof(tVaccineNode));
        list->first->next = NULL;
        vaccine_cpy(&(list->first->vaccine), vaccine);
    } else {    
        // Point the first element
        pNode = list->first;
        pPrev = pNode;
                
        // Advance in the list up to the insertion point or the end of the list
        while(pNode != NULL && strcmp(pNode->vaccine.name, vaccine.name) < 0) {            
            pPrev = pNode;
            pNode = pNode->next;
        }
                
        if (pNode == pPrev) {
            // Insert as first element
            list->first = (tVaccineNode*) malloc(sizeof(tVaccineNode));
            list->first->next = pNode;
            vaccine_cpy(&(list->first->vaccine), vaccine);            
        } else {
            // Insert after pPrev
            pPrev->next = (tVaccineNode*) malloc(sizeof(tVaccineNode));        
            vaccine_cpy(&(pPrev->next->vaccine), vaccine);
            pPrev->next->next = pNode;            
        }
    }
    list->count ++;
}

// Remove a vaccine
void vaccineList_del(tVaccineList* list, const char* vaccine) {
    tVaccineNode *pNode = NULL;
    tVaccineNode *pPrev = NULL;
    
    assert(list != NULL);
    
    // Check if the list has elements
    if (list->count > 0) {
        pNode = list->first;
        
        // Check if we are removing the first position
        if (strcmp(pNode->vaccine.name, vaccine) == 0) {
            list->first = pNode->next;
        } else {    
            // Search in the list
            pPrev = pNode;
            while (pNode != NULL) {                
                if (strcmp(pNode->vaccine.name, vaccine) == 0) {
                    // Link previous and next nodes
                    pPrev->next = pNode->next;
                    // Remove node
                    vaccine_free(&(pNode->vaccine));
                    free(pNode);
                    list->count ++;                    
                    pNode = NULL;
                } else {
                    pPrev = pNode;
                    pNode = pNode->next;
                }                
            }            
        }            
    }
}


// Initialize the vaccine lots data
void vaccineLotData_init(tVaccineLotData* data) {
    assert(data != NULL);
    
    // Set the initial number of elements to zero.
    data->count = 0;    
    data->elems = NULL;
}

// Remove all elements
void vaccineLotData_free(tVaccineLotData* data) {
    int i;
    if (data->elems != NULL) {
        for(i=0; i < data->count; i++) {
            vaccineLot_free(&(data->elems[i]));
        }
        free(data->elems);
    }
    vaccineLotData_init(data);    
}

// Get the number of lots
int vaccineLotData_len(tVaccineLotData data) {
    // Return the number of lots
    return data.count;
}

// Add a new vaccine lot
void vaccineLotData_add(tVaccineLotData* data, tVaccineLot lot) {
    int idx = -1;

    // Check input data (Pre-conditions)
    assert(data != NULL);    
    
    // Check if an entry with this data already exists
    idx = vaccineLotData_find(*data, lot.cp, lot.vaccine->name, lot.timestamp);
    
    // If it does not exist, create a new entry, otherwise add the number of doses
    if (idx < 0) {    
        if (data->elems == NULL) {
            data->elems = (tVaccineLot*) malloc(sizeof(tVaccineLot));
        } else {
            data->elems = (tVaccineLot*) realloc(data->elems, (data->count + 1) * sizeof(tVaccineLot));
        }
        assert(data->elems != NULL);        
        vaccineLot_cpy(&(data->elems[data->count]), lot);
        data->count ++;        
    } else {
        data->elems[idx].doses += lot.doses;
    }    
}

// Remove vaccines from a lot
void vaccineLotData_del(tVaccineLotData* data, const char* cp, const char* vaccine, tDateTime timestamp, int doses) {
    int idx;
    int i;
    
    assert(cp != NULL);
    assert(vaccine != NULL);   
    
    // Check if an entry with this data already exists
    idx = vaccineLotData_find(*data, cp, vaccine, timestamp);
    
    if (idx >= 0) {
        // Reduce the number of doses
        data->elems[idx].doses -= doses;
        // Shift elements to remove selected
        if (data->elems[idx].doses <= 0) {
            for(i = idx; i < data->count-1; i++) {
                // Remove the data from this position
                vaccineLot_free(&(data->elems[i]));
                // Copy element on position i+1 to position i
                vaccineLot_cpy(&(data->elems[i]), data->elems[i+1]);
            }            
            // Update the number of elements
            data->count--;     
            // Free last position
            vaccineLot_free(&(data->elems[data->count]));
        }        
        if (data->count > 0) {
            data->elems = (tVaccineLot*) realloc(data->elems, data->count * sizeof(tVaccineLot));
            assert(data->elems != NULL);
        } else {
            free(data->elems);
            data->elems = NULL;
        }     
    }
}

// Return the position of a vaccine lot entry with provided information. -1 if it does not exist
int vaccineLotData_find(tVaccineLotData data, const char* cp, const char* vaccine, tDateTime timestamp) {
    int i;
    
    assert(cp != NULL);
    assert(vaccine != NULL);    
    
    for(i = 0; i < data.count; i++) {
        if(strcmp(data.elems[i].cp, cp) == 0 && strcmp(data.elems[i].vaccine->name, vaccine) == 0 && dateTime_equals(data.elems[i].timestamp, timestamp)) {
            return i;
        }
    }
    
    return -1;
}

