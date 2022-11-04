

#include "main.h"

void SysTick_Handler(void)
{
	update_global_tick_count();
	unblock_task();
	*ICSR_REG |= (1 << 28);
}

__attribute__((naked)) void PendSV_Handler(void)
{
	/* Here, context switching is done */
	// Save the context (state) of the current task
	__asm volatile("PUSH {LR}");
	__asm volatile("MRS R0, PSP");
	__asm volatile("STMDB R0!, {R4-R11}");
	__asm volatile("BL save_psp_value");

	// Schedule the next task
	__asm volatile("BL update_task");

	// Retrieve the context (state) of the next scheduled task
	 __asm volatile("BL get_psp_value");
	 __asm volatile("LDMIA R0!, {R4-R11}");
	 __asm volatile("MSR PSP, R0");

	 // Return to thread mode
	 __asm volatile("POP {LR}");
	 __asm volatile("BX LR");

}

void HardFault_Handler(void)
{
	while (1);
}

void UsageFault_Handler(void)
{
	while (1);
}

void MemManage_Handler(void)
{
	while (1);
}

void BusFault_Handler(void)
{
	while (1);
}
