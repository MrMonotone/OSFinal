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
            free(mutex_node->data);
            free(mutex_node);
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
        //if(mutex_to_lock->lock == 0){
        printf("Current process pid:%d got a lock on mutex: %s\n", current_process->pid, mutex_to_lock->name);
        mutex_to_lock->current_process = current_process;
        mutex_to_lock->lock =1;
       // }
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
+ Function for a process, to lock a particular mutex
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

