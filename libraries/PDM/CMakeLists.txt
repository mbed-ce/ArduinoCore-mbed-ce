set(PDM_SOURCES
	src/utility/PDMDoubleBuffer.cpp)

if("NRF52" IN_LIST MBED_TARGET_LABELS)
	list(APPEND PDM_SOURCES src/nrf52/PDM.cpp)
elseif("RP2040" IN_LIST MBED_TARGET_LABELS)
	list(APPEND PDM_SOURCES
		src/rp2040/OpenPDMFilter.c
		src/rp2040/PDM.cpp)
elseif("ARDUINO_PORTENTA" IN_LIST MBED_TARGET_LABELS)
	list(APPEND PDM_SOURCES
		src/STM32H747_SAI/audio.c
		src/STM32H747_SAI/PDM.cpp)
elseif("ARDUINO_GIGA" IN_LIST MBED_TARGET_LABELS OR "ARDUINO_NICLA_VISION" IN_LIST MBED_TARGET_LABELS)
	list(APPEND PDM_SOURCES
		src/STM32H747_dfsdm/audio.c
		src/STM32H747_dfsdm/PDM.cpp)
endif()

add_library(arduino-PDM STATIC ${PDM_SOURCES})
target_link_libraries(arduino-PDM PUBLIC arduino-core)
target_include_directories(arduino-PDM PUBLIC src)

build_arduino_examples(PDM examples)

install(DIRECTORY . DESTINATION libraries/PDM)
