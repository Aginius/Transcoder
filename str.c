#include <stdio.h>
#include <stdlib.h>

int strCount(char *, char *);
int strCompare(char *, char *, int);
int strCompareTail(char *, char *);
char * appendString(char *, char *);
char * init(int);
void appendChar(char *, char);
int ch_index(char *, char, int);
char checkLast(char *);
void emptyStr(char *);
int strLen(char *);
void noSpace(char *);
/*
int main (int argc, char * argv[])
{
	int i;
	printf("%s\n",appendString("yeah","oh"));

	for (i = 0; appendString("yeah","oh")[i] != '\0'; i++)
		if (i == 100)
			break;
	printf("%d\n",i);

	return 0;
}
*/
//Compare the first string from the given index with the second string
int strCompare(char str[], char word[], int index)
{
	int i,j;

	for (i = index, j = 0; word[j] != '\0'; i++, j++)
	{
		if (str[i] != word[j])
			return 0;
	}	
	return 1;
}
//Compare the end of the first string with the second string
int strCompareTail(char str[], char word[])
{
	int i,j;
	int len1;
	int len2;
	
	for (len1 = 0; str[len1] != '\0'; len1++)
		;
	for (len2 = 0; word[len2] != '\0'; len2++)
		;
	for (i = len1-len2, j = 0; word[j] != '\0'; i++, j++)
	{
		if (str[i] != word[j])
			return 0;
	}
	return 1;
}

//Checks how many times a given word is in a given string
int strCount(char str[], char word[])
{
	int count;
	int i,j,h;
	
	count = 0;

	for (i = 0; str[i] != '\0'; i++)
	{
		for (j = i, h = 0; str[j] == word[h] && word[h] != '\0'; h++, j++)
			;
		if (word[h] == '\0')
			count++;
	}
	return count;
}

char * init(int len)
{
	char * str;

	if (str = malloc(sizeof(char)*len))
		;
	else
		printf("ALLOCATION ERROR\n");
	return str;
}

int charCount(char str[], char ch)
{
	int count;
	int i;

	count = 0;

	for (i = 0; str[i] != '\0'; i++)
		if (str[i] == ch)
			count++;
	return count;
}

char * appendString(char str[], char word[])
{
	char * nu_str = NULL;
	int i,j;

	for (i = 0; str[i] != '\0'; i++)
		;
	for (j = 0; word[j] != '\0'; j++)
		;
	if (nu_str = malloc(sizeof(char)*(i+j)))
	{
		for (i = 0; str[i] != '\0'; i++)
			nu_str[i] = str[i];
		for (j = 0; word[j] != '\0'; i++, j++)
			nu_str[i] = word[j];
		//nu_str[i] = '\0';
	}
	else
		printf("ALLOCATION ERROR\n");
	return nu_str;
}
//UNSTABLE, TO USE ONLY ON STATIC ARRAYS WITH SOME FREE SPACE LEFT
void appendChar(char str[], char ch)
{
	int i;

	for (i = 0; str[i] != '\0'; i++)
		;
	str[i] = ch;
}

int ch_index(char str[], char el, int occurance)
{
	int i;

	for (i = 0; str[i] != '\0'; i++)
		if (str[i] == el)
		{
			occurance--;
			if (occurance == 0)
				return i;
		}
	return -1;
}
//Returns the last character of a string
char checkLast(char str[])
{
	int i;
	char last;

	for (i = 0; str[i] != '\0'; i++)
	{
		if (str[i] != ' ')
			last = str[i];
	}
	return last;
}

void emptyStr(char str[])
{
	int i;

	for (i = 0; str[i] != '\0'; i++)
		str[i] = ' ';
	str[0] = '\0';
}

int strLen(char str[])
{
	int i;

	for (i = 0; str[i] != '\0'; i++)
		;
	return i;
}
//RETURNS THE FIRST CHAR IN A STRING THAT'S NOT A SPACE AFTER THE GIVEN INDEX
char nextChar(char str[], int index)
{
	int i;

	for (i = index+1; str[i] != '\0'; i++)
	{
		if (str[i] != ' ')
			return str[i];
	}
	return ' ';
}

void noSpace(char str[])
{
	char * tmp;
	int len;
	int check;
	int count;
	int i;

	len = strLen(str);
	check = 0;
	count = 0;

	if (tmp = malloc(sizeof(char)*(len+1)))
	{
		for (i = 0; i < len; i++)
		{
			if (str[i] != ' ' || check)
			{
				tmp[i] = str[i];
				check = 1;
				count++;
			}
		}
		for (i = 0; i < count; i++)
			str[i] = tmp[i];
		//THIS MIGHT CAUSE AN ERROR
		str[i] = '\0';
		free(tmp);
	}
}
