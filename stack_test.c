/********************************************************************************
* stack_test.c
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 1/25/16
*
* Description:
* This source file contains a few tests to ensure stack is working correctly
*
*******************************************************************************/
#include "stack.h"

int main(void){
    Stack_p sys_stack = Stack_constructor();
    Stack_print(sys_stack);
    int i=0, j = 0;
    
    for(i = 0; i < 36; i++){
        Stack_push(sys_stack,(void*) i);
        //Stack_peek(sys_stack);
        Stack_print(sys_stack);
        if(i > 18){
            int randy = (rand()  + i) % 7;
            for(j=0; j < randy; j++){
                if(sys_stack->size != 0){
                    //Stack_peek(sys_stack);
                    Node_p temp = Stack_pop(sys_stack);
                   // Stack_print(sys_stack);
                    if(temp != NULL){
                         Node_destructor(temp);
                     }
               }
            }
        }
    }
    Stack_print(sys_stack);
    return 0;
}

