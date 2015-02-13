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
#include <time.h>
#include "queue.h"

static QUEUE *queue;

typedef struct example {
	long double value;
}example_t;

void *listData(void *data)
{
	example_t *t = (example_t *)data;
	printf("%Lf ", t->value);
}

int compare_func(void *a, void *b)
{
	example_t *data_a = a;
	example_t *data_b = b;
	int ret = 0;

	if (data_a->value < data_b->value)
		ret = -1;
	else if (data_a->value > data_b->value)
		ret = 1;

	return ret;
}

void *showQueue(void *data)
{
	printf("Queue (L%d): ", q_length(queue));
	q_recursive(queue, listData);
	printf("\n");
}

void *check_if_empty(QUEUE *queue)
{
	if (q_isEmpty(queue))
		printf("Queue is empty...\n");
	else
		printf("Queue is filled...\n");
}

void *run_test(void *param)
{
	int* val = param;

	printf("(%d)Start filling.\n", *val);
	check_if_empty(queue);

	// Pop first element
	example_t *data62;
	if(!q_pop_head(queue, (void *)&data62))
		printf("(%d)Pop head, saved to (2): %Lf\n", *val, data62->value);
	else
		printf("(%d)Pop head: Nothing to pop\n", *val);

	// Peek last element
	example_t *data63;
	if(!q_peek_tail(queue, (void *)&data63))
		printf("(%d)Tail value: %Lf\n", *val, data63->value);
	else
		printf("(%d)Peek head: Nothing to peek\n", *val);


	// Push to tail
	example_t *data1 = (example_t *)calloc(1, sizeof(example_t));
	data1->value = 2;
	if(!q_push_tail(queue, data1))
		printf("(%d)Push %Lf to tail\n", *val, data1->value);
	else
		free(data1);

	showQueue(queue);

	// Push to tail
	example_t *data2 = (example_t *)calloc(1, sizeof(example_t));
	data2->value = 4;
	if(!q_push_tail(queue, data2))
		printf("(%d)Push %Lf to tail\n", *val, data2->value);
	else
		free(data2);

	showQueue(queue);

	// Pop first element
	example_t *data41;
	if(!q_pop_head(queue, (void *)&data41))
		printf("(%d)Pop head, saved to (2): %Lf\n", *val, data41->value);
	else
		printf("(%d)No head value\n", *val);

	showQueue(queue);

	// Push to head
	example_t *data3 = (example_t *)calloc(1, sizeof(example_t));
	data3->value = 5;
	if(!q_push_head(queue, data3))
		printf("(%d)Push %Lf to head\n", *val, data3->value);
	else
		free(data3);

	showQueue(queue);

	example_t *data50;
	if(!q_peek_head(queue, (void *)&data50))
		printf("(%d)Head value: V%Lf\n", *val, data50->value);

	// Create new data with head value, pop the head and do something with it
	example_t *data10 = (example_t *)calloc(1, sizeof(example_t));
	if (!q_extract_head(queue, data10, sizeof(example_t)))
		printf("(%d)Extract head value to (1): %Lf\n", *val, data10->value);
	else
		free(data10);

	// Push to head
	example_t *data4 = (example_t *)calloc(1, sizeof(example_t));
	data4->value = 3;
	if(!q_push_head(queue, data4))
		printf("(%d)Push %Lf to head\n", *val, data4->value);
	else
		free(data4);

	showQueue(queue);

	// Create new data with head value and do something with it
	example_t *data8 = (example_t *)calloc(1, sizeof(example_t));
	if (!q_extract_head(queue, data8, sizeof(example_t)))
	{
		printf("(%d)Extract head value: %Lf\n", *val, data8->value);
		// Push previous to the end
		if(!q_push_tail(queue, data8))
			printf("(%d)Push head (%Lf) to tail\n", *val, data8->value);
		else
			free(data8);
	}
	else
		free(data8);

	showQueue(queue);

	// Push to tail
	example_t *data5 = (example_t *)calloc(1, sizeof(example_t));
	data5->value = 6;
	if(!q_push_tail(queue, data5))
		printf("(%d)Push %Lf to tail\n", *val, data5->value);
	else
		free(data5);

	showQueue(queue);

	// Create new data with tail value
	example_t *data9 = (example_t *)calloc(1, sizeof(example_t));
	example_t *data42 = NULL;

	if (!q_extract_tail(queue, data9, sizeof(example_t)))
	{
		printf("(%d)Extract tail value: %Lf\n", *val, data9->value);
		// Push previous to the end

		// Pop last element
		example_t *data44;
		if (!q_pop_tail(queue, (void *)&data44))
			printf("(%d)Pop tail\n", *val);
		else
			printf("(%d)No tail value!\n", *val);
		free(data44);

		showQueue(queue);

		// Pop last element
		if (!q_pop_tail(queue, (void *)&data42))
			printf("(%d)Pop tail, saved to (3): %Lf\n", *val, data42->value);
		else
			printf("(%d)No tail value!\n", *val);

		showQueue(queue);

		if(!q_push_tail(queue, data9))
			printf("(%d)Push %Lf to tail\n", *val, data9->value);
		else
			free(data9);

	}
	else
		free (data9);

	showQueue(queue);

	// Insert Sorted
	example_t *data11 = (example_t *)calloc(1, sizeof(example_t));
	data11->value = 7;
	if(!q_insert_sorted(queue, data11, compare_func))
		printf("(%d)Insert %Lf sorted\n", *val, data11->value);

	showQueue(queue);

	// Search for head and return it
	example_t *data30;
	if(!q_peek_head(queue, (void *)&data30))
		printf("(%d)Head value: %Lf\n", *val, data30->value);

	// Search for tail and return it
	example_t *data31;
	if(!q_peek_tail(queue, (void *)&data31))
		printf("(%d)Tail value: %Lf\n", *val, data31->value);

	if (data10)
	{
		printf("(%d)Extracted value outside queue(1): V%Lf\n", *val, data10->value);
		free(data10);
	}
	if (data41)
	{
		printf("(%d)Extracted value outside queue(2): V%Lf\n", *val, data41->value);
		free(data41);
	}
	if (data42)
	{
		printf("(%d)Extracted value outside queue(3): V%Lf\n", *val, data42->value);
		free(data42);
	}

	check_if_empty(queue);

	return 0;
}

void* test_thread()
{
	int a = 0;
	int b = 1;
	int c = 2;

	pthread_t thread1, thread2, thread3;
	pthread_create(&thread1, NULL, run_test, &a);
	printf("(%d)Thread created\n", a);

	pthread_create(&thread2, NULL, run_test, &b);
	printf("(%d)Thread created\n", b);

	pthread_create(&thread3, NULL, run_test, &c);
	printf("(%d)Thread created\n", b);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
}

void* test_single()
{
	int a = 0;
	run_test(&a);
}

int main(int argc, char *argv[])
{
	if (!queue)
		queue = q_queue_init();

	if (argc == 2)
	{
		if (strcmp(argv[1], "test") == 0)
		{
			printf("Queue Test\n");
			test_single();
		}
		else if (strcmp(argv[1], "thread") == 0)
		{
			printf("Thread Test\n");
			test_thread();
		}
		else
		{
			printf("Wrong arguments:\n");
			printf("%s test\t\tRun test\n", argv[0]);
			printf("%s thread\t\tRun thread test\n", argv[0]);
			return 0;
		}
	}
	else if (argc == 1)
	{
		printf("Queue Test\n");
		test_single();
	}
	else
	{
		printf("Wrong arguments:\n");
		printf("%s test\t\tRun test\n", argv[0]);
		printf("%s thread\t\tRun thread test\n", argv[0]);
		return 0;
	}

	q_queue_free(queue);

	return 0;
}
