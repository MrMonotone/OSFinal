/********************************************************************************
* scheduler.c
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 2/10/16
*
* Description:
* This source file implements the class and functions for scheduler.c
* Currently only handles Timer interrupts.
*
*******************************************************************************/
#include "scheduler.h"
#include <string.h>
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Constructor for Scheduler structure 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
Scheduler_p Scheduler_constructor(){
    Scheduler_p n_schedule = malloc(sizeof(Scheduler));
    
    n_schedule->new_queue = FIFO_queue_constructor();
    FIFO_queue_set_name(n_schedule->new_queue, "[New Processes]");
    
    n_schedule->ready_queue = FIFO_queue_constructor();
    FIFO_queue_set_name(n_schedule->ready_queue, "[Ready Processes]");
    
    n_schedule->running_queue = FIFO_queue_constructor();
    FIFO_queue_set_name(n_schedule->running_queue, "[Running Processes]");
    
    n_schedule->interrupted_queue = FIFO_queue_constructor();
    FIFO_queue_set_name(n_schedule->interrupted_queue, "[Interrupted Processes]");
    
    n_schedule->io_1_waiting_queue = FIFO_queue_constructor();
    FIFO_queue_set_name(n_schedule->io_1_waiting_queue, "[IO_1 Waiting Processes]");
   
    n_schedule->io_2_waiting_queue = FIFO_queue_constructor();
    FIFO_queue_set_name(n_schedule->io_2_waiting_queue, "[IO_2 Waiting Processes]");
   
    n_schedule->terminated_queue = FIFO_queue_constructor();
    FIFO_queue_set_name(n_schedule->terminated_queue, "[Terminated Processes]");
    //n_schedule->state_table[0][0].next_state = INTERRUPTED;
    n_schedule->state_table[0][0].handler = &Scheduler_timer_handler;
    
    n_schedule->state_table[0][1].handler = &Scheduler_io_request_1_handler;
    n_schedule->state_table[0][2].handler = &Scheduler_io_request_2_handler;
    
    n_schedule->state_table[0][3].handler = &Scheduler_io_completion_1_handler;
    n_schedule->state_table[0][4].handler = &Scheduler_io_completion_2_handler;
    
    
    
    n_schedule->currently_running = NULL;
    return n_schedule;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Transfers head of source queue to destination queue
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void Scheduler_transfer_node(FIFO_queue_p source, FIFO_queue_p destination){
    if(source->head != NULL){
        Node_p temp = FIFO_queue_dequeue(source);
        PCB_p temp_data = (PCB_p)(temp->data);
        if(temp != NULL){
            if(temp_data->p_state == READY){
               printf("\nTransferring...\n");
               print_PCB(temp_data);
               printf("Source: %s\tDestination: %s\n",
                    source->name, destination->name);
               
            }
            FIFO_queue_enqueue_node(destination, temp);
        }
   }     
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Transfers all source queue nodes to destination queue
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void Scheduler_transfer_queue(FIFO_queue_p created, FIFO_queue_p ready){
    while(created->size != 0){
        Scheduler_transfer_node(created, ready);
    }
    
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Destructor for Scheduler
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void Scheduler_destructor(Scheduler_p scheduler){
    FIFO_queue_destructor(scheduler->new_queue);
    FIFO_queue_destructor(scheduler->ready_queue);
    FIFO_queue_destructor(scheduler->running_queue);
    FIFO_queue_destructor(scheduler->interrupted_queue);
    FIFO_queue_destructor(scheduler->io_1_waiting_queue);
    FIFO_queue_destructor(scheduler->io_2_waiting_queue);
    //FIFO_queue_destructor(scheduler->waiting_queue);
    FIFO_queue_destructor(scheduler->terminated_queue);
    free(scheduler);
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Interrupt handler for timer interrupt
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void Scheduler_timer_handler(void *scheduler, CPU_p cpu){
    Scheduler_p temp = (Scheduler_p)(scheduler);
    PCB_p temp_pcb = (PCB_p)(temp->running_queue->head->data);
    PCB_set_p_state(temp_pcb, READY);
    
    Scheduler_transfer_node(temp->running_queue, temp->ready_queue);   
    Scheduler_dispatcher(temp, cpu);
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Interrupt handler for io request 1 interrupt
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void Scheduler_io_request_1_handler(void *scheduler, CPU_p cpu){
    Scheduler_p temp = (Scheduler_p)(scheduler);
    PCB_p temp_pcb = (PCB_p)(temp->running_queue->head->data);
    PCB_set_p_state(temp_pcb, BLOCKED);
    
    Scheduler_transfer_node(temp->running_queue, temp->io_1_waiting_queue);   
    Scheduler_dispatcher(temp, cpu);
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Interrupt handler for io request 2 interrupt
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void Scheduler_io_request_2_handler(void *scheduler, CPU_p cpu){
    Scheduler_p temp = (Scheduler_p)(scheduler);
    PCB_p temp_pcb = (PCB_p)(temp->running_queue->head->data);
    PCB_set_p_state(temp_pcb, BLOCKED);
    
    Scheduler_transfer_node(temp->running_queue, temp->io_2_waiting_queue);   
    Scheduler_dispatcher(temp, cpu);
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Interrupt handler for io request 1 interrupt
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void Scheduler_io_completion_1_handler(void *scheduler, CPU_p cpu){
    Scheduler_p temp = (Scheduler_p)(scheduler);
    PCB_p temp_pcb = (PCB_p)(temp->running_queue->head->data);
    PCB_set_p_state(temp_pcb, READY);
    
    Scheduler_transfer_node(temp->io_1_waiting_queue, temp->ready_queue);   
    //Scheduler_dispatcher(temp, cpu);
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Interrupt handler for io completion 2 interrupt
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void Scheduler_io_completion_2_handler(void *scheduler, CPU_p cpu){
    Scheduler_p temp = (Scheduler_p)(scheduler);
    PCB_p temp_pcb = (PCB_p)(temp->running_queue->head->data);
    PCB_set_p_state(temp_pcb, READY);
    
    Scheduler_transfer_node(temp->io_2_waiting_queue, temp->ready_queue);   
    //Scheduler_dispatcher(temp, cpu);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Sends an ISR interrupt to scheduler and depending on running queue
+ sets and transfers a node from ready or the currently running node.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void Scheduler_send_interrupt(Interrupt int_type, Scheduler_p scheduler, CPU_p cpu){
    if(scheduler->running_queue->head){
        PCB_p temp = (PCB_p)scheduler->running_queue->head->data;
        PCB_set_p_state(temp, INTERRUPTED);
        scheduler->state_table[0][int_type].handler(scheduler, cpu);
        //Scheduler_dispatcher(scheduler, cpu);
        Scheduler_reti(cpu);
        scheduler->currently_running = scheduler->ready_queue->head;

    }else {     
        Scheduler_dispatcher(scheduler, cpu);
        Scheduler_reti(cpu);
        scheduler->currently_running = scheduler->ready_queue->head;
         
    }
    
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Dispatcher for scheduler, is called during any ISR
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void Scheduler_dispatcher(Scheduler_p scheduler, CPU_p cpu){
    PCB_p temp;
    if(scheduler->ready_queue->head){
        temp = (PCB_p)scheduler->ready_queue->head->data;
        PCB_set_p_state(temp, RUNNING);
    }
    if(cpu->sys_stack->head){
        unsigned int *sw = (void*)&cpu->sys_stack->head->data;
        unsigned int *pc = (void*)&cpu->sys_stack->head->next->data;
    
        *sw = (unsigned int)temp->sw;
        *pc = (unsigned int)temp->pc;
    }
    Scheduler_transfer_node(scheduler->ready_queue, scheduler->running_queue);
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Transfers head of source queue to destination queue
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void print_Scheduler(Scheduler_p sch){
    printf("Current Status of Scheduler\n");
    print_FIFO_queue(sch->new_queue);
    print_FIFO_queue(sch->ready_queue);
    print_FIFO_queue(sch->running_queue);
    print_FIFO_queue(sch->interrupted_queue);
    print_FIFO_queue(sch->io_1_waiting_queue);
    print_FIFO_queue(sch->io_2_waiting_queue);
    print_FIFO_queue(sch->terminated_queue);
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ A pseudo reti routine for scheduler, is called at end of every ISR.
+ Pops PC and SW from system stack and restores them to cpu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void Scheduler_reti(CPU_p cpu){
    //pseudo reti
   Node_p temp_node;
   temp_node = Stack_pop(cpu->sys_stack); 
   if(temp_node){
    cpu->sw = (unsigned int)temp_node->data;
    free(temp_node);
   }
   temp_node = Stack_pop(cpu->sys_stack);   
   if(temp_node){
    cpu->pc = (unsigned int)temp_node->data;
    free(temp_node);
   }
}
        

