
#include "stm32f429xx.h"
#include "max7219.h"

const uint8_t max7219_customRegValues[16] = {
	0b1111110, //0
	0b0110000, //1
	0b1101101,
	0b1111001,
	0b0110011,
	0b1011011,
	0b1011111,
	0b1110000,
	0b1111111,
	0b1111011,
	0b1110111,
	0b0011111,
	0b1001110,
	0b0111101,
	0b1001111,//E
	0b1000111 //F
};

const uint8_t max7219_barGraphRegValues[16] = {
	0, 1, 3, 7, 15, 31, 63, 127, 255
};

void max7219_init() {
	// Enable clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;

	// SPI4 - 7seg LED driver MAX7219
	// PE2 - SCK
	// PE4 - NSS
	// PE6 - MOSI
	GPIOE->AFR[0]  &= ~(0x0FU << (2-0)*4 | 0x0FU << (4-0)*4 | 0x0FU << (6-0)*4); // Alternate - SPI4
	GPIOE->AFR[0]  |=  (0x05U << (2-0)*4 | 0x05U << (4-0)*4 | 0x05U << (6-0)*4); //
	GPIOE->MODER   &= ~(0x03U <<  2*2 | 0x03U << 4*2 | 0x03U << 6*2); // Alternate mode
	GPIOE->MODER   |=  (0x02U <<  2*2 | 0x02U << 4*2 | 0x02U << 6*2); //
	GPIOE->OSPEEDR &=  (0x03U <<  2*2 | 0x03U << 4*2 | 0x03U << 6*2); // High speed
	GPIOE->OSPEEDR |=  (0x02U <<  2*2 | 0x02U << 4*2 | 0x02U << 6*2); //
	GPIOE->OTYPER  &= ~(0x01U <<  2*1 | 0x01U << 4*1 | 0x01U << 6*1); // Output push-pull (reset state)
	GPIOE->PUPDR   &= ~(0x03U <<  2*2 | 0x03U << 4*2 | 0x03U << 6*2); // No pull-ups

	// Init SPI
	SPI4->CR1  = SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_DFF | SPI_CR1_MSTR; // 1-wire, TX-only, 16bit, MSBfirst, master
	SPI4->CR1 |= 0b001 << SPI_CR1_BR_Pos;
	SPI4->CR2 = SPI_CR2_FRF;
	SPI4->CR1 |= SPI_CR1_SPE; // Enable SPI

	// Init MAX7219
	max7219_cmd(0x0F00); // Turn off segments test
	max7219_cmd(0x0900); // No use of decode table
	max7219_cmd(0x0A0F); // Maximum bright
	max7219_cmd(0x0B04); // Scan line 4 digits + 1 bar graph
	max7219_cmd(0x0C01); // Normal OP
}

extern void delay_ms(uint32_t time_ms);
void max7219_cmd(uint16_t value) {
	// TX value over SPI
	*(uint16_t*)&SPI4->DR = value;
	// Wait for transfer
	while(!(SPI4->SR & SPI_SR_TXE));
	// Delay
	delay_ms(3);
}

void max7219_led(uint32_t leds) {
	max7219_cmd(0x0400 | ((leds & 0x000000FF) >> 0));	// Most right digit
	max7219_cmd(0x0300 | ((leds & 0x0000FF00) >> 8));
	max7219_cmd(0x0200 | ((leds & 0x00FF0000) >> 16));
	max7219_cmd(0x0100 | ((leds & 0xFF000000) >> 24));	// Most left digit
	max7219_cmd(0); // NOP - Some clock bug
}

void max7219_barGraph(uint8_t leds) {
	max7219_cmd(0x0500 | (leds & 0xFF));
	max7219_cmd(0); // NOP - Some clock bug
}
