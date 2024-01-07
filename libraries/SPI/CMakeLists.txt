set(SPI_HEADERS
	SPI.h)
set(SPI_SOURCES
	SPI.cpp)

add_library(arduino-SPI STATIC ${SPI_SOURCES})
target_link_libraries(arduino-SPI PUBLIC arduino-core)
target_include_directories(arduino-SPI PUBLIC .)

install(FILES ${SPI_HEADERS} ${SPI_SOURCES} DESTINATION libraries/SPI)