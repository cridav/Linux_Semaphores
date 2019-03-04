#ifndef shared_QUEUE
#define shared_QUEUE

/*Memory Segment*/
int memid;
int memsize;

/* functions */
int* queue;
int* init_queue(int,int);
int* attach_queue(int);

/*MANAGE QUEUE*/
void q_clear(void);
void push_rear(int);
void push_front(int);
void pop(void);
void q_detache(void);
void show_queue(void);

/*MANAGE BUFFER*/
void buf_push_rear(int);
void buf_push_front(int);
void buf_pop(void);

/*SEMAPHORES*/
int create_semaphores(int);
void del_semaphores(void);
#endif
