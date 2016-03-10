/********************************************************************************
* pcb.h
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 1/24/16
*
* Description:
* This header file defines the class and methods for the
* PCB ( process control block) implementation.
*
*******************************************************************************/
#ifndef _PCB_H
#define _PCB_H
#define MAX_TRAPS 4
#define MAX_REGS 20
#define MAX_PC 2345
#include <time.h>

typedef enum process_state {
    NEW, READY, RUNNING, INTERRUPTED, BLOCKED, WAITING, TERMINATED} State;

typedef struct Register_file {
    int registers[MAX_REGS];
}Register_file;
typedef struct PCB {
    unsigned int pid;
    unsigned int pc;
    unsigned int sw;
    unsigned int priority;
    unsigned int boost;
    int address;
    State p_state;
    Register_file register_file;
    struct tm *creation;
    struct tm *last_run;
    struct tm *termination;
    unsigned int terminate;
    int term_count;
    int IO_1_traps[MAX_TRAPS];
    int IO_2_traps[MAX_TRAPS];
} PCB;

typedef PCB * PCB_p;
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ function prototypes
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
PCB_p PCB_constructor(void);
void PCB_destructor(PCB_p);
//Setters
int PCB_set_pid(PCB_p, int);
int PCB_set_pc(PCB_p, unsigned int);
int PCB_set_sw(PCB_p, unsigned int);
int PCB_set_priority(PCB_p, unsigned int);
int PCB_set_address(PCB_p, int);
int PCB_set_p_state(PCB_p, State);
int PCB_set_reg(PCB_p, int, int);
int PCB_set_creation(PCB_p, struct tm*);
int PCB_set_termination(PCB_p,struct tm*);
int PCB_set_terminate(PCB_p, unsigned int);
int PCB_set_term_count(PCB_p, int);
int PCB_set_IO_trap_1(PCB_p,int, int);
int PCB_set_IO_trap_2(PCB_p,int, int);

//Getters
int PCB_get_pid(PCB_p, int*);
unsigned int PCB_get_pc(PCB_p, int*);
unsigned int PCB_get_sw(PCB_p, int*);
unsigned int PCB_get_priority(PCB_p, int*);
int PCB_get_address(PCB_p, int*);
State PCB_get_p_state(PCB_p, int*);
int PCB_get_reg(PCB_p, int, int*);
struct tm *PCB_get_creation(PCB_p, int*);
struct tm *PCB_get_termination(PCB_p, int*);
int PCB_get_terminate(PCB_p, int*);
int PCB_get_term_count(PCB_p, int*);
int PCB_get_IO_1_traps(PCB_p,int, int*);
int PCB_get_IO_2_traps(PCB_p,int, int*);

//Utility Functions
char * PCB_to_string(PCB_p);
PCB_p PCB_create_random(int);
void create_PCB_TRAPS(PCB_p pcb);
void print_PCB(PCB_p);
#endif
