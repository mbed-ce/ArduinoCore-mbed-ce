add_library(arduino-Ethernet STATIC
	src/Ethernet.cpp
	src/EthernetServer.cpp)

target_link_libraries(arduino-Ethernet PUBLIC arduino-core arduino-SocketWrapper)
target_include_directories(arduino-Ethernet PUBLIC src)

build_arduino_examples(Ethernet examples)

target_link_libraries(Ethernet-example-BarometricPressureWebServer arduino-SPI)

# Workaround for conflict between SPI.h from Mbed and SPI.h from Arduino.
include_directories(../SPI)

install(DIRECTORY . DESTINATION libraries/Ethernet)