set(LWIP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lwip)
set(LWIP_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/lwip/src/include)
set(LWIP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/lwip/src/core)

# file(GLOB_RECURSE LWIP_SOURCES 
#     ${LWIP_SOURCE_DIR}/src/core/*.c
#     ${LWIP_SOURCE_DIR}/src/netif/*.c
# )

add_library(bsp STATIC    
    bsp.c
    # syscalls.c
    # lwip/lwip.c
    # ${LWIP_SOURCES}/init.c
    # ${LWIP_SOURCES}
)

target_include_directories(bsp PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    # ${CMAKE_SOURCE_DIR}/bsp/lwip
    # ${CMAKE_SOURCE_DIR}/bsp/lwip/system
    # ${LWIP_INCLUDES}
    # ${LWIP_INCLUDES}/lwip
    # ${LWIP_INCLUDES}/netif
)

target_link_libraries(bsp PUBLIC 
    stm32f4xx 
)

target_compile_definitions(bsp PUBLIC STM32F439xx)
