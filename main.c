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
	printf("Queue: ");
	q_recursive(queue, listData);
	printf("\n");
}

void *run_test(void *param)
{
	int* val = param;

	printf("(%d)Start filling.\n", *val);

	// Push to tail
	example_t *data1 = (example_t *)calloc(1, sizeof(example_t));
	data1->value = 2;
	q_push_tail(queue, data1);
	printf("(%d)Push Tail (V%Lf, L%d)\n", *val, data1->value, q_length(queue));
	showQueue(queue);

	// Push to tail
	example_t *data2 = (example_t *)calloc(1, sizeof(example_t));
	data2->value = 4;
	q_push_tail(queue, data2);
	printf("(%d)Push Tail (V%Lf, L%d)\n", *val, data2->value, q_length(queue));
	showQueue(queue);

	// Push to head
	example_t *data3 = (example_t *)calloc(1, sizeof(example_t));
	data3->value = 5;
	q_push_head(queue, data3);
	printf("(%d)Push Head (V%d, L%d)\n", *val, data3->value, q_length(queue));
	showQueue(queue);

	// Search for head and return it
	example_t *data40 = q_peek_head(queue);
	if (data40)
		printf("(%d)Head value: V%Lf\n", *val, data40->value);

	// Create new data with head value, pop the head and do something with it
	example_t *data10 = (example_t *)calloc(1, sizeof(example_t));
	q_extract_head(queue, data10, sizeof(example_t));
	printf("Extract Head Value: V%Lf\n", data10->value);

	// Pop first element
	q_pop_head(queue);
	printf("(%d)Pop Head (L%d)\n", *val, q_length(queue));
	showQueue(queue);

	// Push to head
	example_t *data4 = (example_t *)calloc(1, sizeof(example_t));
	data4->value = 3;
	q_push_head(queue, data4);
	printf("(%d)Push Head (V%Lf, L%d)\n", *val, data4->value, q_length(queue));
	showQueue(queue);

	// Create new data with head value, pop the head and do something with it
	example_t *data8 = (example_t *)calloc(1, sizeof(example_t));
	q_extract_head(queue, data8, sizeof(example_t));
	printf("Extract Head Value: V%Lf\n", data8->value);
	q_pop_head(queue);
	// Push previous to the end
	q_push_tail(queue, data8);
	printf("(%d)Push Tail (V%Lf, L%d)\n", *val, data8->value, q_length(queue));
	showQueue(queue);

	// Push to tail
	example_t *data5 = (example_t *)calloc(1, sizeof(example_t));
	data5->value = 6;
	q_push_tail(queue, data5);
	printf("(%d)Push Tail (V%Lf, L%d)\n", *val, data5->value, q_length(queue));
	showQueue(queue);

	// Pop last element
	q_pop_tail(queue);
	printf("(%d)Pop Tail (L%d)\n", *val, q_length(queue));
	showQueue(queue);

	// Create new data with tail value
	example_t *data9 = (example_t *)calloc(1, sizeof(example_t));
	q_extract_tail(queue, data9, sizeof(example_t));
	printf("Extract Tail Value: V%Lf\n", data9->value);
	// Push previous to the end
	q_push_tail(queue, data9);
	printf("(%d)Push Tail (V%Lf, L%d)\n", *val, data9->value, q_length(queue));
	showQueue(queue);

	// Insert Sorted
	example_t *data11 = (example_t *)calloc(1, sizeof(example_t));
	data11->value = 1;
	printf("(%d)Insert Sorted (V%Lf, L%d)\n", *val, data11->value, q_length(queue));
	q_insert_sorted(queue, data11, compare_func);
	showQueue(queue);

	// Search for head and return it
	example_t *data6 = q_peek_head(queue);
	if (data6)
		printf("(%d)Head value: V%Lf\n", *val, data6->value);

	// Search for tail and return it
	example_t *data7 = q_peek_tail(queue);
	if (data7)
		printf("(%d)Tail value: V%Lf\n", *val, data7->value);

	free(data10);

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
