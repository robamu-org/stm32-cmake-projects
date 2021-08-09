target_sources(${TARGET_NAME} PRIVATE
    app_ethernet.c
    app_dhcp.c
    ethernetif.c
    stm32h7xx_it.c
    stm32h7xx_hal_timebase_tim.c
    stm32h7xx_nucleo.c
    udp_echoserver.c
    syscalls.c
    hardware_init.c
)