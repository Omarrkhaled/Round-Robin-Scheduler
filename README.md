# Round-Robin-Scheduler

## Description
A cyclic scheduler which assigns a time slice for each task then puts it in blocking state for a certain amount of time

## How it works
- System timer is configured for ticking every 1 ms.
- All tasks are initially in the READY state.
- when a task calls a delay function, it blocks itself for a specified number of ticks.
- Context switching takes place in the PendSV exception handler, Systick timer handler just updates the system tick count and pends the PendSV exception

## Behaviour of tasks and Idle task (which is just a for loop that puts the processor into sleep mode until the next interrupt)
- In my application i set the block times for task 1, 2, 3 as 1000, 500, 250 ticks respectively.
- Idle task doesn't block at all.
![1](https://user-images.githubusercontent.com/29959479/199999579-d87f14b5-6280-4c7c-b4bc-7cbffc46ecf1.PNG)

