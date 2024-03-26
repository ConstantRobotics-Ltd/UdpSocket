#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include "UdpSocket.h"

using namespace std;
using namespace cr::clib;
using namespace std::chrono;

int main(void)
{
    cout<< "Data sender v" << UdpSocket::getVersion() << endl << endl;

    string ip = "";
    cout << "Set destination IP: ";
    cin >> ip;

    int port = 0;
    cout << "Set UDP port: ";
    cin >> port;

    int cyclePeriodMsec = 0;
    cout << "Set sending data period msec: ";
    cin >> cyclePeriodMsec;

    int numBytes = 0;
    cout << "Set num bytes to send [0-8192]: ";
    cin >> numBytes;

    // Open UDP socket: client mode (only to send data).
    UdpSocket udpSocket;
    if (!udpSocket.open(port, false, ip))
        return -1;

    // Main loop.
    uint8_t* data = new uint8_t[numBytes];
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