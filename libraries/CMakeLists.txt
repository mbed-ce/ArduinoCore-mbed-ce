if("ARDUINO_NANO33BLE" IN_LIST MBED_TARGET_LABELS)
	add_subdirectory(MRI)
	add_subdirectory(ThreadDebug)
endif()


# Libraries for all targets
add_subdirectory(PDM)
add_subdirectory(Wire)
add_subdirectory(SPI)
add_subdirectory(Scheduler)
add_subdirectory(USBHID)
add_subdirectory(USBMIDI)
add_subdirectory(USBMSD)
