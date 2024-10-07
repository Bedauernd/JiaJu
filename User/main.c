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
int count = 1;
int main(void)
{
	LED_Init();
	Key_Init();
	Serial_Init();
	OLED_Init();
	//初始化设备
	OLED_Clear();
	OLED_ShowString(0,0,"Linking",OLED_8X16);
	OLED_Update();
	do{
		ESP8266_Init();
	}while(ESP8266_SUB());
	//如果没有订阅成功就返回前面初始化
	while (1)
	{
		OLED_Clear();
		DHT11_REC_Data();
		temperature = DHT11_Get_Temp();
		Humidity = DHT11_Get_Hum();
		OLED_ShowChinese(0,16,"温度：");
		OLED_ShowChinese(0,32,"湿度：");
		OLED_ShowChinese(0,48,"灯光：");
		OLED_ShowFloatNum(48,16,temperature,2,2,OLED_8X16,false);
		OLED_ShowFloatNum(48,32,Humidity,2,2,OLED_8X16,false);
		if (Key_Sign())
		{
			LED_Turn();
		}
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
		//显示屏显示当前采集数据和LED状态
		if(count%200 == 0)
		{
			if(ESP8266_PUB())
			{
				OLED_ShowString(0,0,"Success",OLED_8X16);
			}
			else
			{
				OLED_ShowString(0,0,"Error",OLED_8X16);
			}
			count = 0;
			OLED_Update();
			Delay_ms(500);
		}
		count += 1;
		//大致为10s向云端发送一次数据
	}
	return 0;
}
