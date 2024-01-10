set(USBHID_HEADERS
	src/USBHID_Types.h
	src/PluggableUSBHID.h
	src/USBKeyboard.h
	src/USBMouse.h
	src/USBMouseKeyboard.h)
set(USBHID_SOURCES
	src/USBHID.cpp
	src/USBKeyboard.cpp
	src/USBMouse.cpp
	src/USBMouseKeyboard.cpp)

add_library(arduino-USBHID STATIC ${USBHID_SOURCES})
target_link_libraries(arduino-USBHID PUBLIC arduino-core)
target_include_directories(arduino-USBHID PUBLIC src)

build_arduino_examples(USBHID examples)

install(DIRECTORY . DESTINATION libraries/USBHID)