/*
 * led.h
 *
 *  Created on: 28-Dec-2019
 *      Author: nieki
 */

#ifndef LED_H_
#define LED_H_

#define RCC_BASE_ADDR			0x40023800UL
#define RCC_AHB1ENR_REG			(uint32_t*)(RCC_BASE_ADDR + 0x30)

#define GPIOC_BASE_ADDR			0x40020800UL
#define GPIOB_BASE_ADDR			0x40020400UL
#define GPIOC_MODE_REG			(uint32_t*)GPIOC_BASE_ADDR
#define GPIOC_ODR_REG			(uint32_t*)(GPIOC_BASE_ADDR + 0x14)
#define GPIOB_MODE_REG			(uint32_t*)GPIOB_BASE_ADDR
#define GPIOB_ODR_REG			(uint32_t*)(GPIOB_BASE_ADDR + 0x14)

#define LED_1  		8
#define LED_2 		9
#define LED_3   	6
#define LED_4		5

#define DELAY_COUNT_1MS 		 1250U
#define DELAY_COUNT_1S  		(1000U * DELAY_COUNT_1MS)
#define DELAY_COUNT_500MS  		(500U  * DELAY_COUNT_1MS)
#define DELAY_COUNT_250MS 		(250U  * DELAY_COUNT_1MS)
#define DELAY_COUNT_125MS 		(125U  * DELAY_COUNT_1MS)

void Init_LEDs(void);
void toggle_led(uint16_t);
void delay(uint32_t count);

#endif /* LED_H_ */
