set(MRI_SOURCES
	src/core/buffer.c
	src/core/cmd_break_watch.c
	src/core/cmd_common.c
	src/core/cmd_continue.c
	src/core/cmd_file.c
	src/core/cmd_memory.c
	src/core/cmd_query.c
	src/core/cmd_registers.c
	src/core/cmd_step.c
	src/core/cmd_thread.c
	src/core/cmd_vcont.c
	src/core/context.c
	src/core/gdb_console.c
	src/core/libc.c
	src/core/memory.c
	src/core/mri.c
	src/core/packet.c
	src/core/token.c
	src/core/try_catch.c
	src/memory/native/native-mem.c
	src/semihost/newlib/newlib_stubs.S
	src/semihost/newlib/semihost_newlib.c
	src/semihost/semihost.c
	)

add_library(arduino-MRI STATIC ${MRI_SOURCES})
target_link_libraries(arduino-MRI PUBLIC arduino-core)
target_include_directories(arduino-MRI PUBLIC src)

install(FILES ${MRI_HEADERS} ${MRI_SOURCES} DESTINATION libraries/MRI)