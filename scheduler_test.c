/********************************************************************************
* scheduler_test.c
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 1/27/16
*
* Description:
* This source file contains a few tests to ensure scheduler is working correctly
*
*******************************************************************************/
#include "scheduler.h"
#include <pthread.h>
//#include <unistd.h>

pthread_mutex_t schedule_lock;
pthread_mutex_t timer_lock;
pthread_mutex_t io_1_lock;
pthread_mutex_t io_2_lock;
pthread_cond_t timer_done;

void *timer_thread(void *data);
void *io_device_1_thread(void *data);
void *io_device_2_thread(void *data);
int main(void){
    srand(time(NULL));
    pthread_t timer_t;
    pthread_t io_device_1_t;
    pthread_t io_device_2_t;
    pthread_attr_t attr;
    
    pthread_mutex_init(&schedule_lock, NULL);
    pthread_mutex_init(&timer_lock, NULL);
    pthread_mutex_init(&io_1_lock, NULL);
    pthread_mutex_init(&io_2_lock, NULL);
    pthread_cond_init(&timer_done, NULL);
    
    pthread_attr_init(&attr);
    CPU_p zoidberg = CPU_constructor();
    Scheduler_p woot = Scheduler_constructor();

    thread_data data = {zoidberg, woot};
    //data->sleep_time.tv_sec = 0;
   // data->sleep_time.tv_nec = 300000000L;
     
    PCB_p new = PCB_constructor();
    PCB_set_pid(new, 1231);
    PCB_set_pc(new, 10000);
    PCB_set_sw(new, 553335);
    PCB_p new_2 = PCB_constructor();
    PCB_set_pid(new_2, 1337);
    PCB_set_pc(new_2, 80085);
    PCB_p new_3 = PCB_constructor();
    PCB_set_pid(new_3, 21337);
    PCB_set_pc(new_3, 222222);
    PCB_set_sw(new_3, 3333333333);
    FIFO_queue_enqueue(woot->running_queue, new_2);
    FIFO_queue_enqueue(woot->ready_queue, new_3);
    FIFO_queue_enqueue(woot->ready_queue, new);
    int i =0;
    for(i=0; i < 20; i++){
        PCB_p newer = PCB_constructor();
        newer->pid = i;
        newer->pc = rand() % 500;
        newer->sw = rand() % 300;
        newer->p_state = READY;
        FIFO_queue_enqueue(woot->ready_queue, newer);
    }
    printf("CPU Status PC: %u SW: %u\n", zoidberg->pc, zoidberg->sw); 
   print_Scheduler(woot);
   // printf("----------------------\n");

        //Scheduler_transfer_node(woot->new_queue, woot->ready_queue);
        //Scheduler_transfer_node(woot->new_queue, woot->ready_queue);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&timer_t, &attr, timer_thread, (void *)&data);
    pthread_create(&io_device_1_t, &attr, io_device_1_thread, (void *)&data);
    pthread_create(&io_device_2_t, &attr, io_device_2_thread, (void *)&data);
    pthread_join(timer_t, NULL);
    pthread_join(io_device_1_t, NULL);
    pthread_join(io_device_2_t, NULL);
    
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&schedule_lock);
    pthread_cond_destroy(&timer_done);
    pthread_exit(NULL);
    CPU_destructor(zoidberg);
    Scheduler_destructor(woot);
    return 0;
}
void *timer_thread(void *data){
    thread_data *t_data = (thread_data*)(data);
    struct timespec sleep_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 300000000L; 
    CPU_p cpu = (CPU_p)t_data->cpu;
    Scheduler_p scheduler = (Scheduler_p)t_data->scheduler;
    int i =0;
    for(;;){
        pthread_mutex_trylock(&timer_lock);

        printf("Sending timer interrupt\n");
        Scheduler_send_interrupt(ISR_TIMER, scheduler, cpu);
        printf("CPU Status PC: %u SW: %u\n", cpu->pc, cpu->sw); 
        print_Scheduler(scheduler);

      //  pthread_cond_signal(&timer_done);
        pthread_mutex_unlock(&timer_lock);
        //sleep(1);
        if(nanosleep(&sleep_time, NULL) > 0);
    }
    pthread_exit(NULL);
}
void *io_device_1_thread(void *data){
    thread_data *t_data = (thread_data*)(data);
    struct timespec sleep_time;
    sleep_time.tv_sec = 1;
    sleep_time.tv_nsec = 500000000L; 
    CPU_p cpu = (CPU_p)t_data->cpu;
    Scheduler_p scheduler = (Scheduler_p)t_data->scheduler;
    PCB_p running = (PCB_p)scheduler->running_queue->head->data;
    
    int i =0;
    for(;;){
            if(pthread_mutex_trylock(&io_1_lock) != EBUSY){
                printf("TOP\n");
                if(pthread_mutex_trylock(&timer_lock) != EBUSY){
                    if(pthread_mutex_trylock(&io_2_lock) != EBUSY){
                        printf("SENDING IO_1_REQUEST\n");
                        Scheduler_send_interrupt(ISR_IO_1_REQUEST, scheduler, cpu);
                        print_Scheduler(scheduler);
                    }
                }

                pthread_mutex_unlock(&timer_lock);
                pthread_mutex_unlock(&io_2_lock);
                pthread_mutex_unlock(&io_1_lock);
                // printf("Sending io_request interrupt\n");
                //sleep(4);
                if(nanosleep(&sleep_time, NULL) > 0);
                if(pthread_mutex_trylock(&io_1_lock) != EBUSY){
                    if(pthread_mutex_trylock(&timer_lock) != EBUSY){
                        if(pthread_mutex_trylock(&io_2_lock) != EBUSY){
                            printf("SENDING IO_1_COMPLETION\n");
                            Scheduler_send_interrupt(ISR_IO_1_COMPLETION, 
                                                scheduler, cpu);
                        }
                   }
                }
                pthread_mutex_unlock(&io_2_lock);
                pthread_mutex_unlock(&timer_lock);
                pthread_mutex_unlock(&io_1_lock);
           }  
    }
    pthread_exit(NULL);

}
void *io_device_2_thread(void *data){
    thread_data *t_data = (thread_data*)(data);
    struct timespec sleep_time;
    sleep_time.tv_sec = 1;
    sleep_time.tv_nsec = 800000000L; 
    CPU_p cpu = (CPU_p)t_data->cpu;
    Scheduler_p scheduler = (Scheduler_p)t_data->scheduler;
    PCB_p running = (PCB_p)scheduler->running_queue->head->data;
    
    int i =0;
    for(;;){
            if(pthread_mutex_trylock(&io_2_lock) != EBUSY){
                printf("TOP\n");
                if(pthread_mutex_trylock(&timer_lock) != EBUSY){
                    if(pthread_mutex_trylock(&io_1_lock) != EBUSY){
                        printf("SENDING IO_2_REQUEST\n");
                        Scheduler_send_interrupt(ISR_IO_2_REQUEST, scheduler, cpu);
                        print_Scheduler(scheduler);
                    }
                }

                pthread_mutex_unlock(&timer_lock);
                pthread_mutex_unlock(&io_1_lock);
                pthread_mutex_unlock(&io_2_lock);
                // printf("Sending io_request interrupt\n");
                //sleep(4);
                if(nanosleep(&sleep_time, NULL) > 0);
                if(pthread_mutex_trylock(&io_2_lock) !=EBUSY){
                    if(pthread_mutex_trylock(&timer_lock) != EBUSY){
                        if(pthread_mutex_trylock(&io_1_lock) != EBUSY){
                            printf("SENDING IO_2_COMPLETION\n");
                            Scheduler_send_interrupt(ISR_IO_2_COMPLETION, 
                                                scheduler, cpu);
                        }
                   }
                }
                
                pthread_mutex_unlock(&io_1_lock);
                pthread_mutex_unlock(&timer_lock);
                pthread_mutex_unlock(&io_2_lock);
           }
       
    }
    pthread_exit(NULL);
}

