// CS201R-Pgm8Hospital.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Clinic.h"
#include <iostream>
#include <fstream>
#include <cctype> // For isdigit()
#include <string>

//void endOfDayProcessing(Queue& queue, const std::string& departmentName, const std::string& transactionLog, const std::string& csvFile) {
//    std::ofstream log(transactionLog, std::ios::app);
//    std::ofstream csv(csvFile, std::ios::app);
//
//    if (!log.is_open() || !csv.is_open()) {
//        std::cerr << "Error: Unable to open transaction log or CSV file.\n";
//        return;
//    }
//
//    log << departmentName << " END OF DAY REPORT:\n";
//
//    Node* current = queue.getHead(); // Start from the head of the queue
//    while (current) {
//        std::istringstream ss(current->data);
//        std::string firstName, lastName, ssn, type;
//        ss >> firstName >> lastName >> ssn >> type;
//
//        // Write to the transaction log
//        log << departmentName << " Patient: " << firstName << " " << lastName
//            << " was added to the schedule for tomorrow\n";
//
//        // Write to the CSV file in the format: ClinicCode,FirstName,LastName,SSN
//        std::string clinicCode = departmentName.substr(0, 2); // Extract the first two characters as code
//        csv << clinicCode << "," << firstName << "," << lastName << "," << ssn << "\n";
//
//        // Move to the next node
//        current = current->next;
//    }
//
//    log.close();
//    csv.close();
//}
void endOfDayProcessing(Queue& queue, const std::string& departmentName, const std::string& transactionLog, const std::string& csvFile) {
    std::ofstream log(transactionLog, std::ios::app);
    std::ofstream csv(csvFile, std::ios::app);


    if (!log.is_open() || !csv.is_open()) {
        std::cerr << "Error: Unable to open transaction log or CSV file.\n";
        return;
    }

    log << departmentName << " END OF DAY REPORT:\n";

    Node* current = queue.getHead(); // Start from the head of the queue
    while (current != nullptr) {
        // Extract patient data from the node
        std::string data = current->data;
        std::istringstream ss(data);
        std::string firstName, lastName, ssn, type;
        ss >> firstName >> lastName >> ssn >> type;
        std::cout << data << "\n";
        std::cout << "Node data: " << firstName << lastName << ssn << type <<"\n";
        // Write to the transaction log
        log << departmentName << " Patient: " << firstName << " " << lastName
            << " was added to the schedule for tomorrow.\n";

        // Write to the CSV file in the format: ClinicCode,FirstName,LastName,SSN
        std::string clinicCode = departmentName.substr(0, 2); // Extract the first two characters as code
        csv << clinicCode << "," << firstName << "," << lastName << "," << ssn << "\n";

        // Move to the next node
        current = current->next;
    }

    log.close();
    csv.close();
}



void cancelPatient(Queue& queue, const std::string& departmentName, const std::string& transactionLog) {
    std::string ssn;
    std::cout << "Enter Patient SSN: ";
    std::cin >> ssn;

    // Variable to store the removed patient's details
    std::string removedPatient;

    // Attempt to remove the patient
    if (queue.cancelPatient(ssn, removedPatient)) {
        // Patient found and removed
        std::cout << departmentName << " Patient: " << removedPatient << " was removed from the waiting list.\n";

        // Log the transaction
        std::ofstream log(transactionLog, std::ios::app);
        log << departmentName << " Patient: " << removedPatient << " was removed from the waiting list.\n";
        log.close();
    } else {
        // Patient not found
        std::cout << "ERROR " << departmentName << ": Unable to find patient: " << ssn << " in the waiting room.\n";

        // Log the error
        std::ofstream log(transactionLog, std::ios::app);
        log << "ERROR " << departmentName << ": Unable to find patient: " << ssn << " in the waiting room.\n";
        log.close();
    }
}


void takeOutPatientForOperation(Queue& queue, const std::string& departmentName, const std::string& transactionLog) {
    if (!queue.getHead()) { // Check if the queue is empty
        std::cout << "No more patients.\n";
        return;
    }

    try {
        // Dequeue the patient
        std::string patientData = queue.dequeue();

        // Extract patient details from the data string
        std::istringstream ss(patientData);
        std::string firstName, lastName, ssn, type;
        ss >> firstName >> lastName >> ssn >> type;

        // Display patient information
        std::cout << departmentName << " Patient: " << firstName << " " << lastName << " was taken to the operating room.\n";

        // Log the transaction
        std::ofstream log(transactionLog, std::ios::app);
        log << departmentName << " Patient: " << firstName << " " << lastName << " was taken to the operating room.\n";
        log.close();
    }
    catch (const std::runtime_error& e) {
        std::cout << e.what() << "\n";
    }
}


void addCriticalPatient(Queue& queue, const std::string& departmentName, const std::string& transactionLog) {
    if (queue.size() >= 10) { // Check queue size
        std::cout << "The " << departmentName << " is full. Cannot add more patients.\n";
        std::ofstream log(transactionLog, std::ios::app);
        log << departmentName << " Patient Addition Failed: Queue is full.\n";
        log.close();
        return;
    }

    // Collect patient details
    std::string firstName, lastName, ssn;
    std::cout << "Enter Patient's First Name: ";
    std::cin >> firstName;
    std::cout << "Enter Patient's Last Name: ";
    std::cin >> lastName;
    std::cin.ignore(); // Clear input buffer
    std::cout << "Enter Patient's SSN: ";
    std::getline(std::cin, ssn);

    // Validate SSN
    if (ssn.empty()) {
        std::cout << "Operation canceled. SSN cannot be empty.\n";
        std::ofstream log(transactionLog, std::ios::app);
        log << departmentName << " Patient Addition Canceled: Empty SSN.\n";
        log.close();
        return;
    }
    for (char c : ssn) {
        if (!isdigit(c)) {
            std::cout << "Operation canceled. SSN must be numeric.\n";
            std::ofstream log(transactionLog, std::ios::app);
            log << departmentName << " Patient Addition Canceled: Invalid SSN.\n";
            log.close();
            return;
        }
    }

    // Add the critically ill patient
    std::string patientData = firstName + " " + lastName + " " + ssn + " C"; // C for Critical
    queue.enqueueCritical(patientData);

    // Log the transaction
    std::ofstream log(transactionLog, std::ios::app);
    log << departmentName << " Patient (Critical): " << firstName << " " << lastName << " was added to the waiting room.\n";
    log.close();

    // Confirm the addition
    std::cout << departmentName << " Patient (Critical): " << firstName << " " << lastName << " was added to the waiting room.\n";
    std::cout << "Press any key to return to the menu.\n";
    std::cin.get(); // Wait for user input
}


// Function to add a new patient to the queue
void addNewPatient(Queue& queue, const std::string& departmentName, const std::string& transactionLog) {
    if (queue.size() >= 10) { // Check queue size
        std::cout << "The " << departmentName << " is full. Cannot add more patients.\n";
        std::ofstream log(transactionLog, std::ios::app);
        log << departmentName << " Patient Addition Failed: Queue is full.\n";
        log.close();
        return;
    }

    // Collect patient details
    std::string firstName, lastName, ssn;
    std::cout << "Enter Patient's First Name: ";
    std::cin >> firstName;
    std::cout << "Enter Patient's Last Name: ";
    std::cin >> lastName;
    std::cin.ignore(); // Clear input buffer
    std::cout << "Enter Patient's SSN: ";
    std::getline(std::cin, ssn);

    // Validate SSN
    if (ssn.empty()) {
        std::cout << "Operation canceled. SSN cannot be empty.\n";
        std::ofstream log(transactionLog, std::ios::app);
        log << departmentName << " Patient Addition Canceled: Empty SSN.\n";
        log.close();
        return;
    }
    for (char c : ssn) {
        if (!isdigit(c)) {
            std::cout << "Operation canceled. SSN must be numeric.\n";
            std::ofstream log(transactionLog, std::ios::app);
            log << departmentName << " Patient Addition Canceled: Invalid SSN.\n";
            log.close();
            return;
        }
    }

    // Add the patient to the queue
    std::string patientData = firstName + " " + lastName + " " + ssn + " R"; // R for Regular
    queue.enqueue(patientData);

    // Write the patient data to the CSV file
    std::ofstream csv("Patients.csv", std::ios::app); // Open in append mode
    if (!csv.is_open()) {
        std::cerr << "Error: Unable to open the CSV file: Patients.csv\n";
        return;
    }
    csv << departmentName.substr(0, 2) << "," // Write department code (first two letters)
        << firstName << ","
        << lastName << ","
        << ssn << "\n"; // Write patient details in CSV format
    csv.close();

    // Log the transaction
    std::ofstream log(transactionLog, std::ios::app);
    log << departmentName << " Patient (Regular): " << firstName << " " << lastName << " was added to the waiting room.\n";
    log.close();

    // Confirm the addition
    std::cout << departmentName << " Patient (Regular): " << firstName << " " << lastName << " was added to the waiting room.\n";
    std::cout << "Press any key to return to the menu.\n";
    std::cin.get(); // Wait for user input
}



void actionMenu(Queue& queue, const std::string& departmentName, const std::string& transactionLog,const std::string& csvFile) {
    int choice;
    do {
        std::cout << "\nWelcome to the " << departmentName << "\n";
        std::cout << "1: - Add Patient\n";
        std::cout << "2: - Add Critical Patient\n";
        std::cout << "3: - Take out Patient for Operation\n";
        std::cout << "4: - Cancel Patient\n";
        std::cout << "5: - List Patients in Queue\n";
        std::cout << "6: - Change Department or exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            addNewPatient(queue, departmentName, transactionLog);
            break;
        case 2:
            // Add Critical Patient logic here
            addCriticalPatient(queue, departmentName, transactionLog);
            break;
        case 3:
            // Take out Patient logic here
            takeOutPatientForOperation(queue, departmentName, transactionLog);
            break;
        case 4:
            // Cancel Patient logic here
            cancelPatient(queue, departmentName, transactionLog);
            break;
        case 5:
            std::cout << "\nCurrent Patient Queue:\n";
            queue.display();
            break;
        case 6:
            std::cout << "Changing department or exiting menu.\n";
            //endOfDayProcessing(queue, departmentName, transactionLog, csvFile);
            break;
        default:
            std::cerr << "Invalid choice! Please select again.\n";
        }
    } while (choice != 6);
}



void loadPatients(const std::string& filename, Queue& queue) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string clinicCode, firstName, lastName, ssn;

        // Parse the CSV line
        if (!std::getline(ss, clinicCode, ',') ||
            !std::getline(ss, firstName, ',') ||
            !std::getline(ss, lastName, ',') ||
            !std::getline(ss, ssn)) {
            std::cerr << "Error: Malformed record in file\n";
            continue;
        }

        // Skip invalid records
        if (clinicCode == "ERR" || firstName.empty() || lastName.empty() || ssn.empty()) {
            std::cerr << "Skipping invalid record: " << line << "\n";
            continue;
        }

        // Format the data and enqueue it
        std::string patientData = clinicCode + "," + firstName + "," + lastName + "," + ssn;
        queue.enqueue(patientData);
    }

    file.close();
}


void clinicMenu(Queue& queue) {
    int choice;
    std::string transactionLog = "transactionLog.txt";
    std::string csvFile = "dataFile.csv";
    
    do {
        std::cout << "\n---  KC RESEARCH HOSPITAL  ---\n";
        std::cout << "1. Heart Clinic \n";
        std::cout << "2. Pulmonary Clinic \n";
        std::cout << "3. Plastic Surgery\n";
        std::cout << "4. Quit \n";
        std::cout << "Please Enter the clinic of your Interest: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
     
          //  actionMenu(heartClinicQueue, "Heart Clinic", transactionLog);
            actionMenu(queue, "Heart Clinic" , transactionLog, csvFile );
            /*std::cout << "\nPatient Queue:\n";
            queue.display();*/
            break;
        case 2: {
            actionMenu(queue, "Pulmonary Clinic" , transactionLog, csvFile);
            //std::cin.ignore(); // Clear newline character
            //std::string patient;
            //std::cout << "Enter patient details (Name, Age, Ailment): ";
            //std::getline(std::cin, patient);
            //queue.enqueue(patient);
            break;
        }
        case 3:
            actionMenu(queue, "Plastic Surgery", transactionLog, csvFile);
            break;
        case 4:
            std::cout << "Exiting clinic menu.\n";
            endOfDayProcessing(queue, "Hospital", transactionLog, csvFile);
            break;
        default:
            std::cerr << "Invalid choice!\n";
        }
    } while (choice != 4);
}



