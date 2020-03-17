/*
 * Wifi.h
 *
 * Created: 2/9/2020 3:48:57 PM
 *  Author: david
 */

#ifndef WIFI_H_
#define WIFI_H_

#include <asf.h>
#include <string.h>


#define BOARD_ID_USART             ID_USART0
#define WIFI_USART				   USART0
#define BOARD_USART_BAUDRATE       115200
#define USART_Handler              USART0_Handler
#define USART_IRQn                 USART0_IRQn

#define ALL_INTERRUPT_MASK  0xffffffff

/** USART1 pin RX */
#define PIN_USART0_RXD    {PIO_PA5A_RXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_RXD_IDX        (PIO_PA5_IDX)
#define PIN_USART0_RXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART1 pin TX */
#define PIN_USART0_TXD    {PIO_PA6A_TXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_TXD_IDX        (PIO_PA6_IDX)
#define PIN_USART0_TXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART1 pin CTS */
#define PIN_USART0_CTS    {PIO_PA8A_CTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_CTS_IDX        (PIO_PA8_IDX)
#define PIN_USART0_CTS_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART1 pin RTS */
#define PIN_WIFI_RTS    {PIO_PA7A_RTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_WIFI_RTS_IDX        (PIO_PA7_IDX)
#define PIN_WIFI_RTS_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)

#define WIFI_COMM_ID            ID_PIOB
#define WIFI_COMM_PIO           PIOB
#define WIFI_COMM_PIN_MSK       PIO_PB11
#define WIFI_COMM_ATTR          PIO_IT_RISE_EDGE

#define WEB_SETUP_BUTTON_ID            ID_PIOA
#define WEB_SETUP_BUTTON_PIO           PIOA
#define WEB_SETUP_BUTTON_PIN_MSK       PIO_PA22
#define WEB_SETUP_BUTTON_ATTR          PIO_IT_RISE_EDGE

#define WIFI_RESET_PIN					PIO_PB0_IDX
#define WIFI_STATUS						PIO_PB13_IDX					
#define RESET_CAMERA_PIN				PIO_PA20_IDX
#define IMAGE_WIDTH                    (320UL)
#define IMAGE_HEIGHT                   (240UL)


void wifi_usart_handler(void);
void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask);
void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask);
void configure_usart_wifi(void);
void configure_wifi_command_pin(void);
void configure_wifi_web_setup_pin(void);
void write_wifi_command(char* comm, uint8_t cnt);
void write_image_to_file(void);
void process_incoming_byte_wifi(uint8_t in_byte);
void process_data_wifi();


volatile char input_line_wifi[1000];
volatile uint32_t received_byte_wifi;
volatile unsigned int input_pos_wifi;
volatile uint32_t wifi_setup_button_flag;
volatile uint32_t wifi_comm_success;
volatile uint32_t image_length;
volatile uint32_t start_image_transfer;
volatile uint32_t wait_flag;

#endif /* WIFI_H_ */