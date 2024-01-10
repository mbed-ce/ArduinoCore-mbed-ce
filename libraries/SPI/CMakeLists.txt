set(SPI_SOURCES
	SPI.cpp)

add_library(arduino-SPI STATIC ${SPI_SOURCES})
target_link_libraries(arduino-SPI PUBLIC arduino-core)
target_include_directories(arduino-SPI PUBLIC .)

build_arduino_examples(SPI examples)

install(DIRECTORY . DESTINATION libraries/SPI)