/*
 * Camera.c
 *
 * Created: 2/9/2020 3:49:35 PM
 *  Author: david
 */ 

#include <asf.h>
#include "camera.h"

static volatile uint32_t g_ul_vsync_flag = false;

void vsync_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);

	g_ul_vsync_flag = true;
}

void init_vsync_interrupts(void)
{
	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(OV2640_VSYNC_PIO, OV2640_VSYNC_ID, OV2640_VSYNC_MASK,
			OV2640_VSYNC_TYPE, vsync_handler);

	/* Enable PIO controller IRQs */
	NVIC_EnableIRQ((IRQn_Type)OV2640_VSYNC_ID);
	
	pio_enable_interrupt(OV2640_VSYNC_PIO, OV2640_VSYNC_MASK)
}

void configure_twi(void)
{
	twi_options_t opt;
	
	/* Enable TWI peripheral */
	pmc_enable_periph_clk(ID_BOARD_TWI);

	/* Init TWI peripheral */
	opt.master_clk = sysclk_get_cpu_hz();
	opt.speed      = TWI_CLK;
	twi_master_init(BOARD_TWI, &opt);

	/* Configure TWI interrupts */
	NVIC_DisableIRQ(BOARD_TWI_IRQn);
	NVIC_ClearPendingIRQ(BOARD_TWI_IRQn);
	NVIC_SetPriority(BOARD_TWI_IRQn, 0);
	NVIC_EnableIRQ(BOARD_TWI_IRQn);
	
}



void pio_capture_init(Pio *p_pio, uint32_t ul_id)
{
	/* Enable peripheral clock */
	pmc_enable_periph_clk(ul_id);

	/* Disable pio capture */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_PCEN);

	/* Disable rxbuff interrupt */
	p_pio->PIO_PCIDR |= PIO_PCIDR_RXBUFF;

	/* 32bit width*/
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_DSIZE_Msk);
	p_pio->PIO_PCMR |= PIO_PCMR_DSIZE_WORD;

	/* Only HSYNC and VSYNC enabled */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_ALWYS);
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_HALFS);



}

static uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *uc_buf,
uint32_t ul_size)
{
	/* Check if the first PDC bank is free */
	if ((p_pio->PIO_RCR == 0) && (p_pio->PIO_RNCR == 0)) {
		p_pio->PIO_RPR = (uint32_t)uc_buf;
		p_pio->PIO_RCR = ul_size;
		p_pio->PIO_PTCR = PIO_PTCR_RXTEN;
		return 1;
		} else if (p_pio->PIO_RNCR == 0) {
		p_pio->PIO_RNPR = (uint32_t)uc_buf;
		p_pio->PIO_RNCR = ul_size;
		return 1;
		} else {
		return 0;
	}
}


void init_camera(void)
{
	gpio_configure_pin(OV_RST_GPIO, OV_RST_FLAGS);
	gpio_configure_pin(OV_HSYNC_GPIO, OV_HSYNC_FLAGS);
	gpio_configure_pin(OV_VSYNC_MASK, OV2640_VSYNC_TYPE);
	gpio_configure_pin(OV_DATA_BUS_D0, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D1, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D2, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D3, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D4, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D5, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D6, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D7, OV_DATA_BUS_FLAGS);
	
	pmc_enable_pllbck(7, 0x1, 1);
	
	
		
	/* Init Vsync handler*/
	init_vsync_interrupts();
	pio_disable_interrupt(OV2640_VSYNC_PIO, OV2640_VSYNC_MASK)
	vsync_flag = false;

	/* Init PIO capture*/
	pio_capture_init(OV_DATA_BUS_PIO, OV_DATA_BUS_ID);



	/* Init PCK1 to work at 24 Mhz */
	/* 96/4=24 Mhz */
	PMC->PMC_PCK[0] = (PMC_PCK_PRES_CLK_4 | PMC_PCK_CSS_PLLB_CLK);
	PMC->PMC_SCER = PMC_SCER_PCK0;
	while (!(PMC->PMC_SCSR & PMC_SCSR_PCK0)) {
	}
	
	configure_twi();
	
}

void configure_camera(void)
{
	/* ov7740 Initialization */
	while (ov_init(BOARD_TWI) == 1) {
	}

	/* ov7740 configuration */
	ov_configure(BOARD_TWI,JPEG_INIT);
	ov_configure(BOARD_TWI, YUV422);
	ov_configure(BOARD_TWI, JPEG);
	ov_configure(BOARD_TWI, JPEG_320x240);

	/* Wait 3 seconds to let the image sensor to adapt to environment */
	delay_ms(3000);
}


uint8_t start_capture(void)
{

	/* Enable vsync interrupt*/
	pio_enable_interrupt(OV2640_VSYNC_PIO, OV2640_VSYNC_MASK);

	/* Capture acquisition will start on rising edge of Vsync signal.
	 * So wait g_vsync_flag = 1 before start process
	 */
	while (!g_ul_vsync_flag) {
	}

	/* Disable vsync interrupt*/
	pio_disable_interrupt(OV2640_VSYNC_PIO, OV2640_VSYNC_MASK);

	/* Enable pio capture*/
	pio_capture_enable(OV_DATA_BUS_PIO);

	/* Capture data and send it to external SRAM memory thanks to PDC
	 * feature */
	pio_capture_to_buffer(OV_DATA_BUS_PIO, image_buffer,
			IMAGE_MAX >> 2);

	/* Wait end of capture*/
	while (!((OV_DATA_BUS_PIO->PIO_PCISR & PIO_PCIMR_RXBUFF) ==
			PIO_PCIMR_RXBUFF)) {
	}

	/* Disable pio capture*/
	pio_capture_disable(OV_DATA_BUS_PIO);

	/* Reset vsync flag*/
	g_ul_vsync_flag = false;
	
	uint8_t val = find_image_len();
	return val;
}


uint8_t find_image_len(void){
		uint8_t start_exists = 0;
		uint32_t arrSize = 0;
		start_of_image = 0;
		end_of_image = 0;
		while( arrSize+1 < IMAGE_MAX){
			if(image_buffer[arrSize] == 0xff && image_buffer[arrSize+1] == 0xd8){
				start_of_image = arrSize;
				start_exists = 1;
			}
			if(image_buffer[arrSize] == 0xff && image_buffer[arrSize+1] == 0xd9 && start_exists == 1){
				end_of_image = arrSize+2;
				return 1;
			}
			arrSize = arrSize + 1;
		}
		start_of_image = 0;
		end_of_image = 0;
		
		return 0;
	}
}