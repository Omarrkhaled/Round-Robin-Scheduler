
#include<stdint.h>
#include "led.h"
#include "main.h"



void delay(uint32_t count)
{
  for(uint32_t i = 0 ; i < count ; i++);
}

void Init_LEDs(void)
{

	*RCC_AHB1ENR_REG |= (1 << 2);
	*GPIOC_MODE_REG |= (1 << (2 * LED_1));
	*GPIOC_MODE_REG |= (1 << (2 * LED_2));
	*GPIOC_MODE_REG |= (1 << (2 * LED_3));
	*GPIOC_MODE_REG |= (1 << (2 * LED_4));

#if 0
	//configure the outputtype
	*pGpioOpTypeReg |= ( 1 << (2 * LED_GREEN));
	*pGpioOpTypeReg |= ( 1 << (2 * LED_ORANGE));
	*pGpioOpTypeReg |= ( 1 << (2 * LED_RED));
	*pGpioOpTypeReg |= ( 1 << (2 * LED_BLUE));
#endif
}

void toggle_led(uint16_t led_no)
{
  *GPIOC_ODR_REG ^= (1 << led_no);
}

