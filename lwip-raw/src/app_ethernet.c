/* Includes ------------------------------------------------------------------*/
#include "app_ethernet.h"
#include "ethernetif.h"
#include "udp_config.h"

#if LWIP_DHCP
#include "app_dhcp.h"
#endif

#include <lwipopts.h>
#include <lwip/netif.h>
#include <stm32h7xx_nucleo.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ethernetLinkTimer = 0;

/* Private function prototypes -----------------------------------------------*/
void handle_status_change(struct netif* netif, bool link_up);


/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Notify the User about the network interface config status
 * @param  netif: the network interface
 * @retval None
 */
void ethernet_link_status_updated(struct netif *netif) 
{
	if (netif_is_link_up(netif))
	{
		set_eth_cable_connected(true);
		handle_status_change(netif, true);
	}
	else
	{
        set_eth_cable_connected(false);
		handle_status_change(netif, false);
	}
}

void set_lwip_addresses(ip_addr_t* ipaddr, ip_addr_t* netmask, ip_addr_t* gw) {
	IP4_ADDR(ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
	IP4_ADDR(netmask, NETMASK_ADDR0, NETMASK_ADDR1 ,
			NETMASK_ADDR2, NETMASK_ADDR3);
	IP4_ADDR(gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
}


void handle_status_change(struct netif* netif, bool link_up) {
	if(link_up) {
#if LWIP_DHCP
		/* Update DHCP state machine */
		set_dhcp_state(DHCP_START);
#else
		uint8_t iptxt[20];
		sprintf((char *)iptxt, "%s", ip4addr_ntoa(netif_ip4_addr(netif)));
		printf("\rNetwork cable connected. Static IP address: %s | Port: %d\n\r", iptxt,
		        UDP_SERVER_PORT);
		BSP_LED_On(LED1);
		BSP_LED_Off(LED2);
#endif /* LWIP_DHCP */
	}
	else {
		printf("Network cable disconnected\n\r");
#if LWIP_DHCP
		/* Update DHCP state machine */
		set_dhcp_state(DHCP_LINK_DOWN);
#else
		BSP_LED_Off(LED1);
		BSP_LED_On(LED2);
#endif /* LWIP_DHCP */
	}
}

#if LWIP_NETIF_LINK_CALLBACK

/**
 * @brief  Ethernet Link periodic check
 * @param  netif
 * @retval None
 */
void ethernet_link_periodic_handle(struct netif *netif)
{
	/* Ethernet Link every 100ms */
	if (HAL_GetTick() - ethernetLinkTimer >= 100)
	{
		ethernetLinkTimer = HAL_GetTick();
		ethernet_link_check_state(netif);
	}
}

#endif /* LWIP_NETIF_LINK_CALLBACK */
