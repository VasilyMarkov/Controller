set(elf_file stm32.elf)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/nanopb/extra)
find_package(Nanopb REQUIRED)

nanopb_generate_cpp(TARGET proto simple.proto)

add_executable(${elf_file}    
    main.c
    syscalls.c
)

target_link_libraries(${elf_file} PUBLIC  
    bsp
    proto
)

target_include_directories(${elf_file} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

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
