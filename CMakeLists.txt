# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(button_IoT C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# set main code path
file(GLOB_RECURSE MAIN "src/button_IoT.c")
# set path to utility files
file(GLOB_RECURSE UTILS "src/utils/**/*.c")
# set path to drivers
file(GLOB_RECURSE DRIVERS "src/drivers/*.c")

# Add executable. Default name is the project name, version 0.1
add_executable(button_IoT 
    ${MAIN}
    ${UTILS}
    ${DRIVERS}
)

pico_set_program_name(button_IoT "button_IoT")
pico_set_program_version(button_IoT "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(button_IoT 0)
pico_enable_stdio_usb(button_IoT 1)

# Add the standard library to the build
target_link_libraries(button_IoT
    pico_stdlib
    hardware_timer
    hardware_i2c
    hardware_adc
    pico_cyw43_arch_lwip_threadsafe_background
)

# Add the standard include files to the build
target_include_directories(button_IoT PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers
    ${CMAKE_CURRENT_LIST_DIR}/src/utils/button
    ${CMAKE_CURRENT_LIST_DIR}/src/utils/display
    ${CMAKE_CURRENT_LIST_DIR}/src/utils/sensor_hcsr04
    ${CMAKE_CURRENT_LIST_DIR}/src/utils/wifi
    ${CMAKE_CURRENT_LIST_DIR}/src/utils/server
    ${CMAKE_CURRENT_LIST_DIR}/src/utils/joystick
)

# Add any user requested libraries
target_link_libraries(button_IoT 
    
)

pico_add_extra_outputs(button_IoT)

