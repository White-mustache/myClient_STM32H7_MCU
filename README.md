# myClient_STM32H7_MCU
## 概述
STM32H743XI开发板，通过wifi模块AP6181连接路由器
移植FreeRTOS，LWIP，socket编程，实现自定义应用层协议TCP客户端
## 环境
IDE： KEIL MDK 5.27
开发板： 野火STM32H743XI-PRO开发板
开发板配置环境：1、需要将开发板的天线连接好；2、将开发板的跳帽J78、J66安装好；3、需要将自己的电脑与开发板处于同一局域网下；
# 文件名
/WiFi_SDK：官方SDK  
/wifi_lwip_client_socket： KEIL工程
主要用户代码在/wifi_lwip_client_socket/User中
# 自定义协议 - 客户端
参考MQTT实现了简化版MQTT协议
## 连接 CONNECT 
客户端向服务器发送连接报文，向服务器注册其主题。
报文格式：1位控制报文类型 + 2位剩余长度 + 1位TOPIC长度 + TOPIC 
## 订阅 SUBSCRIBE
客户端向服务器发送订阅主题，服务器会转发其订阅的主题给客户端。
报文格式：1位控制报文类型 + 2位剩余长度 + 1位SUB_TOPIC长度 + SUB_TOPIC 
## 发布 PUBLISH
客户端向服务器发布其主题，例如采集到的温湿度信息，向其订阅者发布控制命令，传感器信息等。
报文格式：1位控制报文类型 + 2位剩余长度 + 1位TOPIC长度 + TOPIC + 消息
## 心跳 PING
长连接，保持通信活性，每隔10s向服务器发送心跳报文，同时接收服务器心跳报文，超出15s则关闭连接。
报文格式：1位控制报文类型 + 2位剩余长度 + 1位TOPIC长度 + TOPIC

# 实现demo功能
服务器：个人PC_linux
客户端：1、STM32H7_MCU_Freertos; 2、STM32MP157_MPU_Linux
## 功能：
1、STM32H7_MCU_Freertos发布温湿度信息，订阅STM32MP157_MPU_Linux信息来控制LED灯的颜色和亮灭。
2、STM32MP157_MPU_Linux发布灯控制信息来控制其订阅者LED灯的颜色和亮灭。