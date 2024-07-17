![udp_socket_web_logo](./static/udp_socket_web_logo.png)



# **UdpSocket C++ library**

**v3.1.3**



# Table of contents

- [Overview](#overview)
- [Versions](#versions)
- [UdpSocket class description](#udpsocket-class-description)
  - [UdpSocket class declaration](#udpsocket-class-declaration)
  - [getVersion method](#getversion-method)
  - [open method](#open-method)
  - [read method](#read-method)
  - [send method](#send-method)
  - [isOpen method](#isopen-method)
  - [close method](#close-method)
  - [getIp-method](#getip-method)
  - [getPort method](#getport-method)
- [Examples](#examples)
  - [Data sender](#data-sender)
  - [Data receiver](#data-receiver)
- [Build and connect to your project](#build-and-connect-to-your-project)



# Overview

**UdpSocket** C++ library provides methods to work with UDP port (open, close, send data and receive data). **UdpSocket** library is cross-platform and compatible with Windows and Linux OS. Main file **UdpSocket.h** includes declaration of **UdpSocket** class which provides methods to work with UDP socket. The library requires C++17 standard. The library doesn't have any third-party dependency. The library is licensed under the **Apache 2.0** license.



# Versions

**Table 1** - Library versions.

| Version | Release date | What's new                                                   |
| ------- | ------------ | ------------------------------------------------------------ |
| 1.0.0   | 10.10.2021   | First version                                                |
| 2.0.0   | 17.05.2022   | - Class interface changed.<br />- Added Tracker library to print debug info. |
| 3.0.0   | 13.05.2023   | - Class interface changed.<br />- Tracer library excluded.   |
| 3.0.1   | 13.05.2023   | - Fixed send data to external address.                       |
| 3.1.0   | 27.08.2023   | - License added.<br />- Repository made public.              |
| 3.1.1   | 26.03.2024   | - Documentation updated.                                     |
| 3.1.2   | 22.05.2024   | - Documentation updated.                                     |
| 3.1.3   | 17.07.2024   | - CMake updated.                                             |



# UdpSocket class description



## UdpSocket class declaration

**UdpSocket** interface class declared in **UdpSocket.h** file. Class declaration:

```cpp
namespace cr
{
namespace clib
{
/// UDP Socket class.
class UdpSocket
{
public:

    /// Get current library version.
    static std::string getVersion();

    /// Class constructor.
    UdpSocket();

    /// Class destructor.
    ~UdpSocket();

    /// Open UDP socket.
    bool open(uint16_t port,
              bool serverType = false,
              std::string dstIp = "127.0.0.1",
              int timeoutMsec = 100);

    /// Read data.
    int read(uint8_t* data,
             int size,
             sockaddr_in* srcAddr = nullptr);

    /// Send data.
    int send(uint8_t* data, int size, sockaddr_in* dstAddr = nullptr);

    /// Check if UDP socket open.
    bool isOpen();

    /// Close UDP socket.
    void close();

    /// Get IP of data source.
    std::string getIp(sockaddr_in* srcAddr);

    /// Get UDP port of data source.
    int getPort(sockaddr_in* srcAddr);
};
}
}
```



## getVersion method

The **getVersion()** method returns string of current class version. Method declaration:

```cpp
static std::string getVersion();
```

Method can be used without **UdpSocket** class instance:

```cpp
std::cout << "UdpSocket v: " << cr::clib::UdpSocket::getVersion() << std::endl;
```

Console output:

```bash
UdpSocket v: 3.1.3
```



## open method

The **open(...)** method initializes UDP socket. Method declaration:

```cpp
bool open(uint16_t port, bool serverType = false, std::string dstIp = "127.0.0.1", int timeoutMsec = 100);
```

| Parameter   | Value                                                        |
| ----------- | ------------------------------------------------------------ |
| port        | UDP port. Must have values from 0 to 65535.                  |
| serverType  | Socket type: TRUE - socket will be able to read and write data, FALSE - socket will be able only send data. |
| dstIp       | Destination IP address to send data.                         |
| timeoutMsec | Wait data timeout. Method sets timeout to UDP socket properties. Timeout determines behavior of **read(...)** method: method will wait input data maximum **timeoutMsec** milliseconds and will return negative results if no input data. |

**Returns:** TRUE if the UDP port open or FALSE if not.



## read method

The **read(...)** method designed to read (wait) input data. After receiving input data the method will return control immediately or will return control after timeout (set in [open(...)](#open-method) method) if no input data.  Method declaration:

```cpp
int read(uint8_t* data, int size, sockaddr_in* srcAddr = nullptr);
```

| Parameter | Value                                                        |
| --------- | ------------------------------------------------------------ |
| data      | Pointer to data buffer.                                      |
| size      | Size of data buffer and maximum data size to read from socket. |
| srcAddr   | Optional pointer to address structure. Method returns address structure of data source. User can use it to send data back to data source. |

**Returns:** Number of bytes or **-1** if no input data or timeout expired (set in **open(...)** method) or UDP socket not open.



## send method

The **send(...)** method sends data. Method declaration:

```cpp
int send(uint8_t* data, int size, sockaddr_in* dstAddr = nullptr);
```

| Parameter | Value                                                        |
| --------- | ------------------------------------------------------------ |
| data      | Pointer to data buffer.                                      |
| size      | Size of data to send.                                        |
| dstAddr   | Optional pointer to address structure. If address structure provide method will send data to this address. |

**Returns:** Number of bytes sent or **-1** if data not sent or UDP socket not open.



## isOpen method

The **isOpen()** method returns UDP socket open status. Method declaration:

```cpp
bool isOpen();
```

**Returns:** TRUE if UPD socket open or FALSE if not.



## close method

The **close()** method closes socket if it open. Method declaration:

```cpp
void close();
```



## getIp method

The **getIp(...)** method extracts IP address from address structure. Method declaration:

```cpp
std::string getIp(sockaddr_in* srcAddr);
```

| Parameter | Value                         |
| --------- | ----------------------------- |
| srcAddr   | Pointer to address structure. |

**Returns:** IP string.



## getPort method

The **getPort(...)** method extracts UDP port from address structure. Method declaration:

```cpp
int getPort(sockaddr_in* srcAddr);
```

| Parameter | Value                         |
| --------- | ----------------------------- |
| srcAddr   | Pointer to address structure. |

**Returns:** UDP port.



# Examples



## Data sender

Test application shows how to create socket only to send data. Test application send random data periodically.

```cpp
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
```



## Data receiver

Test application shows how to create socket to read and send. Test application receives data and shows info about sender.

```cpp
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
```



# Build and connect to your project

Typical commands to build **UdpSocket** library (on Linux OS):

```bash
cd UdpSocket
mkdir build
cd build
cmake ..
make
```

If you want connect **UdpSocket** library to your CMake project as source code you can make follow. For example, if your repository has structure:

```bash
CMakeLists.txt
src
    CMakeList.txt
    yourLib.h
    yourLib.cpp
```

Create **3rdparty** folder in your repository and copy **UdpSocket** repository folder there. New structure of your repository:

```bash
CMakeLists.txt
src
    CMakeList.txt
    yourLib.h
    yourLib.cpp
3rdparty
    UdpSocket
```

Create CMakeLists.txt file in **3rdparty** folder. CMakeLists.txt should contain:

```cmake
cmake_minimum_required(VERSION 3.13)

################################################################################
## 3RD-PARTY
## dependencies for the project
################################################################################
project(3rdparty LANGUAGES CXX)

################################################################################
## SETTINGS
## basic 3rd-party settings before use
################################################################################
# To inherit the top-level architecture when the project is used as a submodule.
SET(PARENT ${PARENT}_YOUR_PROJECT_3RDPARTY)
# Disable self-overwriting of parameters inside included subdirectories.
SET(${PARENT}_SUBMODULE_CACHE_OVERWRITE OFF CACHE BOOL "" FORCE)

################################################################################
## CONFIGURATION
## 3rd-party submodules configuration
################################################################################
SET(${PARENT}_SUBMODULE_UDP_SOCKET                      ON  CACHE BOOL "" FORCE)
if (${PARENT}_SUBMODULE_UDP_SOCKET)
    SET(${PARENT}_UDP_SOCKET                            ON  CACHE BOOL "" FORCE)
    SET(${PARENT}_UDP_SOCKET_EXAMPLES                   OFF CACHE BOOL "" FORCE)
endif()

################################################################################
## INCLUDING SUBDIRECTORIES
## Adding subdirectories according to the 3rd-party configuration
################################################################################
if (${PARENT}_SUBMODULE_UDP_SOCKET)
    add_subdirectory(UdpSocket)
endif()
```

File **3rdparty/CMakeLists.txt** adds folder **UdpSocket** to your project and excludes examples from compiling (by default examples excluded from compiling if **UdpSocket** included as sub-repository). Your repository new structure will be:

```bash
CMakeLists.txt
src
    CMakeList.txt
    yourLib.h
    yourLib.cpp
3rdparty
    CMakeLists.txt
    UdpSocket
```

Next you need include folder 3rdparty in main **CMakeLists.txt** file of your repository. Add string at the end of your main **CMakeLists.txt**:

```cmake
add_subdirectory(3rdparty)
```

Next you have to include UdpSocket library in your **src/CMakeLists.txt** file:

```cmake
target_link_libraries(${PROJECT_NAME} UdpSocket)
```

Done!