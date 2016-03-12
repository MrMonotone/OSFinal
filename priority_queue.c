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
    priority_queue_set_name(queue, "Default");
    queue->size =0;
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
  PCB_p temp = (PCB_p)data;
  FIFO_queue_enqueue(queue->queue[priority], temp);
  if(queue->head == NULL)
  {
      queue->head = queue->queue[temp->priority];
  }
  else if(temp->priority < ((PCB_p)queue->head->head->data)->priority)
  {
      queue->head = queue->queue[priority];
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
  Node_p temp = FIFO_queue_dequeue(queue->head);
  temp->next = NULL;
  if (queue->head->head == NULL)
  {
    queue->head = priority_queue_find_head(queue);
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
FIFO_queue_p priority_queue_find_head(priority_queue_p queue)
{
  FIFO_queue_p temp = NULL;
  for(int i = 0; i < PRIORITY_LEVELS; i++)
  {
    if(queue->queue[i]->head != NULL)
    {
      temp = queue->queue[i];
      break;
    }
  }
  return temp;
}
void update_priority_queue(priority_queue_p the_queue){
    int i =1;
    int j =0;
    Node_p temp;
    PCB_p temp_pcb;
    for(i =1; i < PRIORITY_LEVELS; i++){
        if(the_queue->queue[i]->head){
            for(j=0; j < the_queue->queue[i]->size; j++){
                temp = FIFO_queue_dequeue(the_queue->queue[i]);
                temp_pcb = (PCB_p)temp->data;
            //boost-a-tize me cap'n
            priority_queue_enqueue_node(the_queue, 
                                    temp, temp_pcb->priority - temp_pcb->boost);
                
            }
        }
    }
}
            
