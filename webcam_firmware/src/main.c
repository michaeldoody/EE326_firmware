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
	
	write_wifi_command("set uart.flow 0 on \r\n");
	write_wifi_command("set bus.command.rx_bufsize 5000\r\n");
	write_wifi_command("set sy i g wlan 20\r\n");
	write_wifi_command("set sy i g network 19\r\n");
	write_wifi_command("set sy i g softap 21\r\n");
	write_wifi_command("set system.cmd.gpio 13\r\n");
	write_wifi_command("set wl n o 14\r\n");
	write_wifi_command("save\r\n");
	write_wifi_command("reboot\r\n");
	
		
	
	
	// tell Wifi to turn off command prompt and echo

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