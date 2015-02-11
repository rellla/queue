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
QUEUE *q_queue_init(void)
{
	QUEUE *queue = (QUEUE *) calloc(1, sizeof(QUEUE));
	queue->head = queue->tail = NULL;
	queue->length = 0;
	pthread_mutex_init(&queue->mutex, NULL);
	return queue;
}

/* allocate node data
 *
 */
NODE *allocate_node(void *data)
{
	NODE *node = (NODE *)calloc(1, sizeof(NODE));
	node->next = node->prev = NULL;
	node->data = data;
	return (node);
}

/* push node to the tail position
 *
 */
qStatus q_push_tail(QUEUE *queue, void *data)
{
	if (!queue || !data)
		return Q_ERROR;

	NODE *node = allocate_node(data);

	if (!node)
		return Q_ERROR;

	pthread_mutex_lock(&queue->mutex);

	if (queue->head == NULL)
		queue->head = node;
	else
		queue->tail->next = node;

	node->prev = queue->tail;
	queue->tail = node;
	queue->length++;

	pthread_mutex_unlock(&queue->mutex);

	return Q_SUCCESS;
}

/* push node to the head position
 *
 */
qStatus q_push_head(QUEUE *queue, void* data)
{
	if (!queue || !data)
		return Q_ERROR;

	NODE *node = allocate_node(data);

	if (!node)
		return Q_ERROR;

	pthread_mutex_lock(&queue->mutex);

	if (queue->head == NULL)
		queue->tail = node;
	else
		queue->head->prev = node;

	node->next = queue->head;
	queue->head = node;
	queue->length++;

	pthread_mutex_unlock(&queue->mutex);

	return Q_SUCCESS;
}

/* fill queue sorted
 *
 */
qStatus q_insert_sorted(QUEUE *queue, void* data, action2 func)
{

	if (!queue || !data || !func)
		return Q_ERROR;

	NODE *node_new = allocate_node(data);

	if(!node_new)
		return Q_ERROR;

	pthread_mutex_lock(&queue->mutex);
	NODE *node = queue->head;

	while(node && (func(node->data, node_new->data) < 0))
		node = node->next;

	if (node == queue->head)
	{
		if (queue->head == NULL)
			queue->tail = node_new;
		else
			queue->head->prev = node_new;

		node_new->next = queue->head;
		queue->head = node_new;
	}
	else if (node == NULL)
	{
		if (queue->head == NULL)
			queue->head = node_new;
		else
			queue->tail->next = node_new;

		node_new->prev = queue->tail;
		queue->tail = node_new;
	}
	else
	{
		node_new->prev = node->prev;
		node_new->next = node;
		node->prev = node_new;
		if (node_new->prev)
			node_new->prev->next = node_new;
	}

	queue->length++;

	pthread_mutex_unlock(&queue->mutex);

	return Q_SUCCESS;
}

/* drop node at head position
 *
 */
qStatus q_pop_head(QUEUE *queue)
{
	if (!queue)
		return Q_ERROR;

	pthread_mutex_lock(&queue->mutex);

	if (queue->head)
	{
		NODE *tmp = queue->head;
		queue->head = queue->head->next;

		if (queue->head == NULL)
			queue->tail = NULL;
		else
			queue->head->prev = NULL;

		queue->length--;
		q_node_free(tmp);

		pthread_mutex_unlock(&queue->mutex);
		return Q_SUCCESS;
	}

	pthread_mutex_unlock(&queue->mutex);
	return Q_EMPTY_NODE;
}

/* drop node at tail position
 *
 */
qStatus q_pop_tail(QUEUE *queue)
{
	if (!queue)
		return Q_ERROR;

	pthread_mutex_lock(&queue->mutex);

	if (queue->tail)
	{
		NODE* tmp = queue->tail;
		queue->tail = queue->tail->prev;

		if (queue->tail == NULL)
			queue->head = NULL;
		else
			queue->tail->next = NULL;

		queue->length--;
		q_node_free(tmp);

		pthread_mutex_unlock(&queue->mutex);
		return Q_SUCCESS;
	}

	pthread_mutex_unlock(&queue->mutex);
	return Q_EMPTY_NODE;
}

/* extract node at head position into extra node
 *
 */
qStatus q_extract_head(QUEUE *queue, void *data, int size)
{
	if (!queue || !data)
		return Q_ERROR;

	pthread_mutex_lock(&queue->mutex);

	if (queue->head)
		memcpy(data, queue->head->data, size);

	pthread_mutex_unlock(&queue->mutex);

	return Q_SUCCESS;
}

/* extract node at tail position into extra node
 *
 */
qStatus q_extract_tail(QUEUE *queue, void *data, int size)
{
	if (!queue || !data)
		return Q_ERROR;

	pthread_mutex_lock(&queue->mutex);

	if (queue->tail)
		memcpy(data, queue->tail->data, size);

	pthread_mutex_unlock(&queue->mutex);

	return Q_SUCCESS;
}

/* find node at head position and return it
 *
 */
qStatus q_peek_head(QUEUE *queue, void **data)
{
	if (!queue)
		return Q_ERROR;

	pthread_mutex_lock(&queue->mutex);

	if (queue->head)
	{
		*data = queue->head->data;
		pthread_mutex_unlock(&queue->mutex);
		return Q_SUCCESS;
	}

	pthread_mutex_unlock(&queue->mutex);
	return Q_ERROR;
}

/* find node at tail position and return it
 *
 */
qStatus q_peek_tail(QUEUE *queue, void **data)
{
	if (!queue)
		return Q_ERROR;

	pthread_mutex_lock(&queue->mutex);

	if (queue->tail->data)
	{
		*data = queue->tail->data;
		pthread_mutex_unlock(&queue->mutex);
		return Q_SUCCESS;
	}

	pthread_mutex_unlock(&queue->mutex);
	return Q_ERROR;
}

/* check, if queue is empty
 *
 */
qStatus q_isEmpty(QUEUE *queue)
{
	if (queue->length == 0)
		return Q_EMPTY;

	return Q_SUCCESS;
}

/* return the length of the queue
 *
 */
int q_length(QUEUE *queue)
{
	return queue->length;
}

/* free queue and all nodes
 *
 */
qStatus q_queue_free(QUEUE *queue)
{
	if (!queue)
		return Q_ERROR;

	pthread_mutex_lock(&queue->mutex);

	NODE *tmp = queue->head;
	while(tmp)
	{
		NODE *next = tmp->next;
		q_node_free(tmp);
		tmp = next;
	}
	pthread_mutex_unlock(&queue->mutex);
	pthread_mutex_destroy(&queue->mutex);
	free(queue);

	return Q_SUCCESS;
}

/* free node and allocated data
 *
 */
qStatus q_node_free(NODE *node)
{
	if (!node)
		return Q_ERROR;

	if (node->data)
		free(node->data);

	free(node);

	return Q_SUCCESS;
}


/* do something recursive with the queue's data
 *
 */
qStatus q_recursive(QUEUE *queue, action func)
{
	if (!queue || !func)
		return Q_ERROR;

	pthread_mutex_lock(&queue->mutex);
	NODE *node = queue->head;

	while(node != NULL)
	{
		func(node->data);
		node = node->next;
	}
	pthread_mutex_unlock(&queue->mutex);

	return Q_SUCCESS;
}
