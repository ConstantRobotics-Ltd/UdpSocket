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
         * @brief Get string of current library version.
         * @return String of current library version.
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
         * @brief Open UPD socket.
         * @param isServer Server type - will be able to receive data.
         * @param timeoutMs Timeout ms to wait data when you call read data.
         * @return true If socket is open.
         * @return false If socket not open.
         */
        bool open(bool isServer = false, uint32_t timeoutMs = 100);

        /**
         * @brief Set host address (IP and port).
         * @param hostIp Host IP address.
         * @param port UDP port.
         * @return true if the address is set.
         * @return false if thre address not set.
         */
        bool setHostAddr(std::string hostIp, uint16_t port);

        /**
         * @brief Set host address (IP and port).
         * @param hostAddr Host address structure.
         * @return true if the address set.
         */
        bool setHostAddr(sockaddr_in hostAddr);

        /**
         * @brief Set destination address (IP and port).
         * @param dstIp Destination IP.
         * @return true if the address is set.
         * @return false if thre address not set.
         */
        bool setDstAddr(std::string dstIp, uint16_t port);

        /**
         * @brief Set destination address (IP and port).
         * @param dstAddr Destination address.
         * @return true if the address is set.
         * @return false if thre address not set.
         */
        bool setDstAddr(sockaddr_in dstAddr);

		/**
         * @brief Read data.
         * @param buf pointer to data buffer to copy data (not nullptr).
         * @param bufSize size of buf.
         * @param srcAddr pointer to structure from which the data was read.
		 * @return Number of read bytes or return -1 in case error.
		 */
        int readData(uint8_t* buf, uint32_t bufSize, sockaddr_in* srcAddr = nullptr);

		/**
         * @brief Send data.
         * @param data pointer to data to send.
         * @param dataSize size of data to send.
         * @param dstAddr pointer to structure to data to sendю
		 * @return Number of bytes sent or return -1 if UDP socket not open.
		 */
        int sendData(uint8_t* data, uint32_t dataSize, sockaddr_in* dstAddr = nullptr);

		/**
         * @brief Check if UDP socket open.
		 * @return TRUE if socke open or FALSE.
		 */
		bool isOpen(); 

		/**
         * @brief close - Methos to close UDP socket.
		 */
        void close();

	private:

        bool m_isOpen;                  // Open flag.
        struct sockaddr_in m_hostAddr;  // Host net address structure.
        struct sockaddr_in m_dstAddr;   // Destination net address structure.
#if defined(linux) || defined(__linux) || defined(__linux__)
        int m_sock;                     // Socket.
#else
        SOCKET m_sock;                  // Socket.
#endif
	};

}
}

