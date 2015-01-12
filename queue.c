#include "queue.h"

Queue* q_init(void)
{
	Queue* queue = (Queue*) calloc(sizeof(Queue), 1);
	queue->head = NULL;
	queue->tail = NULL;
	pthread_mutex_init(&queue->mutex, NULL);
	return queue;
}

Task* t_init(void)
{
	Task* task = (Task*) calloc(sizeof(Task), 1);
	task->prev = NULL;
	task->next = NULL;
	return task;
}

void q_push_head(Queue* queue, Task* task)
{
	pthread_mutex_lock(&queue->mutex);
	if (queue->head == NULL)
		queue->tail = task;
	else
		queue->head->prev = task;

	task->next = queue->head;
	queue->head = task;
	pthread_mutex_unlock(&queue->mutex);

}

void q_push_tail(Queue* queue, Task* task)
{
	pthread_mutex_lock(&queue->mutex);
	if (queue->head == NULL)
		queue->head = task;
	else
		queue->tail->next = task;

	task->prev = queue->tail;
	queue->tail = task;
	pthread_mutex_unlock(&queue->mutex);
}

void q_pop_head(Queue* queue)
{
	Task* tmp;
	pthread_mutex_lock(&queue->mutex);
	tmp = queue->head;
	queue->head = queue->head->next;

	if (queue->head == NULL)
		queue->tail = NULL;
	else
		queue->head->prev = NULL;

	if (tmp != NULL)
		tmp->next = NULL;

	pthread_mutex_unlock(&queue->mutex);
}

void q_pop_tail(Queue* queue)
{
	Task* tmp;
	pthread_mutex_lock(&queue->mutex);
	tmp = queue->tail;
	queue->tail = queue->tail->prev;

	if (queue->tail == NULL)
		queue->head = NULL;
	else
		queue->tail->next = NULL;

	if (tmp != NULL)
		tmp->prev = NULL;

	pthread_mutex_unlock(&queue->mutex);
}

Task* q_peek_head(Queue* queue)
{
	pthread_mutex_lock(&queue->mutex);
	Task* t = queue->head;
	Task* tmp = t_init();

	if (t == NULL)
	{
		pthread_mutex_unlock(&queue->mutex);
		return NULL;
	}
	else
	{
		tmp->value = t->value;
		pthread_mutex_unlock(&queue->mutex);
		return tmp;
	}
}

Task* q_peek_tail(Queue* queue)
{
	pthread_mutex_lock(&queue->mutex);
	Task* t = queue->tail;
	Task* tmp = t_init();
	if (t == NULL)
	{
		pthread_mutex_unlock(&queue->mutex);
		return NULL;
	}
	else
	{
		tmp->value = t->value;
		pthread_mutex_unlock(&queue->mutex);
		return tmp;
	}
}

int q_isEmpty(Queue* queue)
{
	if (queue->head == NULL && queue->tail == NULL)
		return 1;
	return 0;
}

void q_free(Queue* queue)
{
	if (queue)
		free(queue);
}

void t_free(Task* task)
{
	if (task)
		free(task);
}

void q_showQueue(Queue* queue)
{
	Task* help = queue->head;

	while(help != NULL)
	{
		printf("%d ", help->value);
		help = help->next;
	}

	printf("\n");
}
