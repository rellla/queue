#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct Task
{
	struct Task*	prev;
	struct Task*	next;
	int value;
} Task;


typedef struct Queue
{
	struct Task* head;
	struct Task* tail;
	int length;
	pthread_mutex_t mutex;
} Queue;

Queue* q_init(void);
Task* t_init(void);

void q_push_head(Queue* queue, Task* task);
void q_push_tail(Queue* queue, Task* task);
void q_pop_head(Queue* queue);
void q_pop_tail(Queue* queue);
Task* q_peek_head(Queue* queue);
Task* q_peek_tail(Queue* queue);

int q_isEmpty(Queue* queue);
int q_length(Queue* queue);

void q_free(Queue* queue);
void t_free(Task* task);

void q_showQueue(Queue* queue);
