#ifndef WIFI_BASE_CONFIG_H
#define WIFI_BASE_CONFIG_H
#include "lwip/opt.h"
#include "lwip/icmp.h"
#include "lwip/inet_chksum.h"
#include "lwip/sockets.h"
#include "lwip/mem.h"
#include "lwip/inet.h"
#include "netif/etharp.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "wwd_network.h"
#include "wwd_management.h"
#include "wwd_wifi.h"
#include "wwd_debug.h"
#include "wwd_assert.h"
#include "platform/wwd_platform_interface.h"
#include "RTOS/wwd_rtos_interface.h"
#include "wwd_buffer_interface.h"

/******************************************************
 *                      Macros
 ******************************************************/
/** @cond */
#define MAKE_IPV4_ADDRESS(a, b, c, d)          ((((uint32_t) (a)) << 24) | (((uint32_t) (b)) << 16) | (((uint32_t) (c)) << 8) | ((uint32_t) (d)))

/* �ڷ��а��У�����UART��ӡ�������ƣ��Ӷ�����
��removef��malloc�������Ӷ����ż������ڴ�ʹ��*/
#ifndef DEBUG
#undef  WPRINT_APP_INFO
#define WPRINT_APP_INFO(args) printf args
#undef  WPRINT_APP_ERROR
#define WPRINT_APP_ERROR(args) printf args
#endif


/******************************************************
 *                    ����
 ******************************************************/
//#define AP_SSID              "embedfire_AP6181"    /* ·������ */
#define AP_SSID              "HUAWEIP30"    /* ·������ */
#define AP_PASS              "26773311"         /* ·������ */
#define AP_SEC               WICED_SECURITY_WPA2_MIXED_PSK  /* ·�ɼ��� */

#define COUNTRY              WICED_COUNTRY_AUSTRALIA    /* ѡ����� ��˵�Ĵ����ǵ��źŸ�ǿһЩ������ѡ��Ϊ�Ĵ����� */
//#define USE_DHCP             WICED_TRUE     /* �Ƿ�ʹ�� DHCP */
#define USE_DHCP             WICED_FALSE     /* �Ƿ�ʹ�� DHCP */
#define IP_ADDR              MAKE_IPV4_ADDRESS( 192, 168,   100,  128 )  /* ���USE_DHCPΪWICED_TRUE������Ҫ */
#define GW_ADDR              MAKE_IPV4_ADDRESS( 192, 168,   100,   1 )   /* ���USE_DHCPΪWICED_TRUE������Ҫ */
#define NETMASK              MAKE_IPV4_ADDRESS( 255, 255, 255,   0 )    /* ���USE_DHCPΪWICED_TRUE������Ҫ */
/* #define PING_TARGET          MAKE_IPV4_ADDRESS( 192, 168,   1, 2 ) */  /* ���Ҫping�ض�IP���������أ���ȡ��ע��*/

#define DEST_IP_ADDR0               192
#define DEST_IP_ADDR1               168
#define DEST_IP_ADDR2               43
#define DEST_IP_ADDR3               135

#define DEST_PORT                  8089

#define IP_ADDR0                    192
#define IP_ADDR1                    168
#define IP_ADDR2                    100
#define IP_ADDR3                    200
/*
*/
#define LOCAL_PORT 									5001

static const wiced_ssid_t ap_ssid =
{
    .length = sizeof(AP_SSID)-1,
    .value  = AP_SSID,
};


#define PING_RCV_TIMEOUT     (1000)    /** ping���ճ�ʱ-�Ժ���Ϊ��λ*/
#define PING_DELAY           (1000)    /** ping��Ӧ/��ʱ����һ��ping����֮����ӳ�-�Ժ���Ϊ��λ */
#define PING_ID              (0xAFAF)
#define PING_DATA_SIZE       (32)      /** ping �������ݴ�С*/
#define JOIN_TIMEOUT         (10000)   /** ������������ĳ�ʱʱ�䣨�Ժ���Ϊ��λ��= 10��*/
#define APP_THREAD_STACKSIZE (5120)

static struct netif wiced_if;
extern ip4_addr_t target;
extern int socket_hnd;
extern wwd_time_t send_time;



extern void Config_WIFI_LwIP_Info(void);
err_t ping_send( int socket_hnd, ip4_addr_t *addr );
err_t ping_recv( int socket_hnd );
void app_main( void );

#endif 
