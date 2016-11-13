#include <avr/io.h>        //io�˿ڼĴ��������ļ����������
#include <util/delay.h>       //GCC�е���ʱ����ͷ�ļ�

//�˿�λ����
#define RS PD5           //����/������ƶ� 0���1����
#define RW PD6           //��/дѡ����ƶ� 0д��1��
#define E PD7            //ʹ�ܶ�          �½��ض����ߵ�ƽд
//#define PSB PA3          //���ݴ��䷽ʽѡ��ˣ�H��8λ��4λ���ڷ�ʽ��L�����ڷ�ʽ 

//ȫ�ֱ�������
unsigned char* strs[] = {"adadw           ",
						 "LCD12864Һ������",
						 "    �����      ",
						 "  ��ѧ�뼼��ϵ  ",
						 "  hahhahhhhhhh  ",
						 "  hahhaaaahhhh  ",
						 "  ffffffffffff  ",
						 "  dddddddddddd  ",};

unsigned char* lines[] = {"                ",
						  "                ",
						  "                ",
						  "                "};

//��������
void LCD12864PortInit(void);   //�˿ڳ�ʼ��
void LCD12864Init(void);       //LCD��ʼ��
void LCD12864WriteCommand(unsigned char ucCommand);   //LCDдָ��
void LCD12864WriteData(unsigned char ucData);  //LCDд����
void LCD12864BusyCheck(void);   //��д��⺯����ÿ�ζ�Һ������ǰ��Ҫ��
//�ж�д���

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
	
	LCD12864PortInit();   //�˿ڳ�ʼ��
	LCD12864Init();       //LCD��ʼ��

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
	//LCD���ݶ˿�����
	PORTB = 0x00;         //
	DDRB = 0xFF;           //���ö˿�PBȫ��Ϊ�����
	 
	PORTD = 0x00;
	DDRD |= (1 << RS) | (1 << RW) | (1 << E);
	_delay_ms(15);
}

void LCD12864Init()
{
	LCD12864WriteCommand(0x01);  //����
	_delay_ms(15);
	LCD12864WriteCommand(0x38);  //��ʾģʽ 
	_delay_ms(5);
	LCD12864WriteCommand(0x0f);  //��ʾ���ؿ��ƣ�����ʾ�������ʾ����
//����˸
	LCD12864WriteCommand(0x0c);  //��ʾ���ؿ��ƣ�����ʾ����겻��ʾ��
//��겻��˸
	_delay_ms(5);
	LCD12864WriteCommand(0x06);  //������ã�����дһ���ַ��󣬵�ַָ
//���һ������һ���������ƶ�
	_delay_ms(5);
}

void LCD12864WriteCommand(unsigned char ucCommand)
{
	LCD12864BusyCheck();
	
	PORTD &= ~(1 << RS);     //RS=0��д����
	PORTD &= ~(1 << RW);     //RW=0��дָ��
	PORTD |= (1 << E);       //E=1��д����
	_delay_ms(2);
	PORTB = ucCommand;         //ָ�������ݶ˿�
	PORTD &= ~(1 << E);     //E=0��ֹͣд����
    _delay_ms(5);
	
}

void LCD12864WriteData(unsigned char ucData)
{
	LCD12864BusyCheck();

	PORTD |= (1 << RS);      //RS=1��д����
	PORTD &= ~(1 << RW);    //RW=0��дָ��
	PORTD |= (1 << E);      //E=1��д����
	_delay_ms(2);  
	PORTB = ucData;        // ���������ݶ˿�   
	PORTD &= ~(1 << E);    //E=0��ֹͣд����
	_delay_ms(5);
	
}

void LCD12864BusyCheck(void)
{
    unsigned int uiTemp;
	uiTemp = 3000;
	
	DDRB = 0x00;             //PB����Ϊ����ڣ�׼����ȡ����
	PORTD &= ~(1 << RS);      //RS=0��������
	PORTD |= (1 << RW);    //RW=1����ָ��
	PORTD |= (1 << E);      //E=1��ʹ��
	
	while((0x80 & PINB) && (uiTemp--));   //���æ�źţ�ֱ��æ�ź�Ϊ0�����ܽ��ж�д����
	
	PORTD &= ~(1 << E);   //E=0
	DDRB = 0xFF;          //PA����Ϊ����ڣ�׼����˿ڷ�������
	
}

