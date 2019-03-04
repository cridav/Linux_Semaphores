#include "BUFFER.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
/* This programm only initializes memory in order to execute by myself the proca, procb and rea scripts */
int ret;
int main(int argc, char* argv[]){
char command[100];
int memkey;
memkey=atoi(argv[1]);
memsize=atoi(argv[2]);

init_queue(memkey,memsize);
q_clear();
show_queue();
ret=create_semaphores(memsize);
printf("Create semaphores: %d\n",ret);

/*-=-=-= PROA & PROB & REA -=-=-=*/

printf("\t-=-=-=-=-=bproa & bprob & brea\n");
sprintf(command,"./bproa %d %d & ./bprob %d %d & ./brea %d %d",memid,memsize,memid,memsize,memid,memsize);
/*sprintf(command,"./bproa %d %d & ./bprob %d %d",memid,memsize,memid,memsize);*/
system(command);

printf("Finishing, detaching buffer and semaphores\n");
show_queue();
/*q_clear();
show_queue();*/

del_semaphores();
q_detache();

return 0;
}
