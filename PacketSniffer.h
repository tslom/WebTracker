#ifndef PACKETSNIFFER_H
#define PACKETSNIFFER_H
#include "PcapLiveDeviceList.h"


class PacketSniffer {
public:
    PacketSniffer();
    bool startSniffing();
private:
    pcpp::PcapLiveDevice* liveDevice;
    bool setupLiveDevice();
};



#endif //PACKETSNIFFER_H
