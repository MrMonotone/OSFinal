/********************************************************************************
* stack.h
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 1/24/16
*
* Description:
* This header file defines the class and methods for the Stack adt
*
*******************************************************************************/
#ifndef _STACK_H
#define _STACK_H
#include "node.h"
#include <stdlib.h>
#include <stdio.h>
typedef struct stack {
    Node_p head;
    unsigned int size;
} Stack;

typedef Stack *Stack_p;
Stack_p Stack_constructor();
void Stack_destructor(Stack_p);
void Stack_push(Stack_p, void*);
Node_p Stack_pop(Stack_p);
void Stack_peek(Stack_p);
void Stack_print(Stack_p stack);

#endif
