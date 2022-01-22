#include "hw.h"
#include "stm32h7xx_nucleo.h"

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>


#include <cstdio>

TaskHandle_t LED_TASK;
TaskHandle_t CTRL_TASK;

static constexpr uint32_t CMD_FREQ = 2000;

enum LedCmd: uint8_t {
  BLINK_NONE = 0,
  BLINK_LED1 = 1 << 0,
  BLINK_LED2 = 1 << 1,
  BLINK_LED3 = 1 << 2,
};

enum CtrlCmd: uint8_t {
  DUMMY_EVENT = 1 << 0
};

enum CtrlState: uint8_t {
  NONE = 0,
  ONE_LED = 1,
  ALL_LEDS = 2,
};

static void ledTask(void *arg) {
  uint32_t currentLedBitField = 0;
  uint32_t notificationValue = 0;
  bool eventSent = false;
  for (;;) {
    // Check notification value, but only poll
    BaseType_t retval = xTaskNotifyWait(0, 0, &notificationValue, 0);
    if(retval == pdTRUE) {
      eventSent = false;
      // LED blinky mode might have changed
      if(notificationValue != currentLedBitField) {
        printf("LED: Mode update | LED1 %ld | LED2 %ld | LED3 %ld \n", notificationValue & BLINK_LED1,
            (notificationValue >> 1 & 0x01), (notificationValue >> 2 & 0x01));
        // Has changed, so reset LEDs
        currentLedBitField = notificationValue;
        BSP_LED_Off(LED1);
        BSP_LED_Off(LED2);
        BSP_LED_Off(LED3);
      }
    }
    if((notificationValue & BLINK_LED1) == BLINK_LED1) {
      BSP_LED_Toggle(LED1);
    }
    if((notificationValue & BLINK_LED2) == BLINK_LED2) {
      BSP_LED_Toggle(LED2);
    }
    if((notificationValue & BLINK_LED2) == BLINK_LED2) {
      BSP_LED_Toggle(LED3);
    }
    if(notificationValue == (BLINK_LED1 | BLINK_LED2 | BLINK_LED3) and not eventSent) {
      // In this mode, set an event bit in the control task once
      xTaskNotify(CTRL_TASK, 1, eSetBits);
      eventSent = true;
    }
    // Blink frequency
    vTaskDelay(200);
  }
}

static void controlTask(void *args) {
  CtrlState state = CtrlState::NONE;
  BaseType_t retval = pdTRUE;
  uint32_t notificationValue = 0;
  uint32_t lastCmdTime = 0;
  for(;;) {
    uint32_t currentTime = xTaskGetTickCount();
    if(currentTime - lastCmdTime >= CMD_FREQ) {
      printf("CTRL: Set control bit in LED task\n");
      if( state == CtrlState::NONE) {
        // Notify the LED task to keep LEDs off
        retval = xTaskNotify(LED_TASK, BLINK_NONE, eSetValueWithOverwrite);
        state = CtrlState::ONE_LED;
      } else if(state == CtrlState::ONE_LED) {
        // Notify the LED task to only blink LED1
        retval = xTaskNotify(LED_TASK, BLINK_LED1, eSetValueWithOverwrite);
        state = CtrlState::ALL_LEDS;
      } else if(state == CtrlState::ALL_LEDS) {
        // Notify the LED task to only blink all LEDs
        retval = xTaskNotify(LED_TASK, BLINK_LED1 | BLINK_LED2 | BLINK_LED3, eSetValueWithOverwrite);
        state = CtrlState::NONE;
      }
      if(retval != pdPASS) {
        printf("Error: Could not set LED Task notification value\n");
      }
      lastCmdTime = currentTime;
    }
    // Check special event bit set by LED task
    retval = xTaskNotifyWait(0, 0xffffffff, &notificationValue, 0);
    if((retval == pdTRUE) and (notificationValue & DUMMY_EVENT) == DUMMY_EVENT) {
      printf("CTRL: Event bit was set by LED task\n");
    }
    vTaskDelay(500);
  }
}

int main(void) {
  hw::init();

  printf("-- STM32 Task Notification Test --\n");
  xTaskCreate(ledTask, "LED", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, &LED_TASK);
  xTaskCreate(controlTask, "CTRL", configMINIMAL_STACK_SIZE * 4, NULL, tskIDLE_PRIORITY + 2, &CTRL_TASK);
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
