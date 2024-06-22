set(STM32H747_SYSTEM_SOURCES
	src/STM32H747_System.cpp
	src/wiced_filesystem.cpp)

if(TARGET_ARDUINO_PORTENTA IN_LIST MBED_TARGET_LABELS)
	list(APPEND STM32H747_SYSTEM_SOURCES
		src/Portenta_System.cpp)
elseif(TARGET_ARDUINO_NICLA_VISION IN_LIST MBED_TARGET_LABELS)
	list(APPEND STM32H747_SYSTEM_SOURCES
		src/Nicla_Vision_System.cpp)
endif()

add_library(arduino-STM32H747_System STATIC ${STM32H747_SYSTEM_SOURCES})
target_link_libraries(arduino-STM32H747_System PUBLIC
	arduino-core
	arduino-Wire
	mbed-storage-qspif
	mbed-storage-blockdevice
	mbed-storage-fat
	mbed-wifi)
target_include_directories(arduino-STM32H747_System PUBLIC src)

build_arduino_examples(STM32H747_System examples)

install(DIRECTORY . DESTINATION libraries/STM32H747_System)
