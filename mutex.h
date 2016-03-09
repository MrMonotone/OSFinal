#ifndef _mutex
#include "pcb.h"
#include "
typedef struct mutex {
    int lock;
    PCB_p current_process;
    FIFO_queue_p blocked_processes;   
} mutex;
typedef struct mutex *mutex_p;
typedef struct cond_var {
   mutex_p input_mutex;  
}mutex_cond_var;
typedef mutex_cond_var *mutex_cond_p;
mutex_p create_mutex(PCB_p);
mutex_cond_p create_mutex_cond(mutex_p);
void mutex_lock(mutex_p mutex_to_lock);
void mutex_trylock(mutex_p mutext_to_lock);
void mutex_cond_signal(mutex_cond_p);
void mutex_cond_wait(mutex_cond_p);
#endif
