set(USBAUDIO_SOURCES
	USBAudio.cpp)

add_library(arduino-USBAudio STATIC ${USBAUDIO_SOURCES})
target_link_libraries(arduino-USBAudio PUBLIC arduino-core)
target_include_directories(arduino-USBAudio PUBLIC .)

install(DIRECTORY . DESTINATION libraries/USBAudio)