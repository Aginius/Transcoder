#include <stdio.h>
#include <stdlib.h>
#include "StrList.c"
#include "IntList.c"
#include "str.c"

int smartCount(list *, char);
int noPseudo(char *, int);
void tab_line(FILE *, int);
int isIncrement(char *);
void Classify(char *, int, int *, int *, int *, int *, int *, int *, int *, int *, int *);


//Counts the character if not in strings
int smartCount(list * head, char ch)
{
	int count;
	int inStr;
	int inChar;

	//int DEBUG_IND = 0;

	count = 0;
	inStr = 0;
	inChar = 0;

	for ( ; head; head = head->next)
	{
		if (head -> c == ch && !inStr && !inChar)
			count++;
		else if (head -> c == 39 && !inStr)
		{
			if (inChar)
				inChar = 0;
			else
				inChar = 1;
		}
		else if (head -> c == '"' && !inChar)
		{
			if (inStr)
				inStr = 0;
			else
				inStr = 1;
		}
		//DEBUG_IND++;
	}
	return count;
}
//Returns the index of the string right after the pseudo type
int noPseudo(char str[], int ind)
{
	if (str[ind] == '{' || str[ind] == '}' || str[ind] == '/' || strCompare(str, "return", ind) || strCompare(str, "MAIN", ind) || strCompare(str, "IMPORT", ind))
		return ind;
	for ( ; str[ind] != ' '; ind++)
		;
	ind++;
	return ind;
}

void tab_line(FILE * fptr, int tabs)
{
	int i;

	for (i = 0; i < tabs; i++)
		fprintf(fptr, "\t");
}

int isIncrement(char str[])
{
	int i;
	for (i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == '+' && str[i+1] == '+')
			return 1;
		else if (str[i] == '-' && str[i+1] == '-')
			return 2;
	}
	return 0;
}

void Classify(char str[], int start,  int * variable, int * function, int * condition, int * declaration, int * initialization, int * call, int * intType, int * charType, int * voidType)
{
	list * type = NULL;
	int compare;
	int i;

	compare = 1;

	if (str[start] == '{' || str[start] == '}')
		return;

	for (i = start; str[i] != ' ' && str[i] != '\0'; i++)
	{
		if (str[i] == '.')
			compare = 1;
		else if (compare)
		{
			//printf("Index: %d\n", i);
			if (strCompare(str, "V", i))
				*variable = 1;
			else if (strCompare(str, "F", i))
				*function = 1;
			else if (strCompare(str, "Cl", i))
				*call = 1;
			else if (strCompare(str, "D", i))
				*declaration = 1;
			else if (strCompare(str, "I", i))
				*initialization = 1;
			else if (strCompare(str, "C", i))
				*condition = 1;
			else if (strCompare(str, "i", i))
				*intType = 1;
			else if (strCompare(str, "c", i))
				*charType = 1;
			else if (strCompare(str, "v", i))
				*voidType = 1;
			else
				//printf("NOT RECOGNIZED\n");
			compare = 0;
		}
	}
}

