/********************************************************************************
* fifo_queue.h
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 1/24/16
*
* Description:
* This header file defines the class and methods for the FIFO Queue
*
*******************************************************************************/
#ifndef _FIFO_Q_H
#define _FIFO_Q_H
#include "pcb.h"
#include <stdlib.h>
#include <stdio.h>
#include "node.h"
#define NAME_LEN 41

typedef struct FIFO_queue{
    char name[NAME_LEN];
    Node_p head;
    Node_p tail;
    int size;
} FIFO_queue;

typedef FIFO_queue * FIFO_queue_p;


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ function prototypes
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
FIFO_queue_p FIFO_queue_constructor(void);
void FIFO_queue_destructor(FIFO_queue_p);

//Utility Functions
void FIFO_queue_enqueue(FIFO_queue_p, void*);
void FIFO_queue_enqueue_node(FIFO_queue_p, Node_p);
Node_p FIFO_queue_dequeue(FIFO_queue_p);
void print_FIFO_queue(FIFO_queue_p queue);
void FIFO_queue_set_name(FIFO_queue_p queue, char[]);

#endif

