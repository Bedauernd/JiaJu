#include <stm32f10x.h>
#include <DHT11.h>
#include <Delay.h>

//数据手册的主机为STM32，从机为DHT11
float temp, hum;
//温湿度

void DH11_GPIO_Init_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}//stm32输出


void DH11_GPIO_Init_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}//stm32输入


//主机发送开始信号
void DHT11_Start(void)
{
	DH11_GPIO_Init_OUT();
	//配置stm32为输出模式
	DHT11_H; 
	/*
	因为DHT11在空闲的时候总线为高电平
	在要使用时，提前拉高总线
	*/
	Delay_us(30);
	//等待总线拉高
	DHT11_L;
	Delay_ms(20);
	/*
	根据DHT11手册要求，主机信号拉低，即stm32对应端口输出低电平
	时间应为：18~30ms，type为20ms
	*/
	DHT11_H;
	Delay_us(30);
	/*
	stm32拉高电平20~40us
	进行拉高等待，DHT11采集数据
	*/
	DH11_GPIO_Init_IN();
	//配置stm32为输入模式，等待接收DHT11采集数据
}


//获取一个字节
char DHT11_Rec_Byte(void)
{
	unsigned char i = 0;
	unsigned char data = 0;
	/*为什么采用unsigned char
	因为DHT11一次是传输8个bit的数据，也就是8位大小
	一个char是一个字节，也就是8个bit，但是第一位是符号位，表示正负
	一个unsigned char （无符号char） 也是一个字节，但是第一位是记数位
	*/
	for( i = 0 ; i < 8 ; i++ )
	{
		while( Read_Data == 0 );
		/*从第一个bit开始，等待低电平变高电平
		因为DHT11中位数据表示0和表示1的区别在于高电平的持续时长
		所以用while等待低电平时间过去
		*/
		Delay_us(35); 
		//延迟35us是区别数据0和数据1，0只有26~28us
		
		data <<= 1;
		/*数据左移，在data末尾添加0
		比如 00000010 -> 00000100
		*/
		
		if( Read_Data == 1 )
		{
			data |= 1; //数据+1
		}
		//如果过了35us还是高电平的话就是数据1
		//否则不变
		
		while( Read_Data == 1 );
		//高电平变低电平，等待高电平结束
	}
	return data;
}

//获取数据

void DHT11_REC_Data(void)
{
	unsigned char  H_H,H_L,T_H,T_L,CHECK;
	
	DHT11_Start();
	//主机发送信号,等待数据
	DHT11_H;
	//拉高电平
	if( Read_Data == 0 ) //判断DHT11是否响应
	{
		while( Read_Data == 0); //低电平变高电平，等待低电平结束
		while( Read_Data == 1); //高电平变低电平，等待高电平结束
		/*
		DHT11在发送数据前会先拉低发送响应信号再拉高准备输出
		*/
		H_H = DHT11_Rec_Byte();
		H_L = DHT11_Rec_Byte();
		T_H = DHT11_Rec_Byte();
		T_L = DHT11_Rec_Byte();
		CHECK = DHT11_Rec_Byte();
		//接收5个数据
		DHT11_L;
		Delay_us(54);
		/*
		在发送完一轮数据后，DHT11会释放总线即总线为低电平
		时间应为：52~56us，type为54us
		*/
		DHT11_H; 
		//随后总线拉高进入空闲状态。
		
		if(H_H + H_L + T_H + T_L == CHECK)
		{
			temp = ( T_H * 100 + T_L ) * 1.0 / 100;
			hum = ( H_H * 100 + H_L ) * 1.0 / 100;
		}
		//和检验位对比，判断校验接收到的数据是否正确
	}
}

float DHT11_Get_Temp()
{
	return temp;
}

float DHT11_Get_Hum()
{
	return hum;
}
