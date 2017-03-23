   //ͷ�ļ�����
#include <avr/io.h>        //io�˿ڼĴ��������ļ����������
#include <util/delay.h>       //GCC�е���ʱ����ͷ�ļ�

//�˿�λ����
#define RS PD5           //����/������ƶ� 0���1����
#define RW PD6           //��/дѡ����ƶ� 0д��1��
#define E PD7            //ʹ�ܶ�          �½��ض����ߵ�ƽд
//#define PSB PA3       //���ݴ��䷽ʽѡ��ˣ�H��8λ��4λ���ڷ�ʽ��L�����ڷ�ʽ 

//��������
void LCD12864PortInit(void);   //�˿ڳ�ʼ��
void LCD12864Init(void);       //LCD��ʼ��
void LCD12864WriteCommand(unsigned char ucCommand);   //LCDдָ��
void LCD12864WriteData(unsigned char ucData);  //LCDд����
void LCD12864BusyCheck(void);   //��д��⺯����ÿ�ζ�Һ������ǰ��Ҫ���ж�д���

int main(void)            
{
	LCD12864PortInit();   //�˿ڳ�ʼ��

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
	//LCD���ݶ˿�����
	PORTB = 0x00;         //
	DDRB = 0xFF;           //���ö˿�PBȫ��Ϊ�����
	
	PORTD = 0x00;
	DDRD |= (1 << RS) | (1 << RW) | (1 << E);
	_delay_ms(15);
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
	PORTD &= ~(1 << E);    //E=0��ֹͣд��������������д��ô��
	_delay_ms(5);          // _delay_us(5)?
}

void LCD12864BusyCheck(void)
{
    unsigned int uiTemp;
	uiTemp = 3000;
	
	DDRB = 0x00;             //PB����Ϊ����ڣ�׼����ȡ����
	PORTD &= ~(1 << RS);      //RS=0��������
	PORTD |= (1 << RW);    //RW=1����ָ��
	PORTD |= (1 << E);      //E=1��ʹ��
	
	while((0x80 & PINB) && (uiTemp--));   //���æ�źţ�ֱ��æ�ź�Ϊ0���ܶ�д
	
	PORTD &= ~(1 << E);   //E=0
	DDRB = 0xFF;          //PA����Ϊ����ڣ�׼����˿ڷ�������
}
