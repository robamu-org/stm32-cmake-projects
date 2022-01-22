#ifndef PROJECTS_FREERTOS_TASK_NOTIFICATION_HW_H_
#define PROJECTS_FREERTOS_TASK_NOTIFICATION_HW_H_

#include <cstdint>

namespace hw {

void init();

void MX_USART3_UART_Init(uint32_t baudRate);
void SystemClock_Config();
void CPU_CACHE_Enable();
void BSP_Config();

}

#endif /* PROJECTS_FREERTOS_TASK_NOTIFICATION_HW_H_ */
