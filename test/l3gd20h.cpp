#include <freertos.h>
#include <task.h>

#include <stm32h7xx_hal_gpio.h>

int l3gd20h() {
    for(;;) {
        vTaskDelay(500);
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    }
}



