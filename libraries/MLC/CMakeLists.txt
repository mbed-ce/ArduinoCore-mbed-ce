# For this library, we cannot build the examples as part of Mbed CE, because they rely on driver libraries
# which are maintained outside this repo.
install(DIRECTORY . DESTINATION libraries/MLC)