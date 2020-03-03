#include <asf.h>
#include "wifi.h"
#include "camera.h"
#include "conf_board.h"
#include "conf_clock.h"

int main (void)
{
	sysclk_init();
	wdt_disable(WDT);
	board_init();

	configure_tc();
	configure_usart_wifi();
	configure_wifi_command_pin();
	configure_wifi_web_setup_pin();
	
	
	//ioport_set_pin_level(WIFI_RESET_PIN, 0);
	delay_ms(100);
	//ioport_set_pin_level(WIFI_RESET_PIN, 1);
	
	init_camera();
	configure_camera();

	while (ioport_get_pin_level(WIFI_STATUS_PIN)==0) { //wait for network connection
		if (wifi_setup_button_flag){
			write_wifi_command(web setup);
			delay_ms(100);
			wifi_setup_button_flag = 0;
		}
		
	}
	
	// tell Wifi to turn off command prompt and echo
	while(1) {
		if(wifi_setup_button_flag){
			write_wifi_command(web setup);
			delay_ms(100);
			wifi_setup_button_flag=0;
			while (ioport_get_pin_level(WIFI_STATUS_PIN)==0) { //wait for network connection
				if (wifi_setup_button_flag){
					write_wifi_command(web setup);
					delay_ms(100);
					wifi_setup_button_flag = 0;
				}
				
			}
		}
		
	}
	
	
	
}