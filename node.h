/********************************************************************************
* node.h
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 1/24/16
*
* Description:
* This header file defines the class and methods for a Node
*
*******************************************************************************/
#ifndef _NODE_H
#define _NODE_H
#include <stdlib.h>
typedef struct Node{
    void *data;
    struct Node *next;
}Node;
typedef Node * Node_p;
Node_p Node_constructor(void*);
void Node_destructor(Node_p);
#endif
