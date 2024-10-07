#include <LED.h>
#include <Delay.h>
#include <string.h>
#include <Serial.h>
#include <ESP8266.h>
#include <stdbool.h>

char RxData[200];
unsigned char i = 0;

float temperature;
float Humidity;
int LED;

const char* WiFi = "Liu";
const char* WiFiPW = "12345678";
const char* clientID = "k1s5o00BfgU.test|securemode=2\\,signmethod=hmacsha256\\,timestamp=1727532625806|";
const char* username = "test&k1s5o00BfgU";
const char* passwd = "02ccc21418610676398d8f7a319e16a1aa954abef41a48fac0027a08827c3b1f";
const char* url = "iot-06z00dr44joxkb8.mqtt.iothub.aliyuncs.com";
const char* pubtopic = "/sys/k1s5o00BfgU/test/thing/event/property/post";
const char* subtopic = "/sys/k1s5o00BfgU/test/thing/event/property/post_reply";
const char* settopic = "/sys/k1s5o00BfgU/test/thing/service/property/set";

const char* func1 = "temperature";
const char* func2 = "Humidity";
const char* func3 = "LightSwitch";

void ESP8266_Init(void){
	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+RST\r\n");
	Delay_s(4);
	//重启esp8266模块
	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+CWMODE=1\r\n");
	Delay_s(2);
	//配置8266模块为station模式
	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n");
	Delay_s(2);
	//链接阿里云网站校准时间
	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+CWJAP=\"%s\",\"%s\"\r\n",WiFi,WiFiPW);
	Delay_s(3);
	//链接WiFi
	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+MQTTUSERCFG=0,1,\"NULL\",\"%s\",\"%s\",0,0,\"\"\r\n",username,passwd);
	Delay_s(2);
	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+MQTTCLIENTID=0,\"%s\"\r\n",clientID);
	Delay_s(2);
	//链接阿里云设备
	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+MQTTCONN=0,\"%s\",1883,1\r\n",url);
	Delay_s(2);
	//发送MQTT域名
}


bool ESP8266_SUB()
{
	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+MQTTSUB=0,\"%s\",1\r\n",subtopic);
	Delay_ms(200);
	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+MQTTSUB=0,\"%s\",1\r\n",settopic);
	Delay_ms(200);
	//订阅MQTT主题
	if(strcmp(RxData,"OK"))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ESP8266_PUB()
{
	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+MQTTPUB=0,\"%s\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"%s\\\":%.2f\\,\\\"%s\\\":%.2f\\,\\\"%s\\\":%d}}\",0,0\r\n"
	,pubtopic,func1,temperature,func2,Humidity,func3,LED);
	Delay_ms(300);
	//发送格式
	if(strncmp(RxData,"+MQTTSUBRECV:",13)==0)
	{
		unsigned char i = 0;
		while(RxData[i++] != '\0')
		{
			if(strncmp((RxData+i),"success",7)==0)
			{
				return true;
			}
			if(strncmp((RxData+i),"error",5)==0)
			{
				return false;
			}
		}
	}
	//比较云端返回信息词
	return false;
}

void ESP8266_GET()
{
	if(strncmp(RxData,"+MQTTSUBRECV:",13)==0)
	{
		uint8_t i=0;
		while(RxData[i++] != '\0')
		{
			if(strncmp((RxData+i),func3,11)==0)
			{
				while(RxData[i++] != ':');
				LED = RxData[i];
				if(LED == '1')
				{
					LED_ON();
				}
				else if(LED == '0')
				{
					LED_OFF();
				}
			}
		}
	}
	//解析云端下传Get
}
