set(EA_MALLOC_SOURCES
	malloc_freelist.c)

add_library(arduino-ea_malloc STATIC ${EA_MALLOC_SOURCES})
target_link_libraries(arduino-ea_malloc PUBLIC arduino-core)
target_include_directories(arduino-ea_malloc PUBLIC .)

install(DIRECTORY . DESTINATION libraries/ea_malloc)