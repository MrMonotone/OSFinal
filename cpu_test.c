/********************************************************************************
* cpu_test.c
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 1/26/16
*
* Description:
* This source file contains a few tests to ensure CPU struct is working correctly
*
*******************************************************************************/
#include "cpu.h"

int main(void){
    CPU_p new_cpu = CPU_constructor();
    new_cpu->pid = 11;
    new_cpu->pc = 11100;
    new_cpu->sw = 2442828;
    new_cpu->address = 12322;
    new_cpu->p_state = READY;
    printf("CPU: PID:%u PC:%u SW:%u ADDRESS:%u\n",
            new_cpu->pid, new_cpu->pc, new_cpu->sw, new_cpu->address);
    CPU_destructor(new_cpu); 
    return 0;
}
