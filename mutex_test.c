#include "mutex.h"

int main(void){
    PCB_p process_1 = PCB_constructor();
    process_1->pid = 1337;
    PCB_p process_2 = PCB_constructor();
    process_2->pid = 21337;
    PCB_p process_3 = PCB_constructor();
    process_3->pid = 2343947;
    mutex_p mutex_A = create_mutex("A");
    mutex_p mutex_B = create_mutex("B");
    mutex_lock(mutex_A, process_1);
    mutex_lock(mutex_A, process_2);
    mutex_lock(mutex_A, process_3);
    mutex_unlock(mutex_A);
    mutex_unlock(mutex_A);
    mutex_lock(mutex_A, process_3);
    mutex_cond_p condition =  mutex_cond_var_constructor("luke at me", mutex_A);
    mutex_cond_signal(condition, mutex_A, process_1);
    mutex_cond_wait(condition, mutex_B, process_3);
    
    PCB_destructor(process_1);
   PCB_destructor(process_2);
    PCB_destructor(process_3);

    mutex_destructor(mutex_A);
    mutex_destructor(mutex_B);
    return 0;
}
