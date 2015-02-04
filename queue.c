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

/* initialize queue
 *
 */
QUEUE *q_init(void)
{
	QUEUE *queue = (QUEUE *) calloc(sizeof(QUEUE), 1);
	queue->head = queue->tail = NULL;
	queue->length = 0;
	pthread_mutex_init(&queue->mutex, NULL);
	return queue;
}

/* allocate task data
 *
 */
TASK *allocate_task(void *data)
{
	TASK *task = (TASK *)calloc(1, sizeof(TASK));
	task->next = task->prev = NULL;
	task->data = data;
	return (task);
}

/* push task to the tail position
 *
 */
void q_push_tail(QUEUE *queue, void *data)
{
	if (!queue || !data)
		return;

	pthread_mutex_lock(&queue->mutex);

	TASK *task = allocate_task(data);

	if (queue->head == NULL)
		queue->head = task;
	else
		queue->tail->next = task;

	task->prev = queue->tail;
	queue->tail = task;
	queue->length++;

	pthread_mutex_unlock(&queue->mutex);
}

/* push task to the head position
 *
 */
void q_push_head(QUEUE *queue, void* data)
{
	if (!queue || !data)
		return;

	pthread_mutex_lock(&queue->mutex);

	TASK *task = allocate_task(data);

	if (queue->head == NULL)
		queue->tail = task;
	else
		queue->head->prev = task;

	task->next = queue->head;
	queue->head = task;
	queue->length++;

	pthread_mutex_unlock(&queue->mutex);
}

/* drop task at head position
 *
 */
void q_pop_head(QUEUE *queue)
{
	if (!queue)
		return;

	pthread_mutex_lock(&queue->mutex);

	if (queue->head)
	{
		TASK *tmp = queue->head;
		queue->head = queue->head->next;

		if (queue->head == NULL)
			queue->tail = NULL;
		else
			queue->head->prev = NULL;

		queue->length--;
		t_free(tmp);
	}

	pthread_mutex_unlock(&queue->mutex);
}

/* drop task at tail position
 *
 */
void q_pop_tail(QUEUE *queue)
{
	if (!queue)
		return;

	pthread_mutex_lock(&queue->mutex);

	if (queue->tail)
	{
		TASK* tmp = queue->tail;
		queue->tail = queue->tail->prev;

		if (queue->tail == NULL)
			queue->head = NULL;
		else
			queue->tail->next = NULL;

		queue->length--;
		t_free(tmp);
	}

	pthread_mutex_unlock(&queue->mutex);
}

/* extract task at head position into extra task
 *
 */
void q_extract_head(QUEUE *queue, void *data)
{
	if (!queue || !data)
		return;

	pthread_mutex_lock(&queue->mutex);

	if (queue->head)
		memcpy(data, queue->head->data, sizeof(*data));
	else
		data = NULL;

	pthread_mutex_unlock(&queue->mutex);
}

/* extract task at tail position into extra task
 *
 */
void q_extract_tail(QUEUE *queue, void *data)
{
	if (!queue || !data)
		return;

	pthread_mutex_lock(&queue->mutex);

	if (queue->tail)
		memcpy(data, queue->tail->data, sizeof(*data));
	else
		data = NULL;

	pthread_mutex_unlock(&queue->mutex);
}

/* find task at head position and return it
 *
 */
void* q_peek_head(QUEUE *queue)
{
	if (!queue)
		return;

	pthread_mutex_lock(&queue->mutex);
	TASK* task;

	if (queue->head)
		task = queue->head;

	pthread_mutex_unlock(&queue->mutex);

	if (task)
		return task->data;
	else
		return NULL;
}

/* find task at tail position and return it
 *
 */
void* q_peek_tail(QUEUE *queue)
{
	if (!queue)
		return;

	pthread_mutex_lock(&queue->mutex);
	TASK* task;

	if (queue->tail)
		task = queue->tail;

	pthread_mutex_unlock(&queue->mutex);

	if (task)
		return task->data;
	else
		return NULL;
}

/* check, if queue is empty
 *
 */
int q_isEmpty(QUEUE *queue)
{
	if (queue->length == 0)
		return 1;
	return 0;
}

/* return the length of the queue
 *
 */
int q_length(QUEUE *queue)
{
	return queue->length;
}

/* free queue and all tasks
 *
 */
void q_free(QUEUE *queue)
{
	if (queue)
	{
		pthread_mutex_lock(&queue->mutex);

		TASK *tmp = queue->head;
		while(tmp)
		{
			TASK *next = tmp->next;
			t_free(tmp);
			tmp = next;
		}
		pthread_mutex_unlock(&queue->mutex);
		pthread_mutex_destroy(&queue->mutex);
		free(queue);
	}
}

/* free task and allocated data
 *
 */
void t_free(TASK *task)
{
	if (task)
	{
		if (task->data)
			free(task->data);
		free(task);
	}
}


/* do something recursive with the queue's data
 *
 */
void q_recursive(QUEUE *queue, action func)
{
	if (queue)
	{
		pthread_mutex_lock(&queue->mutex);
		TASK *task = queue->head;

		while(task != NULL)
		{
			func(task->data);
			task = task->next;
		}
		pthread_mutex_unlock(&queue->mutex);
	}
}
