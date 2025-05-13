#ifndef PACKETSTATS_H
#define PACKETSTATS_H
#include <chrono>
#include <Packet.h>
#include <unordered_map>

/**
 * A struct for collecting packet statistics
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
     * Clear all stats
     */
    void clear();

    /**
     * Collect stats from a packet
     */
    void consumePacket(const pcpp::Packet& packet);

    /**
     * Print stats to console
     */
    void printToConsole();
};

#endif //PACKETSTATS_H
