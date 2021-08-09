#include "hardware_init.h"

#include "stm32h743xx.h"
#include "stm32h7xx_hal_rcc_ex.h"
#include "stm32h7xx_nucleo.h"

#include "app_ethernet.h"
#include "ethernetif.h"
#include <lwip/netif.h>
#include <lwip/init.h>
#include <lwip/ip_addr.h>
#include <netif/ethernet.h>

#include <stdio.h>

/* Forward declarations */
void MPU_Config(void);
void SystemClock_Config(void);
void BSP_Config(void);
void CPU_CACHE_Enable(void);
void MX_USART3_UART_Init(uint32_t baudRate);

/**
 * ST-LINK UART3
 * CN5 pins on board
 */
UART_HandleTypeDef huart3;
GPIO_InitTypeDef gpio_uart_init_struct;
struct netif gnetif;

bool debugAvailable = false;

void performHardwareInit() {
	/* Configure the MPU attributes as Device memory for ETH DMA descriptors */
	MPU_Config();

	/* Enable the CPU Cache */
	CPU_CACHE_Enable();

	MX_USART3_UART_Init(115200);

	HAL_StatusTypeDef retval = HAL_Init();
	if(retval != HAL_OK) {
		printf("Error: HAL initialization failed!\n\r");
	}

	/* Configure the system clock to 400 MHz */
	SystemClock_Config();

	BSP_Config();
}
void MX_USART3_UART_Init(uint32_t baudRate)
{
	__HAL_RCC_USART3_CONFIG(RCC_USART3CLKSOURCE_HSI);
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_ENABLE();
	/*Configure GPIO pins : PD8 PD9 */
	gpio_uart_init_struct.Pin = GPIO_PIN_8|GPIO_PIN_9;
	gpio_uart_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_uart_init_struct.Pull = GPIO_NOPULL;
	gpio_uart_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart_init_struct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOD, &gpio_uart_init_struct);

	int result;
	huart3.Instance = USART3;
	huart3.Init.BaudRate = baudRate;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	//huart3.Init.FIFOMode = UART_FIFOMODE_DISABLE;
	//huart3.Init.TXFIFOThreshold = UART_TXFIFO_THRESHOLD_1_8;
	//huart3.Init.RXFIFOThreshold = UART_RXFIFO_THRESHOLD_1_8;
	huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	// we can't do error handling (simple print out first) here because UART3 is our print interface
	result = HAL_UART_Init(&huart3);
	if(result == HAL_OK) {
		//print_uart3("\rUART3 configured successfully !\r\n\0");
		debugAvailable = true;
	}
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE BYPASS)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 400
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	HAL_StatusTypeDef ret = HAL_OK;

	/*!< Supply configuration update enable */
	MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0); // @suppress("Field cannot be resolved")

	/* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1); // @suppress("Field cannot be resolved")

	while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {} // @suppress("Field cannot be resolved")

	/* Enable D2 domain SRAM3 Clock (0x30040000 AXI)*/
	__HAL_RCC_D2SRAM3_CLK_ENABLE(); // @suppress("Field cannot be resolved")

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
	if(ret != HAL_OK)
	{
		while(1);
	}

	/* Select PLL as system clock source and configure  bus clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
	        RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2  |
	        RCC_CLOCKTYPE_D3PCLK1);

	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
	if(ret != HAL_OK)
	{
		while(1);
	}
}

/*Configure the MPU attributes */
void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct;

	/* Disable the MPU */
	HAL_MPU_Disable();

	/* Configure the MPU attributes as Device not cacheable
     for ETH DMA descriptors */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x30040000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Configure the MPU attributes as Cacheable write through
     for LwIP RAM heap which contains the Tx buffers */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x30044000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_16KB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER1;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Enable the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/*CPU L1-Cache enable*/
void CPU_CACHE_Enable(void)
{
	/* Enable I-Cache */
	SCB_EnableICache();

	/* Enable D-Cache */
	SCB_EnableDCache();
}

void BSP_Config(void)
{
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);
	BSP_LED_Init(LED3);
}

void Netif_Config(void)
{
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;
#if LWIP_DHCP
	ip_addr_set_zero_ip4(&ipaddr);
	ip_addr_set_zero_ip4(&netmask);
	ip_addr_set_zero_ip4(&gw);
#else

	/* IP address default setting */
	set_lwip_addresses(&ipaddr, &netmask, &gw);

#endif

	/* add the network interface */
	struct netif* netif_valid = netif_add(&gnetif, (ip4_addr_t*)&ipaddr,
			(ip4_addr_t*)&netmask, (ip4_addr_t*) &gw, NULL, &ethernetif_init,
			&ethernet_input);
	if(netif_valid == NULL) {
		printf("Error: netif initialization failed!\n\r");
		return;
	}
	/*  Registers the default network interface */
	netif_set_default(&gnetif);

	ethernet_link_status_updated(&gnetif);

#if LWIP_NETIF_LINK_CALLBACK
	netif_set_link_callback(&gnetif, ethernet_link_status_updated);
#endif
}
