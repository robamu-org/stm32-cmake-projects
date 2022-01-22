/**
 ******************************************************************************
 * @file    LwIP/LwIP_UDP_Echo_Server/Src/main.c
 * @author  MCD Application Team
 * @brief   This sample code implements a http server application based on LwIP
 *          Raw API of LwIP stack. This application uses the STM32Cube ETH HAL
 *          API to transmit and receive data.
 *          The communication is done with a web browser of a remote PC.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "hardware_init.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/opt.h"
#include "lwip/timeouts.h"
#include "lwip_raw_conf.h"
#include "netif/etharp.h"
#if LWIP_DHCP
#include "lwip/dhcp.h"
#endif
#include "app_dhcp.h"
#include "app_ethernet.h"
#include "ethernetif.h"
#include "udp_echoserver.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void) {
  /* Hardware initialization */
  performHardwareInit();

  printf("-- STM32H743ZI LwIP Raw Demo application --\n");

  /* Initialize the LwIP stack */
  lwip_init();

  /* Configure the Network interface */
  Netif_Config();

  /* UDP echoserver init */
  udp_echoserver_init();

  /* Infinite loop */
  while (1) {
    /* Read a received packet from the Ethernet buffers and send it
       to the lwIP for handling */
    ethernetif_input(&gnetif);

    /* Handle timeouts */
    sys_check_timeouts();

#if LWIP_NETIF_LINK_CALLBACK
    ethernet_link_periodic_handle(&gnetif);
#endif

#if LWIP_DHCP
    dhcp_periodic_handle(&gnetif);
#endif
  }
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line) {
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1) {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
