set(USBMIDI_HEADERS
	PluggableUSBMIDI.h)
set(USBMIDI_SOURCES
	USBMIDI.cpp)

add_library(arduino-USBMIDI STATIC ${USBMIDI_SOURCES})
target_link_libraries(arduino-USBMIDI PUBLIC arduino-core)
target_include_directories(arduino-USBMIDI PUBLIC .)

build_arduino_examples(USBMIDI examples)

install(DIRECTORY . DESTINATION libraries/USBMIDI)