cmake_minimum_required(VERSION 3.5)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake)

enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

#Output directory
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

project(stm32)

set(MCPU cortex-m4)

add_compile_options(-mfloat-abi=soft)
 
add_compile_options(-mcpu=${MCPU} -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

add_definitions(-DSTM32F439xx)
add_compile_definitions(HSE_VALUE=8000000)

include_directories(
    ${PROJECT_SOURCE_DIR}/bsp
    ${PROJECT_SOURCE_DIR}/lib
    ${PROJECT_SOURCE_DIR}/lib/CMSIS
    ${PROJECT_SOURCE_DIR}/lib/STM32F4xx
    ${PROJECT_SOURCE_DIR}/lib/STM32F4xx_StdPeriph_Driver/inc
    )

file(GLOB_RECURSE SOURCES
        "main.c"
        "bsp/bsp.c"
        "system_stm32f4xx.c"
        "startup_stm32f4xx.S"
        "lib/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c"
        "lib/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c"
    )

set(LINKER_SCRIPT ${PROJECT_SOURCE_DIR}/stm32_flash.ld)

add_link_options(-specs=nosys.specs)
add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=${MCPU} -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

set(EXECUTABLE ${PROJECT_NAME}.elf)

add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
            COMMAND ${CMAKE_SIZE_UTIL} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXECUTABLE}
)

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -O ihex ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXECUTABLE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex
            COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXECUTABLE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.bin
)

