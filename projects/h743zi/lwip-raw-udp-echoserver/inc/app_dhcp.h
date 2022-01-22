#ifndef BSP_STM32_STM32CUBEH7_BOARDS_NUCLEO_H743ZI_INC_APP_DHCP_H_
#define BSP_STM32_STM32CUBEH7_BOARDS_NUCLEO_H743ZI_INC_APP_DHCP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lwipopts.h"

#if LWIP_DHCP == 1

#include "lwip/netif.h"

/* DHCP process states */
#define DHCP_OFF (uint8_t)0
#define DHCP_START (uint8_t)1
#define DHCP_WAIT_ADDRESS (uint8_t)2
#define DHCP_ADDRESS_ASSIGNED (uint8_t)3
#define DHCP_TIMEOUT (uint8_t)4
#define DHCP_LINK_DOWN (uint8_t)5

uint8_t get_dhcp_state();
void set_dhcp_state(uint8_t new_state);

void dhcp_process(struct netif *netif);
void dhcp_periodic_handle(struct netif *netif);

#endif /* LWIP_DHCP == 1 */

#ifdef __cplusplus
}
#endif

#endif /* BSP_STM32_STM32CUBEH7_BOARDS_NUCLEO_H743ZI_INC_APP_DHCP_H_ */
