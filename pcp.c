/********************************************************************************
* pcp.c
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 3/11/16
*
* Description:
* This source file implements functions for the PCP from pcp.h
*
*******************************************************************************/
#include "pcp.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Constructor for PCP structure
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
PCP_p PCP_constructor(){
  // TODO do we need to allocate for a new pcb if we are just pointing to an existing one???
    PCP_p new = malloc(sizeof(PCP));
    new->producer = NULL;
    new->consumer = NULL;
    new->sharedmutex = NULL;
    new->sharedcon = NULL;
    return new;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Destructor for PCP structure
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void PCP_destructor(PCP_p pcp){
  // TODO decide if want to destroy everything inside pcp or just the pcp itself
}
