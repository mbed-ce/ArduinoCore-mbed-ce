# Arduino Core for Mbed CE Arduino devices
This repository is a work-in-progress attempt to bundle Mbed CE as an Arduino core, so that Mbed CE can be used as a platform in the Arduino IDE.  It is designed to perform a similar function as the mainline [ArduinoCore-mbed](https://github.com/arduino/ArduinoCore-mbed) repository while being significantly less janky.  

The build scripts in this repo perform two distinct functions:
- Create a precompiled build of Mbed OS, and then install it plus the supporting files in the correct directory structure to be an Arduino core
- Compile the Arduino core library and dependency libraries in-place.  This is purely done to check that they compile, and is not intended for actual usage (though theoretically you could use this repo to compile and link Mbed CE Arduino applications).