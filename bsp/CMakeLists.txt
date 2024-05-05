set(LWIP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lwip/src)
set(LWIP_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/lwip/src/include)
set(LWIP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/lwip/src/core)

add_library(bsp STATIC    
    bsp.c
    stm32f4xx_it.c
    lwip/lwip.c
    lwip/ethernetif.c
    lwip/stm32f4_eth_periph/src/stm32f4xx_hal_eth.c
    ${LWIP_SOURCE_DIR}/api/err.c
    ${LWIP_SOURCE_DIR}/core/def.c
    ${LWIP_SOURCE_DIR}/core/init.c
    ${LWIP_SOURCE_DIR}/core/ip.c
    ${LWIP_SOURCE_DIR}/core/mem.c
    ${LWIP_SOURCE_DIR}/core/memp.c
    ${LWIP_SOURCE_DIR}/core/netif.c
    ${LWIP_SOURCE_DIR}/core/pbuf.c
    ${LWIP_SOURCE_DIR}/core/timeouts.c
    ${LWIP_SOURCE_DIR}/core/udp.c
    ${LWIP_SOURCE_DIR}/core/ipv4/dhcp.c
    ${LWIP_SOURCE_DIR}/core/ipv4/etharp.c
    ${LWIP_SOURCE_DIR}/core/ipv4/icmp.c
    ${LWIP_SOURCE_DIR}/core/ipv4/ip4.c
    ${LWIP_SOURCE_DIR}/core/ipv4/ip4_addr.c
    ${LWIP_SOURCE_DIR}/core/ipv4/ip4_frag.c
    ${LWIP_SOURCE_DIR}/netif/ethernet.c
)

target_include_directories(bsp PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/bsp/lwip
    ${CMAKE_SOURCE_DIR}/bsp/lwip/system
    ${LWIP_INCLUDES}
    ${LWIP_INCLUDES}/lwip
    ${LWIP_INCLUDES}/netif
    ${LWIP_INCLUDES}/stm32f4_eth_periph/inc
)

target_link_libraries(bsp PUBLIC 
    stm32f4xx 
)

target_compile_definitions(bsp PUBLIC STM32F439xx)
