#include "UdpSocket.h"
#include <iostream>
#include <string>
#include "Tracer.h"

#include "UdpSocketVersion.h"

using namespace cr::clib;
using namespace cr::utils;

static std::shared_ptr<Tracer>
g_tracer = Tracer::createTracer(WARNING, "UdpSocket");

std::string UdpSocket::getVersion()
{
    return UDP_SOCKET_VERSION;
}


UdpSocket::UdpSocket(SocketType type) :
    m_socketType(type),
    m_isHostAddrSet(false),
    m_isDstAddrSet(false),
    m_isOpen(false),
    m_udpPortNum(0),
    m_sock(0)
{
    memset(&m_hostAddr, 0, sizeof(sockaddr_in));
    memset(&m_dstAddr, 0, sizeof(sockaddr_in));
}


UdpSocket::~UdpSocket()
{
    close();
}


bool UdpSocket::open(uint16_t timeoutMs)
{

    int retVal;


    if (m_socketType == SERVER && !m_isHostAddrSet)
    {
        TRACE(g_tracer, LONG_PRINT, EXCEPTION,
              "%s\n", "Host IP Addr or Port is not setup");
        return false;
    }

    if (m_socketType == CLIENT && !m_isDstAddrSet)
    {
        TRACE(g_tracer, LONG_PRINT, EXCEPTION,
              "%s\n", "Dst IP Addr or Port is not setup");
        return false;
    }

    // Check if socket already open.
    if (m_isOpen)
    {
        TRACE(g_tracer, LONG_PRINT, EXCEPTION,
              "%s\n", "Socket already open");
        return false;
    }

    // Init params in Windows OS.
#if defined(linux) || defined(__linux) || defined(__linux__)
#else
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        TRACE(g_tracer, LONG_PRINT, EXCEPTION,
              "%s%d\n", "WSAStartup failed: ", iResult);
        return false;
    }
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
        TRACE(g_tracer, LONG_PRINT, EXCEPTION,
              "%s\n", "Socket creation failed");
        return false;
    }

    // Bing socket if server type.
    if (m_socketType == SERVER) {
#if defined(linux) || defined(__linux) || defined(__linux__)
        retVal = ::bind(m_sock, (struct sockaddr*)&m_hostAddr,sizeof(m_hostAddr));
#else
        retVal = ::bind(m_sock, (SOCKADDR*)&m_hostAddr, sizeof(m_hostAddr));
#endif
#if defined(linux) || defined(__linux) || defined(__linux__)
        if (retVal < 0) {
#else
        if (retVal == SOCKET_ERROR) {
#endif
            // Close socket in case fail bind.
#if defined(linux) || defined(__linux) || defined(__linux__)
            ::close(m_sock);
#else
            closesocket(m_sock);
            WSACleanup();
#endif
            TRACE(g_tracer, LONG_PRINT, EXCEPTION,
                  "%s\n", "Socket bind exception");
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
            TRACE(g_tracer, LONG_PRINT, EXCEPTION,
                  "%s\n", "Socket set timeout exception");
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
        TRACE(g_tracer, LONG_PRINT, EXCEPTION,
              "%s\n", "Socket set broadcast exception");
        return false;
    }

    // Init flag
    m_isOpen = true;

    return true;
}


bool UdpSocket::setDstAddr(std::string dstIP, uint16_t dstPort)
{
    m_dstAddr.sin_family = AF_INET;

    if (!inet_pton(AF_INET, dstIP.c_str(), &m_dstAddr.sin_addr))
    {
        TRACE(g_tracer, LONG_PRINT, EXCEPTION,
              "%s%s\n", "Invalid Destination IP Addr: ", dstIP.c_str());
        return false;
    }

    if (dstPort >= 1 && dstPort <= 65535)
    {
        m_dstAddr.sin_port = htons(dstPort);
    }else
    {
        TRACE(g_tracer, LONG_PRINT, EXCEPTION,
              "%s%d\n", "Invalid Destination Port: ", dstPort);
        return false;
    }

    m_isDstAddrSet = true;
    return m_isDstAddrSet;
}


bool UdpSocket::setDstAddr(sockaddr_in dstAddr)
{
    memcpy(&m_dstAddr, &dstAddr, sizeof (sockaddr_in));
    m_isDstAddrSet = true;
    return m_isDstAddrSet;
}

bool UdpSocket::setHostAddr(std::string hostIP, uint16_t hostPort)
{
    m_hostAddr.sin_family = AF_INET;

    if (!inet_pton(AF_INET, hostIP.c_str(), &m_hostAddr.sin_addr))
    {
        TRACE(g_tracer, LONG_PRINT, EXCEPTION,
              "%s%s\n", "Invalid Host IP Addr: ", hostIP.c_str());
        return false;
    }

    if (hostPort >= 1 && hostPort <= 65535)
    {
        m_hostAddr.sin_port = htons(hostPort);
    }else
    {
        TRACE(g_tracer, LONG_PRINT, EXCEPTION,
              "%s%d\n", "Invalid Host Port: ", hostPort);
        return false;
    }

    m_isHostAddrSet = true;
    return m_isHostAddrSet;
}

bool UdpSocket::setHostAddr(sockaddr_in hostAddr)
{
    memcpy(&m_hostAddr, &hostAddr, sizeof (sockaddr_in));
    m_isHostAddrSet = true;
    return m_isHostAddrSet;
}


int UdpSocket::readData(uint8_t* buf, uint32_t bufSize, sockaddr_in* srcAddr)
{
    // Check if socket not open.
    if (!m_isOpen)
    {
        TRACE(g_tracer, LONG_PRINT, EXCEPTION,
              "%s\n", "Socket not open");
		return -1;
    }

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

    if (srcAddr != nullptr)
        memcpy(srcAddr, &_srcAddr, _srcAddrSize);

    return ret;
}


int UdpSocket::sendData(uint8_t* data, uint32_t dataSize,
                            sockaddr_in* dstAddr)
{
    // Check if socket not open.
    if (!m_isOpen)
    {
        TRACE(g_tracer, LONG_PRINT, EXCEPTION,
              "%s\n", "Socket not open");
        return -1;
    }

	// Send data.
    if (dstAddr != nullptr)
        return sendto(m_sock, reinterpret_cast<char*>(data), dataSize, 0,
                      (struct sockaddr*)dstAddr, sizeof(dstAddr));
    else
        return sendto(m_sock, reinterpret_cast<char*>(data), dataSize, 0,
                      (struct sockaddr*)&m_dstAddr, sizeof(m_dstAddr));
}


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
	// Reset flags.
    m_udpPortNum = 0;
    m_isOpen = false;
}


bool UdpSocket::isOpen()
{
    return m_isOpen;
}

