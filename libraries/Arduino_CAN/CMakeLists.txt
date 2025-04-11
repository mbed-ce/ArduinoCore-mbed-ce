set(ARDUINO_CAN_SOURCES
	src/Arduino_CAN.cpp)

add_library(arduino-Arduino_CAN STATIC ${ARDUINO_CAN_SOURCES})
target_link_libraries(arduino-Arduino_CAN PUBLIC arduino-core)
target_include_directories(arduino-Arduino_CAN PUBLIC src)

build_arduino_examples(Arduino_CAN examples)

install(DIRECTORY . DESTINATION libraries/Arduino_CAN)