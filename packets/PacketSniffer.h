#ifndef PACKETSNIFFER_H
#define PACKETSNIFFER_H
#include "../logging/Logger.h"
#include "PacketStats.h"
#include "PcapLiveDeviceList.h"

/**
 * @class PacketSniffer
 * @brief Captures live network packets and processes statistics.
 */
class PacketSniffer {
public:
    /**
     * @class PacketSniffer
     * @brief Manages the lifecycle of a live packet capture session, including initialization, capture, logging, and cleanup.
     */
    explicit PacketSniffer(const std::string& fileName);

    /**
     * @brief Destructor, ensures capture is stopped and resources are freed.
     */
    ~PacketSniffer();

    /**
     * @brief Begins sniffing in a separate thread.
     */
    void startSniffing();

    /**
     * @brief Stops the ongoing capture session and logs final statistics.
     */
    void stopSniffing();
    PacketStats stats;

    /**
     * @brief Returns whether packet sniffing is currently active.
     * @return True if active, false otherwise.
     */
    [[nodiscard]] bool getIsCapturing();
private:
    pcpp::PcapLiveDevice* liveDevice;

    /**
     * @brief Sets up the default live device for packet capture.
     * @return True on success.
     */
    bool setupLiveDevice();

    /**
     * @brief Periodic thread function to log and clear stats.
     */
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
