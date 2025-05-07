#include "PacketSniffer.h"

#include <iostream>

PacketSniffer::PacketSniffer() {
    if (!setupLiveDevice()) {
        throw std::runtime_error("Failed to find a suitable device to sniff!");
    }
}

bool isValidIPv4Address(pcpp::IPv4Address address) {
    return  address != pcpp::IPv4Address::Zero && pcpp::IPv4Address::isValidIPv4Address(address.toString());
}

bool PacketSniffer::setupLiveDevice() {
    // assuming that the first non 0.0.0.0 non loopback (locally hosted and not connected to wifi) and valid IPv4 address is the correct personal interface)
    auto devices = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
    for (auto dev : devices) {
        if (!dev->getLoopback() && isValidIPv4Address(dev->getIPv4Address().toString())) {
            std::cout << "Connected to: " << dev->getIPv4Address() << std::endl;
            return true;
        }
    }
    return false;
}



bool PacketSniffer::startSniffing() {
    std::cout << "no" << std::endl;
}

