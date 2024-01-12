add_library(arduino-Nano33BLE_System INTERFACE)
target_link_libraries(arduino-Nano33BLE_System INTERFACE arduino-core)
target_include_directories(arduino-Nano33BLE_System INTERFACE src)

build_arduino_examples(Nano33BLE_System examples)

install(DIRECTORY . DESTINATION libraries/Nano33BLE_System)