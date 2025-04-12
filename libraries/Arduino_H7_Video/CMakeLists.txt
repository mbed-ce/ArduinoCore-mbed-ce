set(ARDUINO_H7_VIDEO_SOURCES
	src/anx7625.cpp
	src/Arduino_H7_Video.cpp
	src/dsi.cpp
	src/edid.c
	src/H7DisplayShield.cpp
	src/st7701.cpp
	src/video_modes.c)

add_library(arduino-Arduino_H7_Video STATIC ${ARDUINO_H7_VIDEO_SOURCES})
target_link_libraries(arduino-Arduino_H7_Video PUBLIC arduino-core arduino-Portenta_SDRAM)
target_include_directories(arduino-Arduino_H7_Video PUBLIC src)

# For this library, we cannot build the examples as part of Mbed CE, because they rely on driver libraries
# which are maintained outside this repo.
install(DIRECTORY . DESTINATION libraries/Arduino_H7_Video)