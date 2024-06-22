set(PORTENTA_SDRAM_SOURCES
	src/ram_internal.c
	src/SDRAM.cpp)

add_library(arduino-Portenta_SDRAM STATIC ${PORTENTA_SDRAM_SOURCES})
target_link_libraries(arduino-Portenta_SDRAM PUBLIC arduino-core arduino-ea_malloc)
target_include_directories(arduino-Portenta_SDRAM PUBLIC src)

build_arduino_examples(Portenta_SDRAM examples)

install(DIRECTORY . DESTINATION libraries/Portenta_SDRAM)
