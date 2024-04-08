cmake_minimum_required(VERSION 3.5)

set(CMAKE_TOOLCHAIN_FILE toolchain-arm-none-eabi.cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(stm32)

add_subdirectory(bsp)
add_subdirectory(lib)



