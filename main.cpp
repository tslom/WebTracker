#include <memory>

#include "./packets/PacketSniffer.h"
#include "./logging/LogReader.h"

int main() {

    const std::string fileName = "usage-data.csv";

    // auto sniffer = std::make_unique<PacketSniffer>(fileName);
    //
    // sniffer->startSniffing();

    auto logReader = std::make_unique<LogReader>(fileName);

    logReader->parseData();

    logReader->graphOverTime("total_bytes");

    return 0;
}
