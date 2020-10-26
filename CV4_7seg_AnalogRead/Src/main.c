
#include "stm32f429xx.h"
#include "max7219.h"

/*
 * PE2 - SPI4_SCK - MAX7219_CLK
 * PE4 - SPI4_NSS - MAX7219_LOAD_NSS
 * PE6 - SPI4_MOSI - MAX7219_DIN
 *
 * PE3 - GPIO_IN_PULLUP - Input button S1
 * PE5 - GPIO_IN_PULLUP - Input button S2
 *
 * PC3 - ADC1_IN13 - Analog input
 */

volatile uint32_t S1 = 0, S2 = 0;
volatile uint32_t delay_ms_cnt = 0;

void SysTick_Handler() {
	if(S1 > 0)S1--;
	if(S2 > 0)S2--;
	if(delay_ms_cnt > 0)delay_ms_cnt--;
}

void delay_ms(uint32_t time_ms) {
	delay_ms_cnt = time_ms;
	while(delay_ms_cnt);
}

#define MAX7219_DECIMAL_POINT 0x800000
void show(uint32_t value, uint32_t decimalPoint) {
	value %= 10000;
	uint32_t a = value / 1000;
	value -= a * 1000;
	uint32_t b = value / 100;
	value -= b * 100;
	uint32_t c = value / 10;
	value -= c * 10;
	uint32_t d = value;

	max7219_led(
			(max7219_customRegValues[a] << 24 |
			max7219_customRegValues[b] << 16 |
			max7219_customRegValues[c] << 8 |
			max7219_customRegValues[d]) | decimalPoint);
}

void button_init() {
	// Enable clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;

//	GPIOE->AFR[0]  &= ~(0x0FU << (5-0)*4 | 0x0FU << (3-0)*4); // No alternate
//	GPIOE->AFR[0]  |=  (0x00U << (5-0)*4 | 0x00U << (3-0)*4); //
	GPIOE->MODER   &= ~(0x03U <<  5*2 | 0x03U << 3*2); // Input mode
//	GPIOE->MODER   |=  (0x00U <<  5*2 | 0x00U << 3*2); //
	GPIOE->OSPEEDR &=  (0x03U <<  5*2 | 0x03U << 3*2); // Low speed
//	GPIOE->OSPEEDR |=  (0x00U <<  5*2 | 0x00U << 3*2); //
	GPIOE->OTYPER  &= ~(0x01U <<  5*1 | 0x01U << 3*1); // Output push-pull (reset state)
	GPIOE->PUPDR   &= ~(0x03U <<  5*2 | 0x03U << 3*2); // Pull-ups
	GPIOE->PUPDR   |=  (0x01U <<  5*2 | 0x01U << 3*2); //
}

volatile uint16_t raw_trimr;
volatile uint16_t raw_temp;
volatile uint16_t raw_volt;

void ADC_IRQHandler(void) {
	if(ADC1->SR & ADC_SR_EOC) {
		static uint32_t i = 0;

		switch(i) {
			case 0:
				raw_trimr = *(uint16_t*)&ADC1->DR;
				i++;
				break;
			case 1:
				raw_temp = *(uint16_t*)&ADC1->DR;
				i++;
				break;
			case 2:
				raw_volt = *(uint16_t*)&ADC1->DR;
				i = 0;
				break;
		}

		ADC1->SR &= ~ADC_SR_EOC; //clear flag
		ADC1->SR &= ~ADC_SR_OVR; //clear flag
	}
}

void analog_init() {
	// Enable clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

//	GPIOC->AFR[0]  &= ~(0x0FU << (3-0)*4); // No alternate
//	GPIOC->AFR[0]  |=  (0x00U << (3-0)*4); //
//	GPIOC->MODER   &= ~(0x03U <<  3*2); // Analog mode
	GPIOC->MODER   |=  (0x03U <<  3*2); //
//	GPIOC->OSPEEDR &=  (0x03U <<  3*2); // Low speed
//	GPIOC->OSPEEDR |=  (0x00U <<  3*2); //
//	GPIOC->OTYPER  &= ~(0x01U <<  3*1); // Output push-pull (reset state)
	GPIOC->PUPDR   &= ~(0x03U <<  3*2); // No - Pull-ups
//	GPIOC->PUPDR   |=  (0x01U <<  3*2); //

	ADC1->CR1  &= ~ADC_CR1_RES;                 // ADC1 resolution 12 bit
	ADC1->CR1  |= ADC_CR1_SCAN | ADC_CR1_EOCIE;

	ADC1->CR2   |= ADC_CR2_CONT | ADC_CR2_EOCS; // Continuous conversion

	ADC1->SQR1 &= ~ADC_SQR1_L;              // 3 channel sequence length
	ADC1->SQR1 |= (3-1) << ADC_SQR1_L_Pos;  //
	ADC1->SQR3 |= (13 << ADC_SQR3_SQ1_Pos); // Channel 13
	ADC1->SQR3 |= (16 << ADC_SQR3_SQ2_Pos); // Temp. sensor
	ADC1->SQR3 |= (17 << ADC_SQR3_SQ3_Pos); // Vrefint
	ADC1->SMPR1 |= 7 << ADC_SMPR1_SMP13_Pos; // channel 13 sample 84 cycles
	ADC1->SMPR1 |= 7 << ADC_SMPR1_SMP16_Pos; // channel 16 sample 480 cycles
	ADC1->SMPR1 |= 7 << ADC_SMPR1_SMP17_Pos; // channel 17 sample 480 cycles
	ADC123_COMMON->CCR |= ADC_CCR_TSVREFE; // Enable read internal temp and voltage

	ADC1->CR2 |= ADC_CR2_ADON; //ADC on
	ADC1->CR2 |= ADC_CR2_SWSTART; //start conversion

	NVIC_SetPriority(ADC_IRQn, 4);
	NVIC_EnableIRQ(ADC_IRQn); //IRQ handler
}

uint16_t analog_read() {
	ADC1->CR2 |= ADC_CR2_SWSTART; //start conversion
	while(!(ADC1->SR & ADC_SR_EOC)); //wait
	return ADC1->DR;
}

#define OLD_VALUES_COUNT 6 // I need more then 1 old values
uint32_t filter(uint32_t input) {
	static uint32_t last_values[OLD_VALUES_COUNT] = {0};

	// shift data
	for(uint32_t i=0; i<OLD_VALUES_COUNT-1; ++i) {
		last_values[i] = last_values[i+1];
	}

	// load new value
	last_values[OLD_VALUES_COUNT-1] = input;

	// Calculate summary
	uint32_t sum = 0;
	for(uint32_t i=0; i<OLD_VALUES_COUNT; ++i) {
		sum += last_values[i];
	}

	return sum / OLD_VALUES_COUNT;
}


/* Internal voltage reference calibration value address */
#define VREFINT_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFF7A2A))
uint32_t getVrefVoltage() {
	uint32_t voltage = 330 * (*VREFINT_CAL_ADDR) / raw_volt;
	return voltage;
}

/* Temperature sensor calibration value address */
#define TEMP110_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFF7A2C))
#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFF7A2E))
int32_t getCoreTemperature() {
	/*
		int32_t temperature = (raw_temp - (int32_t)(*TEMP30_CAL_ADDR));
		temperature = temperature * (int32_t)(110 - 30);
		temperature = temperature / (int32_t)(*TEMP110_CAL_ADDR - *TEMP30_CAL_ADDR);
		temperature = temperature + 25;
	*/
	float temperature = (float)raw_temp/4095.0f*((float)getVrefVoltage(raw_volt)/100.0f);
	temperature -= 0.75f;
	temperature /= 0.0025;
	temperature += 25;

	return temperature * 100.0f;
}

int main() {
	SystemInit();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000); //1ms

	max7219_init(); // Init, do not use internal decode table
	analog_init();
	button_init();
	while(1) {

		if(!(GPIOE->IDR & GPIO_IDR_ID3) || S1>5) {
			show( filter(getCoreTemperature()) , MAX7219_DECIMAL_POINT);
			if(!S1)S1 = 3000;
		} else if(!(GPIOE->IDR & GPIO_IDR_ID5) || S2>5) {
			show( filter(getVrefVoltage()) , MAX7219_DECIMAL_POINT);
			if(!S2)S2 = 3000;
		} else {
			// Print analog value (I have more then 3 digits, so lets print all of them)
			show(filter(raw_trimr), 0);
		}

		// BarGraph
		static int32_t bar = 0;
		// Recalculate
		bar = raw_trimr / 511;
		// Clamp
		bar = bar > 8 ? 8 : bar;
		bar = bar < 0 ? 0 : bar;
		// Show
		max7219_barGraph(max7219_barGraphRegValues[bar]);

		delay_ms(25);
	}
}
