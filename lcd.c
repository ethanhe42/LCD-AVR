#include <avr/io.h>        //io端口寄存器配置文件，必须包含
#include <util/delay.h>       //GCC中的延时函数头文件

//端口位定义
#define RS PD5           //数据/命令控制端 0命令，1数据
#define RW PD6           //读/写选择控制端 0写，1读
#define E PD7            //使能端          下降沿读，高电平写
//#define PSB PA3          //数据传输方式选择端，H，8位或4位并口方式；L，串口方式 

//全局变量声明
unsigned char* strs[] = {"adadw           ",
						 "LCD12864液晶测试",
						 "    计算机      ",
						 "  科学与技术系  ",
						 "  hahhahhhhhhh  ",
						 "  hahhaaaahhhh  ",
						 "  ffffffffffff  ",
						 "  dddddddddddd  ",};

unsigned char* lines[] = {"                ",
						  "                ",
						  "                ",
						  "                "};

//函数声明
void LCD12864PortInit(void);   //端口初始化
void LCD12864Init(void);       //LCD初始化
void LCD12864WriteCommand(unsigned char ucCommand);   //LCD写指令
void LCD12864WriteData(unsigned char ucData);  //LCD写数据
void LCD12864BusyCheck(void);   //读写检测函数，每次对液晶操作前都要进
//行读写检测

void println(unsigned char* ucData){
	int i =0;
	for (i=0;i<3;i++){
		lines[i]=lines[i+1];
	}
	lines[3] = ucData;
		LCD12864WriteCommand(0x80+0x00);
		for(i = 0;i < 16;i++)
		{
			LCD12864WriteData(lines[0][i]);
		}
		LCD12864WriteCommand(0x90+0x00);
		for(i = 0;i < 16;i++)
		{
			LCD12864WriteData(lines[1][i]);
		}
	LCD12864WriteCommand(0x88+0x00);
		for(i = 0;i < 16;i++)
		{
			LCD12864WriteData(lines[2][i]);
		}
	LCD12864WriteCommand(0x98+0x00);
		for(i = 0;i < 16;i++)
		{
			LCD12864WriteData(lines[3][i]);
		}
}

int main(void)            
{
  	unsigned char i;
	
	LCD12864PortInit();   //端口初始化
	LCD12864Init();       //LCD初始化

	LCD12864WriteCommand(0x80+0x00);

	int j=0;
	while(1)
	{	
	println(strs[j]);
		j++;
		if (j==8) j=0;
		_delay_ms(1000);
	}
}

void LCD12864PortInit()
{      
	//LCD数据端口设置
	PORTB = 0x00;         //
	DDRB = 0xFF;           //配置端口PB全部为输出口
	 
	PORTD = 0x00;
	DDRD |= (1 << RS) | (1 << RW) | (1 << E);
	_delay_ms(15);
}

void LCD12864Init()
{
	LCD12864WriteCommand(0x01);  //清屏
	_delay_ms(15);
	LCD12864WriteCommand(0x38);  //显示模式 
	_delay_ms(5);
	LCD12864WriteCommand(0x0f);  //显示开关控制，开显示，光标显示，光
//标闪烁
	LCD12864WriteCommand(0x0c);  //显示开关控制，开显示，光标不显示，
//光标不闪烁
	_delay_ms(5);
	LCD12864WriteCommand(0x06);  //光标设置，读或写一个字符后，地址指
//针加一，光标加一，整屏不移动
	_delay_ms(5);
}

void LCD12864WriteCommand(unsigned char ucCommand)
{
	LCD12864BusyCheck();
	
	PORTD &= ~(1 << RS);     //RS=0，写命令
	PORTD &= ~(1 << RW);     //RW=0，写指令
	PORTD |= (1 << E);       //E=1，写操作
	_delay_ms(2);
	PORTB = ucCommand;         //指令送数据端口
	PORTD &= ~(1 << E);     //E=0，停止写操作
    _delay_ms(5);
	
}

void LCD12864WriteData(unsigned char ucData)
{
	LCD12864BusyCheck();

	PORTD |= (1 << RS);      //RS=1，写数据
	PORTD &= ~(1 << RW);    //RW=0，写指令
	PORTD |= (1 << E);      //E=1，写操作
	_delay_ms(2);  
	PORTB = ucData;        // 数据送数据端口   
	PORTD &= ~(1 << E);    //E=0，停止写操作
	_delay_ms(5);
	
}

void LCD12864BusyCheck(void)
{
    unsigned int uiTemp;
	uiTemp = 3000;
	
	DDRB = 0x00;             //PB口置为输入口，准备读取数据
	PORTD &= ~(1 << RS);      //RS=0，读命令
	PORTD |= (1 << RW);    //RW=1，读指令
	PORTD |= (1 << E);      //E=1，使能
	
	while((0x80 & PINB) && (uiTemp--));   //监测忙信号，直到忙信号为0，才能进行读写操作
	
	PORTD &= ~(1 << E);   //E=0
	DDRB = 0xFF;          //PA口置为输出口，准备向端口发送数据
	
}

