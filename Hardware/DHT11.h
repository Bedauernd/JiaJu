#ifndef __DHT11_H
#define __DHT11_H
#include <stm32f10x.h>


#define DHT11_H GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define DHT11_L GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define Read_Data GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)

void DHT11_GPIO_Init_OUT(void);
void DHT11_GPIO_Init_IN(void);
void DHT11_Start(void);
unsigned char DHT11_REC_Byte(void);
void DHT11_REC_Data(void);
float DHT11_Get_Hum();
float DHT11_Get_Temp();

#endif

