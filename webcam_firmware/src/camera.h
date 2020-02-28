/*
 * Camera.h
 *
 * Created: 2/9/2020 3:49:19 PM
 *  Author: david
 */ 


#ifndef CAMERA_H_
#define CAMERA_H_

#define OV7740_VSYNC_PIO			PIOA
#define OV7740_VSYNC_ID				ID_PIOA
#define OV7740_VSYNC_MASK			PIO_PA15
#define OV7740_VSYNC_TYPE			PIO_PULLUP
#define OV7740_DATA_BUS_PIO			PIOA
#define OV_DATA_BUS_MASK            (0xFF000000UL)
#define OV_DATA_BUS_PIO             PIOA
#define OV_DATA_BUS_ID              ID_PIOA
#define OV_POWER_PIO				PIOC
#define OV_POWER_MASK				PIO_PC10




#define CAP_DEST                    (0x60000000UL)

#define ID_BOARD_TWI				ID_TWI0
#define TWI_CLK						(400000UL)
#define BOARD_TWI			        TWI0
#define BOARD_TWI_IRQn		        TWI0_IRQn


typedef struct twi_options {
	//! MCK for TWI.
	uint32_t master_clk;
	//! The baud rate of the TWI bus.
	uint32_t speed;
	//! The desired address.
	uint8_t chip;
	//! SMBUS mode (set 1 to use SMBUS quick command, otherwise don't).
	uint8_t smbus;
} twi_options_t;

#endif /* CAMERA_H_ */