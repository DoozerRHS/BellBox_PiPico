cmake_minimum_required(VERSION 3.13)

include(C:/Users/rob/Documents/Pico-v1.5.1/pico-examples/pico_sdk_import.cmake)

project(bellbox)

pico_sdk_init()

add_executable(bellbox
        main.cpp
        servoCntrl.cpp
        )


# pull in common dependencies
target_link_libraries(bellbox pico_stdlib hardware_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(bellbox)

# add url via pico_set_program_url
pico_set_program_url(bellbox "https://github.com/DoozerRHS/BellBox_PiPico")
