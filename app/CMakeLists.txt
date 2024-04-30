set(elf_file stm32.elf)

add_executable(${elf_file}    
    app.c 
    main.c
    syscalls.c
)

target_link_libraries(${elf_file} PUBLIC  
    bsp
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
