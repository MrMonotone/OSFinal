/********************************************************************************
* pcb.c
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 2/10/16
*
* Description:
* This source file implements the class and methods for pcb.h
*
*******************************************************************************/
#include "pcb.h"
#include <stdio.h>
#include <stdlib.h>
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Constructor for PCB object.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
PCB_p PCB_constructor(void){
    //srand(time(NULL));
    time_t rawtime;
    time(&rawtime);
    int i =0;
    int temp_1 =0;
    int temp_2 = 0;
    PCB_p new_pcb = malloc(sizeof(PCB));
    new_pcb->pid = 0;
    new_pcb->pc = 0;
    new_pcb->sw = 0;
    new_pcb->priority = 0;
    new_pcb->address = 0;
    new_pcb->p_state = NEW;
    new_pcb->creation = localtime(&rawtime);
    new_pcb->termination = 0;
    new_pcb->terminate = 0;
    new_pcb->term_count = 0;

    for(i =0; i < MAX_TRAPS; i++){
        new_pcb->IO_1_traps[i] =0;
        new_pcb->IO_2_traps[i] =0;
    }
    create_PCB_TRAPS(new_pcb);
        
    for(i=0; i < MAX_REGS; i++){
        new_pcb->register_file.registers[i] = 0;
    }
    return new_pcb;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Destructor for PCB object.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void PCB_destructor(PCB_p pcb){
    free(pcb);
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Setter for PCB object pid field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_set_pid(PCB_p pcb, int new_pid){
    int err = 0;
    if(pcb){
        pcb->pid = new_pid;
    }else {
        err = -1;
    }
    return err;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Setter for PCB object pc field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_set_pc(PCB_p pcb, unsigned int new_pc){
    int err = 0;
    if(pcb){
        pcb->pc = new_pc;
    }else {
        err = -1;
    }
    return err;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Setter for PCB object sw field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_set_sw(PCB_p pcb, unsigned int new_sw){
    int err = 0;
    if(pcb){
        pcb->sw = new_sw;
    }else {
        err = -1;
    }
    return err;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Setter for PCB object priority field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_set_priority(PCB_p pcb, unsigned int new_priority){
    int err = 0;
    if(pcb){
        pcb->priority = new_priority;
    }else {
        err = -1;
    }
    return err;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Setter for PCB object address field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_set_address(PCB_p pcb, int new_add){
    int err = 0;
    if(pcb){
        pcb->address = new_add;
    }else {
        err = -1;
    }
    return err;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Setter for PCB object p_state field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_set_p_state(PCB_p pcb, State new_state){
    int err = 0;
    if(pcb){
        pcb->p_state = new_state;
    }else {
        err = -1;
    }
    return err;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Setter for PCB object register field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_set_reg(PCB_p pcb, int index, int val){
    int err = 0;
    if(pcb){
        pcb->register_file.registers[index] = val;
    }else {
        err = -1;
    }
    return err;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Setter for PCB object creation field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_set_creation(PCB_p pcb, struct tm *c){
    int err = 0;
    if(pcb){
        pcb->creation = c;
    }else {
        err = -1;
    }
    return err;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Setter for PCB object termination field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_set_termination(PCB_p pcb, struct tm *t){
    int err = 0;
    if(pcb){
        pcb->termination = t;
    }else {
        err = -1;
    }
    return err;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Setter for PCB object terminate field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_set_terminate(PCB_p pcb, unsigned int t){
    int err = 0;
    if(pcb){
        pcb->terminate = t;
    }else {
        err = -1;
    }
    return err;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Setter for PCB object term count field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_set_term_count(PCB_p pcb, int count){
    int err = 0;
    if(pcb){
        pcb->term_count = count;
    }else {
        err = -1;
    }
    return err;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Setter for PCB object IO traps 2 field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_set_IO_1_traps(PCB_p pcb,int index, int value){
    int err = 0;
    if(pcb){
        pcb->IO_1_traps[index] = value;
    }else {
        err = -1;
    }
    return err;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Setter for PCB object IO traps 2 field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_set_IO_2_traps(PCB_p pcb,int index, int value){
    int err = 0;
    if(pcb){
        pcb->IO_2_traps[index] = value;
    }else {
        err = -1;
    }
    return err;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Getter for PCB object pid field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_get_pid(PCB_p pcb, int* err){
    int pid =0;
    if(pcb){
        pid = pcb->pid;
    }else {
        pid = -1;
    }
    *err = pid;
    return pid;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Getter for PCB object pc field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
unsigned int PCB_get_pc(PCB_p pcb, int* err){
    unsigned int pc = 0; // default so if return val = 0, then pcb is null
                         // since we can't return -1 for unsigned values
    if(pcb){
        pc = pcb->pc;
        *err = 0;
    }else {
        *err = -1;
    }
    return pc;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Getter for PCB object sw field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
unsigned int PCB_get_sw(PCB_p pcb, int* err){
    unsigned int sw = 0; // default so if return val = 0, then pcb is null
                         // since we can't return -1 for unsigned values
    if(pcb){
        sw = pcb->sw;
        *err = 0;
    }else {
        *err = -1;
    }
    return sw;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Getter for PCB object priority field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
unsigned int PCB_get_priority(PCB_p pcb, int *err){
    unsigned int prio = 0;
    if(pcb){
        prio = pcb->priority;
        *err = 0;
    }else {
        *err = -1;
    }
    return prio;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Getter for PCB object address field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_get_address(PCB_p pcb, int *err){
    int addr = 0;
    if(pcb){
        addr = pcb->address;
        *err = 0;
    }else {
        *err = -1;
    }
    return addr;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Getter for PCB object p_state field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
State PCB_get_p_state(PCB_p pcb, int *err){
    State state = NEW;
    if(pcb){
        state = pcb->p_state;
        *err = 0;
    }else {
        *err = -1;
    }
    return state;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Getter for PCB object register field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_get_reg(PCB_p pcb, int index, int *err){
    int val =0;
    if(pcb && index < MAX_REGS){
        val = pcb->register_file.registers[index];
        *err = 0;
    }else {
        *err = -1;
    }
    return val;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Getter for PCB object creation time field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
struct tm *PCB_get_creation(PCB_p pcb, int *err){
    struct tm *c;
    if(pcb){
        c = pcb->creation;
        *err = 0;
    }else {
        *err = -1;
    }
    return c;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Getter for PCB object termination field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
struct tm *PCB_get_termination(PCB_p pcb, int *err){
    struct tm *t;
    if(pcb){
        t = pcb->termination;
        *err = 0;
    }else {
        *err = -1;
    }
    return t;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Getter for PCB object terminate field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_get_terminate(PCB_p pcb, int *err){
    int t = 0;
    if(pcb){
        t = pcb->terminate;
        *err = 0;
    }else {
        *err = -1;
    }
    return t;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Getter for PCB object term_count field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_get_term_count(PCB_p pcb, int *err){
    int t = 0;
    if(pcb){
        t = pcb->term_count;
        *err = 0;
    }else {
        *err = -1;
    }
    return t;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Getter for PCB object IO traps 1 field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_get_IO_1_traps(PCB_p pcb,int index, int *err){
    int IO1 =0;
    if(pcb){
    	IO1 = pcb->IO_1_traps[index];
        *err = 0;
    }else {
        *err = -1;
    }
    return IO1;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Getter for PCB object IO traps 2 field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int PCB_get_IO_2_traps(PCB_p pcb,int index, int *err){
    int IO2 =0;
    if(pcb){
    	IO2 = pcb->IO_2_traps[index];
        *err = 0;
    }else {
        *err = -1;
    }
    return IO2;
}

//Utility Functions
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ To_string function for PCB object, returns char* representation of
+ a PCB.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
char * PCB_to_string(PCB_p pcb){
    char *process_state_names[] =
           { "NEW", "READY", "RUNNING", "INTERRUPTED", "WAITING", "TERMINATED"};
    char *to_string = calloc(200, 1);
    if(pcb){
        sprintf(to_string,
         "PID:%d PC:%u SW:%u Priority:%u State:%s Address Space:%d"
         "\nCreation Time:%s\n\n",
          pcb->pid, pcb->pc,pcb->sw, pcb->priority,
          process_state_names[pcb->p_state], pcb->address, asctime(pcb->creation));
    }

    return to_string;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Calls to_string function for PCB object, and prints contents to stdio.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void print_PCB(PCB_p pcb){
    if(pcb){
        char* to_string = PCB_to_string(pcb);
        printf("%s", to_string);
        free(to_string);
    }
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Fills the trap arrays in each pcb with random ints.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void create_PCB_TRAPS(PCB_p pcb){
    int i= 0, j =0;
    int rand_1 =0;
    int rand_2 = 0;
    for(i=0; i < MAX_TRAPS; i++){
        rand_1 = rand() % 100;
        rand_2 = rand() % 100;
        if(rand_1 == rand_2){
           rand_2 = rand() % MAX_PC;
        }
         pcb->IO_1_traps[i] = rand_1;
         
         pcb->IO_2_traps[i] = rand_2;

    }
        
   
}       
