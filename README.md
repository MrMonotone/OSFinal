OSFinal
Final Project for 422
To compile to the current scheduler run:
gcc -g -l pthread node.c pcb.c fifo_queue.c priority_queue.c stack.c cpu.c scheduler.c cpu_main_loop.c -o threaded_scheduler

then run: ./threaded_scheduler
