#include "app_dhcp.h"
#include "app_ethernet.h"
#include "udp_config.h"
#include "ethernetif.h"

#include "lwip/dhcp.h"
#include "stm32h7xx_nucleo.h"

#include <stdbool.h>

#if LWIP_DHCP == 1

#ifndef MAX_DHCP_TRIES
#define MAX_DHCP_TRIES 4
#endif

uint8_t DHCP_state = DHCP_OFF;
uint32_t DHCPfineTimer = 0;

void handle_dhcp_timeout(struct netif* netif);
void handle_dhcp_start(struct netif* netif);
void handle_dhcp_wait(struct netif* netif, struct dhcp** dhcp);
void handle_dhcp_down(struct netif* netif);

/**
 * @brief  DHCP_Process_Handle
 * @param  None
 * @retval None
 */
void DHCP_Process(struct netif *netif)
{
    struct dhcp* dhcp = NULL;
    switch (DHCP_state) {
    case DHCP_START: {
        handle_dhcp_start(netif);
        break;
    }
    case DHCP_WAIT_ADDRESS: {
        handle_dhcp_wait(netif, &dhcp);
        break;
    }

    case DHCP_LINK_DOWN: {
        handle_dhcp_down(netif);
        break;
    }
    default: {
        break;
    }
    }
}

void handle_dhcp_timeout(struct netif* netif) {
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;

    DHCP_state = DHCP_TIMEOUT;

    /* Stop DHCP */
    dhcp_stop(netif);

    /* Static address used */
    set_lwip_addresses(&ipaddr, &netmask, &gw);
    netif_set_addr(netif, &ipaddr, &netmask, &gw);

    printf("DHCP Timeout\n\r");
    uint8_t iptxt[20];
    sprintf((char *)iptxt, "%s", ip4addr_ntoa(netif_ip4_addr(netif)));
    printf("Assigning static IP address: %s\n", iptxt);

    ETH_HandleTypeDef* handle = getEthernetHandle();
    handle->gState = HAL_ETH_STATE_READY;

#if OBSW_ETHERNET_TMTC_COMMANDING == 1
#if OBSW_ETHERNET_USE_LED1_LED2 == 1
    BSP_LED_On(LED1);
    BSP_LED_Off(LED2);
#endif
#endif
}

/**
 * @brief  DHCP periodic check
 * @param  netif
 * @retval None
 */
void DHCP_Periodic_Handle(struct netif *netif)
{
    /* Fine DHCP periodic process every 500ms */
    if (HAL_GetTick() - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS) {
        DHCPfineTimer =  HAL_GetTick();
        /* process DHCP state machine */
        DHCP_Process(netif);
    }
}

void handle_dhcp_start(struct netif* netif) {
    printf("handle_dhcp_start: Looking for DHCP server ...\n\r");
#if OBSW_ETHERNET_TMTC_COMMANDING == 1
#if OBSW_ETHERNET_USE_LED1_LED2 == 1
    BSP_LED_Off(LED1);
    BSP_LED_Off(LED2);
#endif
#endif
    ip_addr_set_zero_ip4(&netif->ip_addr);
    ip_addr_set_zero_ip4(&netif->netmask);
    ip_addr_set_zero_ip4(&netif->gw);
    dhcp_start(netif);
    DHCP_state = DHCP_WAIT_ADDRESS;
}

void handle_dhcp_wait(struct netif* netif, struct dhcp** dhcp) {
    if (dhcp_supplied_address(netif)) {
        DHCP_state = DHCP_ADDRESS_ASSIGNED;
        printf("IP address assigned by a DHCP server: %s\n\r", ip4addr_ntoa(netif_ip4_addr(netif)));
        printf("Listener port: %d\n\r", UDP_SERVER_PORT);
#if OBSW_ETHERNET_TMTC_COMMANDING == 1
#if OBSW_ETHERNET_USE_LED1_LED2 == 1
        BSP_LED_On(LED1);
        BSP_LED_Off(LED2);
#endif
#endif
    }
    else {
        *dhcp = (struct dhcp*) netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

        /* DHCP timeout */
        if ((*dhcp)->tries > MAX_DHCP_TRIES)
        {
            handle_dhcp_timeout(netif);
        }
    }
}

void handle_dhcp_down(struct netif* netif) {
    DHCP_state = DHCP_OFF;
#if OBSW_ETHERNET_TMTC_COMMANDING == 1
    printf("DHCP_Process: The network cable is not connected.\n\r");
#if OBSW_ETHERNET_USE_LED1_LED2 == 1
    BSP_LED_Off(LED1);
    BSP_LED_On(LED2);
#endif
#endif

    /* Global boolean to track ethernet connection */
    set_eth_cable_connected(false);
}

uint8_t get_dhcp_state() {
    return DHCP_state;
}

void set_dhcp_state(uint8_t new_state) {
    DHCP_state = new_state;
}

#endif /* LWIP_DHCP == 1 */
