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
    std::cout<< "UdpSocketDataSender " << UdpSocket::getVersion()   << std::endl;
    std::cout<< "=================================================" << std::endl;
    std::cout<< "Library versions: "                                << std::endl;
    std::cout<< "Tracer:............"<< Tracer::getVersion()        << std::endl;
    std::cout<< "UdpSocket:........."<< UdpSocket::getVersion()     << std::endl;
    std::cout<< "-------------------------------------------------" << std::endl;
    std::cout<< std::endl;

    // Enter destination IP.
    std::string dstIp = "";
    std::cout << "Enter destination IP: ";
    std::cin >> dstIp;

    // Enter UDP port.
    int udpPort = 0;
    std::cout << "Enter destination UDP port: ";
    std::cin >> udpPort;

    // Enter sending data period ms.
    int cyclePeriodMs = 0;
    std::cout << "Enter sending data period ms: ";
    std::cin >> cyclePeriodMs;

    // Enter number of bytes.
    int numBytesToSend = 0;
    std::cout << "Enter num bytes to send [0-1024]: ";
    std::cin >> numBytesToSend;

    // Init UDP socket.
    UdpSocket udpSocket(UdpSocket::CLIENT);
    if (!udpSocket.setDstAddr(dstIp, udpPort))
    {
        std::cout << "ERROR: Destination IP not set. Exit." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return -1;
    }
    if (!udpSocket.open())
    {
        std::cout << "ERROR: Udp socket not init. Exit." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return -1;
    }

    // Init variables.
    uint8_t* outputData = new uint8_t[numBytesToSend];

    // Main loop.
    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
    while (true)
    {
        // Prepare random data.
        for (int i = 0; i < numBytesToSend; ++i)
            outputData[i] = (uint8_t)(rand() % 255);

        // Send data.
        std::cout << udpSocket.sendData(outputData, numBytesToSend) << " bytes sent" << std::endl;

        // Wait according to parameters.
        int waitTime = (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count();
        waitTime = cyclePeriodMs - waitTime;
        if (waitTime > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
        startTime = std::chrono::system_clock::now();
    }

    return 1;
}
