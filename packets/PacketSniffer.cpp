#include "PacketSniffer.h"

#include <iostream>
#include "SystemUtils.h"

PacketSniffer::PacketSniffer(const std::string& fileName)
    : logger(fileName)
    , isCapturing(false)
    , stopCapturing(false) {
    if (!setupLiveDevice()) {
        liveDevice = nullptr;
        std::cerr << "Unable to locate device to sniff" << std::endl;
    }
}

bool isValidIPv4Address(const pcpp::IPv4Address address) {
    return address != pcpp::IPv4Address::Zero && pcpp::IPv4Address::isValidIPv4Address(address.toString());
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

void PacketSniffer::startSniffing() {
    if (isCapturing) {
        std::cerr << "Already capturing packets" << std::endl;
        return;
    }

    if (!liveDevice->open()) {
        std::cerr << "Cannot open device" << std::endl;
        return;
    }

    std::cout << std::endl << "Starting packet capture..." << std::endl;

    if (!liveDevice->startCapture(onPacketArrives, &stats)) {
        std::cerr << "Cannot start capture" << std::endl;
        liveDevice->close();
        return;
    }

    stopCapturing = false;
    isCapturing = true;

    thread = std::thread(&PacketSniffer::whileInThread, this);
    thread.detach();
}

void PacketSniffer::stopSniffing() {
    if (!isCapturing) {
        return;
    }

    stopCapturing = true;

    if (thread.joinable()) {
        thread.join();
    }

    // Stop the actual capture
    liveDevice->stopCapture();
    liveDevice->close();

    isCapturing = false;

    std::cout << "Packet capture stopped" << std::endl;
}

void PacketSniffer::whileInThread() {
    while (!stopCapturing) {
        // every 10 seconds, locks the thread, sends the data to logger
        pcpp::multiPlatformSleep(10);

        {
            std::lock_guard<std::mutex> lock(std::mutex);
            logger.logToFile(stats);
            stats.clear();
            logger.flush();
        }
    }
}

PacketSniffer::~PacketSniffer() {
    if (isCapturing) {
        stopSniffing();
    }
}

bool PacketSniffer::getIsCapturing() {
    return isCapturing;
}


