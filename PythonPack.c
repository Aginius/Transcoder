#include "str.c"
#include "StrList.c"
#include "TransLib.c"

#define SIZE 500

void print_Python(char *);
char * output_Python(char *, int *, int *, int *, int *, char *, int *);
void forGet(char *, char *, char *, char *);
void forArgsLen(char *, int *, int *, int *);
void simpleForGet(char *, char *, char *, char *, int *);
int isSimpleFor(char *);
list * translateIncrement(char *, int, int);

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
	char queue[10];

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
	isFor = 0;

	if (input = fopen(fname, "r"))
	{
		if (output = fopen("output.py", "w"))
		{
			while (!feof(input))
			{
				fscanf(input, "%[^\n]\n", row);
				printf("Scanning %s\n",row);
				trans = output_Python(row, &indent, &pass, &mainFunc, &closingBracket, queue, &isFor);

				if (!pass)
				{
					printf("Printing %s\n", trans);

					tab_line(output, indent);
					/*If it's a the end of a for loop, it prints the increment at the end of the transcripted while loop*/
					if (isFor == indent && row[0] == '}')
					{
						tab_line(output, 1);
						fprintf(output, "%s\n", queue);
						isFor = 0;
					}
					
					fprintf(output, "%s\n", trans);
					if (row[0] != '}')
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

char * output_Python(char row[], int * indent, int * pass, int * mainFunc, int * closingBracket, char queue[], int * isFor)
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

	Classify(row, &variable, &function, &condition, &declaration, &initialization, &call, &intType, &charType, &voidType);
	
	tmp = noPseudo(row);
	
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
				output = translateIncrement(row, tmp, increment);
				return toStr(output);
			}
			else
			{
				for (i = tmp; row[i] != '\0'; i++)
					output = list_appendCh(output, row[i]);
				return toStr(output);	
			}
		}
		else if (declaration)
		{
			printf("Declaration\n");
			if (call)
			{
				for (i = tmp; row[i] != '\0'; i++)
					output = list_appendCh(output, row[i]);
				return toStr(output);
			}
			else if (intType)
			{
				for (i = tmp; row[i] != '\0'; i++)
					output = list_appendCh(output, row[i]);
				output = list_append(output, " = 0");
				return toStr(output);
			}
			else if (charType)
			{
				for (i = tmp; row[i] != '\0'; i++)
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
			for (i = tmp; row[i] != '\0'; i++)
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
			for (i = tmp; row[i] != '\0'; i++)
				output = list_appendCh(output, row[i]);
			return toStr(output);
		}
	}
	else if (condition)
	{
		printf("Condition\n");
		if (strCompare(row, "if", tmp))
		{
			for (i = tmp; row[i] != '\0'; i++)
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
			for (i = tmp; row[i] != '\0'; i++)
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
			for (i = tmp; row[i] != '\0'; i++)
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
			for (i = i+1; row[i] != '\0'; i++)
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
				char * assign;
				char * cond;
				char * increment;
				int len1 = 0;
				int len2 = 0;
				int len3 = 0;

				forArgsLen(row, &len1, &len2, &len3);

				if (assign = malloc(sizeof(int)*len1))
				{
					if (cond = malloc(sizeof(int)*len2))
					{
						if (increment = malloc(sizeof(int)*len3))
						{
							forGet(row, assign, cond, increment);
							output = list_append(output, assign);
							output = list_appendCh(output, '\n');
							for (i = 0; i < *indent; i++)
								output = list_appendCh(output, '\t');
							output = list_append(output, "while (");
							output = list_append(output, cond);
							output = list_append(output, "):");
							
							for (i = 0; increment[i] != '\0'; i++)
								queue[i] = increment[i];
							queue[i] = '\0';
							
							free(assign);
							free(cond);
							free(increment);
										
							*isFor = *indent;
						}
						else
							printf("ALLOCATION ERROR\n");
					}
					else
						printf("ALLOCATION ERROR\n");
				}
				else
					printf("ALLOCATION ERROR\n");
			}
			return toStr(output);
		}
	}
	else if (row[0] == '{')
	{
		printf("Indent\n");
		*indent = *indent + 1;
		*pass = 1;
		return " ";
	}
	else if (row[0] == '}')
	{
		printf("Unindent\n");
		*indent = *indent - 1;
		if (*closingBracket)
			*pass = 1;
		return " ";
	}
	else if (strCompare(row, "/*", 0))
	{
		printf("Comment\n");
		output = list_appendCh(output, '#');
		for (i = 2; row[i] == ' '; i++)
			;
		for (i = 2; row[i] != '\0' && (row[i] != '*' && row[i] != '/'); i++)
			output = list_appendCh(output, row[i]);
		return toStr(output);
	}
	else if (strCompare(row, "IMPORT", 0))
	{
		printf("Import\n");
		output = list_append(output, "import ");
		for (i = tmp; row[i] != '\0'; i++)
			output = list_appendCh(output, row[i]);
		return toStr(output);
	}
	else if (strCompare(row, "MAIN DECLARATION", 0))
	{
		printf("Main declaration\n");
		*indent = *indent - 1;
		*mainFunc = 1;
		*pass = 1;
		return " ";	
	}
	else if (*mainFunc && strCompare(row, "MAIN RETURN", 0))
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
		for (i = 0; row[i] != '\0'; i++)
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
void forGet(char str[], char arg1[], char arg2[], char arg3[])
{
	int i,j;
	list * tmpArg1 = NULL;
	list * tmpArg2 = NULL;
	list * tmpArg3 = NULL;
	char * ar1;
	char * ar2;
	char * ar3;
	int inStr = 0;
	/*GOING RIGHT AFTER THE FOR DECLARATION*/
	for (i = 0; str[i] != '('; i++)
		;
	/*SCANNING THE FIRST ARGUMENT UNTIL THE ;*/
	for (i = i+1; str[i] != ';'; i++)
		tmpArg1 = list_appendCh(tmpArg1, str[i]);
	ar1 = toStr(tmpArg1);
	for (j = 0; ar1[j] != '\0'; j++)
		arg1[j] = ar1[j];
	arg1[j] = '\0';
	free(ar1);
	/*SKIPPING EVENTUAL SPACES*/
	for (i = i+1; str[i] == ' '; i++)
		;
	/*SCANNING THE SECOND ARGUMENT UNTIL THE ;, IF THE FORMER IS NOT INSIDE A STRING*/
	for ( ; !(str[i] == ';' && !inStr); i++)
	{
		if (str[i] == 39 || str[i] == '"')
		{
			if (inStr)
				inStr = 0;
			else
				inStr = 1;
		}
		if (str[i] == '&' && str[i+1] == '&')
		{
			tmpArg2 = list_append(tmpArg2, "and");
			i++;
		}
		else if (str[i] == '|' && str[i+1] == '|')
		{
			tmpArg2 = list_append(tmpArg2, "or");
			i++;
		}
		else
			tmpArg2 = list_appendCh(tmpArg2, str[i]);
	}
	
	ar2 = toStr(tmpArg2);
	for (j = 0; ar2[j] != '\0'; j++)
		arg2[j] = ar2[j];
	arg2[j] = '\0';
	free(ar2);
	/*SKIPPING EVENTUAL SPACES*/
	for (i = i+1; str[i] == ' '; i++)
		;
	/*SCANNING THE THIRD ARGUMENT UNTIL THE ), IF THE FORMER IS NOT INSIDE A STRING*/
	for ( ; !(str[i] == ')' && !inStr); i++)
	{
		if (str[i] == 39 || str[i] == '"')
		{
			if (inStr)
				inStr = 0;
			else
				inStr = 1;
		}
		tmpArg3 = list_appendCh(tmpArg3, str[i]);
	}
	/*HAVE TO CHANGE TRANSCRYPTING SYSTEM DOWN BELOW, IT'S BAD OPTIMIZED*/
	ar3 = toStr(tmpArg3);
	/*TRANSLATING THE INCREMENT TO PYTHON SYNTAX*/
	tmpArg3 = translateIncrement(ar3, 0, isIncrement(ar3));
	free(ar3);
	list_print(tmpArg3);
	ar3 = toStr(tmpArg3);
	
	for (j = 0; ar3[j] != '\0'; j++)
		arg3[j] = ar3[j];
	arg3[j] = '\0';
	free(ar3);
}

void forArgsLen(char str[], int * len1, int * len2, int * len3)
{
	int i;
	int inStr = 0;

	for (i = 0; str[i] != '('; i++)
		;

	for (i = i+1; str[i] != ';'; i++)
		*len1 = *len1 + 1;

	for (i = i+1; str[i] != ';' && !inStr; i++)
	{
		if (str[i] == 39 || str[i] == '"')
		{
			if (inStr)
				inStr = 0;
			else
				inStr = 1;
		}
		*len2 = *len2 + 1;
	}

	for (i = i+1; str[i] != ';' && !inStr; i++)
	{
		if (str[i] == 39 || str[i] == '"')
		{
			if (inStr)
				inStr = 0;
			else
				inStr = 1;
		}
		*len3 = *len3 + 1;
	}
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
		for (start; row[start] != '+'; start++)
			output = list_appendCh(output, row[start]);
		
		output = list_append(output, " += 1");
	}
	else if (type == 2)
	{
		for (start; row[start] != '-'; start++)
			output = list_appendCh(output, row[start]);
		output = list_append(output, " -= 1");
	}
	return output;
}
