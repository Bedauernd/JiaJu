#include <stm32f10x.h>
#include <Delay.h>
#include <KEY.h>

bool sign = false;

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
 
bool Key_Sign(void)
{
	sign = false;
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 1)//if判断按键1按下没有松手。
	{
		Delay_ms(50);
		//消抖动
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 1);
		//还没有松手，在while里面循环。
		Delay_ms(50);
		//松手，跳出while，延时消抖动。
		sign = true;
		//按键1返回1
	}
	return sign;
}
