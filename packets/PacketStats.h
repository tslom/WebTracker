#ifndef PACKETSTATS_H
#define PACKETSTATS_H
#include <chrono>
#include <Packet.h>
#include <unordered_map>

/**
 * @struct PacketStats
 * @brief Collects detailed statistics from captured packets, including protocol counts, total bytes, timing, and DNS/domain mappings.
 */
struct PacketStats {
    // packet counts
    unsigned int packetCount;
    unsigned int ethPacketCount;
    unsigned int ipv4PacketCount;
    unsigned int ipv6PacketCount;
    unsigned int tcpPacketCount;
    unsigned int udpPacketCount;
    unsigned int dnsPacketCount;
    unsigned int httpRequestPacketCount;
    unsigned int httpResponsePacketCount;
    unsigned int sslPacketCount;

    // time tracking
    std::chrono::nanoseconds totalTime;

    // packet data
    unsigned long long totalBytes;
    std::unordered_map<std::string, int> domainCount;
    std::unordered_map<std::string, int> ipv4Count;

    // dns tracking
    std::unordered_map<std::string, std::string> ipv4ToDomain;


    /**
     * @brief Resets all stored statistics.
     */
    void clear();

    /**
     * @brief Processes a packet and updates statistics.
     * @param packet Parsed packet object.
     */
    void consumePacket(const pcpp::Packet& packet);

    /**
     * @brief Outputs statistics to the console.
     */
    void printToConsole();
};

#endif //PACKETSTATS_H
