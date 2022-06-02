#ifndef MY_CONN_H
#define MY_CONN_H

#include "stm32h7xx.h"
#include <string.h>
#include "lwipopts.h"
#include "bsp_led.h"
#include "client.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "arch/sys_arch.h"
#include "wifi_base_config.h"


#define READ_BUFFER_SIZE  1024
//报文类型
enum msgTypes
{
	CONNECT = 1, CONNACK, PUBLISH, PUBACK, PUBREC, PUBREL,
	PUBCOMP, SUBSCRIBE, SUBACK, UNSUBSCRIBE, UNSUBACK,
	PINGREQ, PINGRESP, DISCONNECT
};
//灯控制
enum msgCtl
{
	LEDOFF = 0, LEDRED, LEDGREEN, LEDBLUE
};


//自定义协议客户端结构体
struct my_conn
{
    int32_t m_socket;                       //客户端套接字
    const char *m_topic;                    //客户端主题
    const char *m_sub_topic;                //订阅主题
    uint16_t live_time_out;                 //10s发送一次心跳报文
    uint16_t live_time_in;                  //读报文超时时间最多为15s
    uint32_t m_timeout_in;                  //读报文超时时间
    uint32_t m_timeout_out;                 //写报文超时时间
    char m_read_buf[READ_BUFFER_SIZE];      //读缓冲区
    int m_read_idx;                         //读到的长度
};
typedef struct my_conn *my_conn_t;


void my_conn_init(struct my_conn *my_connHandler,int32_t socket);

uint8_t send_connect_packet(struct my_conn *connHandle);
uint8_t send_subscribe_packet(struct my_conn *connHandle);
void my_conn_recv(void *thread_param);
void my_conn_send(void *thread_param);
void my_ping_check(void *thread_param);

#endif
