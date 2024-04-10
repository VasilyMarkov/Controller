set(target_base_name stm32)

project(${target_base_name})

set(elf_file ${target_base_name}.elf)

set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/lwip)

find_package(LwIP REQUIRED)

add_executable(${elf_file}    
    ${CMAKE_SOURCE_DIR}/app/app.c 
    bsp.c
    main.c
    syscalls.c
)

target_include_directories(${elf_file} PRIVATE 
    ${CMAKE_SOURCE_DIR}/app
    ${CMAKE_SOURCE_DIR}/bsp
    # ${CMAKE_SOURCE_DIR}/lwip/src/include/lwip 
)

target_compile_options(${elf_file} PRIVATE -Wall -Wextra -Os)

target_link_libraries(${elf_file} PRIVATE stm32f4xx)

set(EXECUTABLE ${PROJECT_NAME}.elf)

add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
            COMMAND ${CMAKE_SIZE_UTIL} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXECUTABLE}
)

add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -O ihex ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXECUTABLE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex
            COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXECUTABLE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.bin
)
