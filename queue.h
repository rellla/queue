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

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void q_extract_head(Queue* queue, Task* task);
void q_extract_tail(Queue* queue, Task* task);
Task* q_peek_head(Queue* queue);
Task* q_peek_tail(Queue* queue);

int q_isEmpty(Queue* queue);
int q_length(Queue* queue);

void q_free(Queue* queue);
void t_free(Task* task);
