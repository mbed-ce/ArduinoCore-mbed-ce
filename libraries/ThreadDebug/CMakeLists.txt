set(THREADDEBUG_SOURCES
	src/ThreadDebug.cpp
	src/ThreadDebug_asm.S)

add_library(arduino-ThreadDebug STATIC ${THREADDEBUG_SOURCES})
target_link_libraries(arduino-ThreadDebug PUBLIC arduino-core arduino-MRI)
target_include_directories(arduino-ThreadDebug PUBLIC src)

build_arduino_examples(ThreadDebug examples)

install(DIRECTORY . DESTINATION libraries/ThreadDebug)