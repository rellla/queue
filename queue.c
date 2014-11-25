#include "queue.h"

Queue* q_init(void)
{
	Queue* queue = (Queue*) calloc(sizeof(Queue), 1);
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}

TaskData* td_init(void)
{
	TaskData* taskdata = (TaskData*) calloc(sizeof(TaskData), 1);
	return taskdata;
}

Task* t_init(void)
{
	Task* task = (Task*) calloc(sizeof(Task), 1);
	task->prev = NULL;
	task->next = NULL;
	task->data = td_init();
	return task;
}

void q_push_head(Queue* queue, Task* task)
{
	Task* tmp = (Task*) malloc(sizeof(Task));
	tmp = task;

	if (queue->head == NULL)
		queue->tail = tmp;
	else
		queue->head->prev = tmp;

	tmp->next = queue->head;
	queue->head = tmp;
}

void q_push_tail(Queue* queue, Task* task)
{
	Task* tmp = (Task*) malloc(sizeof(Task));
	tmp = task;

	if (queue->head == NULL)
		queue->head = tmp;
	else
		queue->tail->next = tmp;

	tmp->prev = queue->tail;
	queue->tail = tmp;
}

void q_pop_head(Queue* queue)
{
	Task* tmp;
	tmp = queue->head;
	queue->head = queue->head->next;

	if (queue->head == NULL)
		queue->tail = NULL;
	else
		queue->head->prev = NULL;

	if (tmp != NULL)
	{
		tmp->next = NULL;
		free(tmp);
	}
}

void q_pop_tail(Queue* queue)
{
	Task* tmp;
	tmp = queue->tail;
	queue->tail = queue->tail->prev;

	if (queue->tail == NULL)
		queue->head = NULL;
	else
		queue->tail->next = NULL;

	if (tmp != NULL)
	{
		tmp->prev = NULL;
		free(tmp);
	}
}

void q_insert_sorted(Queue* queue, Task* task, compare_func cmp)
{
	// This only works, if the queue is already sorted
	// Otherwise it compares the task with the values 
	// starting from the head

	Queue* q_tmp = queue;
	signed int comp;

	if (queue->head == 0)
	{
		Queue* q_new = q_init();
		q_push_tail(q_new, task);
		*q_tmp = *q_new;
		return;
	}

	Queue* q_tmphead = q_init();

	comp = (*cmp)(task, q_tmp->head);
	while ((q_tmp->head) && (comp > 0))
	{
		q_push_tail(q_tmphead, q_tmp->head);
		if (q_tmp->head->next) {
			q_tmp->head = q_tmp->head->next;
			comp = (*cmp)(task, q_tmp->head);
		}
		else
			q_tmp->head = NULL;
	}

	q_push_tail(q_tmphead, task);

	while ((q_tmp->head))
	{
		q_push_tail(q_tmphead, q_tmp->head);
		q_tmp->head = q_tmp->head->next;
	}

	*q_tmp = *q_tmphead;
}

Task* q_peek_head(Queue* queue)
{
	Task* t = queue->head;
	Task* tmp = t_init();

	if (t == NULL)
		return NULL;
	else
	{
		tmp->data = t->data;
		return tmp;
	}
}

Task* q_peek_tail(Queue* queue)
{
	Task* t = queue->tail;
	Task* tmp = t_init();
	if (t == NULL)
		return NULL;
	else
	{
		tmp->data = t->data;
		return tmp;
	}
}

int q_isEmpty(Queue* queue)
{
	if (queue->head == NULL && queue->tail == NULL)
		return 1;
	return 0;
}

void q_showQueue(Queue* queue)
{
	Task* help = queue->head;

	while(help != NULL)
	{
		printf("%d ", help->data->value);
		help = help->next;
	}

	printf("\n");
}

signed int sort_insert(Task* a, Task* b)
{
	Task* task_a = a;
	Task* task_b = b;

	int val_a = task_a->data->value;
	int val_b = task_b->data->value;

	if (val_a < val_b)
		return -1;
	else if (val_a > val_b)
		return 1;
	else
		return 0;
}
