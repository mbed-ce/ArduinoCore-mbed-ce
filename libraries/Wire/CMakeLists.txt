set(WIRE_SOURCES
	Wire.cpp)

add_library(arduino-Wire STATIC ${WIRE_SOURCES})
target_link_libraries(arduino-Wire PUBLIC arduino-core)
target_include_directories(arduino-Wire PUBLIC .)

build_arduino_examples(Wire examples)

install(DIRECTORY . DESTINATION libraries/Wire)