#include "Clinic.h"
#include <iostream>
#include <fstream>
#include <cctype> // For isdigit()
#include <string>
#include "Functions.h"

int main() {
    Queue queue;
    std::string filename = "Patients.csv";

    loadPatients(filename, queue);
    //std::string filename2 = "dataFile.csv";

    //loadPatients(filename2, queue);
    clinicMenu(queue);

    return 0;
}
