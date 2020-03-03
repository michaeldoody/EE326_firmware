/**
 * \file
 *
 * \brief User board configuration template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef CONF_BOARD_H_
#define CONF_BOARD_H_

#define PIN_LED                       PIO_PA20_IDX
#define WIFI_CTS_PIN                  PIO_PA7_IDX
#define WIFI_RTS_PIN					PIO_PA8_IDX
#define RESET_WIFI_PIN					PIO_PB0_IDX
#define RESET_CAMERA_PIN				PIO_PA20_IDX
#define IMAGE_WIDTH                    (320UL)
#define IMAGE_HEIGHT                   (240UL)



#endif /* CONF_BOARD_H_ */ 
