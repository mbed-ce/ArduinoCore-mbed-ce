set(PORTENTA_SDCARD_SOURCES
	src/SDMMCBlockDevice.cpp
	src/BSP.c)

add_library(arduino-Portenta_SDCARD STATIC ${PORTENTA_SDCARD_SOURCES})
target_link_libraries(arduino-Portenta_SDCARD PUBLIC
	arduino-core)
target_include_directories(arduino-Portenta_SDCARD PUBLIC src)

build_arduino_examples(Portenta_SDCARD examples)

install(DIRECTORY . DESTINATION libraries/Portenta_SDCARD)
