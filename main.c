   //头文件包含
#include <avr/io.h>        //io端口寄存器配置文件，必须包含
#include <util/delay.h>       //GCC中的延时函数头文件

//端口位定义
#define RS PD5           //数据/命令控制端 0命令，1数据
#define RW PD6           //读/写选择控制端 0写，1读
#define E PD7            //使能端          下降沿读，高电平写
//#define PSB PA3       //数据传输方式选择端，H，8位或4位并口方式；L，串口方式 

//函数声明
void LCD12864PortInit(void);   //端口初始化
void LCD12864Init(void);       //LCD初始化
void LCD12864WriteCommand(unsigned char ucCommand);   //LCD写指令
void LCD12864WriteData(unsigned char ucData);  //LCD写数据
void LCD12864BusyCheck(void);   //读写检测函数，每次对液晶操作前都要进行读写检测

int main(void)            
{
	LCD12864PortInit();   //端口初始化

	int circle_x[6] = {0, 1, 2, 3, 4, 5};
	int circle_y[6] = {8, 8, 7, 6, 5, 4};
	int point[16][16] = {0};
	uint8_t  output[16][4] = {0};
	
	int i,j;
	for (i=0;i<16;i++) {
		point[i][i/2] = 1;
	}	
	for (i=0;i<16;i++) {
		point[i][16-i/2] = 1;
	}	
	for (i=0;i<16;i++) {
		for (j=0;j<16;j++) {
			if (point[i][j] == 1) {
				output[i][j/8+1] = output[i][j/8+1] + 1 << (j%8);
			}
		}
	}

	
	LCD12864WriteCommand(0x36);
	for (i=0; i<32; i++) {
		LCD12864WriteCommand(0x80+i);
		LCD12864WriteCommand(0x80);
		for (j=0;j<16;j++) {
			if (i<16 && j<2) {
				LCD12864WriteData(output[i][2-j]);
			} else {
				LCD12864WriteData(0x00);
			}
		}	
	}
	for (i=0; i<32; i++) {
		LCD12864WriteCommand(0x80+i);
		LCD12864WriteCommand(0x88);
		for (j=0;j<16;j++) {
				LCD12864WriteData(0x00);
		}	
	}
	while(1);
	LCD12864WriteCommand(0x36);	
	
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
	PORTD &= ~(1 << E);    //E=0，停止写操作；可以连续写入么？
	_delay_ms(5);          // _delay_us(5)?
}

void LCD12864BusyCheck(void)
{
    unsigned int uiTemp;
	uiTemp = 3000;
	
	DDRB = 0x00;             //PB口置为输入口，准备读取数据
	PORTD &= ~(1 << RS);      //RS=0，读命令
	PORTD |= (1 << RW);    //RW=1，读指令
	PORTD |= (1 << E);      //E=1，使能
	
	while((0x80 & PINB) && (uiTemp--));   //监测忙信号，直到忙信号为0才能读写
	
	PORTD &= ~(1 << E);   //E=0
	DDRB = 0xFF;          //PA口置为输出口，准备向端口发送数据
}
