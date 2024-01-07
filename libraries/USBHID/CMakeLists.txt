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

add_library(arduino-UsbHid STATIC ${USBHID_SOURCES})
target_link_libraries(arduino-UsbHid PUBLIC arduino-core)
target_include_directories(arduino-UsbHid PUBLIC .)

install(FILES ${USBHID_HEADERS} ${USBHID_SOURCES} DESTINATION libraries/USBHID/src)
install(DIRECTORY examples DESTINATION libraries/USBHID)