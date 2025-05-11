#ifndef LOGGER_H
#define LOGGER_H
#include <fstream>

#include "PacketStats.h"


class Logger {
public:
    explicit Logger(const std::string& fileName);
    void logToFile(const PacketStats& stats);
    void flush();
private:
    std::ofstream logFile;
};



#endif //LOGGER_H
