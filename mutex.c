#include "mutex.h"
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Constructor for mutex
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
mutex_p create_mutex(char name[MAX_NAME]){
    mutex_p some_mutex = malloc(sizeof(mutex));
    int i =0;
    for(i =0; i < MAX_NAME -1; i++){
        some_mutex->name[i] = name[i];
    }
    some_mutex->name[MAX_NAME] = '\0';
    some_mutex->lock =0;
    some_mutex->blocked_processes = FIFO_queue_constructor();
    some_mutex->current_process = NULL;
    return some_mutex;
}
void mutex_destructor(mutex_p some_mutex){
    FIFO_queue_destructor(some_mutex->blocked_processes);
    free(some_mutex);
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Function for a process to unlock a mutex
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void mutex_unlock(mutex_p mutex_to_unlock){
    if((mutex_to_unlock->blocked_processes->size) != 0){
       Node_p mutex_node = (Node_p) FIFO_queue_dequeue(mutex_to_unlock->blocked_processes);
       mutex_to_unlock->current_process = (PCB_p)mutex_node->data;
       printf("Mutex %s unlocked, new current process pid:%d\n",mutex_to_unlock->name, mutex_to_unlock->current_process->pid);
       mutex_to_unlock->lock = 1;
       mutex_to_unlock->state = BUSY;
       if(mutex_node->data == NULL){
            //free(mutex_node->data);
            free(mutex_node);
       }else {
            Node_destructor(mutex_node);
        }   

    }else {
        mutex_to_unlock->current_process = NULL;
        mutex_to_unlock = 0;
        mutex_to_unlock->state = FREE;
    }

}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Function for a process, to lock a particular mutex
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void mutex_lock(mutex_p mutex_to_lock, PCB_p current_process){
    if(mutex_to_lock->current_process != NULL && mutex_to_lock->lock == 1){
        PCB_p clone = PCB_constructor();
        clone->pid = current_process->pid;
        
        printf("Current process pid:%d requested mutex: %s blocked by pid:%d\n",
            clone->pid,mutex_to_lock->name, mutex_to_lock->current_process->pid);
        FIFO_queue_enqueue(mutex_to_lock->blocked_processes,(void*) clone);
        print_FIFO_queue(mutex_to_lock->blocked_processes);
        mutex_to_lock->lock = 1;
    }else {

        printf("Current process pid:%d got a lock on mutex: %s\n", current_process->pid, mutex_to_lock->name);
        mutex_to_lock->current_process = current_process;
        mutex_to_lock->lock =1;

    }
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Function for a process to attempt to lock a given mutex, if successful
+ will return a mutex_state, FREE else if already lock it will 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
mutex_state mutex_trylock(mutex_p mutex_to_lock, PCB_p the_process){
    mutex_state state = FREE;
    if(mutex_to_lock->lock != 0){
        state = FREE;
        mutex_lock(mutex_to_lock, the_process);
    }else {
        state = BUSY;        
    }
    return state;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Function for two processes that share a mutex to communicate,
+ And get a lock on a mutex under certain conditions. 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
mutex_cond_p mutex_cond_var_constructor(char name[], mutex_p mutex){
  mutex_cond_p cond_var = malloc(sizeof(mutex_cond_var));
  int i= 0;
  for(i =0; i < MAX_NAME-1; i++){
    cond_var->name[i] = name[i];
  }
  cond_var->name[MAX_NAME] = '\0'; 
  cond_var->input_mutex = mutex;
  cond_var->state = NONE;
  return cond_var;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Function to communicate between processes sharing a mutex, through a cond var.
+ Cond wait updates the state of the condition variable to wait
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void mutex_cond_wait(mutex_cond_p cond_var, mutex_p some_mutex, PCB_p some_process){
    cond_var->state = WAIT;
    cond_var->input_mutex = some_mutex;
    printf("PID %d requested condition wait on %s, with mutex %s\n",some_process->pid,
             cond_var->name, some_mutex->name); 
            
   // mutex_unlock(cond_var->input_mutex);  
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Function to communicate between processes sharing a mutex, through a cond var.
+ Cond signal updates the state of the condition variable that is waiting
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void mutex_cond_signal(mutex_cond_p cond_var, mutex_p some_mutex, PCB_p some_process){
    cond_var->state = SIGNAL;
    cond_var->input_mutex = some_mutex;
    printf("PID %d sent signal on condition %s, with mutex %s\n",
            some_process->pid, cond_var->name, some_mutex->name); 
    //mutex_lock(cond_var->input_mutex, some_process);
}



