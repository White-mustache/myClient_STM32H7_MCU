#ifndef __LED_H
#define	__LED_H

#include "stm32h7xx.h"

//重定义
typedef struct
{
  __IO uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
  __IO uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
  __IO uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
  __IO uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  __IO uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
  __IO uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
  __IO uint16_t BSRRL;    /*!< GPIO port bit set/reset low register,  Address offset: 0x18      */
  __IO uint16_t BSRRH;    /*!< GPIO port bit set/reset high register, Address offset: 0x1A      */
  __IO uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  __IO uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_TypeDefm;

#define GPIOAm               ((GPIO_TypeDefm *) GPIOA_BASE)
#define GPIOBm               ((GPIO_TypeDefm *) GPIOB_BASE)
#define GPIOCm               ((GPIO_TypeDefm *) GPIOC_BASE)
#define LED1_GPIO_PORTm            GPIOBm  
#define LED2_GPIO_PORTm            GPIOBm 
#define LED3_GPIO_PORTm            GPIOAm    
//引脚定义
/*******************************************************/
//R 红色灯
#define LED1_PIN                  GPIO_PIN_0                 
#define LED1_GPIO_PORT            GPIOB                      
#define LED1_GPIO_CLK_ENABLE()    __GPIOB_CLK_ENABLE()

//G 绿色灯
#define LED2_PIN                  GPIO_PIN_1                 
#define LED2_GPIO_PORT            GPIOB                     
#define LED2_GPIO_CLK_ENABLE()    __GPIOB_CLK_ENABLE()

//B 蓝色灯
#define LED3_PIN                  GPIO_PIN_3                
#define LED3_GPIO_PORT            GPIOA                       
#define LED3_GPIO_CLK_ENABLE()    __GPIOA_CLK_ENABLE()

/************************************************************/


/** 控制LED灯亮灭的宏，
	* LED低电平亮，设置ON=0，OFF=1
	* 若LED高电平亮，把宏设置成ON=1 ，OFF=0 即可
	*/
#define ON  GPIO_PIN_RESET
#define OFF GPIO_PIN_SET

/* 带参宏，可以像内联函数一样使用 */
#define LED1(a)	HAL_GPIO_WritePin(LED1_GPIO_PORT,LED1_PIN,a)


#define LED2(a)	HAL_GPIO_WritePin(LED2_GPIO_PORT,LED2_PIN,a)


#define LED3(a)	HAL_GPIO_WritePin(LED2_GPIO_PORT,LED3_PIN,a)


//#define LED4(a)	HAL_GPIO_WritePin(LED4_GPIO_PORT,LED4_PIN,a)


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)				{p->BSRRL=i;}			  //设置为高电平		
#define digitalLo(p,i)				{p->BSRRH=i;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态


/* 定义控制IO的宏 */
#define LED1_TOGGLE		digitalToggle(LED1_GPIO_PORT,LED1_PIN)
#define LED1_OFF			digitalHi(LED1_GPIO_PORTm,LED1_PIN)
#define LED1_ON				digitalLo(LED1_GPIO_PORTm,LED1_PIN)

#define LED2_TOGGLE		digitalToggle(LED2_GPIO_PORT,LED2_PIN)
#define LED2_OFF			digitalHi(LED2_GPIO_PORTm,LED2_PIN)
#define LED2_ON				digitalLo(LED2_GPIO_PORTm,LED2_PIN)

#define LED3_TOGGLE		digitalToggle(LED3_GPIO_PORT,LED3_PIN)
#define LED3_OFF			digitalHi(LED3_GPIO_PORTm,LED3_PIN)
#define LED3_ON				digitalLo(LED3_GPIO_PORTm,LED3_PIN)


/* 基本混色，后面高级用法使用PWM可混出全彩颜色,且效果更好 */

//红
#define LED_RED  \
					LED1_ON;\
					LED2_OFF\
					LED3_OFF

//绿
#define LED_GREEN		\
					LED1_OFF;\
					LED2_ON\
					LED3_OFF

//蓝
#define LED_BLUE	\
					LED1_OFF;\
					LED2_OFF\
					LED3_ON

					
//黄(红+绿)					
#define LED_YELLOW	\
					LED1_ON;\
					LED2_ON\
					LED3_OFF
//紫(红+蓝)
#define LED_PURPLE	\
					LED1_ON;\
					LED2_OFF\
					LED3_ON

//青(绿+蓝)
#define LED_CYAN \
					LED1_OFF;\
					LED2_ON\
					LED3_ON
					
//白(红+绿+蓝)
#define LED_WHITE	\
					LED1_ON;\
					LED2_ON\
					LED3_ON
					
//黑(全部关闭)
#define LED_RGBOFF	\
					LED1_OFF;\
					LED2_OFF\
					LED3_OFF
					




void LED_GPIO_Config(void);

#endif /* __LED_H */
