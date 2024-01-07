# Install Arduino Core and make a build system target for it
set(ARDUINO_CORE_SOURCES
	arduino/abi.cpp
	arduino/arm_hal_random.c
	arduino/Interrupts.cpp
	arduino/itoa.c
	arduino/main.cpp
	arduino/pinToIndex.cpp
	arduino/random_seed.cpp
	arduino/Serial.cpp
	arduino/timer.cpp
	arduino/Tone.cpp
	arduino/wiring.cpp
	arduino/wiring_analog.cpp
	arduino/wiring_digital.cpp
	arduino/wiring_pulse.cpp
	arduino/wiring_shift.cpp
	arduino/WMath.cpp
	arduino/USB/PluggableUSBDevice.cpp
	arduino/USB/USBCDC.cpp
	arduino/USB/USBSerial.cpp)

add_library(arduino-core STATIC ${ARDUINO_CORE_SOURCES})
target_link_libraries(arduino-core arduino-api arduino-variant mbed-core-flags mbed-rtos-flags mbed-usb)
target_include_directories(arduino-core PUBLIC arduino)

install(DIRECTORY arduino DESTINATION cores)