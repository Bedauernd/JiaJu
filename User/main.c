#include <stm32f10x.h>
#include <ESP8266.h>
#include <Serial.h>
#include <DHT11.h>
#include <Delay.h>
#include <OLED.h>
#include <LED.h>
#include <KEY.h>

extern float temperature;
extern float Humidity;
extern int LED;
bool check;
int count = 0;
int main(void)
{
	LED_Init();
	Key_Init();
	Serial_Init();
	OLED_Init();
	OLED_ShowString(0,0,"Linking",OLED_8X16);
	OLED_Update();
	ESP8266_Init();
	Delay_s(2);
	ESP8266_SUB();
	while (1)
	{
		DHT11_REC_Data();
		temperature = DHT11_Get_Temp();
		Humidity = DHT11_Get_Hum();
		OLED_ShowChinese(48,0,"测试");
		OLED_ShowChinese(0,16,"温度：");
		OLED_ShowChinese(0,32,"湿度：");
		OLED_ShowChinese(0,48,"灯光：");
		OLED_ShowFloatNum(48,16,temperature,2,2,OLED_8X16,false);
		OLED_ShowFloatNum(48,32,Humidity,2,2,OLED_8X16,false);
		if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0)
		{
			OLED_ShowString(56,48,"True",OLED_8X16);
			LED = 1;
		}
		else
		{
			OLED_ShowString(56,48,"False",OLED_8X16);
			LED = 0;
		}
		OLED_Update();
		if (Key_Sign())
		{
			LED_Turn();
		}
		if(count%300 == 0)
		{
			ESP8266_PUB();
			count = 0;
		}
		OLED_Clear();
		count += 1;
	}
	return 0;
}
