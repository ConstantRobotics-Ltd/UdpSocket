#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include <UdpSocket.h>

// Global constants.
constexpr int g_maxNumBytesToRead = 2048;

// Entry point.
int main(void)
{
    std::cout << "###  UDP socket data receiver  ###" << std::endl;
    std::cout << "###  UdpSocket class v" << cr::clib::UdpSocket::getVersion() << std::endl << std::endl;

    // Enter UDP port.
    int udpPort = 0;
    std::cout << "Enter UDP port: ";
    std::cin >> udpPort;

    // Enter wait data timeout.
    int waitDataTimeoutMs = 0;
    std::cout << "Enter wait data timeout ms: ";
    std::cin >> waitDataTimeoutMs;

    // Init UDP socket.
    cr::clib::UdpSocket udpSocket;
    if (!udpSocket.setHostAddr("127.0.0.1", udpPort))
    {
        std::cout << "ERROR: Host IP not set. Exit." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return -1;
    }
    if (!udpSocket.open(true, waitDataTimeoutMs))
    {
        std::cout << "ERROR: Udp socket not init. Exit." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return -1;
    }

    // Init variables.
    uint8_t inputData[g_maxNumBytesToRead];

    // Main loop.
    while (true)
    {
        // Read data.
        int bytes = udpSocket.readData(inputData, g_maxNumBytesToRead);

        // Check input data size.
        if (bytes <= 0)
        {
            std::cout << "No input data" << std::endl;
            continue;
        }

        // Display input data.
        std::cout << bytes << " bytes read: " ;
        for (int i = 0; i < bytes; ++i)
        {
            std::cout << std::hex << (int)inputData[i] << " ";
        }
        std::cout << std::endl << std::dec << std::endl;
    }
}


