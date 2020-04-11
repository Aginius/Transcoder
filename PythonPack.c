#include "str.c"
#include "StrList.c"
#include "Trans.c"

#define SIZE 500

void print_Python(char *);
char * output_Python(char *, int *);
void Classify(char *, int *, int *, int *, int *, int *, int *, int *, int *, int *);

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
	int i;

	indent = 0;

	if (input = fopen(fname, "r"))
	{
		if (output = fopen("output.py", "w"))
		{
			while (!feof(input))
			{
				fscanf(input, "%[^\n]\n", row);
				printf("Scanning %s\n",row);	
				trans = output_Python(row, &indent);
							
				tab_line(output, indent);
				//printf("%s to %s\n\n", row, trans);
				printf("Printing %s\n", trans);
				fprintf(output, "%s\n", trans);
				printf("\n");
				free(trans);
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

char * output_Python(char row[], int * indent)
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
			for (i = tmp; row[i] != '\0'; i++)
				output = list_appendCh(output, row[i]);
			return toStr(output);	
		}
		else if (declaration)
		{
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
			output = list_appendCh(output, ' ');
			return toStr(output);
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
				output = list_appendCh(output, row[i]);
			output = list_appendCh(output, ':');
			return toStr(output);
		}
		else if (strCompare(row, "elif", tmp))
		{
			for (i = tmp; row[i] != '\0'; i++)
				output = list_appendCh(output, row[i]);
			output = list_appendCh(output, ':');
			return toStr(output);
		}
		else if (strCompare(row, "else", tmp))
		{
			for (i = tmp; row[i] != '\0'; i++)
				output = list_appendCh(output, row[i]);
			output = list_appendCh(output, ':');
			return toStr(output);
		}
		else if (strCompare(row, "wh", tmp))
		{
			output = list_append(output, "while ");
			for (i = tmp; row[i] != ' '; i++)
				;
			for (i = i+1; row[i] != '\0'; i++)
				output = list_appendCh(output, row[i]);
			output = list_appendCh(output, ':');
			return toStr(output);
		}
		else if (strCompare(row, "for", tmp))
		{
			for (i = tmp; row[i] != '\0'; i++)
				output = list_appendCh(output, row[i]);
			output = list_appendCh(output, ':');
			return toStr(output);
		}
	}
	else if (row[0] == '{')
	{
		printf("Indent\n");
		*indent = *indent + 1;
		output = list_appendCh(output, '{');
		return toStr(output);
	}
	else if (row[0] == '}')
	{
		printf("Unindent\n");
		*indent = *indent - 1;
		output = list_appendCh(output, '}');
		return toStr(output);
	}
	else if (strCompare(row, "/*", 0))
	{
		printf("Comment\n");
		output = list_appendCh(output, '#');
		for (i = 2 ; row[i] != '\0'; i++)
			output = list_appendCh(output, row[i]);
		toStr(output);
	}
	else if (strCompare(row, "IMPORT", 0))
	{
		printf("Import\n");
		output = list_append(output, "import ");
		for (i = tmp; row[i] != '\0'; i++)
			output = list_appendCh(output, row[i]);
		toStr(output);
	}	
	else
	{
		printf("Not recognized\n");
		for (i = tmp; row[i] != '\0'; i++)
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
			if (strCompare(str, "V", i))
				*variable = 1;
			else if (strCompare(str, "F", i))
				*function = 1;
			else if (strCompare(str, "Cl", i))
				*condition = 1;
			else if (strCompare(str, "D", i))
				*declaration = 1;
			else if (strCompare(str, "I", i))
				*initialization = 1;
			else if (strCompare(str, "C", i))
				*call = 1;
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
