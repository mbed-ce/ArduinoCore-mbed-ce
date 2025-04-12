set(CAMERA_SOURCES
    src/camera.cpp)

add_library(arduino-Camera STATIC ${CAMERA_SOURCES})
target_link_libraries(arduino-Camera PUBLIC arduino-core arduino-Wire)
target_include_directories(arduino-Camera PUBLIC src)

set(EXAMPLES_TO_SKIP GigaCameraDisplay)
if(NOT "ARDUINO_NICLA_VISION" IN_LIST MBED_TARGET_LABELS)
    list(APPEND EXAMPLES_TO_SKIP CameraCaptureZoomPan)
endif()

build_arduino_examples(Camera examples ${EXAMPLES_TO_SKIP})

if("ARDUINO_PORTENTA_H7" IN_LIST MBED_TARGET_LABELS)
    target_link_libraries(Camera-example-CameraCaptureRawBytes arduino-Himax_HM0360)
    target_link_libraries(Camera-example-CameraMotionDetect arduino-Himax_HM0360)
endif()
