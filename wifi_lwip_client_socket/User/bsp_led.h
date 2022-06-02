#ifndef __LED_H
#define	__LED_H

#include "stm32h7xx.h"

//�ض���
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
//���Ŷ���
/*******************************************************/
//R ��ɫ��
#define LED1_PIN                  GPIO_PIN_0                 
#define LED1_GPIO_PORT            GPIOB                      
#define LED1_GPIO_CLK_ENABLE()    __GPIOB_CLK_ENABLE()

//G ��ɫ��
#define LED2_PIN                  GPIO_PIN_1                 
#define LED2_GPIO_PORT            GPIOB                     
#define LED2_GPIO_CLK_ENABLE()    __GPIOB_CLK_ENABLE()

//B ��ɫ��
#define LED3_PIN                  GPIO_PIN_3                
#define LED3_GPIO_PORT            GPIOA                       
#define LED3_GPIO_CLK_ENABLE()    __GPIOA_CLK_ENABLE()

/************************************************************/


/** ����LED������ĺ꣬
	* LED�͵�ƽ��������ON=0��OFF=1
	* ��LED�ߵ�ƽ�����Ѻ����ó�ON=1 ��OFF=0 ����
	*/
#define ON  GPIO_PIN_RESET
#define OFF GPIO_PIN_SET

/* ���κ꣬��������������һ��ʹ�� */
#define LED1(a)	HAL_GPIO_WritePin(LED1_GPIO_PORT,LED1_PIN,a)


#define LED2(a)	HAL_GPIO_WritePin(LED2_GPIO_PORT,LED2_PIN,a)


#define LED3(a)	HAL_GPIO_WritePin(LED2_GPIO_PORT,LED3_PIN,a)


//#define LED4(a)	HAL_GPIO_WritePin(LED4_GPIO_PORT,LED4_PIN,a)


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)				{p->BSRRL=i;}			  //����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)				{p->BSRRH=i;}				//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;}			//�����ת״̬


/* �������IO�ĺ� */
#define LED1_TOGGLE		digitalToggle(LED1_GPIO_PORT,LED1_PIN)
#define LED1_OFF			digitalHi(LED1_GPIO_PORTm,LED1_PIN)
#define LED1_ON				digitalLo(LED1_GPIO_PORTm,LED1_PIN)

#define LED2_TOGGLE		digitalToggle(LED2_GPIO_PORT,LED2_PIN)
#define LED2_OFF			digitalHi(LED2_GPIO_PORTm,LED2_PIN)
#define LED2_ON				digitalLo(LED2_GPIO_PORTm,LED2_PIN)

#define LED3_TOGGLE		digitalToggle(LED3_GPIO_PORT,LED3_PIN)
#define LED3_OFF			digitalHi(LED3_GPIO_PORTm,LED3_PIN)
#define LED3_ON				digitalLo(LED3_GPIO_PORTm,LED3_PIN)


/* ������ɫ������߼��÷�ʹ��PWM�ɻ��ȫ����ɫ,��Ч������ */

//��
#define LED_RED  \
					LED1_ON;\
					LED2_OFF\
					LED3_OFF

//��
#define LED_GREEN		\
					LED1_OFF;\
					LED2_ON\
					LED3_OFF

//��
#define LED_BLUE	\
					LED1_OFF;\
					LED2_OFF\
					LED3_ON

					
//��(��+��)					
#define LED_YELLOW	\
					LED1_ON;\
					LED2_ON\
					LED3_OFF
//��(��+��)
#define LED_PURPLE	\
					LED1_ON;\
					LED2_OFF\
					LED3_ON

//��(��+��)
#define LED_CYAN \
					LED1_OFF;\
					LED2_ON\
					LED3_ON
					
//��(��+��+��)
#define LED_WHITE	\
					LED1_ON;\
					LED2_ON\
					LED3_ON
					
//��(ȫ���ر�)
#define LED_RGBOFF	\
					LED1_OFF;\
					LED2_OFF\
					LED3_OFF
					




void LED_GPIO_Config(void);

#endif /* __LED_H */
