#include <string>
#include <string.h>
#include "UdpSocket.h"
#include "UdpSocketVersion.h"



// Link namespaces.
using namespace cr::clib;



/// Get library version.
std::string UdpSocket::getVersion()
{
    return UDP_SOCKET_VERSION;
}



/// Class constructor.
UdpSocket::UdpSocket()
{
    // Resed address structures.
    memset(&m_dstAddr, 0, sizeof(sockaddr_in));
    memset(&m_srcAddr, 0, sizeof(sockaddr_in));
}



/// Class destructor.
UdpSocket::~UdpSocket()
{
    // Close socket.
    close();
}



/// Open socket.
bool UdpSocket::open(uint16_t port,
                     bool serverType,
                     std::string dstIp,
                     int timeoutMsec)
{
    // Close socket first.
    close();

    // Init params in Windows OS.
#if defined(linux) || defined(__linux) || defined(__linux__)
#else
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return false;
#endif

    // Init socket.
    m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Close socket in case fail initialization
#if defined(linux) || defined(__linux) || defined(__linux__)
    if (m_sock == -1)
    {
#else
    if (m_sock == INVALID_SOCKET)
    {
        WSACleanup();
#endif
        return false;
    }

    // Bing socket if server type.
    if (serverType)
    {
        struct sockaddr_in hostAddr;
        memset(&hostAddr, 0, sizeof(sockaddr_in));
        hostAddr.sin_family = AF_INET;
        std::string ip = "0.0.0.0";
        if (!inet_pton(AF_INET, ip.c_str(), &hostAddr.sin_addr))
            return false;
        hostAddr.sin_port = htons(port);

#if defined(linux) || defined(__linux) || defined(__linux__)
        if (::bind(m_sock, (struct sockaddr*)&hostAddr, sizeof(hostAddr)) < 0)
        {
#else
        if (::bind(m_sock, (SOCKADDR*)&hostAddr, sizeof(hostAddr)) ==
                   SOCKET_ERROR)
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
    timeparams.tv_sec = timeoutMsec / 1000;
    // Timeout in microseconds for read data from socket.
    timeparams.tv_usec = timeoutMsec % 1000;
    if (timeoutMsec != 0)
    {
        // Close socket in case error
        if (setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO,
                       (const char*)&timeparams, sizeof(timeval)) < 0)
        {
            ::close(m_sock);
#else
    // Init timeouts
    // Timeout in milliseconds for read data from socket.
    DWORD _timeout = timeoutMsec;
    if (_timeout != 0)
    {
        // Close socket in case error
        if (setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO,
                       (const char*)&_timeout, sizeof(_timeout)) < 0)
        {
            closesocket(m_sock);
            WSACleanup();
#endif
            return false;
        }
    }

#if defined(linux) || defined(__linux) || defined(__linux__)
    int trueflag = 1;
    if (setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST,
                   &trueflag, sizeof(trueflag)) < 0)
    {
        ::close(m_sock);
#else
    const char trueflag = 1;
    // Close socket in case error
    if (setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST,
                   &trueflag, sizeof(trueflag)) < 0) {
        closesocket(m_sock);
        WSACleanup();
#endif
        return false;
    }

    // Set dst address.
    memset(&m_dstAddr, 0, sizeof(sockaddr_in));
    m_dstAddr.sin_family = AF_INET;
    if (!inet_pton(AF_INET, dstIp.c_str(), &m_dstAddr.sin_addr))
    {
        close();
        return false;
    }
    m_dstAddr.sin_port = htons(port);

    // Init flag
    m_isOpen = true;

    return true;
}



// Read data.
int UdpSocket::read(uint8_t* data, int size, sockaddr_in* srcAddr)
{
    // Check if socket not open.
    if (!m_isOpen)
        return -1;

    // Wait and read data from socket.
    int ret = 0;
#if defined(linux) || defined(__linux) || defined(__linux__)
    socklen_t _srcAddrSize = sizeof (m_srcAddr);
    ret = recvfrom(m_sock, reinterpret_cast<char*>(data), size, 0,
                   (struct sockaddr*)&m_srcAddr, &_srcAddrSize);
#else
    int _srcAddrSize = sizeof(m_srcAddr);
    ret = recvfrom(m_sock, reinterpret_cast<char*>(data), size, 0,
                   (struct sockaddr*)&m_srcAddr, &_srcAddrSize);
#endif

    if (srcAddr != nullptr)
        memcpy(srcAddr, &m_srcAddr, _srcAddrSize);

    return ret;
}



// Send data.
int UdpSocket::send(uint8_t* data, int size, sockaddr_in* dstAddr)
{
    // Check if socket not open.
    if (!m_isOpen)
        return -1;

    // Send data.
    if (dstAddr != nullptr)
        return sendto(m_sock, reinterpret_cast<char*>(data), size, 0,
                      (struct sockaddr*)dstAddr, sizeof(dstAddr));
    else
        return sendto(m_sock, reinterpret_cast<char*>(data), size, 0,
                      (struct sockaddr*)&m_dstAddr, sizeof(m_dstAddr));
}



/// Close socket.
void UdpSocket::close()
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



/// Check open status.
bool UdpSocket::isOpen()
{
    return m_isOpen;
}



/// Get IP of data source.
std::string UdpSocket::getIp(sockaddr_in* srcAddr)
{
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(srcAddr->sin_addr), str, INET_ADDRSTRLEN);

    return std::string(str);
}



/// Get UDP port of data source.
int UdpSocket::getPort(sockaddr_in* srcAddr)
{
    return htons(srcAddr->sin_port);
}




