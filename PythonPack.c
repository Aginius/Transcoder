#include "str.c"
#include "StrList.c"
#include "Trans.c"

#define SIZE 500

void print_Python(char *);
char * output_Python(char *, int *, int *, int *, int *);
void Classify(char *, int *, int *, int *, int *, int *, int *, int *, int *, int *);
void forGet(char *, char *, char *, char *, int *);
int isSimpleFor(char *);

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

	int indent;
	int pass;
	int mainFunc;
	int closingBracket;
	int i;

	indent = 0;
	pass = 0;

	if (input = fopen(fname, "r"))
	{
		if (output = fopen("output.py", "w"))
		{
			while (!feof(input))
			{
				fscanf(input, "%[^\n]\n", row);
				printf("Scanning %s\n",row);
				trans = output_Python(row, &indent, &pass, &mainFunc, &closingBracket);

				if (!pass)
				{
					printf("Printing %s\n", trans);

					tab_line(output, indent);
					
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

char * output_Python(char row[], int * indent, int * pass, int * mainFunc, int * closingBracket)
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
				if (increment == 1)
				{
					for (i = tmp; row[i] != '+'; i++)
					{
						output = list_appendCh(output, row[i]);
					}
					output = list_append(output, " += 1");
				}
				else if (increment == 2)
				{
					for (i = tmp; row[i] != '-'; i++)
						output = list_appendCh(output, row[i]);
					output = list_append(output, " -= 1");
				}
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
				
				forGet(row, name, value, target, &sign);
			
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
		for (i = 2 ; row[i] != '\0' && (row[i] != '*' && row[i] != '/'); i++)
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

/*HAVE TO MOVE THIS FUNCTION TO TRANS.c LIBRARY*/
void Classify(char str[], int * variable, int * function, int * condition, int * declaration, int * initialization, int * call, int * intType, int * charType, int * voidType)
{
	list * type = NULL;
	int compare;
	int i;

	compare = 1;

	for (i = 0; str[i] != ' ' && str[i] != '\0'; i++)
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
//RETURNS ALL ARGUMENTS FROM A SIMPLE FOR LOOP
void forGet(char str[], char name[], char value[], char target[], int sign[])
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

	for (i = i+1 ; str[i] != ';'; i++)
		if (str[i] != ' ')
			tmpValue = list_appendCh(tmpValue, str[i]);

	v = toStr(tmpValue);
	for (j = 0; v[j] != '\0'; j++)
		value[j] = v[j];
	value[j] = '\0';

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
