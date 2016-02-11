/********************************************************************************
* cpu.h
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 1/24/16
*
* Description:
* This header file defines the class and methods for the CPU 
*
*******************************************************************************/
#ifndef _CPU_H
#define _CPU_H
#ifndef MAX_REGS
#define MAX_REGS 20
#endif
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "pcb.h"
  
typedef struct cpu{
    Stack_p sys_stack;
    int pid;
    unsigned int pc;
    unsigned int sw;
    unsigned int priority;
    int address;
    State p_state;
    Register_file register_file;
}CPU;
typedef CPU *CPU_p;
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ function prototypes
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
CPU_p CPU_constructor();
void CPU_destructor(CPU_p);

#endif
