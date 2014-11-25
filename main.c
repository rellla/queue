#include "queue.h"

int main()
{
	Queue* queue = q_init();

	Task* task1 = t_init();
	Task* task2 = t_init();
	Task* task3 = t_init();
	Task* task4 = t_init();

	task1->data->value = 1;
	task2->data->value = 3;
	task3->data->value = 5;
	task4->data->value = 7;

	// Fill queue by pushing elements sorted
	q_insert_sorted(queue, task2, sort_insert);
	q_insert_sorted(queue, task4, sort_insert);
	q_insert_sorted(queue, task3, sort_insert);
	q_insert_sorted(queue, task1, sort_insert);

	printf("Insert sorted: ");
	q_showQueue(queue);

	// Pop first element
	q_pop_head(queue);
	printf("Pop Head: ");
	q_showQueue(queue);

	// Insert element at the right place
	Task* task5 = t_init();
	task5->data->value = 6;
	q_insert_sorted(queue, task5, sort_insert);
	printf("Insert sorted (6): ");
	q_showQueue(queue);

	// Pop last element
	q_pop_tail(queue);
	printf("Pop Tail: ");
	q_showQueue(queue);

	// Append element to the tail
	Task* task6 = t_init();
	task6->data->value = 4;
	q_push_tail(queue, task6);
	printf("Push Tail (4): ");
	q_showQueue(queue);

	// Append element to the head
	Task* task7 = t_init();
	task7->data->value = 28;
	q_push_head(queue, task7);
	printf("Push Head (28): ");
	q_showQueue(queue);

	// Get head value
	Task* task8 = q_peek_head(queue);
	q_push_tail(queue, task8);
	printf("Get head -> tail: ");
	q_showQueue(queue);

	// Get tail value
	Task* task9 = q_peek_tail(queue);
	q_push_head(queue, task9);
	printf("Get tail -> head: ");
	q_showQueue(queue);

	return 0;
}
