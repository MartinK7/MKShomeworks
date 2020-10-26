
extern const uint8_t max7219_customRegValues[16];
extern const uint8_t max7219_barGraphRegValues[16];

void max7219_init();
void max7219_cmd(uint16_t value);
void max7219_led(uint32_t leds);
void max7219_barGraph(uint8_t leds);
