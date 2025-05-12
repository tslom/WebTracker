#include <memory>

#include "PacketSniffer.h"

int main() {

    const std::string fileName = "usage-data.csv";

    auto sniffer = std::make_unique<PacketSniffer>(fileName);

    sniffer->startSniffing();

    return 0;
}
