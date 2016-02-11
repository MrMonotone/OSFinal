/********************************************************************************
* fifo_queue.c
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 1/24/16
*
* Description:
* This source file implements the class and functions defined in fifo_queue.h.
*
*******************************************************************************/

#include "fifo_queue.h"
#include "string.h"
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Constructor for FIFO_queue object.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
FIFO_queue_p FIFO_queue_constructor(){
    FIFO_queue_p queue = malloc(sizeof(FIFO_queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    FIFO_queue_set_name(queue, "Default");
    return queue;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Destructor for FIFO_queue object.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void FIFO_queue_destructor(FIFO_queue_p queue){
    Node_p temp = queue->head;
    if(temp != NULL){
        queue->head = queue->head->next;
  
        FIFO_queue_destructor(queue);
        Node_destructor(temp);
        return;
    }
    free(queue);
}
        
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Enqueue method for FIFO_queue allows any type of data to be enqueued to queue.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void FIFO_queue_enqueue(FIFO_queue_p queue, void *data){
    Node_p new = Node_constructor(data);
    if(queue && data){
        if(queue->size == 0){
            queue->head = queue->tail = new;
         }else {
            queue->tail->next = new;
            queue->tail = new;
         }
     queue->size++;
     }
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Enqueue method for FIFO_queue allows node to be enqueued to queue.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void FIFO_queue_enqueue_node(FIFO_queue_p queue, Node_p node){
    if(queue && node){
        if(queue->size == 0){
            queue->head = queue->tail = node;
        }else {
            queue->tail->next = node;
            queue->tail = node;
        }
        queue->size++;
    }
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Dequeue method for FIFO_queue.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
Node_p FIFO_queue_dequeue(FIFO_queue_p queue){
    Node_p temp;
    
    if(queue->size == 0){
        //do nothing
        return NULL;
    }else if(queue->size ==1){
        temp = queue->head;
        queue->head = NULL;
        queue->tail = NULL;
        queue->size = 0;
        temp->next = NULL;
    }else {
        temp = queue->head;

        queue->head = queue->head->next;
        temp->next = NULL;
        queue->size--;
    }
    return temp;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Print method for FIFO_queue.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void print_FIFO_queue(FIFO_queue_p queue){
    if(queue){
       int i =0;
       Node_p temp = queue->head;
       printf("%s \n", queue->name);
       while(temp != NULL){
            PCB_p pcb = (PCB_p) temp->data;
            if(i == 0){
                printf("(P:%d)", pcb->pid);
            }else {
                printf("<-(P:%d)", pcb->pid);
            }
            i++;
            temp = temp->next;

       }
       printf("(-*) \n");
    }
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Setter for FIFO_queue name field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void FIFO_queue_set_name(FIFO_queue_p queue, char name[]){
     strcpy(queue->name, name);
}
   
        
