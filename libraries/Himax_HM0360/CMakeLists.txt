set(HIMAX_HM0360_SOURCES
	hm0360.cpp)

add_library(arduino-Himax_HM0360 STATIC ${HIMAX_HM0360_SOURCES})
target_link_libraries(arduino-Himax_HM0360 PUBLIC arduino-core arduino-Wire arduino-Camera)
target_include_directories(arduino-Himax_HM0360 PUBLIC .)

install(DIRECTORY . DESTINATION libraries/Himax_HM0360)