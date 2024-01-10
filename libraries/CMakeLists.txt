
# Create build rules for any examples found with a given examples directory
function(build_arduino_examples LIBRARY_NAME EXAMPLES_DIR)
	file(GLOB_RECURSE EXAMPLE_INO_FILES "${EXAMPLES_DIR}/*.ino")
	foreach(EXAMPLE_FILE ${EXAMPLE_INO_FILES})
		cmake_path(GET EXAMPLE_FILE STEM EXAMPLE_NAME)

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
endif()


# Libraries for all targets
add_subdirectory(PDM)
add_subdirectory(Wire)
add_subdirectory(SPI)
add_subdirectory(Scheduler)
add_subdirectory(USBHID)
add_subdirectory(USBMIDI)
add_subdirectory(USBMSD)
add_subdirectory(SocketWrapper)
