#include "Logger.h"

Logger::Logger(const std::string& fileName) {
    const bool fileExists = std::filesystem::exists(fileName);

    logFile.open(fileName, std::ios::app);

    if (!fileExists) {
        logFile << "packet_count,eth_packet_count,ipv4_packet_count,ipv6_packet_count,tcp_packet_count,udp_packet_count,dns_packet_count,http_request_packet_count,http_response_packet_count,ssl_packet_count,packets_per_second (throughput),latency_ns,timestamp_ns,top_domain_1,top_domain_2,top_domain_3,top_domain_4,top_domain_5\n";
;
    }
};

void Logger::flush() {
    logFile.flush();
}


void Logger::logToFile(const PacketStats& stats) {

    logFile << stats.packetCount << ","
        << stats.ethPacketCount << ","
        << stats.ipv4PacketCount << ","
        << stats.ipv6PacketCount << ","
        << stats.tcpPacketCount << ","
        << stats.udpPacketCount << ","
        << stats.dnsPacketCount << ","
        << stats.httpRequestPacketCount << ","
        << stats.httpResponsePacketCount << ","
        << stats.sslPacketCount << ","
        << stats.packetCount/10 << ","
        << (stats.totalTime.count()/stats.packetCount) << ","
        << std::chrono::high_resolution_clock::now().time_since_epoch() << ",";

    std::vector<std::pair<std::string, int>> sorted(stats.domainCount.begin(), stats.domainCount.end());
    std::sort(sorted.begin(), sorted.end(),
              [](auto& a, auto& b) { return a.second > b.second; });
    for (int i = 0; i < std::min(5, static_cast<int>(sorted.size())); ++i) {
        logFile << sorted[i].first << "-" << sorted[i].second << ",";
    }
    logFile << "\n";
}

