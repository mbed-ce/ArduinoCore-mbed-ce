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

if(ARDUINO_VARIANT_NAME STREQUAL "ARDUINO_NANO33BLE")
	build_arduino_examples(USBMSD examples/Nano33BLE_FlashMassStorage)
endif()

if("COMPONENT_QSPIF=1" IN_LIST MBED_TARGET_DEFINITIONS)
	build_arduino_examples(USBMSD examples/Nano33BLE_FlashMassStorage)
endif()

install(DIRECTORY . DESTINATION libraries/USBMSD)