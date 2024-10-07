#include <stm32f10x.h>
#include <stdbool.h>

bool LED_Sign;

void LED_Init(void)//初始化
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_2); 
	//由于初始化后默认是为低电平，SetBits为了让端口置高。设定为了让LED开始是熄灭的状态。
}
 
void LED_ON(void)//LED1点亮程序
{
	GPIO_ResetBits (GPIOA, GPIO_Pin_2);
}
 
void LED_OFF(void)//LED1熄灭程序
{
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
}

void LED_Turn(void)//状态取反。实现按下送开LED亮，按下送开LED熄灭
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	}
}


