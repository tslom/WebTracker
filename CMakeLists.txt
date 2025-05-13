cmake_minimum_required(VERSION 3.15)
project(WebTracker)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        PcapPlusPlus
        GIT_REPOSITORY https://github.com/seladb/PcapPlusPlus.git
        GIT_TAG v24.09
)
FetchContent_MakeAvailable(PcapPlusPlus)

FetchContent_Declare(
        matplotplusplus
        GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus.git
        GIT_TAG origin/master
)
FetchContent_MakeAvailable(matplotplusplus)

# jank fix for random error when building pcapplusplus
if(TARGET Pcap++)
     target_compile_options(Pcap++ PRIVATE -Wno-vla-extension -Wno-error=vla-extension)
endif()

add_executable("${PROJECT_NAME}" main.cpp
        packets/PacketSniffer.cpp
        packets/PacketSniffer.h
        packets/PacketStats.cpp
        packets/PacketStats.h
        logging/Logger.cpp
        logging/Logger.h
        logging/LogReader.cpp
        logging/LogReader.h)

set_target_properties("${PROJECT_NAME}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries("${PROJECT_NAME}" PUBLIC Pcap++ matplot)
