/**
 * Additional layer for hardware initialization.
 */

#ifndef STM32_BSP_HARDWARE_INIT_H_
#define STM32_BSP_HARDWARE_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_gpio.h>
#include <stm32h7xx_hal_uart.h>

/**
 * Declaration of global hardware handlers here.
 * Definition inside respective .cpp or .c files.
 */
extern UART_HandleTypeDef huart3;
extern GPIO_InitTypeDef gpio_uart_init_struct;
extern struct netif gnetif; /* network interface structure */

extern bool debugAvailable;

void performHardwareInit();
void Netif_Config(void);

#ifdef __cplusplus
}
#endif

#endif /* STM32_BSP_HARDWARE_INIT_H_ */
