#include <asf.h>
#include "wifi.h"
#include "camera.h"
#include "conf_board.h"
#include "conf_clock.h"

int main (void)
{
	//Initialize clock and board definitions
	sysclk_init();
	wdt_disable(WDT);
	board_init();

	//Configure and start the Timer
	configure_tc();
	
	//Configure the WiFi USART, Command pin, and Web Setup pin
	configure_usart_wifi();
	configure_wifi_command_pin();
	configure_wifi_web_setup_pin();
	
	//Reset wiFi module
	ioport_set_pin_level(WIFI_RESET_PIN, 0);
	delay_ms(100);
	ioport_set_pin_level(WIFI_RESET_PIN, 1);
	
	//Initialize and configure the camera
	init_camera();
	configure_camera();

	while (ioport_get_pin_level(WIFI_STATUS_PIN)==0) { //wait for network connection
		if (wifi_setup_button_flag){
			write_wifi_command("web setup", 20);
			delay_ms(100);
			wifi_setup_button_flag = 0;
		}
		
	}
	
	// tell wifi to turn off command prompt and echo
	while(1) {
		if(wifi_setup_button_flag){
			write_wifi_command("web setup", 20);
			delay_ms(100);
			wifi_setup_button_flag=0;
			while (ioport_get_pin_level(WIFI_STATUS_PIN)==0) { //wait for network connection
				if (wifi_setup_button_flag){
					write_wifi_command("web setup", 20);
					delay_ms(100);
					wifi_setup_button_flag = 0;
				}
				
			}
		}
		
	}
	
	
	
}