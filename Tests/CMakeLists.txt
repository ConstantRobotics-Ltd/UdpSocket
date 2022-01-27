cmake_minimum_required(VERSION 3.13)



###############################################################################
## PROJECT
## name and version
###############################################################################
project(Tests LANGUAGES CXX)



###############################################################################
## CONFIGURATION
## project configuration
###############################################################################
SET(${PARENT}_UDP_SOCKET_TESTS_DATA_RECEIVER        ON CACHE BOOL "" FORCE)
SET(${PARENT}_UDP_SOCKET_TESTS_DATA_SENDER          ON CACHE BOOL "" FORCE)



###############################################################################
## INCLUDING SUBDIRECTORIES
## Adding subdirectories according to the configuration
###############################################################################
if (${PARENT}_UDP_SOCKET_TESTS_DATA_RECEIVER)
    add_subdirectory(UdpSocketDataReceiver)
endif()

if (${PARENT}_UDP_SOCKET_TESTS_DATA_SENDER)
    add_subdirectory(UdpSocketDataSender)
endif()
