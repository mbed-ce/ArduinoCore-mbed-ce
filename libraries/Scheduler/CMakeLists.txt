set(SCHEDULER_SOURCES
	src/Scheduler.cpp)

add_library(arduino-Scheduler STATIC ${SCHEDULER_SOURCES})
target_link_libraries(arduino-Scheduler PUBLIC arduino-core)
target_include_directories(arduino-Scheduler PUBLIC src)

# Example does not compile on boards with only 1 LED or no LEDs
if((NOT ARDUINO_VARIANT_NAME STREQUAL "ARDUINO_NANO33BLE") AND
	(NOT ARDUINO_VARIANT_NAME STREQUAL "RASPBERRY_PI_PICO") AND
	(NOT ARDUINO_VARIANT_NAME STREQUAL "ARDUINO_NICLA_SENSE_ME"))
	build_arduino_examples(Scheduler examples)
endif()

install(DIRECTORY . DESTINATION libraries/Scheduler)