#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include "queue.h"

static Queue* queue = NULL;

void *test_queue(void *param)
{
	int* val = param;

	if (!queue)
		queue = q_init();

	Task* task1 = t_init();
	Task* task2 = t_init();
	Task* task3 = t_init();
	Task* task4 = t_init();
/*
	task1->data->value = 1;
	task2->data->value = 3;
	task3->data->value = 5;
	task4->data->value = 7;
*/
	task1->value = 1;
	task2->value = 3;
	task3->value = 5;
	task4->value = 7;

	printf("(%d)Start filling.\n", *val);

	// Fill queue by pushing elements sorted
	q_insert_sorted(queue, task2, sort_insert);	// printf("..\n");
	q_insert_sorted(queue, task4, sort_insert);	// printf("...\n");
	q_insert_sorted(queue, task3, sort_insert);	// printf("....\n");
	q_insert_sorted(queue, task1, sort_insert);	// printf(".....\n");

	if (*val == 1 )
		usleep(100000);
	printf("(%d)Insert sorted: ", *val);
	q_showQueue(queue);
		usleep (20000);

	// Pop first element
	q_pop_head(queue);
	printf("(%d)Pop Head: ", *val);
	q_showQueue(queue);
		usleep (10000);

	// Insert element at the right place
	Task* task5 = t_init();
//	task5->data->value = 6;
	task5->value = 6;
	q_insert_sorted(queue, task5, sort_insert);
	printf("(%d)Insert sorted (6): ", *val);
	q_showQueue(queue);
		usleep (20000);

	// Pop last element
	q_pop_tail(queue);
	printf("(%d)Pop Tail: ", *val);
	q_showQueue(queue);
		usleep (10040);

	// Append element to the tail
	Task* task6 = t_init();
//	task6->data->value = 4;
	task6->value = 4;
	q_push_tail(queue, task6);
	printf("(%d)Push Tail (4): ", *val);
	q_showQueue(queue);
		usleep (13000);

	// Append element to the head
	Task* task7 = t_init();
//	task7->data->value = 28;
	task7->value = 28;
	q_push_head(queue, task7);
	printf("(%d)Push Head (28): ", *val);
	q_showQueue(queue);
		usleep (10435);

	// Get head value
	Task* task8 = q_peek_head(queue);
	q_push_tail(queue, task8);
	printf("(%d)Get head -> tail: ", *val);
	q_showQueue(queue);
		usleep (10337);

	// Get tail value
	Task* task9 = q_peek_tail(queue);
	q_push_head(queue, task9);
	printf("(%d)Get tail -> head: ", *val);
	q_showQueue(queue);
		usleep (20000);

	/* free memory */
	t_free(task1);
	t_free(task2);
	t_free(task3);
	t_free(task4);
	t_free(task5);
	t_free(task6);
	t_free(task7);
	t_free(task8);
	t_free(task9);
	q_free(queue);

	return 0;
}

void* test_thread()
{
	int a = 0;
	int b = 1;

	pthread_t thread1, thread2;
	pthread_create(&thread1, NULL, test_queue, &a);
	printf("(%d)Thread created\n", a);

	pthread_create(&thread2, NULL, test_queue, &b);
	printf("(%d)Thread created\n", b);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
}

int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		if (strcmp(argv[1], "test") == 0)
		{
			printf("Queue Test\n");
			int a = 0;
			test_queue(&a);
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
	else
	{
		printf("Wrong arguments:\n");
		printf("%s test\t\tRun test\n", argv[0]);
		printf("%s thread\t\tRun thread test\n", argv[0]);
		return 0;
	}
	return 0;
}
