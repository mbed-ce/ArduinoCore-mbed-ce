# Build and install variant-specific sources
set(VARIANT_SOURCES
	variant.cpp
	double_tap_usb_boot.cpp)
set(VARIANT_HEADERS
	pins_arduino.h
	pinmode_arduino.h)

add_library(arduino-variant STATIC ${VARIANT_SOURCES})
target_include_directories(arduino-variant PUBLIC .)
target_link_libraries(arduino-variant PUBLIC arduino-core)

install(FILES ${VARIANT_SOURCES} ${VARIANT_HEADERS} DESTINATION variants/${ARDUINO_VARIANT_NAME})