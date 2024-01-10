/* This example demonstrates how to utilize the KernelDebug library which
   allows kernel debugging of the Portenta H7 with GDB over a UART serial
   connection.

   To connect to the target, launch gdb with the following parameters
 
   arm-none-eabi-gdb -ex "set pagination off" --baud {230400} -ex "set target-charset ASCII" -ex "target remote {debug.port}" {project_name}.elf

   The baud rate needs to match the one provided in KernelDebug constructor, while {debug.port} depends on the operating system (eg. /dev/ttyUSB0 or COM15)
*/

#include <KernelDebug.h>

#if TARGET_NRF52840
// NRF52840 based chips.  Note that the UARTs are not assigned to specific pins, but stdout should
// get UART 0 because it's the first UART created.
KernelDebug kernelDebug(SERIAL1_TX, SERIAL1_RX, UARTE0_UART0_IRQn, 230400, DEBUG_BREAK_IN_SETUP);
#else
// Portenta targets
KernelDebug kernelDebug(SERIAL1_TX, SERIAL1_RX, USART1_IRQn, 230400, DEBUG_BREAK_IN_SETUP);
#endif

void setup() {

}

void loop() {

}
