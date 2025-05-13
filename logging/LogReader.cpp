#include "LogReader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <matplot/matplot.h>

constexpr long long NANO_PER_SECOND = 1e9;

LogReader::LogReader(std::string fileName)
    : dataParsed(false)
    , fileName(std::move(fileName))
    , intCategories(std::unordered_set<std::string>{"packet_count","total_bytes","eth_packet_count","ipv4_packet_count","ipv6_packet_count","tcp_packet_count","udp_packet_count","dns_packet_count","http_request_packet_count","http_response_packet_count","ssl_packet_count","packets_per_second (throughput)","latency_ns"})
{}

bool LogReader::isCategory(const std::string& category) const {
    return categories.contains(category);
}

bool LogReader::isIntCategory(const std::string& category) const {
    return intCategories.contains(category);
}

void LogReader::graphOverTime(const std::string& category) {
    if (dataParsed && isIntCategory(category)) {
        matplot::plot(std::get<std::vector<int>>(csvData["timestamp_ns"]), std::get<std::vector<int>>(csvData[category]));
        matplot::xlabel("Time (s)");
        matplot::ylabel(category);
        matplot::title(category + " over time");
    } else if (dataParsed) {
        std::cerr << "Invalid category" << std::endl;
    } else {
        std::cerr << "Data not parsed yet! Please run 'updatedata'" << std::endl;
    }
}

void LogReader::graphOverTimeWithMap(const std::string& category) {
    if (dataParsed && isIntCategory(category)) {
        matplot::plot(std::get<std::vector<int>>(csvDataWithMap["timestamp_ns"]), std::get<std::vector<int>>(csvDataWithMap[category]));
        matplot::xlabel("Time (s)");
        matplot::ylabel(category);
        matplot::title(category + " over time");

        matplot::save("plot.png");
        std::system("open plot.png");
    } else if (dataParsed) {
        std::cerr << "Invalid category" << std::endl;
    } else {
        std::cerr << "Data not parsed yet! Please run 'updatedata'" << std::endl;
    }
}

void LogReader::convertStringsToInts(const std::string& category, bool map) {
    std::vector<std::string> stringData;
    if (!map) {
        stringData = std::get<std::vector<std::string>>(csvData[category]);
    }
    else {
        stringData = std::get<std::vector<std::string>>(csvDataWithMap[category]);
    }
    std::vector<int> intData;
    intData.reserve(stringData.size());

    for (const auto& value : stringData) {
        try {
            intData.push_back(std::stoi(value));  // convert to integer
        }
        catch (...) {
            intData.push_back(0);
        }
    }

    if (!map) {
        csvData[category] = std::move(intData);
    } else {
        csvDataWithMap[category] = std::move(intData);
    }
}

void LogReader::convertStringsToSeconds(const std::string& category, bool map) {
    std::vector<std::string> stringData;
    if (!map) {
        stringData = std::get<std::vector<std::string>>(csvData[category]);
    } else {
        stringData = std::get<std::vector<std::string>>(csvDataWithMap[category]);
    }
    std::vector<int> secondData;
    secondData.reserve(stringData.size());
    int firstTime = std::stoll(stringData[0]) / NANO_PER_SECOND;

    for (const auto& value : stringData) {
        secondData.push_back((std::stoll(value) / NANO_PER_SECOND) - firstTime);  // convert to seconds
    }

    if (!map) {
        csvData[category] = std::move(secondData);
    } else {
        csvDataWithMap[category] = std::move(secondData);
    }
}

/**
 * @brief Returns the number of rows in a CSV file (excluding the header).
 * @param fileName Path to the CSV file.
 * @return Number of rows.
 */
size_t countCSVRows(const std::string& fileName) {
    std::ifstream file(fileName);
    size_t count = 0;
    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty()) count++;
    }

    // accounting for header
    return count > 0 ? count - 1 : count;
}

void LogReader::parseData() {

    size_t dataSize = countCSVRows(fileName);

    const auto start_time = std::chrono::high_resolution_clock::now();

    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
    }
    std::string line;
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string colName;
        while (std::getline(ss, colName, ',')) {
            colNames.push_back(colName);
            std::vector<std::string> temp;
            temp.reserve(dataSize);
            csvData[colName] = std::move(temp);
        }
    }

    // for each csv row, sort data cell to respective vector in the unordered map
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        int colIdx = 0;
        while (std::getline(ss, cell, ',')) {
            std::get<std::vector<std::string>>(csvData[colNames[colIdx]]).push_back(cell);
            colIdx++;
        }
    }
    file.close();

    // printing the data out temporarily for testing purposes
    for (const auto& [category, data]: csvData) {
        if (isIntCategory(category)) {
            convertStringsToInts(category, false);
        }
    }
    convertStringsToSeconds("timestamp_ns", false);
    dataParsed = true;

    const auto end_time = std::chrono::high_resolution_clock::now();

    const auto timeTaken = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

    std::cout << "Data successfully parsed with std::unordered_map!" << std::endl;
    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::duration<double>>(timeTaken) << std::endl;


    const auto start_time_b = std::chrono::high_resolution_clock::now();

    std::ifstream file_b(fileName);
    if (!file_b.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
    }
    std::string line_b;
    if (std::getline(file_b, line_b)) {
        std::stringstream ss(line_b);
        std::string colName;
        while (std::getline(ss, colName, ',')) {
            colNames.push_back(colName);
            std::vector<std::string> temp;
            temp.reserve(dataSize);
            csvDataWithMap[colName] = std::move(temp);
        }
    }

    // for each csv row, sort data cell to respective vector in the unordered map
    while (std::getline(file_b, line_b)) {
        std::stringstream ss(line_b);
        std::string cell;
        int colIdx = 0;
        while (std::getline(ss, cell, ',')) {
            std::get<std::vector<std::string>>(csvDataWithMap[colNames[colIdx]]).push_back(cell);
            colIdx++;
        }
    }
    file.close();

    // printing the data out temporarily for testing purposes
    for (const auto& [category, data]: csvDataWithMap) {
        if (isIntCategory(category)) {
            convertStringsToInts(category, true);
        }
    }
    convertStringsToSeconds("timestamp_ns", true);
    dataParsed = true;

    const auto end_time_b = std::chrono::high_resolution_clock::now();

    const auto timeTaken_b = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_b - start_time_b);

    std::cout << "Data successfully parsed with std::map!" << std::endl;
    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::duration<double>>(timeTaken_b) << std::endl;
}

void LogReader::printIntCategories() {
    for (auto& category : intCategories) {
        std::cout << category << std::endl;
    }
}