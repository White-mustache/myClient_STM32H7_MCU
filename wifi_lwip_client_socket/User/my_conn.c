#include "my_conn.h"

#define  PRINTF printf

extern SemaphoreHandle_t readMutex;
const char *MYTOPIC = "STM32H7";
const char *MYSUBTOPIC = "STM32MP157";

void my_conn_init(struct my_conn *my_connHandler,int32_t socket)
{
    uint32_t curtime = xTaskGetTickCount();
    my_connHandler->m_socket = socket;
    my_connHandler->m_topic = MYTOPIC;
    my_connHandler->m_sub_topic = MYSUBTOPIC;
    my_connHandler->live_time_out = 10000;	//至少10s发送一次报文
    my_connHandler->live_time_in = 15000;		//超过15s未接收则关闭链接
    my_connHandler->m_timeout_in = curtime + my_connHandler->live_time_in;
    my_connHandler->m_timeout_out = curtime + my_connHandler->live_time_out;
    my_connHandler->m_read_idx = 0;
}

static void writeChar(unsigned char** pptr, char c)
{
	**pptr = c;
	(*pptr)++;
}
static void writeInt16(unsigned char** pptr, uint16_t anInt)
{
	**pptr = (unsigned char)(anInt / 256);
	(*pptr)++;
	**pptr = (unsigned char)(anInt % 256);
	(*pptr)++;
}
static void writeCString(unsigned char** pptr, const char* string)
{
	int len = strlen(string);
	writeChar(pptr, len);
	memcpy(*pptr, string, len);
	*pptr += len;
}
static uint32_t send_packet(int32_t sock, char *buf, int len)
{
    //进入临界区
    xSemaphoreTake(readMutex, portMAX_DELAY);
    int ret = write(sock, buf, len);
    //退出临界区
    xSemaphoreGive(readMutex);
    return ret;
}

uint8_t send_connect_packet(struct my_conn *connHandle)
{
    uint8_t buf[200];
    unsigned char *ptr = buf;

    int len = 0;
    //组装消息
    uint8_t header = CONNECT<<4;
    writeChar(&ptr, (char)header);
    //写剩余长度可最后再写，先把坑占了
    writeInt16(&ptr, 0);
    //topic长度 + topic
    writeCString(&ptr, connHandle->m_topic);
    //发送消息
    len = ptr - buf;
    //把剩余长度写回去
    ptr = buf + 1;
    writeInt16(&ptr, len - 3);
    int32_t ret;
    ret = send_packet(connHandle->m_socket, buf, len);
    //发送成功判断
    if(ret < 0)
    {
        PRINTF("SEND CONNECT PACKET ERROR");
        return 0;
    }
    return 1;
}

static uint8_t send_subscribe_packet(struct my_conn *connHandle)
{
    uint8_t buf[200];
    unsigned char *ptr = buf;

    int len = 0;
    //组装消息
    uint8_t header = SUBSCRIBE<<4;
    writeChar(&ptr, (char)header);
    //写剩余长度可最后再写，先把坑占了
    writeInt16(&ptr, 0);
    //topic长度 + topic
    writeCString(&ptr, connHandle->m_sub_topic);
    //发送消息
    len = ptr - buf;
    //把剩余长度写回去
    ptr = buf + 1;
    writeInt16(&ptr, len - 3);
    int32_t ret;
    ret = send_packet(connHandle->m_socket, buf, len);
    //发送成功判断
    if(ret < 0)
    {
        PRINTF("SEND SUBSCRIBE PACKET ERROR");
        return 0;
    }
    return 1;
}

static uint8_t send_ping_packet(struct my_conn *connHandle)
{
    uint8_t buf[200];
    unsigned char *ptr = buf;
    int len = 0;
    //组装消息
    uint8_t header = PINGREQ<<4;
    writeChar(&ptr, (char)header);
    //写剩余长度可最后再写，先把坑占了
    writeInt16(&ptr, 0);
    //topic长度 + topic
    writeCString(&ptr, connHandle->m_topic);
    //发送消息
    len = ptr - buf;
    //把剩余长度写回去
    ptr = buf + 1;
    writeInt16(&ptr, len - 3);
    int32_t ret;
    ret = send_packet(connHandle->m_socket, buf, len);
    //发送成功判断
    if(ret < 0)
    {
        PRINTF("SEND PING PACKET ERROR");
        return 0;
    }
    return 1;
}

static void STM32MP157_func(const char *msg)
{
    uint8_t  colour = (uint8_t)*msg;
    //不同指令开关不同的灯颜色
    switch(colour)
    {
        case LEDOFF:
            LED_RGBOFF;
            break;
        case LEDRED:
            LED_RED;
            break;
        case LEDGREEN:
            LED_GREEN;
            break;
        case LEDBLUE:
            LED_BLUE;
            break;
        default:
            PRINTF("Invalid information");
            break;
    }
    return ;
}

static void recv_publish_packet(struct my_conn *connHandle)
{
    uint16_t remain_len = (uint16_t)*(connHandle->m_read_buf + 1);
    uint8_t topic_len = (uint8_t)*(connHandle->m_read_buf + 3);
    //对比topic 后续可用链表，不同的topic进入不同的处理函数
    STM32MP157_func(connHandle->m_read_buf + 4);
    return ;
}

static void recv_ping_packet(struct my_conn *connHandle)
{
    uint16_t remain_len = (uint16_t)*(connHandle->m_read_buf + 1);
    uint8_t topic_len = (uint8_t)*(connHandle->m_read_buf + 3);
    //对比topic
    int i = 0;
    char *msg_topic = connHandle->m_read_buf + 4;
    const char *my_topic = connHandle->m_topic;
    for(i = 0; i < topic_len; i ++)
    {
        if(*(msg_topic + i) != *(my_topic + i))
        {
            PRINTF("PING RECV NO MATCH");
            return;
        }
    }
    //更新读超时时间
    uint32_t curtime = xTaskGetTickCount();
    connHandle->live_time_in = curtime + connHandle->live_time_in;
    return ;
}
//读线程
void my_conn_recv(void *thread_param)
{
    struct my_conn *connHandle = (struct my_conn *)thread_param;
    int num;
    uint8_t packet_type = 0;
    for(;;)
    {   
        vTaskDelay(10);
        //阻塞等待
        num = recv(connHandle->m_socket, connHandle->m_read_buf, READ_BUFFER_SIZE, -1);
        if(num <= 3)
		{
		    PRINTF("RECV TOO SHOTE!\n");
            continue;
		}
        //更新读超时时间
        uint32_t curtime = xTaskGetTickCount();
        connHandle->m_timeout_in = curtime + connHandle->live_time_in;
        //解包 收到亮灯指令 亮灯颜色
        connHandle->m_read_idx = num;
        packet_type = (*(connHandle->m_read_buf) & 0xF0) >> 4;
        switch(packet_type)
		{
			case PUBLISH:
				recv_publish_packet(connHandle);
				break;
            case PINGRESP:
                recv_ping_packet(connHandle);
				break;
            default:
                PRINTF("RECV NO MATCH");
                break;
		}
    }
}
//写线程
void my_conn_send(void *thread_param)
{
    struct my_conn *connHandle = (struct my_conn *)thread_param;
    //发送订阅消息
    int ret = send_subscribe_packet(connHandle);
    if(ret == 0)
    {
        PRINTF("SEND SUBSCRIBE_PACKET ERROR");
    }
    else
    {
        //更新写超时时间
        //uint32_t curtime = xTaskGetTickCount();
        //connHandle->m_timeout_out = curtime + connHandle->live_time_out;
    }
    //发送心跳报文
    //推送消息 这条暂不需要
    for(;;)
    {
        vTaskDelay(10);
    }
}
//心跳报文检查线程
void my_ping_check(void *thread_param)
{
    struct my_conn *connHandle = (struct my_conn *)thread_param;
		uint32_t curtime;
    for(;;)
    {
        vTaskDelay(2000);
        curtime = xTaskGetTickCount();
        if(connHandle->m_timeout_in < curtime)
        {
            //接收超时，应断开连接
            PRINTF("RECV PING ERROR");
        }
        if(connHandle->m_timeout_out < curtime)
        {
            send_ping_packet(connHandle);
            //更新写超时时间
            curtime = xTaskGetTickCount();
            connHandle->m_timeout_out = curtime + connHandle->live_time_out;
        }
    }
}
