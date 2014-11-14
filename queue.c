#include "queue.h"

Queue* q_init(void)
{
	Queue* queue = (Queue*) calloc(sizeof(Queue), 1);
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}

TaskData* td_init(void)
{
	TaskData* taskdata = (TaskData*) calloc(sizeof(TaskData), 1);
	return taskdata;
}

Task* t_init(void)
{
	Task* task = (Task*) calloc(sizeof(Task), 1);
	task->prev = NULL;
	task->next = NULL;
	task->data = td_init();
	return task;
}

void q_push_head (Queue* queue, Task* task)
{
	Task* tmp = (Task*) malloc(sizeof(Task));
	tmp = task;

	if(queue->head == NULL)
		queue->tail = tmp;
	else
		queue->head->prev = tmp;

	tmp->next = queue->head;
	queue->head = tmp;

}

void q_push_tail(Queue* queue, Task* task)
{
	Task* tmp = (Task*) malloc(sizeof(Task));
	tmp = task;

	if(queue->head == NULL)
		queue->head = tmp;
	else
		queue->tail->next = tmp;

	tmp->prev = queue->tail;
	queue->tail = tmp;

}

void q_pop_head(Queue* queue)
{
	Task* tmp;
	tmp = queue->head;
	queue->head = queue->head->next;

	if(queue->head == NULL)
		queue->tail = NULL;
	else
		queue->head->prev = NULL;

	if(tmp != NULL)
	{
		tmp->next = NULL;
		free(tmp);
	}
}

void q_pop_tail(Queue* queue)
{
	Task* tmp;
	tmp = queue->tail;
	queue->tail = queue->tail->prev;

	if(queue->tail == NULL)
		queue->head = NULL;
	else
		queue->tail->next = NULL;

	if(tmp != NULL)
	{
		tmp->prev = NULL;
		free(tmp);
	}
}

void q_timeout_pop(Queue* queue, int timeout)
{
}

void q_insert_sorted(Queue* queue, Task* task, compare_func cmp)
{
	// This only works, if the queue is already sorted
	// Otherwise it compares the task with the values 
	// starting from the head

	Queue* q_tmp = queue;
	signed int comp;

	if (queue->head == 0)
	{
		Queue* q_new = q_init();
		q_push_tail(q_new, task);
		*q_tmp = *q_new;
		return;
	}

	Queue* q_tmphead = q_init();

	comp = (*cmp)(task, q_tmp->head);
	while ((q_tmp->head) && (comp > 0))
	{
		q_push_tail(q_tmphead, q_tmp->head);
		if (q_tmp->head->next) {
			q_tmp->head = q_tmp->head->next;
			comp = (*cmp)(task, q_tmp->head);
		}
		else
			q_tmp->head = NULL;
	}

	q_push_tail(q_tmphead, task);

	while ((q_tmp->head))
	{
		q_push_tail(q_tmphead, q_tmp->head);
		q_tmp->head = q_tmp->head->next;
	}

	*q_tmp = *q_tmphead;
}

Task* q_peek_head(Queue* queue)
{
	Task* t = queue->head;
	Task* tmp = t_init();

	if(t == NULL)
		return NULL;
	else
	{
		tmp->data = t->data;
		return tmp;
	}
}

Task* q_peek_tail(Queue* queue)
{
	Task* t = queue->tail;
	Task* tmp = t_init();
	if(t == NULL)
		return NULL;
	else
		tmp->data = t->data;
		return tmp;
}

int q_isEmpty(Queue* queue)
{
    int ret = 0;

    if(queue->head == NULL && queue->tail == NULL)
	ret = 1;

    return ret;
}

void q_showQueue(Queue* queue)
{	Task* help = queue->head;

    while(help != NULL)
    {
        printf("%d ", help->data->value);
        help = help->next;
    }

    printf("\n");
}

signed int sort_insert(Task* a, Task* b)
{
    Task* task_a = a;
    Task* task_b = b;

    int val_a = task_a->data->value;
    int val_b = task_b->data->value;

    if (val_a < val_b)
	return -1;
    else if (val_a > val_b)
	return 1;
    else
	return 0;
}

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

/*
    // Fill queue by pushing elements to the head
    q_push_head(queue, task1);
    q_push_head(queue, task2);
    q_push_head(queue, task3);
    q_push_head(queue, task4);
*/

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
    printf("Insert sorted: ");
    q_showQueue(queue);

    // Pop last element
    q_pop_tail(queue);
    printf("Pop Tail: ");
    q_showQueue(queue);

    // Append element to the tail
    Task* task6 = t_init();
    task6->data->value = 4;
    q_push_tail(queue, task6);
    printf("Push Tail: ");
    q_showQueue(queue);

    // Append element to the head
    Task* task7 = t_init();
    task7->data->value = 28;
    q_push_head(queue, task7);
    printf("Push Head: ");
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
