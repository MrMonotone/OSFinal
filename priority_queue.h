/********************************************************************************
* priority_queue.h
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 2/29/16
*
* Description:
* This header file defines the class and methods for the priority queue
*
*******************************************************************************/
#ifndef _P_Q_H
#define _P_Q_H
#include "pcb.h"
#include <stdlib.h>
#include <stdio.h>
#include "node.h"
#include "fifo_queue.h"
#define  PRIORITY_LEVELS 4
#define NAME_LEN 41
typedef struct priority_queue
{
  char name[NAME_LEN];
  FIFO_queue_p head;
  int size;
  FIFO_queue_p queue[PRIORITY_LEVELS];
}priority_queue;

typedef priority_queue * priority_queue_p;


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ function prototypes
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
priority_queue_p priority_queue_constructor(void);
void priority_queue_destructor(priority_queue_p);

//Utility Functions
void priority_queue_enqueue(priority_queue_p, void*, int);
void priority_queue_enqueue_node(priority_queue_p, Node_p, int);
Node_p priority_queue_dequeue(priority_queue_p);
void print_priority_queue(priority_queue_p);
void priority_queue_set_name(priority_queue_p, char[]);
FIFO_queue_p priority_queue_find_head(priority_queue_p);
void update_priority_queue(priority_queue_p);
#endif
