#ifndef PACKETSNIFFER_H
#define PACKETSNIFFER_H
#include "../logging/Logger.h"
#include "PacketStats.h"
#include "PcapLiveDeviceList.h"


class PacketSniffer {
public:
    explicit PacketSniffer(const std::string& fileName);
    void startSniffing();
    PacketStats stats;
private:
    pcpp::PcapLiveDevice* liveDevice;
    bool setupLiveDevice();
    Logger logger;
};

#endif //PACKETSNIFFER_H
