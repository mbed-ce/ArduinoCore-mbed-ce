add_library(arduino-Nicla_System STATIC
	src/BQ25120A.cpp
	src/Nicla_System.cpp
	src/RGBled.cpp)

target_link_libraries(arduino-Nicla_System PUBLIC arduino-core arduino-Wire)
target_include_directories(arduino-Nicla_System PUBLIC src)

build_arduino_examples(Nicla_System examples NiclaSenseME_BatteryStatus)

install(DIRECTORY . DESTINATION libraries/Nicla_System)