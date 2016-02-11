/********************************************************************************
* node.c
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
#include "node.h"
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Constructor for Node structure 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
Node_p Node_constructor(void *data){
    Node_p new = malloc(sizeof(Node) + sizeof(data));
    new->next = NULL;
    new->data = data;
    return new;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Destructor for Node structure 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void Node_destructor(Node_p node){
    if(node){
        if(node->data){
            free(node->data);
         }
         node->next = NULL;
         free(node);
    }
}
