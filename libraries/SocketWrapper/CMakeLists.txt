set(SOCKETWRAPPER_SOURCES
	src/utility/http_parser/http_parser.c
	src/MbedClient.cpp
	src/MbedServer.cpp
	src/MbedSSLClient.cpp
	src/MbedUdp.cpp
	src/SocketHelpers.cpp)

add_library(arduino-SocketWrapper STATIC ${SOCKETWRAPPER_SOURCES})
target_link_libraries(arduino-SocketWrapper PUBLIC
	arduino-core
	mbed-netsocket-api
	mbed-storage-fat
	mbed-storage-blockdevice)
target_include_directories(arduino-SocketWrapper PUBLIC src)

install(DIRECTORY . DESTINATION libraries/SocketWrapper)