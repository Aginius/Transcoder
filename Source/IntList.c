typedef struct list_I
{
	int n;
	struct list_I * next;
} intList;

void intL_print(intList * );
intList * intL_append(intList *, int);
void intL_free(intList *);
int intL_getLast(intList *);
intList * intL_pop(intList *);

void intL_print(intList * head)
{
	for ( ; head; head = head -> next)
		printf("%d",head->n);
	printf("\n");
}

intList * intL_append(intList * head, int val)
{
	intList * tmp;
	int i;

	if (head)
	{
		tmp = head;
		
		while (tmp->next)
			tmp = tmp->next;
			
		if (tmp->next = malloc(sizeof(list)))
		{
			tmp->next->n = val;
			tmp = tmp->next;
		}
		else
			printf("ALLOCATION ERROR!\n");
	
		tmp-> next = NULL;
		return head;
	}
	
	if (head = malloc(sizeof(list)))
	{
		head -> n = val;
		tmp = head;
	}

	else
		printf("ALLOCATION ERROR\n");
	tmp->next = NULL;
	return head;
}

//MAYBE IT DOESN'T WORK
void intL_free(intList * head)
{
	intList * tmp;
	
	tmp = head;
	
	for (head = head -> next; head; head = head -> next)
	{
		free(tmp);
		tmp = head;
	}
	free(head);
}

int intL_getLast(intList * head)
{
	while (head -> next)
		head = head -> next;
	return head -> n;
}

intList * intL_pop(intList * head)
{
	if (!head -> next)
	{
		free(head);
		return NULL;
	}
	while (head -> next -> next)
		head = head -> next;
	
	free(head -> next);
	head -> next = NULL;
	return head;	
}
