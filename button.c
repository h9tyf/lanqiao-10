#include "var.h"
u8 button_check[4] = {1, 1, 1, 1};
u8 button_count[4] = {0, 0, 0, 0};
u8 button_flag[4] = {0, 0, 0, 0};
u8 button_state[4] = {1, 1, 1, 1};

void check_button()
{
	u8 i;
	if(P30 == 0){
		button_check[S7] = 0;
	} else {
		button_check[S7] = 1;
	}
	if(P31 == 0){
		button_check[S6] = 0;
	} else {
		button_check[S6] = 1;
	}
	if(P32 == 0){
		button_check[S5] = 0;
	} else {
		button_check[S5] = 1;
	}
	if(P33 == 0){
		button_check[S4] = 0;
	} else {
		button_check[S4] = 1;
	}

	
	for(i = 0;i < 4; i++){
		if(button_check[i] != button_state[i]){
			button_count[i]++;
		} else {
			button_count[i] = 0;
		}
	}
	
	for(i = 0; i < 4; i++){
		if(button_count[i] >= 5 && button_check[i] != button_state[i]){
			button_state[i] = 1 - button_state[i];
			if(button_state[i] == 0){
				button_flag[i] = 1;
				show_num = 152;
			}
		}
	}
	
}

void respond_to_button()
{
	if(button_flag[S4] == 1){
		button_flag[S4] = 0;
		show_state = 3 - show_state;
		show_num = 5;
	}
	if(button_flag[S5] == 1){
		button_flag[S5] = 0;
		dac_mode = 3 - dac_mode;
		show_num = 6;
	}
	if(button_flag[S6] == 1){
		button_flag[S6] = 0;
		led_state = 1 - led_state;
		show_num = 7;
	}
	if(button_flag[S7] == 1){
		button_flag[S7] = 0;
		digital_tube_state = 1 - digital_tube_state;
		show_num = 8;
	}
}