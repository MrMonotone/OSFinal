/********************************************************************************
* stack.c
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 1/24/16
*
* Description:
* This source file implements functions for Stack defined in stack.h
*
*******************************************************************************/
#include "stack.h"
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Constructor for stack structure 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
Stack_p Stack_constructor(){
    Stack_p stack = malloc(sizeof(Stack));
    stack->head = NULL;
    stack->size = 0;
    return stack;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Destructor for stack structure 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void Stack_destructor(Stack_p stack){
    if(stack->head != NULL){
        Node_p temp = Stack_pop(stack);
        Stack_destructor(stack);
        Node_destructor(temp);
        return;
    }
    free(stack);    
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Push function for stack structure 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void Stack_push(Stack_p stack, void *data){
    Node_p new = Node_constructor(data);
    Node_p temp = stack->head;
    stack->head = new;
    stack->head->next = temp;
    stack->size++;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Pop function for stack structure 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
Node_p Stack_pop(Stack_p stack){
    Node_p temp = NULL;
    if(stack->size != 0){
        temp = stack->head;
        stack->head = stack->head->next;
        temp->next = NULL;
        stack->size--;
    }
    return temp;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Peek function for stack structure 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void Stack_peek(Stack_p stack){
    int size  = sizeof(stack->head->data);
    if(size == 4){
        printf("Head:%d\n", (int)stack->head->data);
     }
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Print function for stack structure, currently casts data as ints if size
+ of data is 4 bytes.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void Stack_print(Stack_p stack){
    int size = sizeof(stack->head->data);
    Node_p temp;
    if(size == 4){      
        temp = stack->head;
        while(temp != NULL){
            printf("[%d]<-", (int)temp->data);
            temp = temp->next;
        }
        printf("[-*]\n");
    }
}
