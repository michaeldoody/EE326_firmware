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

	configure_usart_wifi();
	configure_wifi_command_pin();

	while (1) {
		delay_ms(500);
		usart_write_line(WIFI_USART, "Hello\r\n");
	}
}