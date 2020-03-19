/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>
#include "wifi.h"

void board_init(void)
{
	
	ioport_init();
	
	ioport_set_pin_dir(WIFI_RESET_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(WIFI_RESET_PIN, true);
	
	//ioport_set_pin_dir(PIN_LED, IOPORT_DIR_OUTPUT);
	//ioport_set_pin_level(PIN_LED, false);
	
	ioport_set_pin_dir(WIFI_CTS_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(WIFI_CTS_PIN, false);
	
	ioport_set_pin_dir(CAM_RST, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(CAM_RST, false);
	
	ioport_set_pin_dir(WIFI_STATUS, IOPORT_DIR_INPUT);
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
}
