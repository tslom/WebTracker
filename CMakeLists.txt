cmake_minimum_required(VERSION 3.30)
project(WebBlocker)

set(CMAKE_CXX_STANDARD 20)

find_package(PcapPlusPlus REQUIRED)

add_executable("${PROJECT_NAME}" main.cpp
        PacketSniffer.cpp
        PacketSniffer.h
        PacketStats.h)

set_target_properties("${PROJECT_NAME}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries("${PROJECT_NAME}" PUBLIC PcapPlusPlus::Pcap++)
