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

# The Arduino IDE adds an arch define based on the target, and this gets used in
# the build of some of the libraries
if("RASPBERRY_PI_PICO" IN_LIST MBED_TARGET_LABELS)
	target_compile_definitions(arduino-core PUBLIC ARDUINO_ARCH_RP2040)
elseif("ARDUINO_NANO33BLE" IN_LIST MBED_TARGET_LABELS)
	target_compile_definitions(arduino-core PUBLIC ARDUINO_ARCH_NRF52840)
endif()