#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include "UdpSocket.h"

// Link namespaces.
using namespace std;
using namespace cr::clib;
using namespace std::chrono;

// Entry point.
int main(void)
{
    cout<< "Data sender v" << UdpSocket::getVersion() << endl << endl;

    // Enter destination IP.
    string ip = "";
    cout << "Enter destination IP: ";
    cin >> ip;

    // Enter UDP port.
    int port = 0;
    cout << "Enter UDP port: ";
    cin >> port;

    // Enter sending data period ms.
    int cyclePeriodMsec = 0;
    cout << "Enter sending data period msec: ";
    cin >> cyclePeriodMsec;

    // Enter number of bytes.
    int numBytes = 0;
    cout << "Enter num bytes to send [0-8192]: ";
    cin >> numBytes;

    // Init UDP socket: clietn (only to send data), default destination IP.
    UdpSocket udpSocket;
    if (!udpSocket.open(port, false, ip))
    {
        cout << "ERROR: Can't init UDP socket. Exit." << endl;
        this_thread::sleep_for(seconds(1));
        return -1;
    }

    // Init variables.
    uint8_t* data = new uint8_t[numBytes];

    // Main loop.
    time_point<system_clock> startTime = system_clock::now();
    while (true)
    {
        // Prepare random data.
        for (int i = 0; i < numBytes; ++i)
            data[i] = (uint8_t)(rand() % 255);

        // Send data.
        cout << udpSocket.send(data, numBytes) << " bytes sent" << endl;

        // Wait according to parameters.
        int waitTime = (int)duration_cast<milliseconds>(system_clock::now() -
                                                        startTime).count();
        waitTime = cyclePeriodMsec - waitTime;
        if (waitTime > 0)
            this_thread::sleep_for(milliseconds(waitTime));
        startTime = system_clock::now();
    }

    return 1;
}
