/********************************************************************************
* priority_queue.c
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 2/29/16
*
* Description:
* This source file implements the class and functions defined in priority_queue.h.
*
*******************************************************************************/

#include "priority_queue.h"
#include "string.h"
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Constructor for priority_queue object.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
priority_queue_p priority_queue_constructor(){
    priority_queue_p queue = malloc(sizeof(priority_queue));

    for (int i = 0; i < PRIORITY_LEVELS; i++)
    {
      queue->queue[i] = FIFO_queue_constructor();
    }
    queue->head = NULL;
    queue->tail = NULL;
    priority_queue_set_name(queue, "Default");
    return queue;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Destructor for priority_queue object.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void priority_queue_destructor(priority_queue_p queue)
{
  for (int i = 0; i < PRIORITY_LEVELS; i++)
  {
    FIFO_queue_destructor(queue->queue[i]);
  }

  free(queue);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Enqueue method for priority_queue allows any type of data to be enqueued to queue.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void priority_queue_enqueue(priority_queue_p queue, void *data, int priority)
{
  FIFO_queue_enqueue(queue->queue[priority], data);
  if(queue->head == NULL)
  {
      queue->head = queue->tail = queue->queue[priority]->head;
  }
  else if(data->priority => queue->tail->priority)
  {
    queue->tail = queue->queue[priority]->tail;
  }
  else if(data->priority < queue->head->priority)
  {
      queue->head = queue->queue[priority]->head;
  }
  queue->size++;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Enqueue method for priority_queue allows node to be enqueued to queue.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void priority_queue_enqueue_node(priority_queue_p queue, Node_p node, int priority)
{
  FIFO_queue_enqueue_node(queue->queue[priority], node);
  queue->size++;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Dequeue method for priority_queue.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
Node_p priority_queue_dequeue(priority_queue_p queue)
{
  int priority = queue->head->priority;
  Node_p temp = FIFO_queue_dequeue(queue->queue[priority]);
  queue->head = queue->queue[priority]->head;
  if (queue->head == NULL)
  {
    for(int i = 0; i < PRIORITY_LEVELS; i++)
    {
      if(queue[i]-> head != NULL)
      {
        queue->head = queue[i]->head;
      }
    }
  }
  queue->size--;
  return temp;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Print method for priority_queue.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void print_priority_queue(priority_queue_p queue)
{
  for (int i = 0; i < PRIORITY_LEVELS; i++)
  {
    print_FIFO_queue(queue->queue[i]);
  }
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Setter for priority_queue name field.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void priority_queue_set_name(priority_queue_p queue, char name[])
{
     strcpy(queue->name, name);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Finds new head if a queue become empty
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
Node_p priority_queue_find_head(priority_queue_p queue)
{
  Node_p temp = NULL
  for(int i = 0; i < PRIORITY_LEVELS; i++)
  {
    if(queue[i]->head != NULL)
    {
      temp = queue[i]->head;
      break;
    }
  }
  return temp;
}
