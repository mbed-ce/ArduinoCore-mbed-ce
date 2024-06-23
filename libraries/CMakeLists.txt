
# Create build rules for any examples found with a given examples directory
function(build_arduino_examples LIBRARY_NAME EXAMPLES_DIR) # ARGN = example names to skip building
	file(GLOB_RECURSE EXAMPLE_INO_FILES "${EXAMPLES_DIR}/*.ino")
	foreach(EXAMPLE_FILE ${EXAMPLE_INO_FILES})
		cmake_path(GET EXAMPLE_FILE STEM EXAMPLE_NAME)

		if("${EXAMPLE_NAME}" IN_LIST ARGN)
			continue()
		endif()

		# Create the executable, making sure that CMake knows that the .ino
		# file is C++
		add_executable(${LIBRARY_NAME}-example-${EXAMPLE_NAME} ${EXAMPLE_FILE})
		set_property(SOURCE ${EXAMPLE_FILE} PROPERTY LANGUAGE CXX)

		# Link libraries.  We will need the arduino core, Mbed OS, and the library itself.
		target_link_libraries(${LIBRARY_NAME}-example-${EXAMPLE_NAME}
			mbed-os
			arduino-${LIBRARY_NAME}
			arduino-core)
		mbed_set_post_build(${LIBRARY_NAME}-example-${EXAMPLE_NAME})
	endforeach()
endfunction(build_arduino_examples)

if("ARDUINO_NANO33BLE" IN_LIST MBED_TARGET_LABELS)
	add_subdirectory(MRI)
	add_subdirectory(ThreadDebug)
	add_subdirectory(KernelDebug)
	add_subdirectory(Nano33BLE_System)
	add_subdirectory(MLC)
	add_subdirectory(PDM)
endif()

if("RASPBERRY_PI_PICO" IN_LIST MBED_TARGET_LABELS)
	add_subdirectory(SFU)
	add_subdirectory(PDM)
endif()

if("ARDUINO_NICLA_SENSE_ME" IN_LIST MBED_TARGET_LABELS)
	add_subdirectory(Nicla_System)
	add_subdirectory(ea_malloc)
endif()

if("STM32H747_ARDUINO" IN_LIST MBED_TARGET_LABELS)
	add_subdirectory(STM32H747_System)
endif()

if("ARDUINO_GIGA" IN_LIST MBED_TARGET_LABELS)
	add_subdirectory(MRI)
	add_subdirectory(ea_malloc)
	add_subdirectory(PDM)
	add_subdirectory(WiFi)
	add_subdirectory(Portenta_SDRAM)
endif()

# Libraries for all targets
add_subdirectory(Basic)
add_subdirectory(Wire)
add_subdirectory(SPI)
add_subdirectory(Scheduler)

if("DEVICE_USBDEVICE=1" IN_LIST MBED_TARGET_DEFINITIONS)
	add_subdirectory(USBHID)
	add_subdirectory(USBMIDI)
	add_subdirectory(USBMSD)
endif()

add_subdirectory(SocketWrapper)
