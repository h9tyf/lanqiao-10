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
long fre;

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
void func() interrupt 1
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

void Timer0Init(void)		//1??@12.000MHz
{
	AUXR |= 0x80;		//?????1T??
	TMOD &= 0xF0;		//???????
	TL0 = 0x20;		//??????
	TH0 = 0xD1;		//??????
	TF0 = 0;		//??TF0??
	TR0 = 1;		//???0????
}

void get_vrb2()
{
	long temp;
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x03);
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
	u8 out_v;
	if(dac_mode == V2){
		out_v = 2 / 5 * 256;
	} else {
		out_v = dac_v100 / 100 / 5 * 256;
	}
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x03);
	IIC_WaitAck();
	IIC_SendByte(out_v);
	IIC_WaitAck();
	IIC_Stop();
}

//get fre of ne555
void get_fre()
{
	fre = 3000;
}

void change_state()
{
	check_button();
	respond_to_button();
}

void main()
{
	Timer0Init();
	EA = 1;
	ET0 = 1;
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
		
		if(tickBkp % 10 == 0){
			EA = 0;
			get_vrb2();
			//change_output_v();
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