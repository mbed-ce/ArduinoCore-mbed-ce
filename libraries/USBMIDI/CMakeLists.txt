set(USBMIDI_HEADERS
	PluggableUSBMIDI.h)
set(USBMIDI_SOURCES
	USBMIDI.cpp)

add_library(arduino-USBMIDI STATIC ${USBMIDI_SOURCES})
target_link_libraries(arduino-USBMIDI PUBLIC arduino-core)
target_include_directories(arduino-USBMIDI PUBLIC src)

install(FILES ${USBMIDI_HEADERS} ${USBMIDI_SOURCES} DESTINATION libraries/USBMIDI)
install(DIRECTORY examples DESTINATION libraries/USBMIDI)