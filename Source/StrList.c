#include <stdlib.h>

typedef struct list_P
{
	char c;
	struct list_P * next;
} list;

typedef struct list_A
{
	list * str;
	struct list_A * next;
} str_list;

typedef struct mat_A
{
	str_list * arr;
	struct mat_A * next;
} str_mat;

/*CURRENTLY IMPLEMENTING LISTS OF DYNAMIC STRINGS*/

void list_print(list * );
void list_debug(list *);
list * list_get();
list * list_push(list *, char);
list * list_append(list *, char *);
list * list_appendCh(list *, char);
list * list_appendL(list *, list *);
int list_compare(list *, char *);
void list_remove(list *, int);
void list_free(list *);
void list_swap(list *, int, int);
int str_to_int(char val[]);
char * toStr(list *);
int listCount(list *, char);
int list_len(list *);
int list_smartLen(list *);

str_list * strl_alloc(str_list *);
void strl_free(str_list *, int);
void strl_print(str_list *);

str_mat * strm_alloc(str_mat *);
str_mat * strm_free(str_mat *, int);
str_mat * strm_pop(str_mat *, int);
void strm_print(str_mat *);

void list_print(list * head)
{
	for ( ; head; head = head -> next)
		printf("%c",head->c);
	printf("\n");
}

void list_debug(list * head)
{
	for ( ; head; head = head -> next)
		printf("%d ",head->c);
	printf("\n");
}

list * list_get()
{
	list * head;
	list * seq;
	int check;
	int i;
	char val[6];

	if (head = malloc(sizeof(list)))
	{
		printf("Write a char, or 'q' to stop\n");
		
		seq = head;

		scanf("%s",val);
		
		do
		{
			check = 0;
			
			seq -> c = str_to_int(val);
			
			scanf("%s",val);

			if (val[0] != 'q')
			{
				if (seq -> next = malloc(sizeof(list)))
				{
					seq = seq -> next;
					check = 1;
				}
				else
					printf("ALLOCATION ERROR!\n");
			}
			
		} while(check);
		

	}
	else
		printf("ALLOCATION ERROR!\n");

	return head;
}

list * list_push(list * head, char val)
{
	list * tmp;

	if (tmp = malloc(sizeof(list)))
	{
		tmp->c = val;
		tmp->next = head;
		head = tmp;
	}
	else
		printf("ALLOCATION ERROR!\n");

	return head;
}

list * list_append(list * head, char val[])
{
	list * tmp;
	int i;

	if (head)
	{
		tmp = head;
		
		while (tmp->next)
			tmp = tmp->next;
		
		for (i = 0; val[i] != '\0'; i++)
		{
			if (tmp->next = malloc(sizeof(list)))
			{
				tmp->next->c = val[i];
				tmp = tmp->next;
			}
			else
			{
				printf("ALLOCATION ERROR!\n");
				break;
			}
		}
		tmp-> next = NULL;
		return head;
	}
	
	if (head = malloc(sizeof(list)))
	{
		head -> c = val[0];
		//printf("IN FUNCTION APPENDING %c\n",val[0]);
		tmp = head;
		for (i = 1; val[i] != '\0'; i++)
		{
			if (tmp->next = malloc(sizeof(list)))
			{
				tmp->next->c = val[i];
				//printf("IN FUNCTION APPENDING %c\n",val[i]);
				tmp = tmp->next;
			}
			else
			{
				printf("ALLOCATION ERROR\n");
				break;
			}
		}
	}

	else
		printf("ALLOCATION ERROR\n");
	tmp->next = NULL;
	return head;
}

list * list_appendCh(list * head, char val)
{
	list * tmp;
		
	if (head)
	{
		tmp = head;
		
		while (tmp->next)
			tmp = tmp->next;
		
		if (tmp->next = malloc(sizeof(list)))
		{
			tmp->next->c = val;
		}
		else
			printf("ALLOCATION ERROR!\n");
		tmp -> next ->  next = NULL;
		return head;
	}

	
	if (head = malloc(sizeof(list)))
		head -> c = val;

	else
		printf("ALLOCATION ERROR\n");
	head -> next = NULL;
	
	return head;
}
/*Appends another list*/
list * list_appendL(list * head, list * lis)
{
	list * tmp;

	if (head)
	{
		tmp = head;
		while (tmp -> next)
			tmp = tmp -> next;
		
		tmp -> next = lis;
		
		return head;
	}
	
	head = lis;

	return head;
}

int list_compare(list * lis, char str[])
{
	int len;
	int i;

	for (len = 0; str[len] != '\0'; len++)
		;
	
	if (len == list_smartLen(lis))
	{
		for (i = 0; i < len; i++, lis = lis -> next)
		{
			if (lis -> c != str[i])
				return 0;
		}
		return 1;
	}
	return 0;
}


void list_remove(list * head, int index)
{
	list * tmp;
	int i;

	tmp = head;

	i = 0;

	while (i < index)
	{
		tmp = tmp->next;
		i++;
	}

	while (tmp->next)
	{
		tmp->c = tmp->next->c;
		if (tmp->next->next)
			tmp = tmp->next;
		else
			tmp->next = NULL;
	}

	free(tmp->next);
	//Potrebbe convenire settare la coda di lista a NULL dopo aver liberato la relativa memoria
	//tmp->next = NULL;

	list_print(head);
}
//MAYBE IT DOESN'T WORK
void list_free(list * head)
{
	list * tmp;
	
	tmp = head;
	
	for (head = head -> next; head; head = head -> next)
	{
		free(tmp);
		tmp = head;
	}
	free(head);
}

void list_swap(list * head, int ind1, int ind2)
{
	list * tmp1;
	list * tmp2;
	int tmpval;
	int i;

	tmp1 = head;
	tmp2 = head;

	for (i = 0; i < ind1; i++)
		tmp1 = tmp1 -> next;
	for (i = 0; i < ind2; i++)
		tmp2 = tmp2 -> next;

	tmpval = tmp1 -> c;
	tmp1 -> c = tmp2 -> c;
	tmp2 -> c = tmpval;
}
//CONVERTS A CHAR LIST INTO A STRING AND DELETES THE LIST
char * toStr(list * head)
{
	list * tmp;
	char * nu_str;
	int i;
	
	if (nu_str = malloc(sizeof(char)*(list_len(head)+1)))
	{
		for (i = 0, tmp = head; tmp; tmp = tmp -> next, i++)	
			nu_str[i] = tmp -> c;
		nu_str[i] = '\0';
	}
	
	else
		printf("ALLOCATION ERROR\n");
	
	list_free(head);

	return nu_str;
}	

int listCount(list * head, char ch)
{
	int count;
	
	count = 0;

	for ( ; head; head = head->next)
		if (head -> c == ch)
			count++;
	return count;
}

int list_len(list * head)
{
	int len;

	for (len = 0; head; head = head -> next, len++)
		;
	return len;
}

int list_smartLen(list * head)
{
	int len;

	for (len = 0; head; head = head -> next)
		if (head -> c != ' ')
			len++;
	return len;
}

int str_to_int(char val[])
{
	int len;
	int tmp;
	int mult;
	int i;

	for (i = 0, len = 0; val[i] != '\0'; i++)
	{
		len++;
	}

	tmp = 0;
	mult = 1;

	for (i = len-1; i >= 0; i--)
	{
		tmp = tmp + (val[i] - '0') * mult;
		mult = mult * 10;
	}

	return tmp;
}
/*ALLOCATES EMPTY SPACE n TIMES, APPENDS IF ALREADY ALLOCATED*/
str_list * strl_alloc(str_list * head)
{
	str_list * tmp;
	int i;

	tmp = head;
	
	if (head)
	{	
		while(tmp->next)
			tmp = tmp -> next;

		if (tmp->next = malloc(sizeof(str_list)))
			;
		else
			printf("ALLOCATION ERROR\n");

		tmp -> next -> next =  NULL;
		return tmp -> next;
	}
	if (head = malloc(sizeof(str_list)))
		head -> next = NULL;
	
	else
		printf("ALLOCATION ERROR\n");
	return head;
}
/*FREES A LIST OF DYNAMIC STRINGS AND STRINGS THEMSELVES*/
void strl_free(str_list * head, int freeStrings)
{
	str_list * tmp;
	tmp = head;
	head = head -> next;
	if (freeStrings)
	{
	
		while (head)
		{
			list_free(tmp -> str);
			free(tmp);
			tmp = head;
			head = head -> next;
		}
		list_free(tmp -> str);
	}
	else
	{
		while (head)
		{	
			free(tmp);
			tmp = head;
			head = head -> next;
		}
	}
	free(tmp);
}

void strl_print(str_list * head)
{
	printf("---\n");
	while (head)
	{
		list_print(head -> str);
		head = head -> next;
	}
	printf("---\n");
}

//Allocates empty space in an array of string arrays
str_mat * strm_alloc(str_mat * head)
{
	str_mat * tmp;

	if (head)
	{
		tmp = head;
		while (tmp -> next)
			tmp = tmp -> next;
		if (tmp -> next = malloc(sizeof(str_mat)))
			;
		else
			printf("ALLOCATION ERROR\n");
		tmp -> next -> next = NULL;
		return head;
	}

	if (head = malloc(sizeof(str_mat)))
		;
	else
		printf("ALLOCATION ERROR\n");
	head -> next = NULL;
	return head;
}

str_mat * strm_free(str_mat * head, int freeStrings)
{
	str_mat * tmp1;
	str_mat * tmp2;
	tmp1 = head;
	tmp2 = head -> next;

	if (!head)
	{
		strl_free(tmp1 -> arr, freeStrings);
		free(tmp1);
		return NULL;
	}

	while (tmp2 -> next)
	{
		strl_free(tmp1 -> arr, freeStrings);
		free(tmp1);
		tmp1 = tmp2;
		tmp2 = tmp2 -> next;
	}
	strl_free(tmp1 -> arr, freeStrings);		
	free(tmp1);
	strl_free(tmp1 -> next -> arr, freeStrings);
	free(tmp1 -> next);
	tmp1 -> next = NULL;
	return head;
}
/*Deallocates last array of the list, optionally deallocates its strings also*/
str_mat * strm_pop(str_mat * head, int freeStrings)
{
	str_mat * tmp;
	tmp = head;
		
	if (!tmp -> next)
	{
		strl_free(head -> arr, freeStrings);
		free(head);
		return NULL;
	}
	while (tmp -> next -> next)
		tmp = tmp -> next;
	strl_free(tmp -> next -> arr, freeStrings);
	free(tmp -> next);
	tmp -> next = NULL;

	return head;
}

void strm_print(str_mat * head)
{
	str_list * tmpArr;
	int ind = 0;
	while (head)
	{
		printf("ARRAY %d:\n", ind);
		tmpArr = head -> arr;
		while (tmpArr)
		{
			list_print(tmpArr -> str);
			tmpArr = tmpArr -> next;
		}
		head = head -> next;
		ind++;
	}
}
