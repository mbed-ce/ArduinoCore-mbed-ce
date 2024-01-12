# SFU OTA update library for RP2040.
# Note that the bootloader source is in the 'extras' directory, and it would be cool to compile
# it as part of the core build... but it needs different build options than the main Arduino core
# (no USB console, small C library), so it would need to be moved to its own repo.

set(SFU_SOURCES
	src/SFU.cpp)

add_library(arduino-SFU STATIC ${SFU_SOURCES})
target_link_libraries(arduino-SFU PUBLIC arduino-core mbed-storage-fat mbed-storage-flashiap)
target_include_directories(arduino-SFU PUBLIC src)

install(DIRECTORY . DESTINATION libraries/SFU)