set(USBMSD_HEADERS
	src/PluggableUSBMSD.h)
set(USBMSD_SOURCES
	src/Singleton.cpp
	src/USBMSD.cpp)

add_library(arduino-USBMSD STATIC ${USBMSD_SOURCES})
target_link_libraries(arduino-USBMSD PUBLIC
	arduino-core
	mbed-storage-blockdevice
	mbed-storage-fat
	mbed-storage-flashiap)
target_include_directories(arduino-USBMSD PUBLIC src)

install(FILES ${USBMSD_HEADERS} ${USBMSD_SOURCES} DESTINATION libraries/USBMSD/src)
install(DIRECTORY examples DESTINATION libraries/USBMSD)