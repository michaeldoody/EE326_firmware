#include <asf.h>
#include "wifi.h"
#include "camera.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "timer_interface.h"

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

	
	//Initialize and configure the camera
	init_camera();
	configure_camera();
	
	write_wifi_command("set uart.flow 0 on \r\n", 2);
	write_wifi_command("set bus.command.rx_bufsize 5000\r\n", 2);
	write_wifi_command("set sy i g wlan 20\r\n", 2);
	write_wifi_command("set sy i g network 19\r\n", 2);
	write_wifi_command("set sy i g softap 21\r\n" ,2);
	write_wifi_command("set system.cmd.gpio 13\r\n", 2);
	write_wifi_command("set wl n o 14\r\n", 2);
	write_wifi_command("set sy c p off\r\n", 2);
	write_wifi_command("set sy c e off\r\n", 2);
	write_wifi_command("save\r\n", 2);
	//write_wifi_command("reboot\r\n", 2);
	
	ioport_set_pin_level(WIFI_RESET_PIN, 0);
	delay_ms(100);
	ioport_set_pin_level(WIFI_RESET_PIN, 1);
	
	wifi_setup_button_flag=false;
	
	while (ioport_get_pin_level(WIFI_STATUS)==0) { //wait for network connection
		if (wifi_setup_button_flag){
			write_wifi_command("setup web\r\n", 1);
			delay_ms(100);
			wifi_setup_button_flag = 0;
		}
		
	}

	
	// tell Wifi to turn off command prompt and echo

	while(1) {
		if(wifi_setup_button_flag){
			write_wifi_command("setup web\r\n", 1);
			delay_ms(100);
			wifi_setup_button_flag=0;
			while (ioport_get_pin_level(WIFI_STATUS)==0) { //wait for network connection
				if (wifi_setup_button_flag){
					write_wifi_command("setup web\r\n", 1);
					delay_ms(100);
					wifi_setup_button_flag = 0;
				}
				
			}
		}
		while(ioport_get_pin_level(WIFI_STATUS)==0){
			ioport_set_pin_level(WIFI_RESET_PIN, 0);
			delay_ms(100);
			ioport_set_pin_level(WIFI_RESET_PIN, 1);
			delay_ms(500);
		}
		
		write_wifi_command("poll all\r\n", 5);
		

		if(wait_flag){
			delay_ms(1000);
			wait_flag=0;
			} else{
			uint8_t get_image = start_capture();
			write_image_to_file();
			
		}
		
		
	}
	
}