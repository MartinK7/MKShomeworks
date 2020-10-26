
#include "main.h"
#include "max7219.h"

extern SPI_HandleTypeDef hspi4;

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

void max7219_init(uint32_t useInternalTable) {
	// Init MAX7219
	max7219_cmd(0x0F00); // Turn off segments test
	if(useInternalTable)
		max7219_cmd(0x090F); // Use internal digits decode table (4 appply on digits)
	else
		max7219_cmd(0x0900); // No use of decode table
	max7219_cmd(0x0A0F); // Maximum bright
	max7219_cmd(0x0B03); // Scan line 4 digits
	max7219_cmd(0x0C01); // Normal OP
}

void max7219_cmd(uint16_t value) {
	HAL_SPI_Transmit(&hspi4, (uint8_t*)&value, 2, 1000);
	while(HAL_SPI_GetState(&hspi4) != HAL_SPI_STATE_READY);
	// Delay
	HAL_Delay(3);
}

void max7219_led(uint32_t leds) {
	max7219_cmd(0x0400 | ((leds & 0x000000FF) >> 0));	// Most right digit
	max7219_cmd(0x0300 | ((leds & 0x0000FF00) >> 8));
	max7219_cmd(0x0200 | ((leds & 0x00FF0000) >> 16));
	max7219_cmd(0x0100 | ((leds & 0xFF000000) >> 24));	// Most left digit
}
