#include "BUFFER.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
/* Producer with higher priority */


int main(int argc, char* argv[]){
int memkey;
memkey=atoi(argv[1]);
memsize=atoi(argv[2]);
int i;
printf("Printing since readera\nmemid: %d\nmemsize: %d\n",memkey,memsize);
/*init_queue(memkey,memsize);*/
attach_queue(memkey);
printf("rea ");
show_queue();
/*srand(time(NULL));*/
for(i=0;i<30;i++){
 pop();
printf("rea ");
show_queue();
}
printf("rea ");
show_queue();



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
