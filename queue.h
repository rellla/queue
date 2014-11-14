#include <stdio.h>
#include <stdlib.h>

typedef struct TaskData
{
	int value;
/*
	struct timespec			when;
	uint32_t				clip_width;
	uint32_t				clip_height;
	VdpOutputSurface		surface;
	unsigned int			wipe_tasks;
	VdpPresentationQueue	queue_id;
*/
} TaskData;

typedef struct Task
{
	struct Task*	prev;
	struct Task*	next;
	TaskData* data;
} Task;


typedef struct Queue
{
	struct Task* head;
	struct Task* tail;
} Queue;

signed int sort_insert(Task* a, Task* b);
typedef signed int (*compare_func)(Task* a, Task* b);

Queue* q_init(void);
Task* t_init(void);
void q_push_head(Queue* queue, Task* task);
void q_push_tail(Queue* queue, Task* task);
void q_pop_head(Queue* queue);
void q_pop_tail(Queue* queue);
void q_insert_sorted(Queue* queue, Task* task, compare_func cmp);
void q_timeout_pop(Queue* queue, int timeout);
Task* q_peek_head(Queue* queue);
Task* q_peek_tail(Queue* queue);

void q_showQueue(Queue* queue);
int q_isEmpty(Queue* queue);
