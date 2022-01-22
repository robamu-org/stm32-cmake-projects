#ifndef PROJECTS_FREERTOS_TASK_NOTIFICATION_HW_H_
#define PROJECTS_FREERTOS_TASK_NOTIFICATION_HW_H_

#include "stm32h743xx.h"
#include "stm32h7xx_hal_rcc_ex.h"
#include "stm32h7xx_nucleo.h"

#include <stdint.h>

extern UART_HandleTypeDef HUART_3;

#ifdef __cplusplus
namespace hw {
#endif

void init();

void MX_USART3_UART_Init(uint32_t baudRate);
void SystemClock_Config();
void CPU_CACHE_Enable();
void BSP_Config();

#ifdef __cplusplus
}
#endif

#endif /* PROJECTS_FREERTOS_TASK_NOTIFICATION_HW_H_ */
