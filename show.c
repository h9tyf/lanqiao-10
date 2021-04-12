#include "var.h"
void change_show()
{
	u8 i;
	u8 led_value = 0xff;
	
	if(debug == 1){
		u8 temp = show_num;
		for(i = 7; i >= 1; i--){
			if(i != 7 && temp == 0){
				digital_tube[i] = OFF;
			} else {
				digital_tube[i] = temp % 10;
			}
			temp /= 10;
		}
		return;
	}
	
	if(digital_tube_state == 1){
		if(show_state == FRE){
			long temp;
			temp = fre;
			digital_tube[0] = SHOW_F;
			digital_tube[1] = OFF;
			for(i = 7; i >= 2; i--){
				if(i != 7 && temp == 0){
					digital_tube[i] = OFF;
				} else {
					digital_tube[i] = temp % 10;
				}
				temp /= 10;
			}
		} else if(show_state == VOL){
			u8 temp;
			if(dac_mode == V2){
				temp = 200;
			} else {
				temp = dac_v100;
			}
			
			digital_tube[0] = SHOW_U;
			for(i = 7; i >= 1; i--){
				if(i < 5 && temp == 0){
					digital_tube[i] = OFF;
				} else {
					digital_tube[i] = temp % 10;
				}
				temp /= 10;
			}
		}
	}else{
		for(i = 0; i < 8; i++){
			digital_tube[i] = OFF;
		}
	}
	
	if(led_state == 1){
		led_value = 0xff;
		if(show_state == VOL){
			led_value &= 0xfe;
		} else if(show_state == FRE){
			led_value &= 0xfd;
		}
		
		if(dac_v100 >= 150 && dac_v100 < 250){
			led_value &= 0xfb;
		}
		if(dac_v100 >= 350){
			led_value &= 0xfb;
		}
		
		if(fre >= 1000 && fre < 5000){
			led_value &= 0xf7;
		}
		if(fre >= 10000){
			led_value &= 0xf7;
		}
		
		if(dac_mode == VRB2){
			led_value &= 0xef;
		}
	}
	LatchControl(4, led_value);
}