#include <iostream>

#include "./packets/PacketSniffer.h"
#include "./logging/LogReader.h"

void graph_function(const std::string& category) {
    std::cout << "Generating graph for category: " << category << "\n";
}

int main() {
    const std::string fileName = "usage-data.csv";

    static PacketSniffer sniffer(fileName);
    static LogReader logReader(fileName);


    std::string command;
    bool running = true;

    while (running) {

        std::cout << "\nAvailable commands: start, stop, updatedata, graphtime, exit\n> ";
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
