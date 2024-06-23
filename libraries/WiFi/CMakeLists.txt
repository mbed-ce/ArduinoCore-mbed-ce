set(WIFI_SOURCES
	src/WiFi.cpp
	src/WiFiServer.cpp)

add_library(arduino-WiFi STATIC ${WIFI_SOURCES})
target_link_libraries(arduino-WiFi PUBLIC
	arduino-core
	arduino-SocketWrapper
	arduino-STM32H747_System
	mbed-wifi)
target_include_directories(arduino-WiFi PUBLIC src)

build_arduino_examples(WiFi examples)

install(DIRECTORY . DESTINATION libraries/WiFi)
