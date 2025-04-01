set(STM32H747_SYSTEM_SOURCES
	src/STM32H747_System.cpp)

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

# STM32H747_manageBootloader does not support Arduino Giga
set(EXAMPLES_TO_SKIP "")
if("ARDUINO_GIGA" IN_LIST MBED_TARGET_LABELS)
	list(APPEND EXAMPLES_TO_SKIP STM32H747_manageBootloader)
endif()

build_arduino_examples(STM32H747_System examples ${EXAMPLES_TO_SKIP})
if(TARGET STM32H747_System-example-STM32H747_manageBootloader)
	target_link_libraries(STM32H747_System-example-STM32H747_manageBootloader mbed-storage-littlefs)
endif()
if(TARGET STM32H747_System-example-QSPIFormat)
	target_link_libraries(STM32H747_System-example-QSPIFormat mbed-storage-littlefs)
endif()
if(TARGET STM32H747_System-example-QSPIFReadPartitions)
	target_link_libraries(STM32H747_System-example-QSPIFReadPartitions mbed-storage)
endif()

install(DIRECTORY . DESTINATION libraries/STM32H747_System)
