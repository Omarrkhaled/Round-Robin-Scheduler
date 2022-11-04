/*
 * main.h
 *
 *  Created on: Oct 18, 2022
 *      Author: omark
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>
#include <stdio.h>
#include "led.h"

#define __vo					volatile


/* Structure Definitions */

// @Task_states
typedef enum
{
	STATE_BLOCKED,
	STATE_READY
} State_t;

// Task Control Block
typedef struct
{
	uint32_t psp_value;						/*!< Last stacked item in the task private stack >*/
	uint32_t block_count;					/*!< Relative time which the task will block for >*/
	State_t current_state;					/*!< Possible task states @Task_states >*/
	void (*task_handler)(void);				/*!< Address of the task handler >*/
} TCB_t;

#define INTERRUPT_DISABLE() do{__asm volatile("MOV R0, #0x1"); __asm volatile("MSR PRIMASK, R0");}while(0)
#define INTERRUPT_ENABLE()  do{__asm volatile("MOV R0, #0x0"); __asm volatile("MSR PRIMASK, R0");}while(0)

#define MAX_TASKS					4

#define SHCSR_REG					(uint32_t*)0xE000ED24UL

/* Stacks Calculations */
#define SRAM_START					0x20000000UL
#define SIZE_SRAM					(128U * 1024U)
#define SRAM_END					(SRAM_START + SIZE_SRAM)

#define SIZE_TASK_STACK				1024U
#define SIZE_SCHED_STACK			1024U
#define TASK1_START_STACK			SRAM_END
#define TASK2_START_STACK			(SRAM_END - (1 * SIZE_TASK_STACK))
#define TASK3_START_STACK			(SRAM_END - (2 * SIZE_TASK_STACK))
#define IDLE_START_STACK			(SRAM_END - (3 * SIZE_TASK_STACK))
#define SCHED_START_STACK			(SRAM_END - (4 * SIZE_TASK_STACK))

/* SYSTICK and SCB related register addresses */
#define SCSR_REG					(uint32_t*)0xE000E010UL // Control and Status Register
#define SRVR_REG					(__vo uint32_t*)0xE000E014UL // Reload Value Register

#define ICSR_REG					(uint32_t*)0xE000ED04UL

#define TICK_HZ						1000U
#define HSI_CLK						16000000U
#define SYSTICK_CLK					HSI_CLK

#define xPSR_DUMMY_VAL				0x01000000UL

extern TCB_t user_tasks[MAX_TASKS];
extern uint32_t g_tick_cnt;
extern uint32_t current_task;

/* User Tasks */
void IdleTask_Handler(void);
void Task1_Handler(void);
void Task2_Handler(void);
void Task3_Handler(void);

void Init_TaskStack(void);
__attribute__((naked)) void SchedStack_Init(uint32_t schedStackStart);
void Init_SystickTimer(uint32_t tick_hz);
void task_delay(uint32_t block_cnt);
__attribute__((naked)) void change_sp_to_psp(void);


void EnableProcessorFaults(void);
void save_psp_value(uint32_t current_psp_value);
uint32_t get_psp_value(void);
void update_task(void);
void update_global_tick_count(void);
void unblock_task(void);
void schedule_next(void);
uint32_t Unblock_Schedule(void);


#endif /* MAIN_H_ */
