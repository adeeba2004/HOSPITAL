#pragma once
#ifndef CLINIC_FUNCTIONS_H
#define CLINIC_FUNCTIONS_H
#include <string>

// Function to handle department menu actions
void actionMenu(Queue& queue, const std::string& departmentName, const std::string& transactionLog, const std::string& csvFile);

// Add a new regular patient to the queue
void addNewPatient(Queue& queue, const std::string& departmentName, const std::string& transactionLog, const std::string& csvFile);

// Add a new critical patient to the queue
void addCriticalPatient(Queue& queue, const std::string& departmentName, const std::string& transactionLog, const std::string& csvFile);

// Take the next patient from the queue for an operation
void takeOutPatientForOperation(Queue& queue, const std::string& departmentName, const std::string& transactionLog);

// Cancel a patient from the queue based on SSN
void cancelPatient(Queue& queue, const std::string& departmentName, const std::string& transactionLog);

// Process the remaining patients in the queue at the end of the day
void endOfDayProcessing(Queue& queue, const std::string& departmentName, const std::string& transactionLog, const std::string& csvFile);

// Load patients into the queue from a file
void loadPatients(const std::string& filename, Queue& queue);

// Display the main clinic menu for department selection
void clinicMenu(Queue& queue);

#endif // CLINIC_FUNCTIONS_H

