/********************************************************************************
* os_main_loop.c
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 1/24/16
*
* Description:
* This source file OS main loop, simulates processes being scheduled, 
in relation tothe simulated interrupts. This test it is merely 
timer_interrupts on aRound Robin scheduler.
*
*******************************************************************************/
#include "scheduler.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_PROCESSES 30
#define RANGE_MIN 3000
#define MAX_PRIORITY 16
#define RANGE_MAX 4000
void create_processes(Scheduler_p);
int ran_range(int min, int max);
int main(void){
    srand(time(NULL));
    CPU_p cpu = CPU_constructor();
    Scheduler_p sch = Scheduler_constructor();
    create_processes(sch);
    int i =0;
    for(i=0; i < MAX_PROCESSES; i++){
        if(i % 4 == 0){
            PCB_p temp_ready;
            PCB_p temp_running;
            printf("\n*****************************************************\n");
            printf("CPU Status\nPC: %u SW: %u\n", cpu->pc, cpu->sw); 
            if(sch->running_queue->head){
                temp_running = (PCB_p)sch->running_queue->head->data;
                printf("\nHead of Running Queue:\n");
                print_PCB(temp_running);
            }
            printf("\nSwitching to:\n");
            if(sch->ready_queue->head->data){
                temp_ready = (PCB_p)sch->ready_queue->head->data;
                printf("\nHead of Ready Queue:\n");
                print_PCB(temp_ready);
            }

            Stack_push(cpu->sys_stack, (void*)cpu->pc);
            Stack_push(cpu->sys_stack, (void*)cpu->sw);
            Scheduler_send_interrupt(ISR_TIMER, sch, cpu);
          
            if(sch->running_queue->head){
                temp_running = (PCB_p)sch->running_queue->head->data;
                 printf("\nHead of Running Queue:\n");
                print_PCB(temp_running);
            }
            if(sch->ready_queue->head){
                temp_ready = (PCB_p)sch->ready_queue->head->data;
                printf("\nHead of Ready Queue:\n");
                print_PCB(temp_ready);
            }
            
            
            printf("CPU Status\nPC: %u SW: %u\n", cpu->pc, cpu->sw);            
            print_FIFO_queue(sch->ready_queue);
            printf("\n*****************************************************\n");
            
       }else {
            Stack_push(cpu->sys_stack, (void*)cpu->pc);
            Stack_push(cpu->sys_stack, (void*)cpu->sw);
            Scheduler_send_interrupt(ISR_TIMER, sch, cpu);
       }
   }
    CPU_destructor(cpu);
    Scheduler_destructor(sch);
    return 0;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Returns a "random" int within a given range
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int ran_range(int min, int max){
    return rand() % (max+1 - min) + min;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Creates MAX_PROCESS number of processes for scheduler structure 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void create_processes(Scheduler_p sch){
    int i =0;
    int pc =0;
    for(i =0; i < MAX_PROCESSES; i++){
        PCB_p pcb = PCB_constructor();
        pc += ran_range(RANGE_MIN, RANGE_MAX);
        pcb->pc = pc;
        pcb->sw = ran_range(RANGE_MIN, RANGE_MAX);
        pcb->pid = ran_range(0, RANGE_MAX);
        pcb->priority = ran_range(0, MAX_PRIORITY);
        pcb->address = ran_range(0, RANGE_MAX+RANGE_MIN);
        pcb->p_state = READY;
        FIFO_queue_enqueue(sch->new_queue, pcb);
    }
    Scheduler_transfer_queue(sch->new_queue, sch->ready_queue);
}
        
