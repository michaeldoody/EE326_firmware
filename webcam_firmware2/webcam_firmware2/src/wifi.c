/*
 * Wifi.c
 *
 * Created: 2/9/2020 3:48:48 PM
 *  Author: david
 */

#include "wifi.h"
#include "camera.h"
#include <string.h>

volatile uint32_t received_byte_wifi = 0;
volatile unsigned int input_pos_wifi = 0;
volatile uint32_t wifi_setup_button_flag=false;
volatile uint32_t counts = 0;
volatile uint32_t wifi_comm_success=false;
volatile uint32_t image_length=0;
volatile uint32_t start_image_transfer = 0;
volatile uint32_t wait_flag = 0;

void wifi_usart_handler(void)
{
	uint32_t ul_status;

	/* Read USART status. */
	ul_status = usart_get_status(WIFI_USART);

	/* Receive buffer is full. */
	if (ul_status & US_CSR_RXBUFF) {
		usart_read(WIFI_USART, &received_byte_wifi);
		process_incoming_byte_wifi((uint8_t)received_byte_wifi);
	}
}

static void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);
	
	wifi_comm_success = true;

	process_data_wifi();
	for (int jj = 0;jj<1000;jj++) input_line_wifi[jj] = 0;
	input_pos_wifi = 0;
}

void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);
	
	wifi_setup_button_flag = true;
}

void configure_usart_wifi(void)
{
	gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);
	gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
	gpio_configure_pin(PIN_USART0_CTS_IDX, PIN_USART0_CTS_FLAGS);
	//gpio_configure_pin(PIN_USART1_RTS_IDX, PIN_USART1_RTS_FLAGS);

	static uint32_t ul_sysclk;
	const sam_usart_opt_t usart_console_settings = {
		BOARD_USART_BAUDRATE,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL,
		/* This field is only used in IrDA mode. */
		0
	};

	/* Get peripheral clock. */
	ul_sysclk = sysclk_get_peripheral_hz();

	/* Enable peripheral clock. */
	sysclk_enable_peripheral_clock(BOARD_ID_USART);

	/* Configure USART. */
	usart_init_hw_handshaking(WIFI_USART, &usart_console_settings, ul_sysclk);

	/* Disable all the interrupts. */
	usart_disable_interrupt(WIFI_USART, ALL_INTERRUPT_MASK);

	/* Enable TX & RX function. */
	usart_enable_tx(WIFI_USART);
	usart_enable_rx(WIFI_USART);

	usart_enable_interrupt(WIFI_USART, US_IER_RXRDY);

	/* Configure and enable interrupt of USART. */
	NVIC_EnableIRQ(USART_IRQn);
}

void configure_wifi_command_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(WIFI_COMM_ID);

	/* Adjust PIO debounce filter using a 10 Hz filter. */
	pio_set_debounce_filter(WIFI_COMM_PIO, WIFI_COMM_PIN_MSK, 10);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WIFI_COMM_PIO, WIFI_COMM_ID, WIFI_COMM_PIN_MSK,
			WIFI_COMM_ATTR, wifi_command_response_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_COMM_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_COMM_PIO, WIFI_COMM_PIN_MSK);
}

void configure_wifi_web_setup_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(WEB_SETUP_BUTTON_ID);

	/* Adjust PIO debounce filter using a 10 Hz filter. */
	pio_set_debounce_filter(WEB_SETUP_BUTTON_PIO, WEB_SETUP_BUTTON_PIN_MSK, 10);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WEB_SETUP_BUTTON_PIO, WEB_SETUP_BUTTON_ID, WEB_SETUP_BUTTON_PIN_MSK,
			WEB_SETUP_BUTTON_ATTR, wifi_web_setup_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WEB_SETUP_BUTTON_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WEB_SETUP_BUTTON_PIO, WEB_SETUP_BUTTON_PIN_MSK);
}

void write_wifi_command(char* comm, uint8_t cnt)
{
	//Write command to the AMW136
	usart_write_line(WIFI_USART, comm);
	
	//Wait for either an acknowledgment or a timeout
	counts = 0;
	while (counts < cnt)
	{
		if (wifi_comm_success==true)
		{
			wifi_comm_success = false;
			return;
		}
		
	}
	return;
}

void write_image_to_file(void)
{
	if (find_image_len()==0)
	{
		return;
	}
	
	image_length = end_of_image - start_of_image;
	char string[50] = {0};
	sprintf(string,"image_transfer %d\r\n", image_length);
	write_wifi_command(string, 20);
	delay_ms(100);
	
	while(!wifi_comm_success)
	{
		
	}
	
	uint32_t img = start_of_image;
	while (img < end_of_image)
	{
		usart_putchar(WIFI_USART, image_buffer[img]);
		img++;
	}
	
	usart_write_line(WIFI_USART, "Complete\r\n");
	delay_s(2);
	
}

void process_incoming_byte_wifi(uint8_t in_byte) {
	
	input_line_wifi[input_pos_wifi++ ] = in_byte;
}

void process_data_wifi(void) {
	
	if (strstr(input_line_wifi, "None")) {
		wait_flag = 1;
	} 
	if (strstr(input_line_wifi, "Image")){
		start_image_transfer = 1;
	}
}