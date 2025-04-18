# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Atividade_1_fase_2 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(Atividade_1_fase_2 Atividade_1_fase_2.c ssd1306.c )

pico_set_program_name(Atividade_1_fase_2 "Atividade_1_fase_2")
pico_set_program_version(Atividade_1_fase_2 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Atividade_1_fase_2 1)
pico_enable_stdio_usb(Atividade_1_fase_2 1)

# Add the standard library to the build
target_link_libraries(Atividade_1_fase_2
        pico_stdlib
        hardware_i2c
        hardware_adc
        hardware_pwm
        hardware_pio)

# Add the standard include files to the build
target_include_directories(Atividade_1_fase_2 PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(Atividade_1_fase_2 
        
        )

pico_add_extra_outputs(Atividade_1_fase_2)

