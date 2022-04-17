## CONTENTS
- [OVERVIEW](#overview)
- [API DESCRIPTION](#api-description)
  - [getVersion(..)](#getversion)
  - [UdpSocket(..)](#udpsocket)
  - [setDstAddr(..)](#setdstaddr)
  - [setHostAddr(..)](#sethostaddr)
  - [open(..)](#open)
  - [readData(..)](#readdata)
  - [sendData(..)](#senddata)
  - [isOpen(..)](#isopen)
  - [close(..)](#close)
- [USAGE EXAMPLE](#usage-example)
  - [Server Socket](#server-socket-type)
  - [Client Socket](#client-socket-type)
  

## OVERVIEW

This project makes it easy to work with a UDP socket to create client-server applications.

## API DESCRIPTION
The **UdpSocket** contains one class `UdpSocket` which can be used 
to simplify sending and receiving data over the UDP transport layer. 

A UdpSocket can be configured as a *server socket*, which will listen 
for data on a specific port, or a *client socket*, which will send data to a 
specific port. 

### getVersion(..)

```cpp
/**
 * @brief Method to get string of current version of library.
 *
 * @return String of current library version.
 */
static std::string getVersion();
```

##### Description

*Static method to get string of current version of library.*


### UdpSocket(..)

```cpp
// Socket Type
typedef enum {
    SERVER,
    CLIENT
}SocketType;

/**
 * @brief Class constructor.
 *
 * @param type Server or Client socket type.
 */
UdpSocket(SocketType type);
```

##### Description

*Class constructor, can be configured as a *server* or *client* socket*

### setDstAddr(..)

```cpp
/**
 * @brief Method to set destination address.
 *
 * @param dstIP Destination IP address.
 * @param dstPort Destination UDP port number.
 *
 * @return TRUE if the address and port are set. FALSE in case any errors.
 */
bool setDstAddr(std::string dstIP, uint16_t dstPort);
```

##### Description

*Method to set destination address (for Client-sockets)*


### setHostAddr(..)

```cpp
/**
 * @brief Method to set host address.
 *
 * @param hostIP Host IP address.
 * @param hostPort Host UDP port number.
 *
 * @return TRUE if the address and port are set. FALSE in case any errors.
 */
bool setHostAddr(std::string hostIP, uint16_t hostPort);
```

##### Description

*Method to set host address (for Server-sockets)*

### open(..)

```cpp
/**
 * @brief Method to open UDP socket.
 *
 * @param timeoutMs Wait data timeout in milliseconds.
 *
 * @return TRUE in case success, FALSE in case any errors.
 */
bool open(uint16_t timeoutMs = 100);
```

##### Description

*Method to open UDP socket.*

> Before calling this method, you must configure the socket with the setDstAddr() method for the client and setHostAddr() method for server socket.

### readData(..)

```cpp
/**
 * @brief Method to read data.
 *
 * @param buf pointer to data buffer to copy data (not nullptr).
 * @param bufSize size of buf.
 * @param srcAddr pointer to structure from which the data was read.
 *
 * @return Number of read bytes or return -1 in case error.
 */
int readData(uint8_t* buf, uint32_t bufSize,
             sockaddr_in* srcAddr = nullptr);
```

##### Description

*Method to read data.*

### sendData(..)

```cpp
/**
 * @brief Method to send data.
 *
 * @param data pointer to data to send.
 * @param dataSize size of data to send.
 * @param dstAddr pointer to structure to data to send.
 *
 * @return Number of bytes sent or return -1 if UDP socket not open.
 */
int sendData(uint8_t* data, uint32_t dataSize,
             sockaddr_in* dstAddr = nullptr);
```

##### Description

*Method to send data.*

### isOpen(..)

```cpp
/**
 * @brief Method to check if UDP socket open.
 * @return TRUE if socket open or FALSE.
 */
bool isOpen();
```

##### Description

*Method to check if UDP socket open.*

### close(..)

```cpp
/**
 * @brief Method to close UDP socket.
 */
void close();
```

##### Description

*Method to close UDP socket.*

## USAGE EXAMPLE

### Server Socket Type
The server socket type is created as follows:

```cpp
#include "UdpSocket.h"
using namespace cr::clib;

int main(void)
{
    UdpSocket server(UdpSocket::SERVER);

    // Set host network params.
    std::string hostIpAddr = "127.0.0.1";
    uint16_t hostUdpPort = 50000;
    server.setHostAddr(hostIpAddr, hostUdpPort);

    // Try open server socket.
    uint16_t timeout = 500; //< read timeout, [ms]
    if (!server.open(timeout))
    {
        std::cerr << "UDP-socket can not open!" << std::endl;
        return -1;
    }

    // Read data from clients.
    int ret = 0;
    sockaddr_in srcSockAddr = {0};
    uint32_t buffSize = 512; //< Any value.
    uint8_t* buff = new uint8_t[buffSize];
    while (true)
    {
        ret = server.readData(buff, buffSize, &srcSockAddr);
        if (ret <= 0) {
            std::cout << "No data received..." << std::endl;
            continue;
        }

        std::cout << ret << " bytes were received" << std::endl;

        // Print source address from which data was received.
        /* 
        char str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(srcSockAddr.sin_addr), str, INET_ADDRSTRLEN);
        std::cout << "Src IP Addr\t: " << str << std::endl;
        std::cout << "Src Port\t: " << ntohs(srcSockAddr.sin_port) << std::endl;
        */

        // Send received data to the client.
        server.setDstAddr(srcSockAddr);
        ret = server.sendData(buff, ret);
        if (ret > 0) 
            std::cout << ret << " bytes were sent" << std::endl;
        else 
            std::cerr << "Data sending error!" << std::endl;
    }
    delete[] buff;
}
```

### Client Socket Type
The client socket type is created as follows:

```cpp
#include "UdpSocket.h"
using namespace cr::clib;

int main(void)
{
    UdpSocket client(UdpSocket::CLIENT);

    // Set destination network params.
    std::string dstIpAddr = "127.0.0.1";
    uint16_t dstUdpPort = 50000;
    client.setDstAddr(dstIpAddr, dstUdpPort);

    // Try open client socket.
    uint16_t timeout = 1000; //< read timeout, [ms]
    if (!client.open(timeout))
    {
        std::cerr << "UDP-socket can not open!" << std::endl;
    }

    // Send data to server.
    int ret = 0;
    int testValue = 0;
    uint32_t dataSize = 512; //< Any value.
    uint8_t* data = new uint8_t[dataSize];
    while (true)
    {
        // Fill the data with an increasing value.
        memset(data, testValue++, dataSize);
        ret = client.sendData(data, dataSize);
        if (ret <= 0) {
            std::cerr << "Data sending error!" << std::endl;
            return -1;
        }

        std::cout << ret << " bytes were sent" << std::endl;

        // Try to receive answer.
        ret = client.readData(data, dataSize);
        if (ret > 0)
            std::cout << ret << " bytes were received" << std::endl;
        else
            std::cerr << "No data received..." << std::endl;
    }
    delete[] data;
}
```
