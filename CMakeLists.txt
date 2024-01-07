# CMake build script used to compile Mbed and the Arduino core into a package

cmake_minimum_required(VERSION 3.20) # Need 3.20 for cmake_path()
cmake_policy(VERSION 3.20)

# Initialize Mbed OS build system. 
# Note: This block must be before the include of app.cmake
set(MBED_APP_JSON_PATH mbed_app.json5)

include(mbed-os/tools/cmake/app.cmake)
add_subdirectory(mbed-os)

project(ArduinoCore-mbed VERSION 1.0.0) # This is the version of the Mbed CE Arduino core, not of Mbed CE

if("${CMAKE_INSTALL_PREFIX}" STREQUAL "")
	message(FATAL_ERROR "Must provide a CMAKE_INSTALL_PREFIX to generate the package at.")
endif()
message(STATUS "Will generate Arduino core library at ${CMAKE_INSTALL_PREFIX}")

# Compile and install libraries needed by Arduino
# ---------------------------------------------------------

# Pass in version defines for the core
target_compile_definitions(mbed-core-flags INTERFACE
	CORE_MAJOR=${ArduinoCore-mbed_VERSION_MAJOR}
	CORE_MINOR=${ArduinoCore-mbed_VERSION_MINOR}
	CORE_PATCH=${ArduinoCore-mbed_VERSION_PATCH})

# Create static library out of mbed-os
# Note that this library MUST be linked with -Wl,--whole-archive to work.
# (CMake's poor support for this is why we use an object library for mbed-os in CMake)
add_library(mbed-os-static)
target_link_libraries(mbed-os-static mbed-os)
install(TARGETS mbed-os-static DESTINATION "variants/${MBED_TARGET}/libs")

# List of additional libraries to install and use for the Arduino core.
# Every library used by each driver/example must be on this list or it won't get installed.
set(MBED_LIBS_TO_INSTALL

	# Networking
	mbed-lwipstack
	mbed-netsocket-api
	mbed-mbedtls

	# Storage / Block Device
	mbed-storage
	mbed-storage-blockdevice
	mbed-storage-flashiap
	mbed-storage-filesystem
	mbed-storage-fat
	mbed-storage-kv-config
	mbed-storage-kv-global-api
	mbed-storage-littlefs
	mbed-storage-littlefs-v2
	mbed-storage-securestore
	mbed-storage-tdbstore

	# USB
	mbed-usb
	mbed-usb-cdc-ecm
	mbed-usb-msd
)

# We also need to know about any precompiled .a files
# under variants/xxx/libs
if("ARDUINO_NANO33BLE" IN_LIST MBED_TARGET_LABELS)
	set(ARDUINO_PRECOMPILED_A_FILES
		libcc_310_core.a
		libcc_310_ext.a
		libcc_310_trng.a)
else()
	set(ARDUINO_PRECOMPILED_A_FILES "")
endif()

# Minor hack: all of the Mbed optional libraries are marked as EXCLUDE_FROM_ALL, so they won't get built during the build
# phase, so it will error when trying to install them.  To fix this, mark them as dependencies of a target that is in ALL.
add_dependencies(mbed-os-static ${MBED_LIBS_TO_INSTALL})

install(TARGETS ${MBED_LIBS_TO_INSTALL} DESTINATION "variants/${MBED_TARGET}/libs")

# Generate compile option files for Arduino IDE
# ---------------------------------------------------------

# Iterate though the Mbed main build target and the optional targets and collect include dirs / defines / etc.
set(TARGETS_TO_SCAN mbed-os ${MBED_LIBS_TO_INSTALL})
set(SCANNED_INCLUDE_DIRS "")
set(SCANNED_DEFINES "")
foreach(TARGET ${TARGETS_TO_SCAN})
	get_property(TARGET_INCLUDE_DIRS TARGET ${TARGET} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
	get_property(TARGET_COMPILE_DEFINITIONS TARGET ${TARGET} PROPERTY INTERFACE_COMPILE_DEFINITIONS)

	# For include dirs, if the include dir points to generated-headers, that's in the bin dir so we will handle it separately.
	foreach(INCLUDE_DIR ${TARGET_INCLUDE_DIRS})
		if(NOT "${INCLUDE_DIR}" MATCHES "generated-headers")
			list(APPEND SCANNED_INCLUDE_DIRS ${INCLUDE_DIR})
		endif()
	endforeach()

	# Defines can just be passed through
	list(APPEND SCANNED_DEFINES ${TARGET_COMPILE_DEFINITIONS})
endforeach()

# Generate defines file.
# TODO do we need to escape defines with spaces?
set(DEFINES_TXT_CONTENTS "")
foreach(DEFINE ${SCANNED_DEFINES})
	string(APPEND DEFINES_TXT_CONTENTS "-D${DEFINE}\n")
endforeach()
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/defines.txt CONTENT ${DEFINES_TXT_CONTENTS})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/defines.txt DESTINATION "variants/${MBED_TARGET}")

# Generate includes file.
set(INCLUDES_TXT_CONTENTS "")
foreach(INCLUDE_DIR ${SCANNED_INCLUDE_DIRS})

	# Make each include path relative to the current source dir
	# (so it starts with the first path component after mbed-os/)
	cmake_path(RELATIVE_PATH INCLUDE_DIR BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os OUTPUT_VARIABLE REL_INCLUDE_DIR)

	string(APPEND INCLUDES_TXT_CONTENTS "-iwithprefixbefore/mbed/${REL_INCLUDE_DIR}\n")
endforeach()
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/includes.txt CONTENT ${INCLUDES_TXT_CONTENTS})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/includes.txt DESTINATION "variants/${MBED_TARGET}")

# Copy sources and headers into the install dir as needed
# ---------------------------------------------------------

# Headers from Mbed.  Currently Mbed CE doesn't provide an easy way to list out the public headers, but we
# do have the list of all the directories being included, so we can just find all the headers in those directories
foreach(INCLUDE_DIR ${SCANNED_INCLUDE_DIRS})
	file(GLOB INCLUDE_FILES ${INCLUDE_DIR}/*.h)
	cmake_path(RELATIVE_PATH INCLUDE_DIR BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os OUTPUT_VARIABLE REL_INCLUDE_DIR)
	install(FILES ${INCLUDE_FILES} DESTINATION cores/arduino/mbed/${REL_INCLUDE_DIR})
endforeach()

# mstd headers need special handling since they don't end in .h
file(GLOB MBED_MSTD_HEADERS mbed-os/platform/cxxsupport/mstd*)
install(FILES ${MBED_MSTD_HEADERS} DESTINATION cores/arduino/mbed/platform/cxxsupport)

# mbed-target-config.h needs to be copied from the build folder
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mbed-os/mbed-target-config.h DESTINATION cores/arduino)

# Build subdirs
# ---------------------------------------------------------
add_subdirectory(ide-config-files)
add_subdirectory(extras)
add_subdirectory(variants)
add_subdirectory(cores)
add_subdirectory(svd)
add_subdirectory(libraries)

mbed_finalize_build()