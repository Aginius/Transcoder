#include "str.c"
#include "StrList.c"
#include "TransLib.c"

#define SIZE 250

typedef struct forArgs
{
	str_list * arg1;
	str_list * arg2;
	str_list * arg3;
} forArgs;

void print_Python(char *);
char * output_Python(char *, int, int *, int *, int *, int *, str_list **, int *);
forArgs forGet(char *);
void simpleForGet(char *, char *, char *, char *, int *);
int isSimpleFor(char *);
list * translateIncrement(char *, int, int);
void isInStr(char, int *, int *);

int main (int argc, char * argv[])
{
	print_Python("PseudoCode.pc");
	return 0;
}

void print_Python(char fname[])
{
	FILE * input;
	FILE * output;

	char row[SIZE];
	char * trans;
	str_list * queue = NULL;
	str_list * tmp;
	int indent;
	int pass;
	int mainFunc;
	int closingBracket;
	int isFor;
	int i;

	indent = 0;
	pass = 0;
	mainFunc = 0;
	closingBracket = 0;
	isFor = -1;

	if (input = fopen(fname, "r"))
	{
		if (output = fopen("output.py", "w"))
		{
			while (fgets(row, SIZE, input))
			{				
				printf("Scanning %s",row);
				/*Searching the first index after tabs*/
				for (i = 0; row[i] == '\t'; i++);
				
				trans = output_Python(row, i, &indent, &pass, &mainFunc, &closingBracket, &queue, &isFor);
				
				/*If it's a the end of a for loop, it prints the increment at the end of the transcripted while loop*/	
				if (isFor == indent && row[i] == '}')
				{	
					/*HAVE TO FIX NESTED FOR LOOPS*/
					tmp = queue;
					for ( ; tmp; tmp = tmp -> next)
					{
						tab_line(output, 1);
						fprintf(output, "%s\n", toStr(tmp -> str));
					}
					strl_free(queue, 0);
					queue = NULL;
					isFor = -1;
				}
				if (!pass)
				{
					printf("Printing %s\n\n", trans);
									
					fprintf(output, "%s\n", trans);
					
					if (row[i] != '}')
					{
						if (closingBracket)
							closingBracket = 0;
						free(trans);
					}
					else if (!closingBracket)
						closingBracket = 1;	
				}
				/*CHANGE THE WAY IT WORKS FOR PERFORMANCE, YOU CAN USE BOOLS FROM output_Python*/
				else if (strCompare(row, "MAIN DECLARATION", 0))
					fprintf(output, "\n");
				else
					pass = 0;
			}
			fclose(output);
		}
		else
			printf("FILE WRITING ERROR\n");
		fclose(input);
	}
	else
		printf("FILE READING ERROR\n");
}

char * output_Python(char row[], int start, int * indent, int * pass, int * mainFunc, int * closingBracket, str_list ** queue, int * isFor)
{
	list * output = NULL;

	int variable;
	int function;
	int condition;
	int declaration;
	int initialization;
	int call;
	int intType;
	int charType;
	int voidType;
	
	int tmp;
	int i;
	
	variable = 0;
	function = 0;
	condition = 0;
	declaration = 0;
	initialization = 0;
	call = 0;
	intType = 0;
	charType = 0;
	voidType = 0;
	
	i = start;
	
	Classify(row, i, &variable, &function, &condition, &declaration, &initialization, &call, &intType, &charType, &voidType);

	tmp = noPseudo(row, i);
	/*
	printf("Translating: ");
	for (i = tmp; row[i] != '\n'; i++)
		printf("%c", row[i]);
	printf("\n");
	*/
	for (i = 0; i < *indent; i++)
		output = list_appendCh(output, '\t');

	if (variable)
	{
		printf("Variable\n");
		if (initialization)
		{
			printf("Initialization\n");
			if (isIncrement(row))
			{
				printf("It's an increment\n");
				int increment = isIncrement(row);
				output = list_appendL(output, translateIncrement(row, tmp, increment));
				printf("Increment is ");
				list_print(output);
				return toStr(output);
			}
			else
			{
				for (i = tmp; row[i] != '\n'; i++)
					output = list_appendCh(output, row[i]);
				return toStr(output);	
			}
		}
		else if (declaration)
		{
			printf("Declaration\n");
			if (call)
			{
				for (i = tmp; row[i] != '\n'; i++)
					output = list_appendCh(output, row[i]);
				return toStr(output);
			}
			else if (intType)
			{
				for (i = tmp; row[i] != '\n'; i++)
					output = list_appendCh(output, row[i]);
				
				output = list_append(output, " = 0");
				return toStr(output);
			}
			else if (charType)
			{
				for (i = tmp; row[i] != '\n'; i++)
					output = list_appendCh(output, row[i]);
				output = list_append(output, " = ''");
				return toStr(output);
			}
		}
	}
	else if (function)
	{
		printf("Function\n");
		if (initialization)
		{
			printf("Initialization\n");
			output = list_append(output, "def ");
			for (i = tmp; row[i] != '\n'; i++)
				output = list_appendCh(output, row[i]);
			output = list_appendCh(output, ':');
			return toStr(output);
		}
		else if (declaration)
		{
			printf("Declaration\n");
			*pass = 1;
			return " ";
		}
		else if (call)
		{
			printf("Call\n");
			for (i = tmp; row[i] != '\n'; i++)
				output = list_appendCh(output, row[i]);
			return toStr(output);
		}
	}
	else if (condition)
	{
		printf("Condition\n");
		if (strCompare(row, "if", tmp))
		{
			for (i = tmp; row[i] != '\n'; i++)
			{
				if (row[i] == '&' && row[i+1] == '&')
				{
					output = list_append(output, "and");
					i++;
				}
				else if (row[i] == '|' && row[i+1] == '|')
				{
					output = list_append(output, "or");
					i++;
				}
				else
					output = list_appendCh(output, row[i]);
			}
			output = list_appendCh(output, ':');
			return toStr(output);
		}
		else if (strCompare(row, "elif", tmp))
		{
			for (i = tmp; row[i] != '\n'; i++)
			{
				if (row[i] == '&' && row[i+1] == '&')
				{
					output = list_append(output, "and");
					i++;
				}
				else if (row[i] == '|' && row[i+1] == '|')
				{
					output = list_append(output, "or");
					i++;
				}
				else
					output = list_appendCh(output, row[i]);
			}
			output = list_appendCh(output, ':');
			return toStr(output);
		}
	       	else if (strCompare(row, "else", tmp) && nextChar(row,tmp+3) == ' ')
		{
			printf("It's an else\n");
			for (i = tmp; row[i] != '\n'; i++)
			{
				if (row[i] == '&' && row[i+1] == '&')
				{
					output = list_append(output, "and");
					i++;
				}
				else if (row[i] == '|' && row[i+1] == '|')
				{
					output = list_append(output, "or");
					i++;
				}
				else
					output = list_appendCh(output, row[i]);
			}
			output = list_appendCh(output, ':');
			return toStr(output);
		}
		else if (strCompare(row, "wh", tmp))
		{
			output = list_append(output, "while ");
			for (i = tmp; row[i] != ' '; i++)
				;
			for (i = i+1; row[i] != '\n'; i++)
			{
				if (row[i] == '&' && row[i+1] == '&')
				{
					output = list_append(output, "and");
					i++;
				}	
				else if (row[i] == '|' && row[i+1] == '|')
				{
					output = list_append(output, "or");
					i++;
				}
				else	
					output = list_appendCh(output, row[i]);
			}
			output = list_appendCh(output, ':');
			return toStr(output);
		}
		else if (strCompare(row, "for", tmp))
		{
			if (isSimpleFor(row))
			{
				char name[10];
				char value[10];
				char target[10];
				int sign = 0;
				
				simpleForGet(row, name, value, target, &sign);
			
				output = list_append(output, "for ");
				output = list_append(output, name);
				output = list_append(output, " in range(");
				if (sign == 1)
				{
					output = list_append(output, value);
					output = list_appendCh(output, ',');
					output = list_append(output, target);
				}
				else if (sign == 2)
				{
					output = list_append(output, target);
					output = list_appendCh(output, ',');
					output = list_append(output, value);
				}
			
				output = list_append(output, "):");
				printf("%s\n%s\n%s\n",name,value,target);
			}
			else
			{
				/*CONVERT THE FOR IN A WHILE*/
				forArgs args = forGet(row);
				str_list * tmp;
				list * tmpStr;
				str_list * tmpQueue;
				int j;

				for (tmp = args.arg1; tmp; tmp = tmp -> next)
				{	
					for (tmpStr = tmp -> str ; tmpStr; tmpStr = tmpStr -> next)
						output = list_appendCh(output, tmpStr -> c);
					output = list_appendCh(output, '\n');
				}
				
				for (i = 0; i < *indent; i++)
					output = list_appendCh(output, '\t');
				output = list_append(output, "while (");
				for (tmpStr = args.arg2 -> str; tmpStr; tmpStr = tmpStr -> next)
					output = list_appendCh(output, tmpStr -> c);
				output = list_append(output, "):");
				
				*queue = strl_alloc(*queue, 1);

				tmpQueue = *queue;

				tmpQueue -> str = NULL;

				tmpQueue = *queue;

				for (tmp = args.arg3, i = 0; tmp; tmp = tmp -> next, i++)
				{
					for (j = 0; j < *indent; j++)
						tmpQueue -> str = list_appendCh(tmpQueue -> str, '\t');

					for (tmpStr = tmp -> str; tmpStr; tmpStr = tmpStr -> next, i++)
						tmpQueue -> str = list_appendCh(tmpQueue -> str, tmpStr -> c);
					
					if (tmp -> next)
					{
						if (tmpQueue -> next = malloc(sizeof(str_list)))
							;
						else
							printf("ALLOCATION ERROR\n");
						tmpQueue = tmpQueue -> next;
					}
				}
				/*
				for (tmpQueue = *queue; tmpQueue; tmpQueue = tmpQueue -> next)
					printf("ITERATION\n");
				*/
				strl_free(args.arg1, 1);
				strl_free(args.arg2, 1);
				strl_free(args.arg3, 1);
					
				*isFor = *indent;
			}
			return toStr(output);
		}
	}
	else if (row[tmp] == '{')
	{
		printf("Indent\n");
		*indent = *indent + 1;
		*pass = 1;
		return " ";
	}
	else if (row[tmp] == '}')
	{
		printf("Unindent\n");
		*indent = *indent - 1;
		if (*closingBracket)
			*pass = 1;
		return " ";
	}
	else if (strCompare(row, "/*", tmp))
	{
		printf("Comment\n");
		output = list_appendCh(output, '#');
		for (i = tmp+2; row[i] == ' '; i++)
			;
		for ( ; row[i] != '\n' && (row[i] != '*' && row[i+1] != '/'); i++)
			output = list_appendCh(output, row[i]);
		return toStr(output);
	}
	else if (strCompare(row, "IMPORT", tmp))
	{
		printf("Import\n");
		output = list_append(output, "import ");
		for (i = tmp; row[i] != '\n'; i++)
			output = list_appendCh(output, row[i]);
		return toStr(output);
	}
	else if (strCompare(row, "MAIN DECLARATION", tmp))
	{
		printf("Main declaration\n");
		*indent = *indent - 1;
		*mainFunc = 1;
		*pass = 1;
		return " ";	
	}
	else if (*mainFunc && strCompare(row, "MAIN RETURN", tmp))
	{
		printf("Main return\n");
		*indent = *indent + 1;
		*mainFunc = 0;
		*pass = 1;
		return " ";
	}
	else
	{
		printf("Not recognized\n");
		for (i = tmp; row[i] != '\n'; i++)
			output = list_appendCh(output, row[i]);
		return toStr(output);
	}
}

//RETURNS ALL ARGUMENTS FROM A SIMPLE FOR LOOP
void simpleForGet(char str[], char name[], char value[], char target[], int sign[])
{
	int i,j;
	list * tmpName = NULL;
	list * tmpValue = NULL;
	list * tmpTarget = NULL;
	char * n;
	char * v;
	char * t;

	for (i = 0; str[i] != '('; i++)
		;
	for (i = i+1; str[i] != '='; i++)
		if (str[i] != ' ')
			tmpName = list_appendCh(tmpName, str[i]);

	n = toStr(tmpName);
	for (j = 0; n[j] != '\0'; j++)
		name[j] = n[j];
	name[j] = '\0';
	free(n);

	for (i = i+1 ; str[i] != ';'; i++)
		if (str[i] != ' ')
			tmpValue = list_appendCh(tmpValue, str[i]);

	v = toStr(tmpValue);
	for (j = 0; v[j] != '\0'; j++)
		value[j] = v[j];
	value[j] = '\0';
	free(v);

	for (i = i+1; str[i] != '\0'; i++)
	{
		if (str[i] == '<')
		{
			*sign = 1;
			break;
		}
		else if (str[i] == '>')
		{
			*sign = 2;
			break;
		}
	}

	for (i = i+1; str[i] != ';'; i++)
		if (str[i] != ' ')
			tmpTarget = list_appendCh(tmpTarget, str[i]);
	
	t = toStr(tmpTarget);
	for (j = 0; t[j] != '\0'; j++)
		target[j] = t[j];
	target[j] = '\0';
	free(t);
}
/*CURRENTLY SUPPORTS ONLY ONE STATEMENT PER ARGUMENT*/
forArgs forGet(char str[])
{
	forArgs args;
	str_list * arg1;
	str_list * arg2;
	str_list * arg3;
	list * lis;
	char * tmp;
	int i,j;
	int inStr = 0;
	int inCh = 0;
	int closedBr = 0;
	int openBr = 0;

	args.arg1 = NULL;
	args.arg2 = NULL;
	args.arg3 = NULL;
	
	args.arg1 = strl_alloc(args.arg1, 1);
	args.arg2 = strl_alloc(args.arg2, 1);
	args.arg3 = strl_alloc(args.arg3, 1);

	arg1 = args.arg1;
	arg2 = args.arg2;
	arg3 = args.arg3;
	
	arg1 -> str = NULL;
	arg2 -> str = NULL;
	arg3 -> str = NULL;
	
	/*GOING RIGHT AFTER THE FOR DECLARATION*/
	for (i = 0; str[i] != '('; i++)
		;

	/*SCANNING THE FIRST ARGUMENT UNTIL THE ;*/
	for (i = i+1; !(str[i] == ';' && !inCh && !inStr); i++)
	{
		isInStr(str[i], &inCh, &inStr);

		if (str[i] == ',' && !inStr && !inCh && closedBr == openBr)
		{
			arg1  = strl_alloc(arg1, 1);
			arg1 = arg1 -> next;
			for (i=i+1; str[i] == ' '; i++)
				;
		}
		else if (str[i] == '(' && !inStr && !inCh)
			openBr++;
		else if (str[i] == ')' && !inStr && !inCh)
			closedBr++;
		else
			arg1 -> str = list_appendCh(arg1 -> str, str[i]);
	}

	/*SKIPPING EVENTUAL SPACES*/
	for (i = i+1; str[i] == ' '; i++)
		;
	/*SCANNING THE SECOND ARGUMENT UNTIL THE ;, IF THE FORMER IS NOT INSIDE A STRING*/
	for ( ; !(str[i] == ';' && !inCh && !inStr); i++)
	{
		isInStr(str[i], &inCh, &inStr);

		if (str[i] == '&' && str[i+1] == '&')
		{
			arg2 -> str = list_append(arg2 -> str, "and");
			i++;
		}
		else if (str[i] == '|' && str[i+1] == '|')
		{
			arg2 -> str = list_append(arg2 -> str, "or");
			i++;
		}
		else
			arg2 -> str = list_appendCh(arg2 -> str, str[i]);
	}
	
	/*SKIPPING EVENTUAL SPACES*/
	for (i = i+1; str[i] == ' '; i++)
		;

	openBr = 0;
	closedBr = 0;

	/*SCANNING THE THIRD ARGUMENT UNTIL THE ), IF THE FORMER IS NOT INSIDE A STRING*/
	for ( ; !(str[i] == ')' && !inCh && !inStr); i++)
	{
		isInStr(str[i], &inCh, &inStr);

		if (str[i] == ',' && !inStr && !inCh && openBr == closedBr)
		{
			arg3 = strl_alloc(arg3, 1);

			arg3 = arg3 -> next;
			for (i=i+1; str[i] == ' '; i++)
				;
			i--;
		}
		else if (str[i] == '(' && !inStr && !inCh)
			openBr++;
		else if (str[i] == ')' && !inStr && !inCh)
			closedBr++;
		else
			arg3 -> str = list_appendCh(arg3 -> str, str[i]);
	}
	arg3 = args.arg3;
	/*TRANSLATING THE INCREMENT TO PYTHON SYNTAX*/
	while (arg3)
	{
		if (tmp = malloc(sizeof(char)*list_len(arg3 -> str)+1))
		{
			for (lis = arg3 -> str, i = 0; lis; lis = lis -> next, i++)
				tmp[i] = lis -> c;
			tmp[i] = '\0';	
			list_free(arg3 -> str);
			arg3 -> str = translateIncrement(tmp, 0, isIncrement(tmp));
			free(tmp);
		}
		else
			printf("ALLOCATION ERROR\n");
		arg3 = arg3 -> next;
	}
	return args;
}

int isSimpleFor(char str[])
{
	int i;
	list * tmp = NULL;
	for (i = 0; str[i] != '('; i++)
		;

	for (i=i+1; str[i] != ';'; i++)
		tmp = list_appendCh(tmp, str[i]);

	if (listCount(tmp, '(') || listCount(tmp, ')') || !listCount(tmp, '='))
		return 0;

	list_free(tmp);
	tmp = NULL;

	for (i =i+1; str[i] != ';'; i++)
		tmp = list_appendCh(tmp, str[i]);

	if (listCount(tmp, '(') || listCount(tmp, ')') || listCount(tmp, '=') || !listCount(tmp, '<') && !listCount(tmp, '>'))
		return 0;

	list_free(tmp);

	return 1;
}

list * translateIncrement(char row[], int start, int type)
{
	list * output = NULL;
	
	if (type == 1)
	{
		for ( ; row[start] != '+'; start++)
			output = list_appendCh(output, row[start]);
		
		output = list_append(output, " += 1");
	}
	else if (type == 2)
	{
		for ( ; row[start] != '-'; start++)
			output = list_appendCh(output, row[start]);
		output = list_append(output, " -= 1");
	}
	else
	{
		for ( ; row[start] != '\0'; start++)
			output = list_appendCh(output, row[start]);
	}
	return output;
}

void isInStr(char ch, int * inCh, int * inStr)
{
	if (ch == 39)
	{
		if (*inCh)
			*inCh = 0;
		else if (!*inStr)
			*inCh = 1;
	}
	else if (ch == '"')
	{
		if (*inStr)
			*inStr = 0;
		else if (!*inCh)
			*inStr = 1;
	}
}
