#pragma once
#include <iostream>
#if defined(linux) || defined(__linux) || defined(__linux__)
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <WS2tcpip.h>
#endif
#include <string>



namespace cr
{
namespace clib
{
/**
 * @brief UDP Socket class.
 */
class UdpSocket
{
public:

    /**
     * @brief Get current library version.
     * @return String of current library version in format "X.Y.Z".
     */
    static std::string getVersion();

    /**
     * @brief Class constructor.
     */
    UdpSocket();

    /**
     * @brief Class destructor.
     */
    ~UdpSocket();

    /**
     * @brief Open UDP socket.
     * @param port UDP port.
     * @param serverType TRUE to send/receive data, FALSE only to send data.
     *        (socket will not be bind).
     * @param timeoutMsec Wait data timeout in milliseconds.
     * @return TRUE in socket open or FALSE if not.
     */
    bool open(uint16_t port,
              bool serverType = false,
              std::string dstIp = "127.0.0.1",
              int timeoutMsec = 100);

    /**
     * @brief Read data.
     * @param data Pointer to data buffer to copy data (not nullptr).
     * @param size Size of buffer.
     * @param srcAddr Pointer to structure from which the data was read.
     * @return Number of read bytes or return -1 in case error.
     */
    int read(uint8_t* data,
             int size,
             sockaddr_in* srcAddr = nullptr);

    /**
     * @brief Send data.
     * @param data Pointer to data to send.
     * @param size Size of data to send.
     * @param dstAddr Pointer to structure to data to send.
     * @return Number of bytes sent or return -1 if UDP socket not open.
     */
    int send(uint8_t* data, int size, sockaddr_in* dstAddr = nullptr);

    /**
     * @brief Check if UDP socket open.
     * @return TRUE if socket open or FALSE if not.
     */
    bool isOpen();

    /**
     * @brief Close UDP socket.
     */
    void close();

    /**
     * @brief Get IP of data source.
     * @param srcAddr Pointer to structure from which the data was read.
     * @return IP of data source.
     */
    std::string getIp(sockaddr_in* srcAddr);

    /**
     * @brief Get UDP port of data source.
     * @param srcAddr Pointer to structure from which the data was read.
     * @return UDP port of data source.
     */
    int getPort(sockaddr_in* srcAddr);

private:

    // Socket open flag.
    bool m_isOpen{false};
    // Destination net address structure.
    struct sockaddr_in m_dstAddr{0};
    // Source net address structure.
    struct sockaddr_in m_srcAddr{0};
    // Socket.
#if defined(linux) || defined(__linux) || defined(__linux__)
    int m_sock{0};
#else
    SOCKET m_sock{0};
#endif

};

}
}

