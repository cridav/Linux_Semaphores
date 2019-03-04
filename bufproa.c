#include "BUFFER.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
/* Producer with higher priority */


int main(int argc, char* argv[]){
int memkey,ret;
memkey=atoi(argv[1]);
memsize=atoi(argv[2]);
int i;
printf("Printing since producera\nmemid: %d\nmemsize: %d\n",memkey,memsize);
/*init_queue(memkey,memsize);*/

ret=create_semaphores(memsize);
printf("Create semaphores bufproa: %d\n",ret);
attach_queue(memkey);
printf("attached_queue bufproa\n");

srand(time(NULL));
for(i=0;i<10;i++){
 usleep(5000);
 printf("\t\t\tWriter VIP: %d\n",i);
 buf_push_front((i+1)*10+rand()%10);
}




/*show_queue();
q_clear();
show_queue();
push_rear(4);
show_queue();
push_rear(8);
show_queue();
push_front(29);
show_queue();
push_front(30);
show_queue();
*/
return 0;
}
