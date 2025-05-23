# Configure and install boards.txt.
# -------------------------------------------------------

# Generate library list
set(BOARDS_TXT_LIBRARY_LIST "")
foreach(MBED_LIB ${MBED_LIBS_TO_INSTALL})
	string(APPEND BOARDS_TXT_LIBRARY_LIST "\"{build.variant.path}/libs/lib${MBED_LIB}.a\" ")
endforeach()

# After all the optional libraries, we want to link Mbed itself (must use whole archive)
string(APPEND BOARDS_TXT_LIBRARY_LIST "-Wl,--whole-archive ")
string(APPEND BOARDS_TXT_LIBRARY_LIST "\"{build.variant.path}/libs/libmbed-os-static.a\" ")
string(APPEND BOARDS_TXT_LIBRARY_LIST "-Wl,--no-whole-archive ")

# Finally grab the binary dependencies
foreach(A_FILE ${ARDUINO_PRECOMPILED_A_FILES})
	string(APPEND BOARDS_TXT_LIBRARY_LIST "\"{build.variant.path}/libs/${A_FILE}\" ")
endforeach()

# Extract some memory bank info
string(REGEX MATCH "MBED_CONFIGURED_ROM_BANK_IROM1_START=(0x[a-zA-Z0-9]+);" _ "${MBED_CONFIG_DEFINITIONS}")
set(ROM_START_ADDR ${CMAKE_MATCH_1})
string(REGEX MATCH "MBED_CONFIGURED_ROM_BANK_IROM1_SIZE=(0x[a-zA-Z0-9]+);" _ "${MBED_CONFIG_DEFINITIONS}")
set(ROM_SIZE ${CMAKE_MATCH_1})

configure_file(board-txts/${ARDUINO_VARIANT_NAME}/boards.txt.in ${CMAKE_CURRENT_BINARY_DIR}/boards.txt @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/boards.txt DESTINATION .)

# Install target-specific config files
# -------------------------------------------------------

file(GLOB TARGET_SPECIFIC_CFGS "target-specific/${MBED_TARGET}/*")
install(FILES ${TARGET_SPECIFIC_CFGS} DESTINATION .)

# Install universal config files
# -------------------------------------------------------

install(FILES universal/programmers.txt DESTINATION .)

configure_file(universal/platform.txt.in ${CMAKE_CURRENT_BINARY_DIR}/platform.txt @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/platform.txt DESTINATION .)

