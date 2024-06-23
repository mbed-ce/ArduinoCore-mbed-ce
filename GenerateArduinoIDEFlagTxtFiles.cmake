##
## This script scans the Mbed CMake targets and collects lists of compiler options.
## It then writes those lists out to text files for the Arduino IDE ot use
##

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

# Generate defines file (defines.txt)
# TODO do we need to escape defines with spaces?
set(DEFINES_TXT_CONTENTS "")
foreach(DEFINE ${SCANNED_DEFINES})
	string(APPEND DEFINES_TXT_CONTENTS "-D${DEFINE}\n")
endforeach()
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/defines.txt CONTENT ${DEFINES_TXT_CONTENTS})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/defines.txt DESTINATION "variants/${ARDUINO_VARIANT_NAME}")

# Generate includes file (includes.txt)
set(INCLUDES_TXT_CONTENTS "")
foreach(INCLUDE_DIR ${SCANNED_INCLUDE_DIRS})

	# Make each include path relative to the current source dir
	# (so it starts with the first path component after mbed-os/)
	cmake_path(RELATIVE_PATH INCLUDE_DIR BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os OUTPUT_VARIABLE REL_INCLUDE_DIR)

	# There is a conflict between Mbed and Arduino both providing SPI.h
	# Remove this specific include path so that you must #include <drivers/SPI.h> to get the Mbed version
	# from the Arduino IDE.
	if(REL_INCLUDE_DIR STREQUAL "drivers/./include/drivers")
		continue()
	endif()

	string(APPEND INCLUDES_TXT_CONTENTS "-iwithprefixbefore/mbed/${REL_INCLUDE_DIR}\n")
endforeach()
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/includes.txt CONTENT ${INCLUDES_TXT_CONTENTS})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/includes.txt DESTINATION "variants/${ARDUINO_VARIANT_NAME}")

# Generate compile options files (cflags.txt and cxxflags.txt)
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

# Generate linker options file (ldflags.txt)

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
