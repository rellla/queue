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
	queue->length++;

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
	queue->length++;

	pthread_mutex_unlock(&queue->mutex);
}

void q_pop_head(Queue* queue)
{
	pthread_mutex_lock(&queue->mutex);

	Task* tmp = queue->head;
	queue->head = queue->head->next;

	if (queue->head == NULL)
		queue->tail = NULL;
	else
		queue->head->prev = NULL;

	queue->length--;
	t_free(tmp);

	pthread_mutex_unlock(&queue->mutex);
}

void q_pop_tail(Queue* queue)
{
	pthread_mutex_lock(&queue->mutex);

	Task* tmp = queue->tail;
	queue->tail = queue->tail->prev;

	if (queue->tail == NULL)
		queue->head = NULL;
	else
		queue->tail->next = NULL;

	queue->length--;
	t_free(tmp);

	pthread_mutex_unlock(&queue->mutex);
}

Task* q_peek_head(Queue* queue)
{
	pthread_mutex_lock(&queue->mutex);
	Task* t = queue->head;
	Task* tmp = t_init();

	if (t == NULL)
	{
		t_free(tmp);
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
		t_free(tmp);
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
	if (queue->length == 0)
		return 1;
	return 0;
}

int q_length(Queue* queue)
{
	return queue->length;
}

void q_free(Queue* queue)
{
	if (queue)
	{
		Task* tmp = queue->head;
		while(tmp)
		{
			Task* next = tmp->next;
			t_free(tmp);
			tmp = next;
		}
		free(queue);
	}
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
