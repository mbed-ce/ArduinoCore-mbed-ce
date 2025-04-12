set(HIMAX_HM01B0_SOURCES
	himax.cpp)

add_library(arduino-Himax_HM01B0 STATIC ${HIMAX_HM01B0_SOURCES})
target_link_libraries(arduino-Himax_HM01B0 PUBLIC arduino-core arduino-Wire arduino-Camera)
target_include_directories(arduino-Himax_HM01B0 PUBLIC .)

install(DIRECTORY . DESTINATION libraries/Himax_HM01B0)