#include "BUFFER.h" /* data structure for the messages in chat room */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>

/* Semaphores' identificators */
#define S_FULL 100
#define S_EMPTY 110
#define S_MUTEX 120

/* Semaphores' variables */
int s_full;
int s_empty;
int s_mutex;
int onoff;

/* Memory segment 

int memid;
int memsize;*/


int s_alloc(key_t,int);
int s_dealloc(int);
int s_ini(int,int);


union semun{
 int val;
 struct semid_ds *buf;
 unsigned short int *array;
 struct seminf *__buf;
};

/************************************ Begin data structure */

int* init_queue(int shmid, int q_size){
 memsize=q_size;
 if((memid=shmget(shmid,(q_size+3)*sizeof(int),0666|IPC_CREAT))<=0){
  printf("Error creating memory segment with id: %d\n",memid);
  return queue;
 }
 printf("[BUFFER.c] memid: %d\n",memid);
 if((queue=shmat(memid,0,0))<=0)
  printf("Error attaching memory");
 
 queue[q_size]=0;/*front of queue*/
 queue[q_size+1]=q_size-1;/*rear of queue*/
 queue[q_size+2]=0;/*counter of elements (last position)*/
 
 return queue;
}

int* attach_queue(int keysh){
 printf("[BUFFER.c] memid: %d\n",keysh);
 if((queue=shmat(keysh,0,0))<=0)
  printf("Error attaching memory"); 
 return queue;
}


/************************************ end structure for queue */

/* -=-=-=-=-=-=-=-=-=-= SEMAPHORES -=-=-=-=-=-=-=-=-=-= */
int create_semaphores(int M){
 
 s_full = s_alloc(S_FULL, 0666 | IPC_CREAT);
 s_empty = s_alloc(S_EMPTY, 0666 | IPC_CREAT);
 s_mutex = s_alloc(S_MUTEX, 0666 | IPC_CREAT);

 printf("Semaphores provided\ns_full: %d\ns_empty: %d\ns_mutex: %d\nM: %d\n",s_full,s_empty,s_mutex,M);
 if( s_full<0 || s_empty<0 || s_mutex<0) return -1;

 /*printf("s_ini s_full: %d\n",s_ini(s_full, M-1));
 printf("s_ini s_empty: %d\n",s_ini(s_empty,0));
 printf("s_ini s_mutex: %d\n",s_ini(s_mutex,1));*/
 s_ini(s_full,0);
 s_ini(s_empty,M);
 s_ini(s_mutex,1);
 onoff=1;
 return 1;
}

/*-=-=-=-=-=-=-= BUFFER_DE ALLOCATION -=-=-=-=-=-=-=*/
void del_semaphores(){
 s_dealloc(s_empty);
 s_dealloc(s_mutex);
 s_dealloc(s_full);
}


/* Allocate semaphore */
int s_alloc(key_t _key_t,int s_flags){
 return semget(_key_t,1,s_flags);
}
/* Deallocate semaphore */
int s_dealloc(int s_id){
 union semun noarg;
 return semctl(s_id,1,IPC_RMID, noarg);
}
/* Initizalization */

int s_ini(int s_id, int val){
 /*printf("initializing semaphores / s_id: %d, val: %d\n",s_id,val);*/
 union semun arg;
 unsigned short values[1];
 values[0]=val;
 arg.array = values;
 return semctl(s_id,0,SETALL,arg);
}

/*===========================DOWN sem_wait -=-=-= block -=-=-= proberen*/
int sem_wait(int s_id){
 int a;
 struct sembuf sb[1];
 sb[0].sem_num=0;
 sb[0].sem_op = -1; /*allocate resources*/
 sb[0].sem_flg = SEM_UNDO; /* Automatically undone when process terminates*/
 /*printf("before semop block\n");*/
 a= semop(s_id,sb,1);
 /*printf("-=-=-= block\ns_id: %d, semop: %d\n",s_id,a);*/
 /*printf("down ");*/
 return a;
}

/*====================UP sem_post -=-=-= unblock -=-=-= verhogen*/
int sem_post(int s_id){
 int b;
 struct sembuf sb[1];
 sb[0].sem_num = 0;
 sb[0].sem_op = 1;
 sb[0].sem_flg = SEM_UNDO;
 /*printf("before semop unblock\n");*/
 b=semop(s_id,sb,1);
 /*printf("-=-=-= unblock\ns_id: %d, semop: %d\n",s_id,b);*/
/*printf("up ");*/
 return b;
}


/* ************************************************************
   ************************************************************
   ******************* MANAGE THE BUFFER ***********************
   ************************************************************
   ************************************************************ */

/*-=-=-=-=-=-=-= BUFFER_PUSH_REAR -=-=-=-=-=-=-=*/
void buf_push_rear(int data){
 if (onoff == 0){
  printf("Semaphores not initialized");
 }
 sem_wait(s_empty);
 sem_wait(s_mutex);
 printf("\nPushing %d\n",data);
 push_rear(data);
 sem_post(s_mutex);
 sem_post(s_full);
}

/*-=-=-=-=-=-=-= BUFFER_PUSH_FRONT -=-=-=-=-=-=-=*/
void buf_push_front(int data){
 if (onoff == 0){
  printf("Semaphores not initialized");
 }
 sem_wait(s_empty);
 sem_wait(s_mutex);
 printf("\nFront pushing %d\n",data);
 push_front(data);
 sem_post(s_mutex);
 sem_post(s_full);
}

/*-=-=-=-=-=-=-= BUFFER_POP -=-=-=-=-=-=-=*/
/*Returns the first element of the queue*/

void buf_pop(){
 if (onoff == 0){
  printf("Semaphores not initialized");
 }
 sem_wait(s_full);
 sem_wait(s_mutex);
 printf("\nPopping %d\n",queue[queue[memsize]]);
 pop();
 sem_post(s_mutex);
 sem_post(s_empty);
}






/* ************************************************************
   ************************************************************
   ******************* MANAGE THE QUEUE ***********************
   ************************************************************
   ************************************************************ */


/*=-=-=-=-=-=-= CLEAR THE QUEUE -=-=-=-=-=-=-=*/
void q_clear(){
 
 int i;
 queue[memsize]=0;/*front of queue*/
 queue[memsize+1]=memsize-1;/*rear of queue*/
 queue[memsize+2]=0;/*counter of elements (last position)*/
 for(i = 0; i<=memsize-1; i++){
  queue[i] = 0;
 }
}



/*-=-=-=-=-=-=-= PUSH_REAR -=-=-=-=-=-=-=*/
void push_rear(int data){
 queue[memsize+1]=(queue[memsize+1]+1)%memsize;/*It makes the cyclic queue*/
 queue[memsize+2]+=1;/*counter*/
 queue[queue[memsize+1]]=data;
}

/*-=-=-=-=-=-=-= PUSH_FRONT -=-=-=-=-=-=-=*/
void push_front(int data){

 if(queue[memsize+2]+1<=memsize){/*if there are free slots*/
  queue[memsize]=(queue[memsize]-1)<0?memsize-1:queue[memsize]-1;/*if below zero, point to the end (cyclic queue)*/
  queue[memsize+2]+=1;
  queue[queue[memsize]]=data;
 }
}

/*-=-=-=-=-=-=-= POP -=-=-=-=-=-=-=*/
/*Returns the first element of the queue*/

void pop(){
 if (queue[memsize+2]>0){/*If there are still elements*/
 queue[memsize+2]-=1;/*decrease counter*/
 queue[queue[memsize]]=0;/*make zero deleted element*/
 queue[memsize]=(queue[memsize]+1)%(memsize);/*Moves the pointer in cyclic way*/
 }
}

/*-=-=-=-=-=-=-= DETACHE MEMORY -=-=-=-=-=-=-=*/
void q_detache(){
shmdt(queue);
}

/*-=-=-=-=-=-=-= SHOW_QUEUE -=-=-=-=-=-=-=*/
void show_queue(){
 /*system("clear");*/
 int i;
 printf("QueueFront: %d\nQueueRear: %d\nCounter: %d\n", queue[memsize]+1,queue[memsize+1]+1,queue[memsize+2]);/*only for visualisation purposes*/
 for(i = 0; i<=memsize-1; i++){
 printf("%d- ", queue[i]);
	}
printf("\n\n");
}
