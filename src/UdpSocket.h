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


namespace cr {
namespace clib {

/**
 * @brief UDP Socket class.
 */
class UdpSocket
{
public:

    typedef enum {
        SERVER,
        CLIENT
    }SocketType;

    /**
     * @brief Method to get string of current library version.
     *
     * @return String of current library version.
     */
    static std::string getVersion();

    /**
     * @brief Class constructor.
     *
     * @param type Server or Client socket type.
     */
    UdpSocket(SocketType type);

    /**
     * @brief Class destructor.
     */
    ~UdpSocket();

    /**
     * @brief Method to set destination address.
     *
     * @param dstIP Destination IP address.
     * @param dstPort Destination UDP port number.
     *
     * @return TRUE if the address and port are set. FALSE in case any errors.
     */
    bool setDstAddr(std::string dstIP, uint16_t dstPort);
    bool setDstAddr(sockaddr_in dstAddr);

    /**
     * @brief Method to set host address.
     *
     * @param hostIP Host IP address.
     * @param hostPort Host UDP port number.
     *
     * @return TRUE if the address and port are set. FALSE in case any errors.
     */
    bool setHostAddr(std::string hostIP, uint16_t hostPort);
    bool setHostAddr(sockaddr_in hostAddr);

    /**
     * @brief Method to open UDP socket.
     *
     * @param timeoutMs Wait data timeout in milliseconds.
     *
     * @return TRUE in case success, FALSE in case any errors.
     */
    bool open(uint16_t timeoutMs = 100);

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

    /**
     * @brief Method to check if UDP socket open.
     * @return TRUE if socket open or FALSE.
     */
    bool isOpen();

    /**
     * @brief Method to close UDP socket.
     */
    void close();

private:
    SocketType m_socketType;
    bool m_isHostAddrSet;
    bool m_isDstAddrSet;
    bool m_isOpen;
    // socket port num.
    uint16_t m_udpPortNum;
    // Host net address structure.
    struct sockaddr_in m_hostAddr;
    // Destination net address structure.
    struct sockaddr_in m_dstAddr;
    // Socket
#if defined(linux) || defined(__linux) || defined(__linux__)
    int m_sock;
#else
    SOCKET m_sock;
#endif

};

}
}

