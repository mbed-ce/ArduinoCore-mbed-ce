set(THREADDEBUG_HEADERS
	src/ThreadDebug.h)
set(THREADDEBUG_SOURCES
	src/ThreadDebug.cpp
	src/ThreadDebug_asm.S)

add_library(arduino-ThreadDebug STATIC ${THREADDEBUG_SOURCES})
target_link_libraries(arduino-ThreadDebug PUBLIC arduino-core arduino-MRI)
target_include_directories(arduino-ThreadDebug PUBLIC src)

install(FILES ${THREADDEBUG_HEADERS} ${THREADDEBUG_SOURCES} DESTINATION libraries/ThreadDebug/src)
install(DIRECTORY examples DESTINATION libraries/ThreadDebug)