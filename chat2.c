#include "BUFFER.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
/* This programm only initializes memory in order to execute by myself the proca, procb and rea scripts */

int main(int argc, char* argv[]){
int memkey;
memkey=atoi(argv[1]);
memsize=atoi(argv[2]);

init_queue(memkey,memsize);
q_clear();
show_queue();
return 0;
}
