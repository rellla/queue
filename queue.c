/*
 * Copyright (c) 2015 Andreas Baierl <ichgeh@imkreisrum.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "queue.h"

/* initialize queue */
//
Queue* q_init(void)
{
	Queue* queue = (Queue*) calloc(sizeof(Queue), 1);
	queue->head = NULL;
	queue->tail = NULL;
	queue->length = 0;
	pthread_mutex_init(&queue->mutex, NULL);
	return queue;
}


/* initialize task */
//
Task* t_init(void)
{
	Task* task = (Task*) calloc(sizeof(Task), 1);
	task->prev = NULL;
	task->next = NULL;
	return task;
}


/* push task to the head position */
//
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


/* push task to the tail position */
//
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


/* drop element at head position */
//
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


/* drop element at tail position */
//
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


/* extract element at head position into task */
//
void q_peek_head(Queue* queue, Task* task)
{
	pthread_mutex_lock(&queue->mutex);

	if (queue->head)
	{
		memcpy(task, queue->head, sizeof(struct Task));
		task->prev = NULL;
		task->next = NULL;
	}
	else
		task = NULL;

	pthread_mutex_unlock(&queue->mutex);
}


/* extract element at tail position into task */
//
void q_peek_tail(Queue* queue, Task* task)
{
	pthread_mutex_lock(&queue->mutex);

	if (queue->tail)
	{
		memcpy(task, queue->tail, sizeof(struct Task));
		task->prev = NULL;
		task->next = NULL;
	}
	else
		task = NULL;

	pthread_mutex_unlock(&queue->mutex);
}


/* check, if queue is empty */
//
int q_isEmpty(Queue* queue)
{
	if (queue->length == 0)
		return 1;
	return 0;
}


/* return the length of the queue */
//
int q_length(Queue* queue)
{
	return queue->length;
}


/* free queue and all tasks */
//
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
		pthread_mutex_destroy(&queue->mutex);
		free(queue);
	}
}


/* free single task */
//
void t_free(Task* task)
{
	if (task)
		free(task);
}
