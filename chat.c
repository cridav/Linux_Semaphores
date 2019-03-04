#include "BUFFER.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
/* Producer with higher priority */

int main(void){
char command[100];
int memkey;/*key given by user*/
memkey=memid=9876;/*after init_queue, memid contains the key given by shmget*/
memsize=30;
system("clear");
init_queue(memkey,memsize);
q_clear();
show_queue();
/*-=-=-= ONLY PROA -=-=-=*/
printf("\t-=-=-=-=-=Only proa\n");
sprintf(command,"./proa %d %d",memid,memsize);
system(command);
show_queue();

/*-=-=-= ONLY PROB -=-=-=*/
printf("\t-=-=-=-=-=Only prob\n");
sprintf(command,"./prob %d %d",memid,memsize);
system(command);
show_queue();

sleep(5);
/*system("clear");*/
q_clear();

/*-=-=-= PROA & PROB -=-=-=*/
printf("\t-=-=-=-=-=proa & prob\n");
show_queue();
sprintf(command,"./proa %d %d & ./prob %d %d",memid,memsize,memid,memsize);
system(command);
show_queue();

sleep(5);

/*q_clear();*/


/*-=-=-= PROA & PROB & REA -=-=-=*/
q_clear();
show_queue();
printf("\t-=-=-=-=-=proa & prob & rea\n");
sprintf(command,"./proa %d %d & ./prob %d %d & ./rea %d %d",memid,memsize,memid,memsize,memid,memsize);
system(command);
show_queue();



/*-=-=-= PROA & PROB -=-=-=*/
q_clear();
show_queue();
printf("\t-=-=-=-=-=proa & prob next rea\n");
sprintf(command,"./proa %d %d & ./prob %d %d",memid,memsize,memid,memsize);
system(command);
show_queue();
sprintf(command,"./rea %d %d",memid,memsize);
system(command);
show_queue();
sleep(5);
/*system("clear");*/







q_detache();
printf("queue detached\n");
attach_queue(memid);
show_queue();
return 0;
}
