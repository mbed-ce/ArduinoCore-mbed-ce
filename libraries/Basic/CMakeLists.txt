# The examples in this folder aren't installed as they are part of the default Arduino IDE.
# we just have them here so that basic functionality can be checked.

set(EXAMPLES_TO_SKIP "")

# Nicla Sense cannot build the blink example because it does not have a regular LED
if("ARDUINO_NICLA_SENSE_ME" IN_LIST MBED_TARGET_LABELS)
	list(APPEND EXAMPLES_TO_SKIP Blink)
endif()

build_arduino_examples(core . ${EXAMPLES_TO_SKIP})