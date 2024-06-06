project(stm32f4_lib C ASM)

set(DEVICE_FAMILY STM32F439xx)
# get_filename_component(LINKER_SCRIPT src/stm32f439zi_flash.ld ABSOLUTE)
get_filename_component(LINKER_SCRIPT src/STM32F439ZITx_FLASH.ld ABSOLUTE)

add_library(stm32f4xx STATIC   
    src/STM32F4xx_StdPeriph_Driver/misc.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_hal_eth.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4x7_eth.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_adc.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_can.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_crc.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_cryp.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_cryp_aes.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_cryp_des.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_cryp_tdes.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_dac.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_dbgmcu.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_dcmi.c
    src/STM32F4xx_StdPeriph_Driver/stm32f4xx_dma.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_exti.c
    src/STM32F4xx_StdPeriph_Driver/stm32f4xx_flash.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_fsmc.c
    src/STM32F4xx_StdPeriph_Driver/stm32f4xx_gpio.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_hash.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_hash_md5.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_hash_sha1.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_i2c.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_iwdg.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_pwr.c
    src/STM32F4xx_StdPeriph_Driver/stm32f4xx_rcc.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_rng.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_rtc.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_sdio.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_spi.c
    src/STM32F4xx_StdPeriph_Driver/stm32f4xx_syscfg.c
    src/STM32F4xx_StdPeriph_Driver/stm32f4xx_tim.c
    src/STM32F4xx_StdPeriph_Driver/stm32f4xx_usart.c
    # src/STM32F4xx_StdPeriph_Driver/stm32f4xx_wwdg.c  
    src/system_stm32f4xx.c
    # src/startup_stm32f4xx.S
    src/startup_stm32f439xx.S
)

target_include_directories(stm32f4xx PUBLIC 
    inc 
    inc/CMSIS
    inc/STM32F4xx_StdPeriph_Driver
)

target_compile_definitions(stm32f4xx PUBLIC USE_STDPERIPH_DRIVER)
target_compile_definitions(stm32f4xx PUBLIC ${DEVICE_FAMILY})
target_compile_definitions(stm32f4xx PUBLIC HSE_VALUE=8000000)
target_compile_definitions(stm32f4xx PUBLIC SYSTEM_CORE_CLOCK=180000000)
target_compile_definitions(stm32f4xx PUBLIC USE_STM324x7I_EVAL)

target_compile_options(stm32f4xx PRIVATE -nostartfiles)
target_link_libraries(stm32f4xx PUBLIC -T${LINKER_SCRIPT})