#ifndef LOGGER_H
#define LOGGER_H
#include <fstream>

#include "../packets/PacketStats.h"

/**
 * @class Logger
 * @brief Handles periodic writing of packet statistics to a CSV file.
 */
class Logger {
public:
    /**
     * @brief Constructs a Logger with the given output file name.
     * @param fileName Name of the CSV file.
     */
    explicit Logger(const std::string& fileName);

    /**
     * @brief Logs packet statistics to the file.
     * @param stats Reference to the packet statistics object.
     */
    void logToFile(const PacketStats& stats);

    /**
     * @brief Ensures all buffered data is flushed to the file.
     */
    void flush();
private:
    std::ofstream logFile;
};



#endif //LOGGER_H
