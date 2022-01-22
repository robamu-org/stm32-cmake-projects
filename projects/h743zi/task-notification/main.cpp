#include "hw.h"

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

#include "stm32h7xx_nucleo.h"

static void blinky(void *arg) {
  for (;;) {
    vTaskDelay(500);
    BSP_LED_Toggle(LED1);
  }
}

int main(void) {
  hw::init();


  xTaskCreate(blinky, "blinky", configMINIMAL_STACK_SIZE * 4, NULL, tskIDLE_PRIORITY + 1, NULL);

  vTaskStartScheduler();
  for (;;)
    ;

  return 0;
}

extern "C" void vApplicationTickHook(void) {}

extern "C" void vApplicationIdleHook(void) {}

extern "C" void vApplicationMallocFailedHook(void) {
  taskDISABLE_INTERRUPTS();
  for (;;)
    ;
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
  (void)pcTaskName;
  (void)pxTask;

  taskDISABLE_INTERRUPTS();
  for (;;)
    ;
}
