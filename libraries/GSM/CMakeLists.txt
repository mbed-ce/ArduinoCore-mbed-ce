set(GSM_SOURCES
	src/buffercmux.c
	src/CMUXClass.cpp
	src/GSM.cpp
	src/GSMClient.cpp
	src/GSMDebug.cpp
	src/GSMSSLClient.cpp
	src/GSMTrace.cpp
	src/PTYSerial.cpp)

add_library(arduino-GSM STATIC ${GSM_SOURCES})
target_link_libraries(arduino-GSM PUBLIC arduino-core arduino-SocketWrapper mbed-cellular)
target_include_directories(arduino-GSM PUBLIC src)

build_arduino_examples(GSM examples)
target_link_libraries(GSM-example-GNSSClient arduino-GPS)
target_link_libraries(GSM-example-MicroNMEA_integration arduino-GPS)

install(DIRECTORY . DESTINATION libraries/GSM)