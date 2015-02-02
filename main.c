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

static Queue* queue = NULL;

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

void *run_test(void *param)
{
	int* val = param;

	// Fill queue by pushing elements to the end
	printf("(%d)Start filling.\n", *val);

	// Get tail do something with it's value
	Task* task10 = q_peek_tail(queue);
	if (task10)
		printf("(%d)Head value: %d\n", *val, task10->value);

	// Get head do something with it's value
	Task* task11 = q_peek_head(queue);
	if (task11)
		printf("(%d)Head value: %d\n", *val, task11->value);

	// Pop first element
	printf("(%d)Pop Head (%d): ", *val, q_length(queue));
	q_pop_head(queue);
	q_showQueue(queue);
		usleep (10000);

	// Pop last element
	printf("(%d)Pop Tail (%d): ", *val, q_length(queue));
	q_pop_tail(queue);
	q_showQueue(queue);
		usleep (10040);

	// Push to tail
	Task* task1 = t_init();
	task1->value = 7;
	q_push_tail(queue, task1);
	printf("(%d)Push Tail (%d, %d): ", *val, task1->value, q_length(queue));
	q_showQueue(queue);
		usleep (20000);

	// Push to tail
	Task* task2 = t_init();
	task2->value = 1;
	q_push_tail(queue, task2);
	printf("(%d)Push Tail (%d, %d): ", *val, task2->value, q_length(queue));
	q_showQueue(queue);
		usleep (20000);

	// Wait a bit in the second thread
	if (*val == 1 )
		usleep(100000);

	// Push to tail
	Task* task3 = t_init();
	task3->value = 5;
	q_push_tail(queue, task3);
	printf("(%d)Push Tail (%d, %d): ", *val, task3->value, q_length(queue));
	q_showQueue(queue);
		usleep (20000);

	// Pop last element
	q_pop_tail(queue);
	printf("(%d)Pop Tail (%d): ", *val, q_length(queue));
	q_showQueue(queue);
		usleep (10040);

	// Push to tail
	Task* task4 = t_init();
	task4->value = 3;
	q_push_tail(queue, task4);
	printf("(%d)Push Tail (%d, %d): ", *val, task4->value, q_length(queue));
	q_showQueue(queue);
		usleep (20000);

	// Pop first element
	printf("(%d)Pop Head (%d): ", *val, q_length(queue));
	q_pop_head(queue);
	q_showQueue(queue);
		usleep (10000);

	// Append element to the head
	Task* task5 = t_init();
	task5->value = 28;
	q_push_head(queue, task5);
	printf("(%d)Push Head (28, %d): ", *val, q_length(queue));
	q_showQueue(queue);
		usleep (10435);

	// Get head value as a single task and push it to the tail
	Task* task6 = t_init();
	q_extract_head(queue, task6);
	q_push_tail(queue, task6);
	printf("(%d)Get head -> tail (%d): ", *val, q_length(queue));
	q_showQueue(queue);
		usleep (10337);

	// Get tail value as a single task and push it to the head
	Task* task7 = t_init();
	q_extract_tail(queue, task7);
	q_push_head(queue, task7);
	printf("(%d)Get tail -> head (%d): ", *val, q_length(queue));
	q_showQueue(queue);
		usleep (20000);

	// Get tail do something with it's value
	Task* task8 = q_peek_tail(queue);
	printf("(%d)Tail value: %d\n", *val, task8->value);

	// Get tail do something with it's value
	Task* task9 = q_peek_head(queue);
	printf("(%d)Head value: %d\n", *val, task9->value);

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
		queue = q_init();

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

	q_free(queue);

	return 0;
}
