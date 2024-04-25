# CMake build script used to compile Mbed and the Arduino core into a package

cmake_minimum_required(VERSION 3.20) # Need 3.20 for cmake_path()
cmake_policy(VERSION 3.20)

# Initialize Mbed OS build system. 
# Note: This block must be before the include of app.cmake
set(MBED_APP_JSON_PATH mbed_app.json5)

include(mbed-os/tools/cmake/app.cmake)
add_subdirectory(mbed-os)

project(ArduinoCore-mbed-ce VERSION 1.1.0) # This is the version of the Mbed CE Arduino core, not of Mbed CE

# Figure out variant name.  This is generally the same as the mbed target, but we need
# to handle a few variations (e.g. the _SWD variants of the mbed targets)
if("ARDUINO_NANO33BLE" IN_LIST MBED_TARGET_LABELS)
	set(ARDUINO_VARIANT_NAME "ARDUINO_NANO33BLE")
elseif("RASPBERRY_PI_PICO" IN_LIST MBED_TARGET_LABELS)
	set(ARDUINO_VARIANT_NAME "RASPBERRY_PI_PICO")
elseif("ARDUINO_NICLA_SENSE_ME" IN_LIST MBED_TARGET_LABELS)
	set(ARDUINO_VARIANT_NAME "ARDUINO_NICLA_SENSE_ME")
else()
	message(FATAL_ERROR "This MBED_TARGET is currently not supported by the arduino core!")
endif()

message(STATUS "Will install Arduino core for ${ARDUINO_VARIANT_NAME} at ${CMAKE_INSTALL_PREFIX}")

# Compile and install libraries needed by Arduino
# ---------------------------------------------------------

# Pass in version defines for the core
target_compile_definitions(mbed-os PUBLIC
	CORE_MAJOR=${ArduinoCore-mbed-ce_VERSION_MAJOR}
	CORE_MINOR=${ArduinoCore-mbed-ce_VERSION_MINOR}
	CORE_PATCH=${ArduinoCore-mbed-ce_VERSION_PATCH})

# Flag to some Mbed and Arduino code that Arduino and Mbed are being used together.
# It can't be added as a "regular" Mbed define in mbed-target.config.h because there's
# some code in ArduinoBLE that looks for this define without including Arduino.h.
target_compile_definitions(mbed-os PUBLIC ARDUINO_ARCH_MBED=1)

# Create static library out of mbed-os
# Note that this library MUST be linked with -Wl,--whole-archive to work.
# (CMake's poor support for this is why we use an object library for mbed-os in CMake)
add_library(mbed-os-static)
target_link_libraries(mbed-os-static mbed-os)
install(TARGETS mbed-os-static DESTINATION "variants/${ARDUINO_VARIANT_NAME}/libs")

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
)

if("TARGET_USBDEVICE" IN_LIST MBED_TARGET_LABELS)
	list(APPEND MBED_LIBS_TO_INSTALL
		mbed-usb
		mbed-usb-cdc-ecm
		mbed-usb-msd)
endif()

if("FEATURE_BLE=1" IN_LIST MBED_TARGET_DEFINITIONS)

	# Note: There are circular dependencies between the BLE libraries, so we have to list some of them twice.
	list(APPEND MBED_LIBS_TO_INSTALL
		mbed-ble
		mbed-ble-cordio_ll
		mbed-ble-cordio
		mbed-ble
		mbed-ble-cordio_ll
		mbed-ble-cordio
		mbed-ble
		mbed-ble-cordio_ll
		mbed-ble-cordio)
endif()

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

set(MBED_UNIQUE_LIBS_TO_INSTALL ${MBED_LIBS_TO_INSTALL})
list(REMOVE_DUPLICATES MBED_UNIQUE_LIBS_TO_INSTALL)

# Minor hack: all of the Mbed optional libraries are marked as EXCLUDE_FROM_ALL, so they won't get built during the build
# phase, so it will error when trying to install them.  To fix this, mark them as dependencies of a target that is in ALL.
add_dependencies(mbed-os-static ${MBED_UNIQUE_LIBS_TO_INSTALL})

install(TARGETS ${MBED_UNIQUE_LIBS_TO_INSTALL} DESTINATION "variants/${ARDUINO_VARIANT_NAME}/libs")

# Generate compile option files for Arduino IDE
# ---------------------------------------------------------

# Iterate though the Mbed main build target and the optional targets and collect include dirs / defines / etc.
set(TARGETS_TO_SCAN mbed-os ${MBED_UNIQUE_LIBS_TO_INSTALL})
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
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/defines.txt DESTINATION "variants/${ARDUINO_VARIANT_NAME}")

# Generate includes file.
set(INCLUDES_TXT_CONTENTS "")
foreach(INCLUDE_DIR ${SCANNED_INCLUDE_DIRS})

	# Make each include path relative to the current source dir
	# (so it starts with the first path component after mbed-os/)
	cmake_path(RELATIVE_PATH INCLUDE_DIR BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os OUTPUT_VARIABLE REL_INCLUDE_DIR)

	string(APPEND INCLUDES_TXT_CONTENTS "-iwithprefixbefore/mbed/${REL_INCLUDE_DIR}\n")
endforeach()
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/includes.txt CONTENT ${INCLUDES_TXT_CONTENTS})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/includes.txt DESTINATION "variants/${ARDUINO_VARIANT_NAME}")

# Generate compile options files.
foreach(LANG C CXX)

	# The profile-specific compile options are set as a property on the mbed-core-flags
	# target so they get applied everywhere.
	# (see e.g. profiles/develop.cmake)
	get_property(MBED_${LANG}_PROFILE_COMPILE_FLAGS TARGET mbed-core-flags PROPERTY INTERFACE_COMPILE_OPTIONS)

	# Slightly janky, but we need to simulate expanding the generator expressions in this property.
	# Use a regex to replace the generator expressions for the language we want, and then
	# use another regex to get rid of the genexes for the other languages.
	string(REGEX REPLACE
		"\\$<\\$<COMPILE_LANGUAGE:${LANG}>:([^>]+)>" "\\1"
		MBED_${LANG}_PROFILE_COMPILE_FLAGS "${MBED_${LANG}_PROFILE_COMPILE_FLAGS}")
	string(GENEX_STRIP "${MBED_${LANG}_PROFILE_COMPILE_FLAGS}" MBED_${LANG}_PROFILE_COMPILE_FLAGS)

	# For the profile flags, we want to strip out the "-include;xxx/mbed-target-config.h" flag
	# as it contains a path specific to the build machine and isn't needed by Arduino anyway.
	# So, remove "-include" and the option after it.
	string(REGEX REPLACE
		"-include;[^;]+" ""
		MBED_${LANG}_PROFILE_COMPILE_FLAGS "${MBED_${LANG}_PROFILE_COMPILE_FLAGS}")

	# for the toolchain flags (processor target, warnings, etc) those get put in CMAKE_<LANG>_FLAGS
	# by the app.cmake toolchain file.
	separate_arguments(CMAKE_${LANG}_FLAGS_LIST NATIVE_COMMAND ${CMAKE_${LANG}_FLAGS})

	# Annoyingly, the "--std" argument won't be in these lists because it's added by CMake.
	# We'll have to do that ourselves.
	if(${LANG} STREQUAL "C")
		set(C_STD_ARGUMENT --std=gnu${CMAKE_C_STANDARD})
	else() # CXX
		set(CXX_STD_ARGUMENT --std=gnu++${CMAKE_CXX_STANDARD})
	endif()

	set(MBED_${LANG}_COMPILE_FLAGS
		${MBED_${LANG}_PROFILE_COMPILE_FLAGS}
		${CMAKE_${LANG}_FLAGS_LIST}
		${${LANG}_STD_ARGUMENT})

	# Write list to a file, with one element on each line
	list(JOIN MBED_${LANG}_COMPILE_FLAGS "\n" ${LANG}_FLAG_FILE_CONTENTS)
	string(TOLOWER "${LANG}flags.txt" ${LANG}_FLAG_FILE_NAME)
	file(GENERATE
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${${LANG}_FLAG_FILE_NAME}
		CONTENT "${${LANG}_FLAG_FILE_CONTENTS}")
	install(FILES
		${CMAKE_CURRENT_BINARY_DIR}/${${LANG}_FLAG_FILE_NAME}
		DESTINATION variants/${ARDUINO_VARIANT_NAME})
endforeach()

# Generate linker options file

# The profile-specific linker options are set as a property on the mbed-core-flags
# target so they get applied everywhere.
# (see e.g. profiles/develop.cmake)
get_property(MBED_PROFILE_LINKER_FLAGS TARGET mbed-core-flags PROPERTY INTERFACE_LINK_OPTIONS)

# for the toolchain flags (processor target, warnings, etc) those get put in CMAKE_<LANG>_FLAGS
# by the app.cmake toolchain file.
separate_arguments(CMAKE_EXE_LINKER_FLAGS_LIST NATIVE_COMMAND ${CMAKE_EXE_LINKER_FLAGS})

set(MBED_LINKER_FLAGS
	${MBED_PROFILE_LINKER_FLAGS}
	${CMAKE_EXE_LINKER_FLAGS_LIST})

# Write list to a file, with one element on each line
list(JOIN MBED_LINKER_FLAGS "\n" LD_FLAG_FILE_CONTENTS)
file(GENERATE
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ldflags.txt
	CONTENT "${LD_FLAG_FILE_CONTENTS}")
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/ldflags.txt
	DESTINATION variants/${ARDUINO_VARIANT_NAME})

# Copy sources and headers into the install dir as needed
# ---------------------------------------------------------

# Headers from Mbed.  Currently Mbed CE doesn't provide an easy way to list out the public headers, but we
# do have the list of all the directories being included, so we can just find all the headers
# in those directories and then install them in a matching structure
foreach(INCLUDE_DIR ${SCANNED_INCLUDE_DIRS})

	# It would be nice to just glob for "*.h".  However, there are some
	# cases where include dirs have subfolders, and those subfolders contain include files we care about.
	# However, we also don't want to GLOB_RECURSE, because that can go 10 levels
	# deep and pick up loads of include files we don't need (since the top level mbed-os folder is
	# an include dir).
	# So, for now, we compromise and go up to 3 levels deep.  Hopefully no include file in Mbed
	# is more than 3 levels deep from the include path.
	file(GLOB INCLUDE_FILES ${INCLUDE_DIR}/*.h)
	file(GLOB INCLUDE_FILES_SUBDIR ${INCLUDE_DIR}/*/*.h)
	file(GLOB INCLUDE_FILES_SUB_SUBDIR ${INCLUDE_DIR}/*/*/*.h)

	foreach(FILE ${INCLUDE_FILES} ${INCLUDE_FILES_SUBDIR} ${INCLUDE_FILES_SUB_SUBDIR})
		cmake_path(GET FILE PARENT_PATH FILE_DIRECTORY)
		cmake_path(RELATIVE_PATH FILE_DIRECTORY BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os OUTPUT_VARIABLE REL_INCLUDE_DIR)
		install(FILES ${FILE} DESTINATION cores/arduino/mbed/${REL_INCLUDE_DIR})
	endforeach()
endforeach()

# mstd headers need special handling since they don't end in .h
file(GLOB MBED_MSTD_HEADERS mbed-os/platform/cxxsupport/mstd*)
list(FILTER MBED_MSTD_HEADERS EXCLUDE REGEX ".*\\.cpp")
install(FILES ${MBED_MSTD_HEADERS} DESTINATION cores/arduino/mbed/platform/cxxsupport)

# mbed-target-config.h needs to be copied from the build folder
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mbed-os/generated-headers/mbed-target-config.h DESTINATION cores/arduino)

# Copy linker scriot.
# The path that the linker script is generated at is saved as a target property of mbed-os
get_property(MBED_OS_LINKER_SCRIPT TARGET mbed-os PROPERTY LINKER_SCRIPT_PATH)
install(FILES ${MBED_OS_LINKER_SCRIPT} DESTINATION variants/${ARDUINO_VARIANT_NAME} RENAME linker_script.ld)

# Build subdirs
# ---------------------------------------------------------
add_subdirectory(bootloaders)
add_subdirectory(ide-config-files)
add_subdirectory(extras)
add_subdirectory(variants)
add_subdirectory(cores)
add_subdirectory(svd)
add_subdirectory(libraries)
add_subdirectory(debugger)

# Set up packaging.  We want CMake to build a zip file containing the core.
set(CPACK_PACKAGE_NAME "ArduinoCore-mbed-ce-${ARDUINO_VARIANT_NAME}-${CMAKE_BUILD_TYPE}")
set(CPACK_PACKAGE_VERSION ${ArduinoCore-mbed-ce_VERSION})
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION})
set(CPACK_GENERATOR ZIP)

include(CPack)

mbed_finalize_build()