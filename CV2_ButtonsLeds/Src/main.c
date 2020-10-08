
#include "stm32f429xx.h"


volatile static uint32_t Tick = 0, LEDTick = 0;

void EXTI0_IRQHandler() {
	if(EXTI->PR & EXTI_PR_PR0) {
		EXTI->PR |= EXTI_PR_PR0;

		LEDTick = 500;
	}
}

void EXTI9_5_IRQHandler() {
	if(EXTI->PR & EXTI_PR_PR5) {
		EXTI->PR |= EXTI_PR_PR5;

		LEDTick = 1500;
	}
}

void SysTick_Handler() {
	Tick++;
	if(LEDTick)LEDTick--;
}

void processBlink() {
	static uint32_t delay = 0;

	if (Tick > delay + 300) {
		GPIOG->ODR ^= GPIO_ODR_OD14;
		delay = Tick;
	}

	if(LEDTick)
		GPIOG->ODR |= GPIO_ODR_OD13;
	else
		GPIOG->ODR &= ~(GPIO_ODR_OD13);
}

int main() {
	SystemInit();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000); //1ms

	// Enable clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN | RCC_AHB1ENR_GPIOAEN;

	// LED diodes - outputs
	// PG13 - LED Green
	// PG14 - LED Red
//	GPIOG->AFR[1]  &= ~(0x0FU << (13-8)*4 | 0x0FU << (14-8)*4); // No alternate
//	GPIOG->AFR[1]  |=  (0x00U << (13-8)*4 | 0x00U << (14-8)*4); //
	GPIOG->MODER   &= ~(0x03U <<  13*2 | 0x03U << 14*2); // General purpose output mode
	GPIOG->MODER   |=  (0x01U <<  13*2 | 0x01U << 14*2); //
	GPIOG->OSPEEDR &=  (0x03U <<  13*2 | 0x03U << 14*2); // Low speed
//	GPIOG->OSPEEDR |=  (0x00U <<  13*2 | 0x00U << 14*2); //
	GPIOG->OTYPER  &= ~(0x01U <<  13*1 | 0x01U << 14*1); // Output push-pull (reset state)
	GPIOG->PUPDR   &= ~(0x03U <<  13*2 | 0x03U << 14*2); // No pull-up, pull-down

	// Buttons - inputs
	// PA0 - Blue discovery button - External resistor pull-down
	// PA5 - OMRON external button - Pull-up enabled
//	GPIOA->AFR[0]  &= ~(0x0FU << (5-0)*4 | 0x0FU << (0-0)*4); // No alternate
//	GPIOA->AFR[0]  |=  (0x00U << (5-0)*4 | 0x00U << (0-0)*4); //
	GPIOA->MODER   &= ~(0x03U <<  5*2 | 0x03U << 0*2); // Input mode
//	GPIOA->MODER   |=  (0x01U <<  5*2 | 0x01U << 0*2); //
	GPIOA->OSPEEDR &=  (0x03U <<  5*2 | 0x03U << 0*2); // Low speed
//	GPIOA->OSPEEDR |=  (0x00U <<  5*2 | 0x00U << 0*2); //
	GPIOA->OTYPER  &= ~(0x01U <<  5*1 | 0x01U << 0*1); // Output push-pull (reset state)
	GPIOA->PUPDR   &= ~(0x03U <<  5*2 | 0x03U << 0*2); // Pull-ups
	GPIOA->PUPDR   |=  (0x01U <<  5*2 | 0x00U << 0*2); // A5-YES A0-NO

	// Interrupts
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;//PA0
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PA;//PA5
	EXTI->IMR |= EXTI_IMR_IM0 | EXTI_IMR_IM5;
	EXTI->FTSR |= EXTI_FTSR_TR5; // OMRON button press 1->0
	EXTI->RTSR |= EXTI_RTSR_TR0; // Blue button press 0->1
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);

	while(1) {
		processBlink();
	}
}
