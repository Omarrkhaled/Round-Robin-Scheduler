# Round-Robin-Scheduler

## Description
A cyclic scheduler which assigns a time slice for each task then puts it in blocking state for a certain amount of time

## How it works
- System timer is configured for ticking every 1 ms.
- All tasks are initially in the READY state.
- when a task calls a delay function, it blocks itself for a specified number of ticks.
- Context switching takes place in the PendSV exception handler, Systick timer handler just updates the system tick count and pends the PendSV exception
