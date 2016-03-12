#ifndef _mutex
#define _mutex
#include "pcb.h"
#include "fifo_queue.h"
#define MAX_NAME 16
typedef enum mutex_state{BUSY, FREE} mutex_state;
typedef struct mutex {
    char name[MAX_NAME]; 
    int lock;
    mutex_state state;
    PCB_p current_process;
    FIFO_queue_p blocked_processes;   
} mutex;
typedef struct mutex *mutex_p;
typedef struct cond_var {
   char name[MAX_NAME];
   mutex_p input_mutex;  
}mutex_cond_var;
typedef mutex_cond_var *mutex_cond_p;
mutex_p create_mutex(char[MAX_NAME]);
void mutex_destructor(mutex_p);
mutex_cond_p create_mutex_cond(mutex_p);
void mutex_unlock(mutex_p mutex_to_lock);
void mutex_lock(mutex_p mutex_to_lock, PCB_p process_to_lock);
mutex_state mutex_trylock(mutex_p mutext_to_lock, PCB_p process_to_lock);

Node_p Mutex_node_constructor(void *data);
mutex_cond_p Mutex_cond_var_constructor();
void Mutex_cond_wait(mutex_cond_p,mutex_p);
void Mutex_cond_signal(mutex_cond_p, mutex_p);
#endif
