
#ifndef DELAY_H
#define DELAY_H

#include "stm32f429xx.h"

void delay_ticks(uint32_t ticks);

#define delay_us(us)\
	delay_ticks((us * (SystemCoreClock / 8)) / 1000000)

#define delay_ms(ms)\
	delay_ticks((ms * (SystemCoreClock / 8)) / 1000)

#endif
