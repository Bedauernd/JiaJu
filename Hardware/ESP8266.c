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
const char* pubtopic= "/sys/k1s5o00BfgU/test/thing/event/property/post";
const char* subtopic= "/sys/k1s5o00BfgU/test/thing/event/property/post_reply";

const char* func1 = "temperature";
const char* func2 = "Humidity";
const char* func3 = "LightSwitch";

void ESP8266_Init(void){
	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+RST\r\n");
	Delay_s(4);
	
	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+CWMODE=1\r\n");
	Delay_s(2);

	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n");
	Delay_s(2);
	
	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+CWJAP=\"%s\",\"%s\"\r\n",WiFi,WiFiPW);
	Delay_s(3);
	
	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+MQTTUSERCFG=0,1,\"NULL\",\"%s\",\"%s\",0,0,\"\"\r\n",username,passwd);
	Delay_s(2);
	
	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+MQTTCLIENTID=0,\"%s\"\r\n",clientID);
	Delay_s(2);
	
	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+MQTTCONN=0,\"%s\",1883,1\r\n",url);
	Delay_s(2);
}
	

bool ESP8266_SUB()
{
	memset(RxData,0,sizeof(RxData));
	Serial_Printf("AT+MQTTSUB=0,\"%s\",1\r\n",subtopic);
	Delay_ms(200);
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
	if(strcmp(RxData,"OK"))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void command(void)
{
	if(strncmp(RxData,"+MQTTSUBRECV:",13)==0)
	{
		uint8_t i=0;
		while(RxData[i++] != '\0')
		{
			if(strncmp((RxData+i),func3,12)==0)
			{
				while(RxData[i++] != ':');
				LED = RxData[i];
			}
		}
	}
}
