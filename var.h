#ifndef _VAR_H
#define _VAR_H

#include <STC/STC15F2K60S2.H>
#include <intrins.h>

typedef unsigned char u8;

extern u8 debug;
extern u8 show_num;

//main
enum SHOW
{
	FRE = 1, VOL
};

enum DAC_MODE
{
	V2 = 1, VRB2
};

extern enum SHOW show_state;
extern enum DAC_MODE dac_mode;
extern u8 led_state;// initial on
extern u8 digital_tube_state;//initial on

#define OFF 10
#define SHOW_U 11
#define SHOW_F 12

extern u8 dac_v100;
extern unsigned int fre;


extern u8 digital_tube[8];

void change_state();
void LatchControl(u8 num, u8 value);

//button
#define S4 0
#define S5 1
#define S6 2
#define S7 3
void check_button();
void respond_to_button();

//show
void change_show();

#endif