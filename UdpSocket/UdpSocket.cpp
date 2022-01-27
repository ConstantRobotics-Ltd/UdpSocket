#include <iostream>
#include "UdpSocket.h"
#include "UdpSocketVersion.h"



// Method to get class version.
std::string cr::clib::UdpSocket::getVersion()
{
    return std::to_string(UDP_SOCKET_MAJOR_VERSION) + "." +
           std::to_string(UDP_SOCKET_MINOR_VERSION) + "." +
           std::to_string(UDP_SOCKET_PATCH_VERSION);
}



// Class constructor.
cr::clib::UdpSocket::UdpSocket() :
    m_isOpen(false),
    m_udpPort(0),
    m_sock(0)
{
    // Reset host address structure.
    memset(&m_hostAddr, 0, sizeof(sockaddr_in));
    m_hostAddr.sin_family = AF_INET;
    std::string hostIp = "127.0.0.1";
    inet_pton(AF_INET, hostIp.c_str(), &m_hostAddr.sin_addr);
    m_hostAddr.sin_port = htons(m_udpPort);

    // Reset destination address structure.
    memset(&m_dstAddr, 0, sizeof(sockaddr_in));
    m_dstAddr.sin_family = AF_INET;
    std::string dstIp = "127.0.0.1";
    inet_pton(AF_INET, dstIp.c_str(), &m_dstAddr.sin_addr);
    m_dstAddr.sin_port = htons(m_udpPort);
}



// Class destructor.
cr::clib::UdpSocket::~UdpSocket()
{
    close();
}



// Method to open socket.
bool cr::clib::UdpSocket::open(uint16_t port, bool isServer, uint32_t timeoutMs)
{
    // Init variables.
    int retVal = 0;

    // Init port.
    if (port == 0)
        return false;
    m_udpPort = port;
    m_hostAddr.sin_port = htons(m_udpPort);
    m_dstAddr.sin_port = htons(m_udpPort);

    // Init params in Windows OS.
#if defined(linux) || defined(__linux) || defined(__linux__)
#else
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
        return false;
#endif

    // Init socket.
    m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Close socket in case fail initialization
#if defined(linux) || defined(__linux) || defined(__linux__)
    if (m_sock == -1) {
#else
    if (m_sock == INVALID_SOCKET)
    {
        WSACleanup();
#endif
        return false;
    }

    // Bing socket if server type.
    if (isServer)
    {
#if defined(linux) || defined(__linux) || defined(__linux__)
        retVal = ::bind(m_sock, (struct sockaddr*)&m_hostAddr,sizeof(m_hostAddr));
#else
        retVal = ::bind(m_sock, (SOCKADDR*)&m_hostAddr, sizeof(m_hostAddr));
#endif
#if defined(linux) || defined(__linux) || defined(__linux__)
        if (retVal < 0) {
#else
        if (retVal == SOCKET_ERROR)
        {
#endif
            // Close socket in case fail bind.
#if defined(linux) || defined(__linux) || defined(__linux__)
            ::close(m_sock);
#else
            closesocket(m_sock);
            WSACleanup();
#endif
            return false;
        }
    }

#if defined(linux) || defined(__linux) || defined(__linux__)
    // Init timeouts
    timeval timeparams;
    timeparams.tv_sec = timeoutMs / 1000;
    // Timeout in microseconds for read data from socket.
    timeparams.tv_usec = timeoutMs * 1000;
    if (timeoutMs != 0) {
        retVal = setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO,
                            (const char*)&timeparams, sizeof(timeval));
        // Close socket in case error
        if (retVal < 0) {
            ::close(m_sock);
#else
    // Init timeouts
    // Timeout in milliseconds for read data from socket.
    DWORD _timeout = timeoutMs;
    if (_timeout != 0) {
        retVal = setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO,
                            (const char*)&_timeout, sizeof(_timeout));
        // Close socket in case error
        if (retVal < 0) {
            closesocket(m_sock);
            WSACleanup();
#endif
            return false;
        }
    }

#if defined(linux) || defined(__linux) || defined(__linux__)
    int trueflag = 1;
    retVal = setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST,
                        &trueflag, sizeof trueflag);
    if (retVal < 0)
    {
        ::close(m_sock);
#else
    const char trueflag = 1;
    retVal = setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST,
                        &trueflag, sizeof(trueflag));
    // Close socket in case error
    if (retVal < 0) {
        closesocket(m_sock);
        WSACleanup();
#endif
        return false;
    }

    // Init flag
    m_isOpen = true;

    return true;
}



// Method to set host IP.
bool cr::clib::UdpSocket::setHostIp(std::string hostIp)
{
    m_hostAddr.sin_family = AF_INET;
    if (!inet_pton(AF_INET, hostIp.c_str(), &m_hostAddr.sin_addr))
        return false;
    m_hostAddr.sin_port = htons(m_udpPort);

    return true;
}



// Method to set host IP.
bool cr::clib::UdpSocket::setHostIp(sockaddr_in hostAddr)
{
    memcpy(&m_hostAddr, &hostAddr, sizeof(sockaddr_in));
    m_hostAddr.sin_port = htons(m_udpPort);

    return true;
}



// Method to set destination IP.
bool cr::clib::UdpSocket::setDstIp(std::string dstIp)
{
    m_dstAddr.sin_family = AF_INET;
    if (!inet_pton(AF_INET, dstIp.c_str(), &m_dstAddr.sin_addr))
        return false;
    m_dstAddr.sin_port = htons(m_udpPort);

    return true;
}



// Method to set destination IP.
bool cr::clib::UdpSocket::setDstIp(sockaddr_in dstAddr)
{
    memcpy(&m_dstAddr, &dstAddr, sizeof(sockaddr_in));
    dstAddr.sin_port = htons(m_udpPort);

    return true;
}



// Method to read data.
int cr::clib::UdpSocket::readData(uint8_t* buf, uint32_t bufSize, sockaddr_in* srcAddr)
{
    // Check if socket not open.
    if (!m_isOpen)
        return -1;

    // Wait and read data from socket.
    sockaddr_in _srcAddr;
    int ret = 0;
#if defined(linux) || defined(__linux) || defined(__linux__)
    socklen_t _srcAddrSize = sizeof (_srcAddr);
    memset(&_srcAddr, 0, _srcAddrSize);
    ret = recvfrom(m_sock, reinterpret_cast<char*>(buf), bufSize, 0,
                   (struct sockaddr*)&_srcAddr, &_srcAddrSize);
#else
    int _srcAddrSize = sizeof (_srcAddr);
    memset(&_srcAddr, 0, _srcAddrSize);
    ret = recvfrom(m_sock, reinterpret_cast<char*>(buf), bufSize, 0,
                   (struct sockaddr*)&_srcAddr, &_srcAddrSize);
#endif

    // Copu source address structure.
    if (srcAddr != nullptr)
        memcpy(srcAddr, &_srcAddr, _srcAddrSize);

    return ret;
}



// Method to send data.
int cr::clib::UdpSocket::sendData(uint8_t* data, uint32_t dataSize,
                            sockaddr_in* dstAddr)
{
    // Check if socket not open.
    if (!m_isOpen)
        return -1;

	// Send data.
    if (dstAddr != nullptr)
        return sendto(m_sock, reinterpret_cast<char*>(data), dataSize, 0,
                      (struct sockaddr*)dstAddr, sizeof(dstAddr));
    else
        return sendto(m_sock, reinterpret_cast<char*>(data), dataSize, 0,
                      (struct sockaddr*)&m_dstAddr, sizeof(m_dstAddr));
}



// Method to close socket.
void cr::clib::UdpSocket::close()
{
	// Close socket
    if (m_isOpen)
	{
#if defined(linux) || defined(__linux) || defined(__linux__)
        ::close(m_sock);
#else
        closesocket(m_sock);
		WSACleanup();
#endif
	}

    m_isOpen = false;
}



// Method to get socket status.
bool cr::clib::UdpSocket::isOpen()
{
    return m_isOpen;
}

