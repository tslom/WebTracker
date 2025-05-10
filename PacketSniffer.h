#ifndef PACKETSNIFFER_H
#define PACKETSNIFFER_H
#include "PacketStats.h"
#include "PcapLiveDeviceList.h"


class PacketSniffer {
public:
    PacketSniffer();
    bool startSniffing();
    PacketStats stats;
private:
    pcpp::PcapLiveDevice* liveDevice;
    bool setupLiveDevice();
};

#endif //PACKETSNIFFER_H
