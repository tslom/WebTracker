#include "LogReader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <matplot/matplot.h>

LogReader::LogReader(std::string fileName)
    : dataParsed(false)
    , fileName(std::move(fileName))
    , numericalCategories(std::unordered_set<std::string>{"packet_count","total_bytes","eth_packet_count","ipv4_packet_count","ipv6_packet_count","tcp_packet_count","udp_packet_count","dns_packet_count","http_request_packet_count","http_response_packet_count","ssl_packet_count","packets_per_second (throughput)","latency_ns"})
{}

bool LogReader::isCategory(const std::string& category) const {
    return categories.contains(category);
}

bool LogReader::isIntegerCategory(const std::string& category) const {
    return numericalCategories.contains(category);
}

void LogReader::graphOverTime(const std::string& category) {
    if (dataParsed && isIntegerCategory(category)) {
        matplot::plot(std::get<std::vector<int>>(csvData["timestamp_ns"]), std::get<std::vector<int>>(csvData[category]));
        matplot::xlabel("Time (s)");
        matplot::ylabel(category);
        matplot::title(category + " over time");
        matplot::grid(matplot::on);

        matplot::show();
    }
}

void LogReader::convertStringsToInts(const std::string& category) {
    auto& stringData = std::get<std::vector<std::string>>(csvData[category]);
    std::vector<int> intData;
    intData.reserve(stringData.size());

    for (const auto& value : stringData) {
        std::cout << category << ":" << value << std::endl;
        try {
            intData.push_back(std::stoi(value));  // convert to integer
        }
        catch (...) {
            intData.push_back(0);
        }
    }

    csvData[category] = std::move(intData);
}

void LogReader::convertStringsToSeconds(const std::string& category) {
    const auto& stringData = std::get<std::vector<std::string>>(csvData[category]);
    std::vector<int> secondData;
    secondData.reserve(stringData.size());

    for (const auto& value : stringData) {
        secondData.push_back(std::stoll(value) / 1e9);  // convert to seconds
    }

    csvData[category] = std::move(secondData);
}

size_t countCSVRows(const std::string& fileName) {
    std::ifstream file(fileName);
    size_t count = 0;
    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty()) ++count;
    }

    // accounting for header
    return count > 0 ? count - 1 : count;
}

void LogReader::parseData() {
    size_t dataSize = countCSVRows(fileName);

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
        if (isIntegerCategory(category)) {
            convertStringsToInts(category);
        }
    }
    convertStringsToSeconds("timestamp_ns");
    dataParsed = true;
}

