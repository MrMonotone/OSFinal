/********************************************************************************
* cpu_main_loop.c
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 2/12/16
*
* Description:
* This source file implements the class and functions for cpu_main_loop.c
* This file creates three 3 additional threads to simulate a timer, and 
* two i/o devices, which will generate asyncronous events. 
*
*******************************************************************************/

#include "scheduler.h"
#include <pthread.h>

void *timer_thread(void *data);
void *io_device_1_thread(void *data);
void *io_device_2_thread(void *data);
void *cpu_thread(void *data);

int main(void){
    srand(time(NULL));
    pthread_t timer_t;
    pthread_t io_device_1_t;
    pthread_t io_device_2_t;
    
    pthread_attr_t attr;
    
    pthread_mutex_init(&timer_lock, NULL);
    pthread_mutex_init(&io_1_lock, NULL);
    pthread_mutex_init(&io_2_lock, NULL);
    
    pthread_attr_init(&attr);
    CPU_p cpu = CPU_constructor();
    Scheduler_p scheduler = Scheduler_constructor();

    thread_data data = {cpu, scheduler};
    PCB_p temp_pcb_1;
    int i =0;
    for(i=0; i < 100; i++){
        PCB_p newer = PCB_constructor();
        newer->pid = i;
        newer->pc = rand() % 100;
        newer->sw = rand() % 300;
        newer->p_state = READY;
        printf("Process created: PID %d at %s\n", newer->pid, asctime(newer->creation));
        FIFO_queue_enqueue(scheduler->ready_queue, newer);
        if(i== 0){
            FIFO_queue_enqueue(scheduler->running_queue, newer);
        }
    }
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&timer_t, &attr, timer_thread, (void *)&data);
    pthread_create(&io_device_1_t, &attr, io_device_1_thread, (void *)&data);
    pthread_create(&io_device_2_t, &attr, io_device_2_thread, (void *)&data);

    pthread_join(timer_t, NULL);
    pthread_join(io_device_1_t, NULL);
    pthread_join(io_device_2_t, NULL);
    for(i =0; i < 1000; i++){
       temp_pcb_1 = (PCB_p)scheduler->running_queue->head->data;
       temp_pcb_1->pc++;  
    }
    pthread_attr_destroy(&attr);
    
    pthread_exit(NULL);
    CPU_destructor(cpu);
    Scheduler_destructor(scheduler);
    return 0;


}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* Timer thread that sends an interrupt every 100000000 nsecs.
* Then puts itself to sleep.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void *timer_thread(void *data){
    thread_data *t_data = (thread_data*)(data);
    struct timespec sleep_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 100000000L; 
    CPU_p cpu = (CPU_p)t_data->cpu;
    Scheduler_p scheduler = (Scheduler_p)t_data->scheduler;
    int i =0;
    PCB_p temp_pcb_1, temp_pcb_2;
    
    for(;;){
        temp_pcb_1 = (PCB_p)scheduler->running_queue->head->data;
        temp_pcb_2 = (PCB_p)scheduler->ready_queue->head->data;
        pthread_mutex_trylock(&timer_lock);
        
       printf("Sending timer interrupt PID:%d was running,"
                                    " PID:%d, dispatched\n", temp_pcb_1->pid,    
                                      temp_pcb_2->pid);
        
        Scheduler_send_interrupt(ISR_TIMER, scheduler, cpu);

        //print_Scheduler(scheduler);

        pthread_mutex_unlock(&timer_lock);

        if(nanosleep(&sleep_time, NULL) > 0);
    }
    pthread_exit(NULL);

}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* IO device 1 thread will send interrupt depending on value of current pcb,
* then sleep, then finish io request. 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void *io_device_1_thread(void *data){
    thread_data *t_data = (thread_data*)(data);
    struct timespec sleep_time;
    int i =0;
    sleep_time.tv_sec = 1;
    sleep_time.tv_nsec = 700000000L; 
    CPU_p cpu = (CPU_p)t_data->cpu;
    Scheduler_p scheduler = (Scheduler_p)t_data->scheduler;
    for(;;){
    PCB_p temp = (PCB_p) scheduler->running_queue->head->data;
    PCB_p temp_pcb_1, temp_pcb_2;
    for(i =0; i < 4; i++){
        temp_pcb_1 = (PCB_p)scheduler->running_queue->head->data;
        temp_pcb_2 = (PCB_p)scheduler->ready_queue->head->data;
                if(temp->pc == temp->IO_1_traps[i]){
                    if(pthread_mutex_trylock(&io_1_lock) != EBUSY){
                        printf("I/O trap request: I/O device 1 PID:%d was put in waiting" 
                            " queue, PID:%d, was dispatched\n", temp_pcb_1->pid,    
                                      temp_pcb_2->pid);
                      Scheduler_send_interrupt(ISR_IO_1_REQUEST, scheduler, cpu);
                      if(nanosleep(&sleep_time, NULL) > 0);
                      temp_pcb_1 = (PCB_p)scheduler->running_queue->head->data;
                      temp_pcb_2 = (PCB_p)scheduler->io_1_waiting_queue->head->data;
                      printf("Sending IO_1_COMPLETION PID:%d is running,"
                                    " PID:%d, put in ready queue\n", temp_pcb_1->pid,    
                                      temp_pcb_2->pid);
                    Scheduler_send_interrupt(ISR_IO_1_COMPLETION, scheduler, cpu);
                      pthread_mutex_unlock(&io_1_lock);
                    }
                }
 
        }
    }
    pthread_exit(NULL);
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* IO device 2 thread will send interrupt depending on value of current pcb,
* then sleep, then finish io request. 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void *io_device_2_thread(void *data){
    int i = 0;
    thread_data *t_data = (thread_data*)(data);
    struct timespec sleep_time;
    sleep_time.tv_sec = 1;
    sleep_time.tv_nsec = 300000000L; 
    CPU_p cpu = (CPU_p)t_data->cpu;
    Scheduler_p scheduler = (Scheduler_p)t_data->scheduler;
    for(;;){
    PCB_p temp = (PCB_p) scheduler->running_queue->head->data;
    PCB_p temp_pcb_1, temp_pcb_2;
    for(i =0; i < 4; i++){
        temp_pcb_1 = (PCB_p)scheduler->running_queue->head->data;
        temp_pcb_2 = (PCB_p)scheduler->ready_queue->head->data;
         if(temp->pc == temp->IO_2_traps[i]){
            if(pthread_mutex_trylock(&io_2_lock) != EBUSY){
                    printf("I/O trap request: I/O device 2 PID:%d was put in waiting" 
                            " queue, PID:%d, was dispatched\n", temp_pcb_1->pid,    
                                      temp_pcb_2->pid);
                    Scheduler_send_interrupt(ISR_IO_2_REQUEST, scheduler, cpu);
                    
                    if(nanosleep(&sleep_time, NULL) > 0);
                      temp_pcb_1 = (PCB_p)scheduler->running_queue->head->data;
                      temp_pcb_2 = (PCB_p)scheduler->io_2_waiting_queue->head->data;
                      printf("Sending IO_2_COMPLETION PID:%d is running,"
                                    " PID:%d, put in ready queue\n", temp_pcb_1->pid,    
                                      temp_pcb_2->pid);
                  Scheduler_send_interrupt(ISR_IO_2_COMPLETION, scheduler, cpu);
                      pthread_mutex_unlock(&io_2_lock);
             }
         }
     }
      
    }
    pthread_exit(NULL);
}

