cmake_minimum_required(VERSION 3.13)



###############################################################################
## PROJECT
## name and version
###############################################################################
project(examples LANGUAGES CXX)



###############################################################################
## INCLUDING SUBDIRECTORIES
## Adding subdirectories according to the configuration
###############################################################################
add_subdirectory(UdpSocketDataReceiver)
add_subdirectory(UdpSocketDataSender)
