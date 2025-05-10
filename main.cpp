#include "PacketSniffer.h"

int main() {
    auto* sniffer = new PacketSniffer();

    sniffer->startSniffing();

    return 0;
}
