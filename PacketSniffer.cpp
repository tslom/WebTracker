#include "PacketSniffer.h"

#include <iostream>
#include "SystemUtils.h"

PacketSniffer::PacketSniffer() {
    if (!setupLiveDevice()) {
        liveDevice = nullptr;
        std::cerr << "Unable to locate device to sniff" << std::endl;
    }
}

bool isValidIPv4Address(const pcpp::IPv4Address address) {
    return  address != pcpp::IPv4Address::Zero && pcpp::IPv4Address::isValidIPv4Address(address.toString());
}

void printDevInfo(const pcpp::PcapLiveDevice* dev) {
    // before capturing packets let's print some info about this interface
    std::cout
        << "Interface info:" << std::endl
        << "   Interface name:        " << dev->getName() << std::endl // get interface name
        << "   Interface description: " << dev->getDesc() << std::endl // get interface description
        << "   MAC address:           " << dev->getMacAddress() << std::endl // get interface MAC address
        << "   Default gateway:       " << dev->getDefaultGateway() << std::endl // get default gateway
        << "   Interface MTU:         " << dev->getMtu() << std::endl; // get interface MTU

    if (!dev->getDnsServers().empty())
    {
        std::cout << "   DNS server:            " << dev->getDnsServers().front() << std::endl;
    }
}

bool PacketSniffer::setupLiveDevice() {
    // assuming that the first non 0.0.0.0 non loopback (locally hosted and not connected to wifi) and valid IPv4 address is the correct personal interface)
    auto devices = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
    for (auto dev : devices) {
        if (!dev->getLoopback() && isValidIPv4Address(dev->getIPv4Address().toString())) {
            std::cout << "Connected to: " << dev->getIPv4Address() << std::endl;
            printDevInfo(dev);
            liveDevice = dev;
            return true;
        }
    }
    return false;
}

/**
 * A callback function for the async capture which is called each time a packet is captured
 */
void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie)
{
    // extract the stats object form the cookie
    auto* stats = static_cast<PacketStats*>(cookie);

    // parsed the raw packet
    pcpp::Packet parsedPacket(packet);

    // collect stats from packet
    stats->consumePacket(parsedPacket);
}

bool PacketSniffer::startSniffing() {
    if (!liveDevice->open())
    {
        std::cerr << "Cannot open device" << std::endl;
        return false;
    }

    std::cout << std::endl << "Starting async capture..." << std::endl;

    // start capture in async mode. Give a callback function to call to whenever a packet is captured and the stats object as the cookie
    liveDevice->startCapture(onPacketArrives, &stats);

    pcpp::multiPlatformSleep(10);

    liveDevice->stopCapture();

    std::cout << "Results: " << std::endl;
    stats.printToConsole();
}

