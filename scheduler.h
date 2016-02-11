/********************************************************************************
* scheduler.h
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 1/24/16
*
* Description:
* This header file defines the class and methods for the Round Robin Scheduler
*
*******************************************************************************/
#ifndef _SCH_H
#define _SCH_H
#define TABLE_ROWS 1
#define TABLE_COLUMNS 5
#include <time.h>
#include <errno.h>
#include "fifo_queue.h"
#include "stack.h"
#include "cpu.h"
#include "pcb.h"


typedef enum process_interrupt{ISR_TIMER, ISR_IO_1_REQUEST, ISR_IO_2_REQUEST, 
            ISR_IO_1_COMPLETION, ISR_IO_2_COMPLETION} Interrupt;


typedef void (*interrupt_handler)(void*, CPU_p);
 
typedef struct schedule_cell{
   // State next_state;
    interrupt_handler handler;
}S_cell;
typedef S_cell *S_cell_p;
typedef struct process_scheduler{
    Node_p currently_running;
    FIFO_queue_p new_queue;
    FIFO_queue_p ready_queue;
    FIFO_queue_p running_queue;
    FIFO_queue_p interrupted_queue;
    FIFO_queue_p io_1_waiting_queue;
    FIFO_queue_p io_2_waiting_queue;
    FIFO_queue_p terminated_queue;
   
    S_cell state_table[TABLE_ROWS][TABLE_COLUMNS];
                                                  
} Scheduler;
typedef Scheduler *Scheduler_p;
typedef struct thread_data{
    CPU_p cpu;
    Scheduler_p scheduler;
    struct timespec sleep_time;
}thread_data;


Scheduler_p Scheduler_constructor();
void Scheduler_dispatcher(Scheduler_p, CPU_p);
void Scheduler_destructor(Scheduler_p);
void Scheduler_transfer_node(FIFO_queue_p, FIFO_queue_p);
void Scheduler_transfer_queue(FIFO_queue_p, FIFO_queue_p);
void Scheduler_send_interrupt(Interrupt int_type, Scheduler_p, CPU_p);
void Scheduler_timer_handler(void*, CPU_p);
void Scheduler_io_request_1_handler(void *scheduler, CPU_p cpu);
void Scheduler_io_request_2_handler(void *scheduler, CPU_p cpu);
void Scheduler_io_completion_1_handler(void *scheduler, CPU_p cpu);
void Scheduler_io_completion_2_handler(void *scheduler, CPU_p cpu);
void Scheduler_reti(CPU_p);
void print_Scheduler(Scheduler_p);
#endif