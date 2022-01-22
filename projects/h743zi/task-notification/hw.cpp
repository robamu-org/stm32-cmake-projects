#include "hw.h"


/**
 * ST-LINK UART3
 * CN5 pins on board
 */
UART_HandleTypeDef HUART_3;
GPIO_InitTypeDef gpio_uart_init_struct;

void hw::init() {
  CPU_CACHE_Enable();
  MX_USART3_UART_Init(115200);
  BSP_Config();
}

void hw::MX_USART3_UART_Init(uint32_t baudRate) {
  __HAL_RCC_USART3_CONFIG(RCC_USART3CLKSOURCE_HSI);
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_USART3_CLK_ENABLE();
  /*Configure GPIO pins : PD8 PD9 */
  gpio_uart_init_struct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  gpio_uart_init_struct.Mode = GPIO_MODE_AF_PP;
  gpio_uart_init_struct.Pull = GPIO_NOPULL;
  gpio_uart_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
  gpio_uart_init_struct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOD, &gpio_uart_init_struct);

  int result;
  HUART_3.Instance = USART3;
  HUART_3.Init.BaudRate = baudRate;
  HUART_3.Init.WordLength = UART_WORDLENGTH_8B;
  HUART_3.Init.StopBits = UART_STOPBITS_1;
  HUART_3.Init.Parity = UART_PARITY_NONE;
  HUART_3.Init.Mode = UART_MODE_TX_RX;
  HUART_3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  HUART_3.Init.OverSampling = UART_OVERSAMPLING_16;
  HUART_3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  HUART_3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  // huart3.Init.FIFOMode = UART_FIFOMODE_DISABLE;
  // huart3.Init.TXFIFOThreshold = UART_TXFIFO_THRESHOLD_1_8;
  // huart3.Init.RXFIFOThreshold = UART_RXFIFO_THRESHOLD_1_8;
  HUART_3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  // we can't do error handling (simple print out first) here because UART3 is our print interface
  result = HAL_UART_Init(&HUART_3);
  if (result == HAL_OK) {
    // print_uart3("\rUART3 configured successfully !\r\n\0");
  }
}

void hw::SystemClock_Config() {
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /*!< Supply configuration update enable */
  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);  // @suppress("Field cannot be resolved")

  /* The voltage scaling allows optimizing the power consumption when the device is
clocked below the maximum system frequency, to update the voltage scaling value
regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(
      PWR_REGULATOR_VOLTAGE_SCALE1);  // @suppress("Field cannot be resolved")

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {
  }  // @suppress("Field cannot be resolved")

  /* Enable D2 domain SRAM3 Clock (0x30040000 AXI)*/
  __HAL_RCC_D2SRAM3_CLK_ENABLE();  // @suppress("Field cannot be resolved")

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if (ret != HAL_OK) {
    while (1)
      ;
  }

  /* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 |
                                 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if (ret != HAL_OK) {
    while (1)
      ;
  }
}

/*CPU L1-Cache enable*/
void hw::CPU_CACHE_Enable() {
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

void hw::BSP_Config() {
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
}
