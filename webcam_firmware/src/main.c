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
	
	
	write_wifi_command("set uart.flow 0 on \r\n");
	write_wifi_command("set bus.command.rx_bufsize 5000\r\n");
	write_wifi_command("set sy i g wlan 20\r\n");
	write_wifi_command("set sy i g network 19\r\n");
	write_wifi_command("set sy i g softap 21\r\n");
	
	
	
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
		while(ioport_get_pin_level(WIFI_STATUS_PIN)==0){
			ioport_set_pin_level(WIFI_RESET_PIN, 0);
			delay_ms(100);
			ioport_set_pin_level(WIFI_RESET_PIN, 1);
			delay_ms(500);
		}
		write_wifi_command();
		if(no_open_streams){
			delay_ms(1000);
			no_open_streams=0;
		} else {
			uint8_t get_image = start_capture();
			write_image_to_file();
		}
		
		
	}
	
	
	
}