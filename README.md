# Arduino Core for Mbed CE Arduino devices
This repository is a wrapper which bundles Mbed CE as an Arduino core so that Mbed CE can be used as a platform in the Arduino IDE.  It is designed to perform a similar function as the mainline [ArduinoCore-mbed](https://github.com/arduino/ArduinoCore-mbed) repository while being a bit less janky.

The CMake build scripts in this repo perform two distinct functions:
- Create a precompiled build of Mbed OS, and then install it plus the supporting files in the correct directory structure to be an Arduino core
- Compile the Arduino core library and dependency libraries in-place.  This is mainly done to check that they compile, and is not intended for actual usage (though you can use this repo to compile and link Mbed CE Arduino applications).

## Compiling an Arduino core for Mbed CE

1. Clone the repo. Don't forget to clone with `--recursive` or run `git submodule update --init` after cloning!
2. Set up the toolchain as [described](https://github.com/mbed-ce/mbed-os/wiki/Toolchain-Setup-Guide) in the Mbed CE wiki.
3. Create a build directory and cd into it: `mkdir build && cd build`
4. Configure CMake: `cmake .. -GNinja -DCMAKE_BUILD_TYPE=<Debug|Release> -DMBED_TARGET=<target> -DUPLOAD_METHOD=NONE`.  The DCMAKE_BUILD_TYPE may be set to Debug or Release to generate a debug or release build of the core.  The MBED_TARGET may be set to any supported target for this repo -- check the directory names under `variants/` to see each supported target.
5. Build: `ninja`
6. Generate the package: `ninja package`.  This will generate a zip file containing the core in the build folder -- for example, mine is called `ArduinoCore-mbed-ce-RASPBERRY_PI_PICO-Develop-1.0.0.zip`.

## Installing the Core Locally

The easiest way to install the core locally, per [here](https://support.arduino.cc/hc/en-us/articles/360021232160-How-to-install-and-use-a-custom-core-version-in-the-IDE), is to copy it to your sketchbook directory.
