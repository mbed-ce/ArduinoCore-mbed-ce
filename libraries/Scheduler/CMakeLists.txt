set(SCHEDULER_HEADERS
	src/Scheduler.h)
set(SCHEDULER_SOURCES
	src/Scheduler.cpp)

add_library(arduino-Scheduler STATIC ${SCHEDULER_SOURCES})
target_link_libraries(arduino-Scheduler PUBLIC arduino-core)
target_include_directories(arduino-Scheduler PUBLIC .)

install(FILES ${SCHEDULER_HEADERS} ${SCHEDULER_SOURCES} DESTINATION libraries/Scheduler/src)
install(DIRECTORY examples DESTINATION libraries/Scheduler)