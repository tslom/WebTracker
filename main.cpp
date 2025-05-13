#include <iostream>

#include "./packets/PacketSniffer.h"
#include "./logging/LogReader.h"

/**
 * @brief Main CLI loop for controlling the packet sniffer, data updates, and graph generation.
 *
 * Supports commands:
 * - "start": Begin packet sniffing.
 * - "stop": End packet sniffing.
 * - "updatedata": Parse and process the logged data.
 * - "graphtime <category>": Generate a time-series graph for a given category.
 * - "graphtime-map <category>": Generate a mapped time-series graph.
 * - "exit": Exit the application.
 */
int main() {
    const std::string fileName = "usage-data.csv";

    static PacketSniffer sniffer(fileName);
    static LogReader logReader(fileName);


    std::string command;
    bool running = true;

    while (running) {

        std::cout << "\nAvailable commands: start, stop, updatedata, graphtime, graphtime-map exit\n> ";
        std::getline(std::cin, command);


        if (command == "start") {
            sniffer.startSniffing();
        }
        else if (command == "stop") {
            sniffer.stopSniffing();
        }
        else if (command == "updatedata") {
            logReader.parseData();
        }
        else if (command.substr(0, 13) == "graphtime-map") {
            std::string category;
            if (command.length() > 14) {
                category = command.substr(14);
                logReader.graphOverTimeWithMap(category);

            } else {
                std::cout << "Please choose a category. \n";
                logReader.printIntCategories();
            }
        }
        else if (command.substr(0, 9) == "graphtime") {
            std::string category;
            if (command.length() > 10) {
                category = command.substr(10);
                logReader.graphOverTime(category);

            } else {
                std::cout << "Please choose a category. \n";
                logReader.printIntCategories();
            }
        }
        else if (command == "exit" || command == "quit") {
            // Make sure to stop sniffing before exiting
            if (sniffer.getIsCapturing()) {
                sniffer.stopSniffing();
            }
            running = false;
        }
        else {
            std::cout << "Unknown command.\n";
        }
    }

    return 0;
}
