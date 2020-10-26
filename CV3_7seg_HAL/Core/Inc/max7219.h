
extern const uint8_t max7219_customRegValues[16];

void max7219_init(uint32_t useInternalTable);
void max7219_cmd(uint16_t value);
void max7219_led(uint32_t leds);
