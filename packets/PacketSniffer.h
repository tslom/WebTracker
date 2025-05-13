#ifndef PACKETSNIFFER_H
#define PACKETSNIFFER_H
#include "../logging/Logger.h"
#include "PacketStats.h"
#include "PcapLiveDeviceList.h"


class PacketSniffer {
public:
    explicit PacketSniffer(const std::string& fileName);
    ~PacketSniffer();
    void startSniffing();
    void stopSniffing();
    PacketStats stats;
    [[nodiscard]] bool getIsCapturing();
private:
    pcpp::PcapLiveDevice* liveDevice;
    bool setupLiveDevice();
    void whileInThread();
    Logger logger;

    // thread stuff
    std::thread thread;
    // atomic bools and mutexes help thread safety making sure no funky business goes on
    std::atomic<bool> stopCapturing;
    std::atomic<bool> isCapturing;
    std::mutex mutex;
};

#endif //PACKETSNIFFER_H
