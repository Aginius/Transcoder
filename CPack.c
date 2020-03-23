#include <stdio.h>
#include "str.c"
#include "StrList.c"

void scan_C(char *);
char * input_C(char *, int *);
int isFunction(char *);
int isCondition_C(char *);

int main (int argc, char * argv[])
{
	scan_C("abc.c");
	return 0;
}

void scan_C(char fname[])
{
	FILE * input;
	FILE * output;
	char word[20];
	list * line = NULL;
	int openBrCount;
	int closedBrCount;
	int condition;
	int indent;
	int i;

	char * DEBUG_STR;
	
	openBrCount = 0;
	closedBrCount = 0;
	indent = 0;
	
	if (input = fopen(fname, "r"))
	{
		if (output = fopen("output.pc", "w"))
		{
			while (!feof(input))
			{
				fscanf(input, "%s\n", word);	
				
				if (line == NULL)
					noSpace(word);

				line = list_append(line, word);
				
				line = list_appendCh(line, ' ');
				openBrCount = listCount(line, '(');
				closedBrCount = listCount(line, ')');
				printf("APPENDING %s\n",word);
				printf("CURRENT LINE: ");
				list_print(line);
				
				if (checkLast(word) == ';' && openBrCount == closedBrCount || checkLast(word) == '{' || checkLast(word) == '}' || checkLast(word) == ')' && openBrCount == closedBrCount)
				{
					printf("List length: %d\n",list_len(line));
					for (i = 0; i < indent; i++)
						fprintf(output, "\t");
					DEBUG_STR = input_C(toStr(line), &indent);
					fprintf(output, "%s\n", DEBUG_STR);
					printf("Printing: %s\n", DEBUG_STR);				
					openBrCount = 0;
					closedBrCount = 0;

					line = NULL;

					printf("\n");
				}
			}
		}
		else
			printf("FILE WRITING ERROR\n");
	}
	else
		printf("FILE OPENING ERROR\n");
}
char * input_C(char row[], int * indent)
{
	list * output = NULL;
	int functionType;
	int i;

	if (strCompare(row,"if",0))
	{
		output = list_append(output, "C.if ");
		for (i = ch_index(row, '(', 1); i < ch_index(row, ')', charCount(row, ')'))+1; i++)
			output = list_appendCh(output, row[i]);
		return toStr(output);
	}
	else if (strCompare(row,"while",0))
	{
		output = list_append(output, "C.wh ");
		for (i = ch_index(row, '(', 1); i < ch_index(row, ')', charCount(row, ')'))+1; i++)
			output = list_appendCh(output, row[i]);
		return toStr(output);
	}
	else if (strCompare(row,"for",0))
	{
		output = list_append(output, "C.for ");
		for (i = ch_index(row, '(', 1); i < ch_index(row, ')', charCount(row, ')'))+1 && row[i] != '\0'; i++)
		{
			output = list_appendCh(output, row[i]);
		}
		return toStr(output);
	}
	else if (strCompare(row,"int",0))
	{
		if (functionType = isFunction(row))
		{
			if (functionType == 1)
			{
				output = list_append(output, "F.D.i ");
				for (i = 4; row[i] != '('; i++)
					output = list_appendCh(output, row[i]);
				output = list_appendCh(output, ' ');
				for ( ; row[i] != ';'; i++)
					output = list_appendCh(output, row[i]);
				return toStr(output);
			}
			else if (functionType == 2)
			{
				output = list_append(output, "F.I.i ");
				for (i = 4; row[i] != '('; i++)
					output = list_appendCh(output, row[i]);
				output = list_appendCh(output, ' ');
				for ( ; i < ch_index(row, ')', charCount(row, ')'))+1; i++)
					output = list_appendCh(output, row[i]);
				return toStr(output);
			}
			else if (functionType == 3)
			{
				output = list_append(output, "V.D.I.i ");
				for (i = 4; row[i] != '='; i++)
					output = list_appendCh(output, row[i]);
				output = list_append(output, "= F.Cl ");
				for (i = i+1 ; row[i] != ';'; i++)
					output = list_appendCh(output, row[i]);
				return toStr(output);
			}
		}
		else
		{
			if (charCount(row, '='))
				output = list_append(output, "V.D.I.i ");
			else
				output = list_append(output, "V.D.i ");
			for (i = 4; row[i] != ';'; i++)
				output = list_appendCh(output, row[i]);
			return toStr(output);
		}
	}
	else if (strCompare(row,"char",0))
	{
		if (functionType = isFunction(row))
		{
			if (functionType == 1)
			{
				output = list_append(output, "F.D.c ");
				for (i = 5; row[i] != '('; i++)
					output = list_appendCh(output, row[i]);
				output = list_appendCh(output, ' ');
				for ( ; row[i] != ';'; i++)
					output = list_appendCh(output, row[i]);
				return toStr(output);
			}
			else if (functionType == 2)
			{
				output = list_append(output, "F.I.c ");
				for (i = 5; row[i] != '('; i++)
					output = list_appendCh(output, row[i]);
				output = list_appendCh(output, ' ');
				for ( ; i < ch_index(row, ')', charCount(row, ')'))+1; i++)
					output = list_appendCh(output, row[i]);
				return toStr(output);
			}
			else if (functionType == 3)
			{
				output = list_append(output, "V.D.I.c ");
				for (i = 5; row[i] != '='; i++)
					output = list_appendCh(output, row[i]);
				output = list_append(output, "= F.Cl ");
				for (i = i+1 ; row[i] != ';'; i++)
					output = list_appendCh(output, row[i]);
				return toStr(output);	
			}
		}
		else
		{
			if (charCount(row, '='))
				output = list_append(output, "V.D.I.c ");
			else
				output = list_append(output, "V.D.c ");
			for (i = 5; row[i] != ';'; i++)
				output = list_appendCh(output, row[i]);
			return toStr(output);
		}
	}
	else if (strCompare(row,"void",0))
	{
		if (functionType = isFunction(row))
		{
			if (functionType == 1)
			{
				output = list_append(output, "F.D.v ");
				for (i = 5; row[i] != '('; i++)
					output = list_appendCh(output, row[i]);
				output = list_appendCh(output, ' ');
				for ( ; row[i] != ';'; i++)
					output = list_appendCh(output, row[i]);
				return toStr(output);
			}
			else if (functionType == 2)
			{
				output = list_append(output, "F.I.v ");
				for (i = 5; row[i] != '('; i++)
					output = list_appendCh(output, row[i]);
				output = list_appendCh(output, ' ');
				for ( ; i < ch_index(row, ')', charCount(row, ')'))+1; i++)
					output = list_appendCh(output, row[i]);
				return toStr(output);
			}
		}
	}
	else if (strCompare(row,"#include",0))
	{
		output = list_append(output, "IMPORT ");
		for (i = 9; row[i] != '\0'; i++)
		{
			output = list_appendCh(output, row[i]);
		}
		return toStr(output);
	}
	else if (strCompare(row,"{",0))
	{
		output = list_appendCh(output, '{');
		*indent = *indent + 1;
		return toStr(output);
	}
	else if (strCompare(row,"}",0 ))
	{
		output = list_appendCh(output, '}');
		*indent = *indent + 1;
		return toStr(output);
	}
	else
	{
		printf("It's an undefined character\n");
		if (isFunction(row))
		{
			printf("It's a function\n");
			if (charCount(row, '='))
			{
				output = list_append(output, "V.I ");
				for (i = 0; row[i] != '='; i++)
					output = list_appendCh(output, row[i]);
				output = list_append(output, "= F.Cl ");
				for (i = i+1; row[i] != ';'; i++)
					output = list_appendCh(output, row[i]);
				return toStr(output);
			}
			output = list_append(output, "F.Cl ");
			for (i = 0; row[i] != ';'; i++)
				output = list_appendCh(output, row[i]);
			return toStr(output);
		}
		else if (charCount(row, '='))
		{
			output = list_append(output, "V.I ");
			for (i = 0; row[i] != ';'; i++)
				output = list_appendCh(output, row[i]);
			return toStr(output);
		}
		else
		{
			output = list_append(output, "NA ");
			for (i = 0; row[i] != '\0'; i++)
				output = list_appendCh(output, row[i]);
			return toStr(output);
		}
	}
}

int isCondition_C(char row[])
{
	if (strCount(row, "if") || strCount(row, "while") || strCount(row, "for"))
		return 1;
	return 0;
}
//Checks if it's a function, returns 1 if a declaration, 2 if an initialization and 3 if a call
int isFunction(char row[])
{
	int i;
	int tmp;
	
	if (charCount(row, '('))
	{
		tmp = ch_index(row, '(', 1) - 1;
		//Checks if there's a mathematic symbol before the first occurance of a parenthesis
		if (row[tmp] != ' ' && row[tmp] != '*' && row[tmp] != '+' && row[tmp] != '-' && row[tmp] != '/')
		{
			if (!charCount(row, '='))
			{
				if (charCount(row, ';'))
				{
					if (strCompare(row,"int",0) || strCompare(row,"char",0) || strCompare(row,"void",0))
						return 1;
					return 3;
				}
				else
					return 2;
			}
			return 3;
		}
	}
	return 0;
}




