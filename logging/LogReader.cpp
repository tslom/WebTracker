#include "LogReader.h"

#include <fstream>
#include <iostream>
#include <sstream>

LogReader::LogReader(const std::string& fileName)
    : fileName(std::move(fileName))
{}

void compileData() {

}

bool LogReader::parseData() {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return false;
    }
    std::string line;
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string colName;
        while (std::getline(ss, colName, ',')) {
            colNames.push_back(colName);
            csvData[colName] = {}; // initializes empty vector for each column
        }
    }

    // for each csv row, sort data cell to respective vector in the unordered map
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        int colIdx = 0;
        while (std::getline(ss, cell, ',')) {
            csvData[colNames[colIdx]].push_back(cell);
            colIdx++;
        }
    }
    file.close();

    // printing the data out temporarily for testing purposes
    for (const auto& [colName, data] : csvData) {
        std::cout << colName << ": ";
        for (const auto& cell : data) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }

    return true;
}


