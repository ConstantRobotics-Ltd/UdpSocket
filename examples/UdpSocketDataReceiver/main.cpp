#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>

#include "Tracer.h"
#include "UdpSocket.h"

using namespace cr::clib;
using namespace cr::utils;

// Entry point.
int main(void)
{
    std::cout<< "=================================================" << std::endl;
    std::cout<< "UdpSocketDataReceiver " << UdpSocket::getVersion() << std::endl;
    std::cout<< "=================================================" << std::endl;
    std::cout<< "Library versions: "                                << std::endl;
    std::cout<< "Tracer:............"<< Tracer::getVersion()        << std::endl;
    std::cout<< "UdpSocket:........."<< UdpSocket::getVersion()     << std::endl;
    std::cout<< "-------------------------------------------------" << std::endl;
    std::cout<< std::endl;

    // Enter host IP.
    std::string hostIp = "";
    std::cout << "Enter host IP: ";
    std::cin >> hostIp;

    // Enter UDP port.
    int udpPort = 0;
    std::cout << "Enter host UDP port: ";
    std::cin >> udpPort;

    // Enter wait data timeout.
    int waitDataTimeoutMs = 0;
    std::cout << "Enter wait data timeout ms: ";
    std::cin >> waitDataTimeoutMs;

    // Init UDP socket.
    UdpSocket udpSocket(UdpSocket::SERVER);
    if (!udpSocket.setHostAddr(hostIp, udpPort))
    {
        std::cout << "ERROR: Host IP not set. Exit." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return -1;
    }
    if (!udpSocket.open(waitDataTimeoutMs))
    {
        std::cout << "ERROR: Udp socket not init. Exit." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return -1;
    }

    // Init variables.
    const uint16_t inputDataSize = 1024;
    uint8_t inputData[inputDataSize];

    // Main loop.
    while (true)
    {
        // Read data.
        int bytes = udpSocket.readData(inputData, inputDataSize);

        // Check input data size.
        if (bytes <= 0)
            std::cout << "No input data" << std::endl;
        else
            std::cout << bytes << " bytes read." << std::endl;
    }
}


