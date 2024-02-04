# Install Arduino API and make a build system target for it
set(ARDUINO_API_SOURCES
	ArduinoCore-API/api/CanMsg.cpp
	ArduinoCore-API/api/CanMsgRingbuffer.cpp
	ArduinoCore-API/api/IPAddress.cpp
	ArduinoCore-API/api/PluggableUSB.cpp
	ArduinoCore-API/api/Print.cpp
	ArduinoCore-API/api/Stream.cpp
	ArduinoCore-API/api/String.cpp)

add_library(arduino-api STATIC ${ARDUINO_API_SOURCES})
target_link_libraries(arduino-api mbed-core-flags arduino-core)
target_include_directories(arduino-api PUBLIC ArduinoCore-API)

install(DIRECTORY ArduinoCore-API/api DESTINATION cores/arduino)