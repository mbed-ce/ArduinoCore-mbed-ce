set(WIRE_HEADERS
	Wire.h)
set(WIRE_SOURCES
	Wire.cpp)

add_library(arduino-Wire STATIC ${WIRE_SOURCES})
target_link_libraries(arduino-Wire PUBLIC arduino-core)
target_include_directories(arduino-Wire PUBLIC .)

install(FILES ${WIRE_HEADERS} ${WIRE_SOURCES} DESTINATION libraries/Wire)