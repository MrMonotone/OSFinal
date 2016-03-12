/********************************************************************************
* pcp.h
*
* Team 4
*Sean Hoyt
*Nick Mousel
*Chris Kubec
*Molly Nelson
*Date: 3/11/16
*
* Description:
* This header file defines the class and methods for the procer consumer pairs.
*
*******************************************************************************/
#ifndef _PCP_H
#define _PCP_H
#include "pcb.h"
#include "mutex.h"

typedef struct pcp{
  PCB_p producer;
  PCB_p consumer;
  mutex_p sharedmutex1;
  mutex_p sharedmutex2;
  mutex_cond_p sharedcon;
}PCP;
typedef PCP *PCP_p;
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ function prototypes
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
PCP_p PCP_constructor();
void PCP_destructor(PCP_p);

#endif
