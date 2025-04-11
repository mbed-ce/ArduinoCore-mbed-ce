set(GPS_SOURCES
	src/GPS.cpp
	MicroNMEA/src/MicroNMEA.cpp)

add_library(arduino-GPS STATIC ${GPS_SOURCES})
target_link_libraries(arduino-GPS PUBLIC arduino-core arduino-GSM)
target_include_directories(arduino-GPS PUBLIC src MicroNMEA/src)

install(DIRECTORY . DESTINATION libraries/GPS)
install(FILES MicroNMEA/src/MicroNMEA.cpp MicroNMEA/src/MicroNMEA.h DESTINATION libraries/GPS)