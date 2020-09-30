
#include "stm32f429xx.h"
#include "delay.h"

static const char *alpha[] = {
    ".-",   //A
    "-...", //B
    "-.-.", //C
    "-..",  //D
    ".",    //E
    "..-.", //F
    "--.",  //G
    "....", //H
    "..",   //I
    ".---", //J
    "-.-",  //K
    ".-..", //L
    "--",   //M
    "-.",   //N
    "---",  //O
    ".--.", //P
    "--.-", //Q
    ".-.",  //R
    "...",  //S
    "-",    //T
    "..-",  //U
    "...-", //V
    ".--",  //W
    "-..-", //X
    "-.--", //Y
    "--..", //Z
};
static const char *num[] = {
    "-----", //0
    ".----", //1
    "..---", //2
    "...--", //3
    "....-", //4
    ".....", //5
    "-....", //6
    "--...", //7
    "---..", //8
    "----.", //9
};

void morseChar(const char *recept, uint32_t delay) {
	for(;*recept != 0; ++recept) {
		// LED on
		GPIOG->BSRR = GPIO_BSRR_BS14;

		if(*recept == '.') {
			// Delay
			delay_ms(delay * 1);
		} else {
			delay_ms(delay * 3);
		}

		// LED off
		GPIOG->BSRR = GPIO_BSRR_BR14;
		delay_ms(delay * 1);
	}
}

void morse(const char *message, uint32_t delay) {
	for(;*message != 0; ++message) {
		if(*message == ' ')
			delay_ms(delay * 5); // Word gap
		else if(*message >= '0' && *message <= '9')
			morseChar(num[*message - '0'], delay);
		else if(*message >= 'A' && *message <= 'Z')
			morseChar(alpha[*message - 'A'], delay);
		else if(*message >= 'a' && *message <= 'z')
			morseChar(alpha[*message - 'a'], delay);

		// Character gap
		delay_ms(delay * 2);
	}
}

int main() {
	SystemInit();
	SystemCoreClockUpdate();

	// Enable clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;

	// Set pins as outputs
//	GPIOG->AFR[1]  &= ~(0x0FU << (13-8)*4 | 0x0FU << (14-8)*4); // No alternate
//	GPIOG->AFR[1]  |=  (0x00U << (13-8)*4 | 0x00U << (14-8)*4); //
	GPIOG->MODER   &= ~(0x03U <<  13*2 | 0x03U << 14*2); // General purpose output mode
	GPIOG->MODER   |=  (0x01U <<  13*2 | 0x01U << 14*2); //
	GPIOG->OSPEEDR &=  (0x03U <<  13*2 | 0x03U << 14*2); // Low speed
//	GPIOG->OSPEEDR |=  (0x00U <<  13*2 | 0x00U << 14*2); //
	GPIOG->OTYPER  &= ~(0x01U <<  13*1 | 0x01U << 14*1); // Output push-pull (reset state)
	GPIOG->PUPDR   &= ~(0x03U <<  13*2 | 0x03U << 14*2); // No pull-up, pull-down

	while(1)
	{
		morse("SOS AHOJ", 100);
		delay_ms(2000);
	}
}
