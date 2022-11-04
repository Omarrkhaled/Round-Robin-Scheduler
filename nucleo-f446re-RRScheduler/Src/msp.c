

#include "main.h"

// Assessing scheduler stack statically
__attribute__((naked)) void SchedStack_Init(uint32_t schedStackStart)
{
	__asm volatile("MSR MSP, %0" ::"r"(schedStackStart));
	__asm volatile("BX LR");
}

// Assessing each task private stack statically
void Init_TaskStack(void)
{
	// Here, we store the values of the addresses of each task stack reached point
	__vo uint32_t *pPSP;

	user_tasks[0].psp_value = IDLE_START_STACK;
	user_tasks[1].psp_value = TASK1_START_STACK;
	user_tasks[2].psp_value = TASK2_START_STACK;
	user_tasks[3].psp_value = TASK3_START_STACK;

	user_tasks[0].current_state = STATE_READY;
	user_tasks[1].current_state = STATE_READY;
	user_tasks[2].current_state = STATE_READY;
	user_tasks[3].current_state = STATE_READY;

	user_tasks[0].task_handler = IdleTask_Handler;
	user_tasks[1].task_handler = Task1_Handler;
	user_tasks[2].task_handler = Task2_Handler;
	user_tasks[3].task_handler = Task3_Handler;

	for (uint32_t i = 0; i < MAX_TASKS; i++)
	{
		pPSP = (uint32_t*)user_tasks[i].psp_value;

		// Initialize xPSR place in the stack frame with a valid value "T-bit position = 1,
		// indicates the use of thumb ISA
		pPSP--;
		*pPSP = xPSR_DUMMY_VAL;

		// Initialize PC place with the next task (to be executed) address value
		pPSP--;
		*pPSP = (uint32_t)(user_tasks[i].task_handler);

		// Hold a valid EXC_RETURN value into the LR register, in order to exit the task to thread mode
		// using PSP pointer
		pPSP--;
		*pPSP = 0xFFFFFFFD;

		// Initially, assign the general purpose register places to 0's
		for (uint32_t j = 0; j < 13; j++)
		{
			pPSP--;
			*pPSP = 0;
		}

		// Save the last reached point in each task stack up until this point
		user_tasks[i].psp_value = (uint32_t)pPSP;
	}
}

// Loads the system timer with the desired number of ticks
void Init_SystickTimer(uint32_t clk_cycles)
{
	// Calculate the count value of the SYSTICK timer
	uint32_t cnt_value = (SYSTICK_CLK / clk_cycles) - 1;
	*SRVR_REG |= cnt_value;

	// Choose the internal processor clock as SYSTICK clock source
	// enable TICKINT to trigger the SYSTICK ISR whenever counter reaches 0
	*SCSR_REG |= (1 << 2);
	*SCSR_REG |= (1 << 1);

	// Enable the SYSTICK timer
	*SCSR_REG |= (1 << 0);
}

// Switches the SP from MSP to PSP
__attribute__((naked)) void change_sp_to_psp(void)
{
	// Push the value of the LR in order to save it before branching to another C function
	__asm volatile("PUSH {LR}");
	__asm volatile("BL get_psp_value");
	__asm volatile("MSR PSP, R0");

	__asm volatile("POP {LR}"); // retrieve the LR register value which will redirect to main()

	// Configure SP to PSP
	__asm volatile("MOV R0, 0x02");
	__asm volatile("MSR CONTROL, R0");
	__asm volatile("BX LR");
}

// Puts the task into blocked state then pends the PendSV exception
void task_delay(uint32_t block_cnt)
{
	INTERRUPT_DISABLE();

	if (current_task)
	{
		user_tasks[current_task].block_count = g_tick_cnt + block_cnt;
		user_tasks[current_task].current_state = STATE_BLOCKED;
		schedule_next();
	}

	INTERRUPT_ENABLE();
}

// Updates the global tick count variable, tracking the system time
void update_global_tick_count(void)
{
	g_tick_cnt++;
}

// Unblocks the next task
void unblock_task(void)
{
	for (uint32_t i = 1; i < MAX_TASKS; i++)
	{
		if (user_tasks[i].current_state == STATE_BLOCKED)
		{
			if (user_tasks[i].block_count == g_tick_cnt)
			{
				user_tasks[i].current_state = STATE_READY;
			}
		}
	}
}

// Returns the last place at which PSP stopped at for the current task
uint32_t get_psp_value(void)
{
	return user_tasks[current_task].psp_value;
}

// Saves the place of the last stacked item of the current task in it's private stack
void save_psp_value(uint32_t current_psp_value)
{
	user_tasks[current_task].psp_value = current_psp_value;
}

// Round-Robin scheduling
void update_task(void)
{
	State_t state = STATE_BLOCKED;

	for (uint32_t i = 0; i < MAX_TASKS; i++)
	{
		current_task++;
		current_task %= MAX_TASKS;
		state = user_tasks[current_task].current_state;
		if ((state == STATE_READY) && (current_task != 0))
		{
			break;
		}
	}
	if (state == STATE_BLOCKED)
	{
		current_task = 0;
	}
}

void EnableProcessorFaults(void)
{
	// setting MEMFAULTENA, BUSFAULTENA, USGFAULTENA bits on the
	// System Handler and Control Status Register (SHCSR) of the
	// System Control Block (SCB) unit of the processor
	*SHCSR_REG |= (0x7 << 16);
}

// Pends the PendSV exception
void schedule_next(void)
{
	*ICSR_REG |= (1 << 28);
}
