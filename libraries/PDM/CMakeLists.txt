set(PDM_HEADERS
	src/PDM.h
	src/utility/PDMDoubleBuffer.h)

set(PDM_SOURCES
	src/utility/PDMDoubleBuffer.cpp)

if("NRF52" IN_LIST MBED_TARGET_LABELS)
	list(APPEND PDM_SOURCES src/nrf52/PDM.cpp)
elseif("RP2040" IN_LIST MBED_TARGET_LABELS)
	list(APPEND PDM_HEADERS
		src/rp2040/OpenPDMFilter.h
		src/rp2040/pdm.pio.h)
	list(APPEND PDM_SOURCES
		src/rp2040/OpenPDMFilter.c
		src/rp2040/PDM.cpp)
endif()

add_library(arduino-PDM STATIC ${PDM_SOURCES})
target_link_libraries(arduino-PDM PUBLIC arduino-core)
target_include_directories(arduino-PDM PUBLIC src)

install(FILES ${PDM_HEADERS} ${PDM_SOURCES} DESTINATION libraries/PDM/src)
install(DIRECTORY examples DESTINATION libraries/PDM)