#include "PacketStats.h"
#include "HttpLayer.h"
#include "EthLayer.h"
#include "IPv4Layer.h"
#include "IPv6Layer.h"
#include "TcpLayer.h"
#include "UdpLayer.h"
#include "DnsLayer.h"
#include "SSLLayer.h"

#include <iostream>

PacketStats::PacketStats() :
    packetCount(0),
    ethPacketCount(0),
    ipv4PacketCount(0),
    ipv6PacketCount(0),
    tcpPacketCount(0),
    udpPacketCount(0),
    dnsPacketCount(0),
    httpRequestPacketCount(0),
    httpResponsePacketCount(0),
    sslPacketCount(0),
    totalTime(std::chrono::nanoseconds::zero()),
    totalBytes(0)
{}

void PacketStats::clear() {
    packetCount = ethPacketCount = ipv4PacketCount = ipv6PacketCount = tcpPacketCount = udpPacketCount = dnsPacketCount = httpRequestPacketCount = httpResponsePacketCount = sslPacketCount = totalBytes = 0;
    totalTime = std::chrono::nanoseconds::zero();
    domainCount.clear();
    ipv4Count.clear();
    // not clearing ipv4 to domain
}



std::string printHttpMethod(pcpp::HttpRequestLayer::HttpMethod httpMethod) {
    switch (httpMethod)
    {
        case pcpp::HttpRequestLayer::HttpGET:
            return "GET";
        case pcpp::HttpRequestLayer::HttpPOST:
            return "POST";
        default:
            return "Other";
    }
}

void PacketStats::consumePacket(const pcpp::Packet &packet) {
    const auto start_time = std::chrono::high_resolution_clock::now();

        packetCount++;
        auto* ethernetLayer = packet.getLayerOfType<pcpp::EthLayer>();
        if (ethernetLayer != nullptr) {
            ethPacketCount++;
        }
        auto* ipv4Layer = packet.getLayerOfType<pcpp::IPv4Layer>();
        if (ipv4Layer != nullptr)
        {
            ipv4PacketCount++;
            const std::string ip = ipv4Layer->getDstIPv4Address().toString();

            ipv4Count[ip]++;
            if (ipv4ToDomain.contains(ip)) {
                domainCount[ipv4ToDomain[ip]]++;
            }

        }
        auto* tcpLayer = packet.getLayerOfType<pcpp::TcpLayer>();
        if (tcpLayer != nullptr)
        {
            tcpPacketCount++;
        }
        auto* httpRequestLayer = packet.getLayerOfType<pcpp::HttpRequestLayer>();
        if (httpRequestLayer != nullptr)
        {
            httpRequestPacketCount++;
        }
        auto* httpResponseLayer = packet.getLayerOfType<pcpp::HttpResponseLayer>();
        if (httpResponseLayer != nullptr)
        {
            httpResponsePacketCount++;
        }
        auto* ipv6Layer = packet.getLayerOfType<pcpp::IPv6Layer>();
        if (ipv6Layer != nullptr)
        {
            ipv6PacketCount++;
        }
        auto* udpLayer = packet.getLayerOfType<pcpp::UdpLayer>();
        if (udpLayer != nullptr)
        {
            udpPacketCount++;
        }
        auto* dnsLayer = packet.getLayerOfType<pcpp::DnsLayer>();
        if (dnsLayer != nullptr)
        {
            dnsPacketCount++;
            if (dnsLayer->getDnsHeader()->numberOfQuestions > 0) {
                auto* query = dnsLayer->getFirstQuery();
                if (query != nullptr) {
                    // storing domain name in map
                    domainCount[query->getName()]++;
                }
            }
            // Track IP addresses returned in A-record answers
            auto* answer = dnsLayer->getFirstAnswer();
            while (answer != nullptr) {
                if (answer->getType() == pcpp::DNS_TYPE_A) {
                    pcpp::DnsResourceDataPtr dataPtr = answer->getData();
                    auto* ipv4Data = dynamic_cast<pcpp::IPv4DnsResourceData*>(dataPtr.get());

                    if (ipv4Data != nullptr) {
                        // Get the IP address as a string
                        std::string ip = ipv4Data->getIpAddress().toString();

                        std::string domain = answer->getName();

                        // Store IP → domain mapping
                        ipv4ToDomain[ip] = domain;
                    }
                }
                answer = dnsLayer->getNextAnswer(answer);
            }

        }
        auto* sslLayer = packet.getLayerOfType<pcpp::SSLLayer>();
        if (sslLayer != nullptr)
        {
            sslPacketCount++;
        }

        const auto end_time = std::chrono::high_resolution_clock::now();

        totalTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
}

void PacketStats::printToConsole() {
    std::cout
            << "PACKET COUNT:                " << packetCount << std::endl
            << "TOTAL TIME:                  " << totalTime << std::endl
            << "AVERAGE TIME:                " << totalTime/packetCount << std::endl
            << "Ethernet packet count:       " << ethPacketCount << std::endl
            << "IPv4 packet count:           " << ipv4PacketCount << std::endl
            << "IPv6 packet count:           " << ipv6PacketCount << std::endl
            << "TCP packet count:            " << tcpPacketCount << std::endl
            << "UDP packet count:            " << udpPacketCount << std::endl
            << "DNS packet count:            " << dnsPacketCount << std::endl
            << "HTTP response packet count:  " << httpResponsePacketCount << std::endl
            << "HTTP request packet count:   " << httpRequestPacketCount << std::endl
            << "SSL packet count:            " << sslPacketCount << std::endl;
    for (const auto& pair : domainCount) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }

    for (const auto& pair : ipv4ToDomain) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }

    for (const auto& pair : ipv4Count) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }
}


