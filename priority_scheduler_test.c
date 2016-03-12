/********************************************************************************
* cpu_main_loop.c
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 3/10/16
*
* Description:
* This source file implements the class and functions for cpu_main_loop.c
* This file creates three 3 additional threads to simulate a timer, and
* two i/o devices, which will generate asyncronous events.
*
*******************************************************************************/

#include "scheduler.h"
#include <pthread.h>
#define STARVATION_TIME 20
#define PCP_COUNT 20
void *timer_thread(void *);
void *io_device_1_thread(void *);
void *io_device_2_thread(void *);
void *starvation_detection_thread(void *);
void *cpu_thread(void *);
static int the_pc = 0;
int main(void){
    srand(time(NULL));
    pthread_t timer_t;
    pthread_t io_device_1_t;
    pthread_t io_device_2_t;
    pthread_t starvation_detect_t;
    pthread_t cpu_t;
    pthread_t deadlock_dectect_t;
    pthread_attr_t attr;

    pthread_mutex_init(&timer_lock, NULL);
    pthread_mutex_init(&io_1_lock, NULL);
    pthread_mutex_init(&io_2_lock, NULL);
    pthread_mutex_init(&ready_queue_lock, NULL);

    pthread_attr_init(&attr);
    CPU_p cpu = CPU_constructor();
    Scheduler_p scheduler = Scheduler_constructor();


    int i =0;
    int cycles =0;
    //TODO PASS THE PCPs somehow
    thread_data data = {cpu, scheduler, &cycles};
    for(i=0; i < 100; i++){
        PCB_p newer = PCB_constructor();
        newer->pid = i;
        newer->pc = 0;
        newer->sw = rand() % 300;
        newer->p_state = READY;
        if(i <5){
            newer->priority = 0;
            newer->terminate = 1;

        }else if(i >= 5 && i < 85){
            newer->priority = 1;
        }else if(i < 85 && i < 95){
            newer->priority = 2;
        }else{
            newer->priority = 3;
        }
       // newer->priority = priority;
        printf("Process created: PID %d at %s\n", newer->pid, asctime(newer->creation));
     //   FIFO_queue_enqueue(scheduler->priority_ready_queue->queue[newer->priority], newer);

        priority_queue_enqueue(scheduler->priority_ready_queue,(void*) newer, newer->priority);
        if(i== 0){
            FIFO_queue_enqueue(scheduler->running_queue, newer);
        }

    }

    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&cpu_t, &attr, cpu_thread, (void*)&data);
    pthread_create(&timer_t, &attr, timer_thread, (void *)&data);
    pthread_create(&io_device_1_t, &attr, io_device_1_thread, (void *)&data);
    pthread_create(&io_device_2_t, &attr, io_device_2_thread, (void *)&data);
    pthread_create(&starvation_detect_t, &attr, starvation_detection_thread, (void *)&data);
    pthread_create(&deadlock_dectect_t, &attr, deadlock_dectect_thread, (void *)&data);
    pthread_join(cpu_t, NULL);
    pthread_join(timer_t, NULL);
    pthread_join(io_device_1_t, NULL);
    pthread_join(io_device_2_t, NULL);
    pthread_join(starvation_detect_t, NULL);
    pthread_join(deadlock_dectect_t, NULL);

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
    PCB_p temp_pcb_1, temp_pcb_2;

    for(;;){
        if(scheduler->running_queue->head){
            temp_pcb_1 = (PCB_p)scheduler->running_queue->head->data;
        }
        FIFO_queue_p priority_level = priority_queue_find_head(scheduler->priority_ready_queue);
        if(priority_level->head){
            temp_pcb_2 = (PCB_p)priority_level->head->data;
            pthread_mutex_trylock(&timer_lock);
            printf("Sending timer interrupt PID:%d was running,"
                                    " PID:%d, dispatched\n", temp_pcb_1->pid,
                                     temp_pcb_2->pid);

            Scheduler_send_interrupt(ISR_TIMER, scheduler, cpu);
           // print_Scheduler(scheduler);
            pthread_mutex_unlock(&timer_lock);
        }
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

    FIFO_queue_p priority_level = NULL;
    for(;;){
        PCB_p temp_pcb_1, temp_pcb_2;
        for(i =0; i < 4; i++){
            if(scheduler->running_queue->head){
                temp_pcb_1 = (PCB_p)scheduler->running_queue->head->data;

            priority_level = priority_queue_find_head(scheduler->priority_ready_queue);
             }
            if(priority_level->head && temp_pcb_1){
                temp_pcb_2 = (PCB_p)priority_level->head->data;
                     if(temp_pcb_1->pc == temp_pcb_1->IO_1_traps[i]){
                         if(pthread_mutex_trylock(&io_1_lock) != EBUSY){
                                printf("I/O trap request: I/O device 1 PID:%d was put in waiting"
                            " queue, PID:%d, was dispatched\n", temp_pcb_1->pid,
                                      temp_pcb_2->pid);
                            Scheduler_send_interrupt(ISR_IO_1_REQUEST, scheduler, cpu);
                            }
                            if(nanosleep(&sleep_time, NULL) > 0);
                            if(scheduler->running_queue->head){

                                temp_pcb_1 = (PCB_p)scheduler->running_queue->head->data;
                            }
                            if(scheduler->io_1_waiting_queue->head){

                            temp_pcb_2 = (PCB_p)scheduler->io_1_waiting_queue->head->data;
                            printf("Sending IO_1_COMPLETION PID:%d is running,"
                            " PID:%d, put in ready queue\n",
                            temp_pcb_1->pid, temp_pcb_2->pid);
                            Scheduler_send_interrupt(ISR_IO_1_COMPLETION, scheduler, cpu);
                            }



                     }
                  pthread_mutex_unlock(&io_1_lock);
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

    PCB_p temp_pcb_1, temp_pcb_2;
    for(i =0; i < 4; i++){
        if(scheduler->running_queue->head){
        temp_pcb_1 = (PCB_p)scheduler->running_queue->head->data;

        FIFO_queue_p priority_level = priority_queue_find_head(scheduler->priority_ready_queue);
        if(priority_level->head){
        temp_pcb_2 = (PCB_p)priority_level->head->data;

        //temp_pcb_2 = (PCB_p)scheduler->ready_queue->head->data;
         if(temp_pcb_1->pc == temp_pcb_1->IO_2_traps[i]){
            if(pthread_mutex_trylock(&io_2_lock) != EBUSY){
                    printf("I/O trap request: I/O device 2 PID:%d was put in waiting"
                            " queue, PID:%d, was dispatched\n", temp_pcb_1->pid,
                                      temp_pcb_2->pid);
                    Scheduler_send_interrupt(ISR_IO_2_REQUEST, scheduler, cpu);


                    if(nanosleep(&sleep_time, NULL) > 0);
                      if(scheduler->running_queue->head){

                      temp_pcb_1 = (PCB_p)scheduler->running_queue->head->data;

                      if(scheduler->io_2_waiting_queue->head){

                      temp_pcb_2 = (PCB_p)scheduler->io_2_waiting_queue->head->data;
                      printf("Sending IO_2_COMPLETION PID:%d is running,"
                                    " PID:%d, put in ready queue\n",
                                     temp_pcb_1->pid, temp_pcb_2->pid);

                      }

                      }
                                            Scheduler_send_interrupt(ISR_IO_2_COMPLETION, scheduler, cpu);
                      pthread_mutex_unlock(&io_2_lock);

             }
           }
        }
     }
      }
    }
    pthread_exit(NULL);
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* Detects starvation of processes
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void *starvation_detection_thread(void *data)
{
    thread_data *t_data = (thread_data*)(data);
    struct timespec sleep_time;
    sleep_time.tv_sec = 3;
    sleep_time.tv_nsec = 10000000L;
    Scheduler_p scheduler = (Scheduler_p)t_data->scheduler;
   // int *quantum = (int *)t_data->quantum;
    time_t rawtime;
    double span = 0;
    for(;;)
    {
      time(&rawtime);

      if (the_pc % MAX_PC == 0)
      {

        pthread_mutex_lock(&ready_queue_lock);
        int i = 0;
        for (i = 1; i < PRIORITY_LEVELS; i++)
        {
          FIFO_queue_p level = scheduler->priority_ready_queue->queue[i];
          Node_p index = level->head;
          while(index != NULL)
          {
            PCB_p temp = (PCB_p) index->data;
            if(temp->last_run){
            span = difftime(rawtime, mktime(temp->last_run));
            }
            if (span >= STARVATION_TIME)
            {
           // printf("ME STARVING!!\n");
              if(temp->priority - temp->boost > 1)
              {
                //printf("SHIT WAS boosted\n");
                temp->boost++;
              }else {
                //printf("NOT STARVING BRO\n");
                temp->boost = 0;
              }
            }
            index = index->next;
        }

      }
            update_priority_queue(scheduler->priority_ready_queue);
            pthread_mutex_unlock(&ready_queue_lock);

    }
   // pthread_mutex_unlock(&ready_queue_lock);
      if(nanosleep(&sleep_time, NULL) > 0);
    }
    pthread_exit(NULL);

}
void *cpu_thread(void *data){
    thread_data *t_data = (thread_data*)(data);
  //  struct timespec sleep_time;
 //   sleep_time.tv_sec = 0;
 //   sleep_time.tv_nsec = 10000000L;
    Scheduler_p scheduler = (Scheduler_p)t_data->scheduler;
   // int *cycles = (int *)t_data->quantum;
    time_t rawtime;
    Node_p temp_node_1;
    PCB_p temp_pcb_1;
    for(;;){
       // (*cycles)++;
       time(&rawtime);
       the_pc++;
       printf("the_pc: %d\n", the_pc);
        if(scheduler->running_queue->head){
            temp_node_1 = (Node_p)scheduler->running_queue->head;
            temp_pcb_1 = (PCB_p)temp_node_1->data;
            temp_pcb_1->last_run = localtime(&rawtime);
            temp_pcb_1->boost = 0;

            printf("CURRENT PC IS %d", temp_pcb_1->pc);
        }
      if(temp_pcb_1->pc > MAX_PC && temp_pcb_1->term_count < 100){
            temp_pcb_1->pc = 0;
           //temp_pcb_1->term_count++;
          // printf("Term count: %d", temp_pcb_1->term_count);

        }
        //check to see if time to terminate
            if(temp_pcb_1 && temp_pcb_1->term_count != 0 &&
             temp_pcb_1->term_count == temp_pcb_1->terminate){
                temp_pcb_1->termination = localtime(&rawtime);

                Scheduler_transfer_node(scheduler->running_queue,
                                    scheduler->terminated_queue);
                temp_pcb_1->last_run= localtime(&rawtime);
             }
             print_Scheduler(scheduler);


     }
    pthread_exit(NULL);
}
void *deadlock_detection_thread(void *data)
{
	thread_data *t_data = (thread_data*)(data);
	struct timespec sleep_time;
	sleep_time.tv_sec = 0;
	sleep_time.tv_nsec = 100000000L;

	for(;;)
  {
		if(the_pc % MAX_PC == 0)
    {
      for(int i = 0; i < PCP_COUNT; i++)
      {
        PCP_p pcp = pcps[i];

        if (pcp->sharedmutex1->current_process == pcp->producer &&
            pcp->sharedmutex2->current_process == pcp->consumer)
            {
              printf("DEADLOCK detected for PID %d and PID %d!\n", pcp->producer->pid, pcp->consumer->pid);
              continue;
            }
            if (pcp->sharedmutex2->current_process == pcp->producer &&
                pcp->sharedmutex1->current_process == pcp->consumer)
                {
                  printf("DEADLOCK detected for PID %d and PID %d!\n", pcp->producer->pid, pcp->consumer->pid);
                  continue;
                }
          printf("%s\n", "NO DEADLOCK");
      }
	}
	pthread_exit(NULL);
}
