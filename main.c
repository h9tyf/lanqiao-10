#include "var.h"
#include "ds1302.h"
#include "iic.h"
#include "onewire.h"

u8 debug;
u8 show_num = 151;

long SysTick;

u8 digital_tube[8] = {1, 2, 3, 4, 5, 6, 7, 8};
u8 tab[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff, 0xc1, 0x8e};
							//0			1			2			3			4			5			6			7			8			9		OFF		U		F

enum SHOW show_state = VOL;
enum DAC_MODE dac_mode = V2;
u8 led_state = 1;// initial on
u8 digital_tube_state = 1;//initial on

u8 dac_v100;
unsigned int fre_count1;
unsigned int fre_count2;
unsigned int fre;

void LatchControl(u8 num, u8 value)
{
	P0 = value;
	_nop_();
	P2 = (num << 5) | 0x1f;
	_nop_();
	P2 &= 0x1f;
	_nop_();
}

u8 index;
void func() interrupt 3
{
	u8 value;
	SysTick++;
	
	LatchControl(7, tab[OFF]);
	LatchControl(6, 1<<index);
	value = tab[digital_tube[index]];
	if(index == 5 && show_state == VOL && digital_tube_state == 1){
		value &= 0x7f;
	}
	LatchControl(7, value);
	index = (index + 1)%8;
	
}

void Timer1Init(void)		//1??@12.000MHz
{
	AUXR |= 0x40;		//?????1T??
	TMOD &= 0x0F;		//???????
	TL1 = 0x20;		//??????
	TH1 = 0xD1;		//??????
	TF1 = 0;		//??TF1??
	TR1 = 1;		//???1????
}

void Timer0Init(void)		//1??@12.000MHz
{
	AUXR |= 0x80;		//?????1T??
	TMOD &= 0xF0;		//???????
	TMOD |= 0x04;
	TL0 = 0;		//??????
	TH0 = 0;		//??????
	TF0 = 0;		//??TF1??
	TR0 = 1;		//???1????
}


void get_vrb2()
{
	long temp;
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x43);
	IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	temp = IIC_RecByte();
	IIC_Ack(0);
	IIC_Stop();
	
	dac_v100 = temp * 500 / 256 ;
	//dac_v100 = temp;
}

//change output of dac
void change_output_v()
{
	long out_v;
	if(dac_mode == V2){
		out_v = 2  * 256 / 5;
	} else {
		out_v = dac_v100 * 256L / 500L;
	}
	if(out_v >= 255){
		out_v = 255;
	}
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x43);
	IIC_WaitAck();
	IIC_SendByte(out_v);
	IIC_WaitAck();
	IIC_Stop();
}

//get fre of ne555
void get_fre()
{
	
	fre_count1 = fre_count2;
	fre_count2 = (TH0 << 8) | TL0;
	fre = fre_count2 - fre_count1;
}

void change_state()
{
	check_button();
	respond_to_button();
}

void main()
{
	Timer0Init();
	Timer1Init();
	EA = 1;
	ET1 = 1;
	P3 |= 0x0f;
	LatchControl(4, 0xff);
	debug = 0;
	
	while(1){
		long tickBkp = SysTick;
		
		if(tickBkp % 2 == 0){
			EA = 0;
			change_state();
			EA = 1;
		}
		
		if(tickBkp % 100 == 0){
			EA = 0;
			get_vrb2();
			change_output_v();
			EA = 1;
		}
		
		if(tickBkp % 1000 == 0){
			EA = 0;
			get_fre();
			EA = 1;
		}
		
		if(tickBkp % 2 == 0){
			EA = 0;
			change_show();
			EA = 1;
		}
		
		
		
		while(tickBkp == SysTick);
	}
	
	
}