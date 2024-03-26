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
    cout<< "Data receiver v" << UdpSocket::getVersion() << endl << endl;

    int port = 0;
    cout << "Set UDP port: ";
    cin >> port;

    int timeoutMsec = 0;
    cout << "Set wait data timeout, msec: ";
    cin >> timeoutMsec;

    // Open UDP socket: server mode (to receive data).
    UdpSocket udpSocket;
    if (!udpSocket.open(port, true, "127.0.0.1", timeoutMsec))
        return -1;

    // Main loop.
    uint8_t data[8192];
    while (true)
    {
        // Read data. Max wait time = timeoutMsec.
        struct sockaddr_in addr;
        int bytes = udpSocket.read(data, 8192, &addr);

        // Check input data size.
        if (bytes <= 0)
        {
            cout << "No input data" << endl;
            continue;
        }

        // Show sender info.
        cout << bytes << " bytes read from " <<
        udpSocket.getIp(&addr) << "/" << udpSocket.getPort(&addr) << endl;
    }
}