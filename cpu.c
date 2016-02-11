/********************************************************************************
* cpu.c
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 1/24/16
*
* Description:
* This source file implements functions for the CPU from cpu.h 
*
*******************************************************************************/
#include "cpu.h"
#include "stack.h"
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Constructor for CPU structure 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
CPU_p CPU_constructor(){
    CPU_p new = malloc(sizeof(CPU));
    new->sys_stack = Stack_constructor();
    new->pid = 0;
    new->pc = 0;
    new->sw = 0;
    new->priority = 0;
    new->address = 0;
    new->p_state = 0;
    int i =0;
    for(i=0; i < MAX_REGS; i++){
        new->register_file.registers[i] = 0;
    }
    return new;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Destructor for CPU structure 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void CPU_destructor(CPU_p cpu){
    Stack_destructor(cpu->sys_stack);
    free(cpu);
}


