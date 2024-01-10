set(KERNELDEBUG_SOURCES
	src/KernelDebug.cpp
	src/armv7-m_asm.S)

add_library(arduino-KernelDebug STATIC ${KERNELDEBUG_SOURCES})
target_link_libraries(arduino-KernelDebug PUBLIC arduino-core arduino-MRI)
target_include_directories(arduino-KernelDebug PUBLIC src)

build_arduino_examples(KernelDebug examples)

install(DIRECTORY . DESTINATION libraries/KernelDebug)