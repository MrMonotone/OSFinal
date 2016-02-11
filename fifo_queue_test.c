/********************************************************************************
* fifo_queue_test.c
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 1/24/16
*
* Description:
* This source file contains a few tests to ensure fifo_queue is working correctly
*
*******************************************************************************/
#include "fifo_queue.h"

int main(void){
    PCB_p pcb1 = PCB_constructor();
    PCB_set_pid(pcb1, 1337);
    PCB_set_pc(pcb1, 1222222);
    //print_PCB(pcb1);
    PCB_p pcb2 = PCB_constructor();
    PCB_set_pid(pcb2, 1338);
    FIFO_queue_p queue = FIFO_queue_constructor();
    FIFO_queue_enqueue(queue, pcb1);
    FIFO_queue_enqueue(queue, pcb2);
    
    int i= 0;
    int j =0;
    printf("Enqueing...\n");
    for(i =0; i < 40; i++){
        int randy = i + rand() % 45;
        PCB_p test = PCB_constructor();
        PCB_set_pid(test, randy);
        randy =  i + (rand() + 100) % 53;
        PCB_set_pc(test, randy);
        FIFO_queue_enqueue(queue, test);
    }
    print_FIFO_queue(queue);
    Node_p temp;
    printf("\n\n Dequeing...\n\n");
    int new_rand = (rand() % 16) + 10 ;
    for(j=0; j < new_rand;j++){
          temp = FIFO_queue_dequeue(queue);
          print_PCB((PCB_p)temp->data);
          if(temp->data != NULL){
              free(temp->data);
          }
          free(temp);
          temp = NULL;
                     
    } 
    print_FIFO_queue(queue);

    FIFO_queue_destructor(queue);

    return 0;
}
