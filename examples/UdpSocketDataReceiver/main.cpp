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
    cout<< "Data receiver v" << UdpSocket::getVersion() << endl << endl;

    // Set UDP port.
    int port = 0;
    cout << "Enter UDP port: ";
    cin >> port;

    // Set wait data timeout.
    int timeoutMsec = 0;
    cout << "Enter wait data timeout, msec: ";
    cin >> timeoutMsec;

    // Init UDP socket: server, default destination IP.
    UdpSocket udpSocket;
    if (!udpSocket.open(port, true, "127.0.0.1", timeoutMsec))
    {
        cout << "ERROR: Can't init UDP socket. Exit." << endl;
        this_thread::sleep_for(seconds(1));
        return -1;
    }

    // Init variables.
    const int bufferSize = 1024;
    uint8_t data[bufferSize];

    // Main loop.
    while (true)
    {
        // Read data. Max wait time = timeoutMsec.
        struct sockaddr_in addr;
        int bytes = udpSocket.read(data, bufferSize, &addr);

        // Check input data size.
        if (bytes <= 0)
        {
            cout << "No input data" << endl;
            continue;
        }

        // Show data about sender.
        cout << bytes << " bytes read from " << udpSocket.getIp(&addr) << "/" <<
                udpSocket.getPort(&addr) << endl;
    }
}


