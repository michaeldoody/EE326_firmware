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
	
	// TODO - pull down Wifi pin
	delay_ms(100);
	// TODO - set Wifi pin high
	

	while (1) { //wait for network connection
		if (wifi_setup_button_flag){
			
		}
		
	}
	init_camera();
	configure_camera();
	// tell Wifi to turn off command prompt and echo
	
	
	
	
}