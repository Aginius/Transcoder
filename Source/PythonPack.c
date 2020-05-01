/*Max size of the input string*/
#define SIZE 500

/*Struct to store for loop arguments*/
typedef struct forArgs
{
	str_list * arg1;
	str_list * arg2;
	str_list * arg3;
} forArgs;

void print_Python(char *);
char * output_Python(char *, int, int *, int *, int *, int *, int *, str_mat **, intList **);
forArgs forGet(char *);
void simpleForGet(char *, char *, char *, char *, int *);
int isSimpleFor(char *);
list * translateIncrement(char *, int, int);

void print_Python(char fname[])
{
	FILE * input;
	FILE * output;

	char row[SIZE];
	char * trans;
	str_mat * queue = NULL;
	str_mat * tmp;
	str_list * tmpStr;
	int indent;
	int pass;
	int mainFunc;
	int mainDeclaration;
	int closingBracket;
	intList * isFor = NULL;
	int i;

	indent = 0;
	pass = 0;
	mainFunc = 0;
	mainDeclaration = 0;
	closingBracket = 0;
	
	if (input = fopen("PseudoCode.pc", "r"))
	{
		if (output = fopen(fname, "w"))
		{
			while (fgets(row, SIZE, input))
			{	
				//printf("Scanning %s",row);
				/*Searching the first index after tabs*/
				for (i = 0; row[i] == '\t'; i++);
				/*Translate the input to Python syntax*/
				trans = output_Python(row, i, &indent, &pass, &mainFunc, &mainDeclaration, &closingBracket, &queue, &isFor);
				/*If it's a the end of a for loop, it prints the increment at the end of the transcripted while loop*/	
				if (isFor && intL_getLast(isFor) == indent && row[i] == '}')
				{
					tmp = queue;
					/*Iterates until it finds the last saved queue, then prints it*/
					while (tmp -> next)
						tmp = tmp -> next;
					for (tmpStr = tmp -> arr ; tmpStr; tmpStr = tmpStr -> next)
					{
						tab_line(output, 1);
						fprintf(output, "%s\n", toStr(tmpStr -> str));
					}
					/*Deallocates the printed queue and its corresponding iteration number*/
					queue = strm_pop(queue, 0);
					isFor = intL_pop(isFor);
				}
				/*The variable pass can prevent some output from being printed*/
				if (!pass)
				{
					//printf("Printing %s\n\n", trans);
					/*Prints the output to file*/
					fprintf(output, "%s\n", trans);
					/*If the input was not a closed bracket it deallocates the used space and resets closingBracket*/
					if (row[i] != '}')
					{
						if (closingBracket)
							closingBracket = 0;
						free(trans);
					}
					/*If it was the first of some closing brackets, it prevents them from causing useless new line printing*/
					else if (!closingBracket)
						closingBracket = 1;	
				}
				/*If the input was the main program declaration it prints an empty line*/
				else if (mainDeclaration)
				{
					mainDeclaration = 0;
					fprintf(output, "\n");
				}
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

char * output_Python(char row[], int start, int * indent, int * pass, int * mainFunc, int * mainDeclaration, int * closingBracket, str_mat ** queueList, intList ** isFor)
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
	/*The scanning starts after all the tabs*/
	i = start;
	/*Calling a function that checks the kind of input*/
	Classify(row, i, &variable, &function, &condition, &declaration, &initialization, &call, &intType, &charType, &voidType);
	/*Returns the index right after the pseudo code declarations*/
	tmp = noPseudo(row, i);
	/*Appends a number of tabs to the output, according to detected curly brackets*/
	for (i = 0; i < *indent; i++)
		output = list_appendCh(output, '\t');

	if (variable)
	{
		//printf("Variable\n");
		if (initialization)
		{
			//printf("Initialization\n");
			if (isIncrement(row))
			{
				//printf("It's an increment\n");
				int increment = isIncrement(row);
				output = list_appendL(output, translateIncrement(row, tmp, increment));
				//printf("Increment is ");
				return toStr(output);
			}
			else
			{
				if (charType && isArr(row))
				{
					for (i = tmp; row[i] != '['; i++)
						output = list_appendCh(output, row[i]);
					for ( ; row[i] != ']'; i++);
					for (i = i+1; row[i] != '\n'; i++)
						output = list_appendCh(output, row[i]);
				}
				else
					for (i = tmp; row[i] != '\n'; i++)
						output = list_appendCh(output, row[i]);
				return toStr(output);	
			}
		}
		else if (declaration)
		{
			//printf("Declaration\n");
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
				//String conversion
				if (isArr(row))
					for (i = tmp; row[i] != '['; i++)
						output = list_appendCh(output, row[i]);
				else
					for (i = tmp; row[i] != '\n'; i++)
						output = list_appendCh(output, row[i]);
				output = list_append(output, " = ''");
				return toStr(output);
			}
		}
	}
	else if (function)
	{
		//printf("Function\n");
		if (initialization)
		{
			//printf("Initialization\n");
			output = list_append(output, "def ");
			for (i = tmp; row[i] != '\n'; i++)
				output = list_appendCh(output, row[i]);
			output = list_appendCh(output, ':');
			return toStr(output);
		}
		else if (declaration)
		{
			//printf("Declaration\n");
			*pass = 1;
			return " ";
		}
		else if (call)
		{
			//printf("Call\n");
			for (i = tmp; row[i] != '\n'; i++)
				output = list_appendCh(output, row[i]);
			return toStr(output);
		}
	}
	else if (condition)
	{
		//printf("Condition\n");
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
			/*Checks if it is a for that can be translated to an 'in range' python loop*/
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
				//printf("%s\n%s\n%s\n",name,value,target);
			}
			/*If it's not a simple increment loop, it converts it in a while, by printing the first argument
			 * before the while, the second argument inside the while condition and the third argument at the
			 * while end, by a dynamic queue which can store multiple increment values*/
			else
			{
				/*Calling a function that extracts the argument from the for loop and stores them into a struct*/
				forArgs args = forGet(row);
				str_mat * tmpQueueList;
				str_list * queue;
				str_list * tmp;
				list * tmpStr;
				intList * tmpIsFor;
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
				
				*queueList = strm_alloc(*queueList);
				tmpQueueList = *queueList;

				while (tmpQueueList -> next)
					tmpQueueList = tmpQueueList -> next;
				tmpQueueList -> arr = NULL;
				tmpQueueList -> arr = strl_alloc(tmpQueueList -> arr);
				queue = tmpQueueList -> arr;
				queue -> str = NULL;
				/*Appending increments of the third argument into a queue that will be printed at the end of the loop*/
				for (tmp = args.arg3, i = 0; tmp; tmp = tmp -> next, i++)
				{
					for (j = 0; j < *indent; j++)
						queue -> str = list_appendCh(queue -> str, '\t');

					for (tmpStr = tmp -> str; tmpStr; tmpStr = tmpStr -> next, i++)
						queue -> str = list_appendCh(queue -> str, tmpStr -> c);
					if (tmp -> next)
					{
						if (queue -> next = malloc(sizeof(str_list)))
							;
						else
							printf("ALLOCATION ERROR\n");
						queue = queue -> next;
					}
				}

				strl_free(args.arg1, 1);
				strl_free(args.arg2, 1);
				strl_free(args.arg3, 1);
				
				*isFor = intL_append(*isFor, *indent);
			}
			return toStr(output);
		}
	}
	/*Increments the indentation level if an open bracket*/
	else if (row[tmp] == '{')
	{
		*indent = *indent + 1;
		*pass = 1;
		return " ";
	}
	/*Decrements the indentation level if a closed bracket*/
	else if (row[tmp] == '}')
	{
		*indent = *indent - 1;
		if (*closingBracket)
			*pass = 1;
		return " ";
	}
	/*Comments formatting*/
	else if (strCompare(row, "/*", tmp))
	{
		output = list_appendCh(output, '#');
		for (i = tmp+2; row[i] == ' '; i++)
			;
		for ( ; row[i] != '\n' && (row[i] != '*' && row[i+1] != '/'); i++)
			output = list_appendCh(output, row[i]);
		return toStr(output);
	}
	/*Libraries import formatting*/
	else if (strCompare(row, "IMPORT", tmp))
	{
		output = list_append(output, "import ");
		for (i = tmp+7; row[i] != '\n'; i++)
			output = list_appendCh(output, row[i]);
		return toStr(output);
	}
	/*Python does not need a main method declaration, so it just accordingly formats the output*/
	else if (strCompare(row, "MAIN DECLARATION", tmp))
	{
		*indent = *indent - 1;
		*mainFunc = 1;
		*mainDeclaration = 1;
		*pass = 1;
		return " ";	
	}
	else if (*mainFunc && strCompare(row, "MAIN RETURN", tmp))
	{
		*indent = *indent + 1;
		*mainFunc = 0;
		*pass = 1;
		return " ";
	}
	/*If the input is not of any known type, it just prints it as it is*/
	else
	{
		for (i = tmp; row[i] != '\n'; i++)
			output = list_appendCh(output, row[i]);
		return toStr(output);
	}
}

/*Returns all arguments from a simple for loop*/
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
/*Returns all arguments from a for loop and dynamically stores them in an ordered struct*/
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

	args.arg1 = strl_alloc(args.arg1);
	args.arg2 = strl_alloc(args.arg2);
	args.arg3 = strl_alloc(args.arg3);

	arg1 = args.arg1;
	arg2 = args.arg2;
	arg3 = args.arg3;
/*STRL_ALLOC RETURNS TMP AND NOT HEAD*/	
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
			arg1  = strl_alloc(arg1);
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
			arg3 = strl_alloc(arg3);

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
/*Checks if the input is a simple increment for loop,
 * with an int initialization, a simple 'less than/more than'
 * comparison and a single increment or decrement*/
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
/*Translates increment from pseudo code to Python syntax*/
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
