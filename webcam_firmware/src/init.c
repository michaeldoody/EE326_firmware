/*
 * init.c
 *
 * Created: 2/27/2020 8:45:57 AM
 *  Author: ece-lab3
 */ 

#include <asf.h>
#include "board.h""
#include "conf_board.h>"
#include "gpio.h"
#include "ioport.h"

void board_init(void)
{
	ioport_init();
	
	ioport_set_pin_dir(PIN_LED, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIN_LED, false);
	
	ioport_set_pin_dir(WIFI_CTS_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(WIFI_CTS_PIN, false);
}