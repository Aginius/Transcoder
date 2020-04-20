#include <stdio.h>
#include "str.c"
#include "StrList.c"
#include "TransLib.c"

void scan_C(char *);
char * input_C(char *, int *, int *);
int isFunction(char *);
int isFunctionL(list *);

#define SIZE 40

int main (int argc, char * argv[])
{
	if (argv[1])
		scan_C(argv[1]);
	else
		scan_C("try.c");
	return 0;
}

void scan_C(char fname[])
{
	FILE * input;
	FILE * output;
	char word[SIZE];
	list * line = NULL;
	int openBrCount;
	int closedBrCount;
	int noBrCondition;
	int isOpenBr;
	int indent;
	int isMain;
	int firstReturn;
	int isComment;
	int elseStatement;
	int bracketQueue;
	int lastIndent;
	int tmp;
	int i;

	char * string_line;
	char * trans;
	
	openBrCount = 0;
	closedBrCount = 0;
	isOpenBr = 0;
	indent = 0;
	isMain = 0;
	firstReturn = 0;
	noBrCondition = 0;
	isComment = 0;
	elseStatement = 0;
	bracketQueue = 0;
	lastIndent = 0;
	tmp = 0;
	
	if (input = fopen(fname, "r"))
	{
		if (output = fopen("PseudoCode.pc", "w"))
		{
			//TEMPORARY DEBUGGING VARIABLE TO MARK ITERATIONS
			int debug = 0;
			while (!feof(input))
			{
				if (elseStatement)
				{
					line = list_append(line, word);
					line = list_appendCh(line, ' ');
					elseStatement = 0;
				}
				else
				{
					fscanf(input, "%s\n", word);	
					printf("SCANNING: %s, %d\n", word, debug);
					debug++;
					if (line == NULL)
						noSpace(word);
					if (word[0] == '/')
					{
						if (word[1] == '*')
							isComment = 1;
						/*
						STILL IMPLEMENTING SINGLE LINE COMMENTS
						else if (word[1] == '/')
						{
							fgets(word, SIZE, input);
							for (i = 0; word[i] != '\0'; i++)
								line = list_appendCh(line, word[i]);
							printf("SINGLE LINE COMMENT DEBUG:\n");
							list_print(line);
							isComment = 1;
						}
						*/
					}
					else if (list_compare(line, "else") &&  !strCompare(word, "if", 0))
						elseStatement = 1;
					
					if (!elseStatement)
					{
						line = list_append(line, word);
						line = list_appendCh(line, ' ');
					}

					openBrCount = smartCount(line, '(');
					closedBrCount = smartCount(line, ')');
					/*
					printf("APPENDING %s\n",word);
					printf("CURRENT LINE: ");
					list_print(line);
					*/
				}
							
				if (!isComment && (checkLast(word) == ';' && openBrCount == closedBrCount || checkLast(word) == '{' || checkLast(word) == '}' || checkLast(word) == ')' && openBrCount == closedBrCount  || list_compare(line, "else") && elseStatement || line -> c == '#' && (checkLast(word) == '>' || checkLast(word) == '"')) || isComment && strCompareTail(word, "*/"))
				{
					string_line = toStr(line);
					printf("CURRENT LINE: %s\n",string_line);
					//printf("Open Brackets: %d\nClosed Brackets: %d\n", openBrCount, closedBrCount);
					/*Translates the main declaration to pseudo code*/
					if (strCompare(string_line, "int main(", 0) || strCompare(string_line, "int main (", 0))
					{
						fprintf(output, "MAIN DECLARATION\n");
						isMain = 1;
						openBrCount = 0;
						closedBrCount = 0;
						line = NULL;
					}
					else if (strCompare(string_line, "return", 0) && !firstReturn && isMain)
					{
						fprintf(output, "MAIN RETURN\n");
						firstReturn = 1;
						openBrCount = 0;
						closedBrCount = 0;
						line = NULL;
					}
					/*Translates comments to pseudo code (currently multi-line only)*/
					else if (isComment)
					{
						tab_line(output, indent);
						fprintf(output, "%s\n", string_line);
						openBrCount = 0;
						closedBrCount = 0;
						isComment = 0;
						line = NULL;
					}
					else
					{
						if (checkLast(string_line) == '{')
						{
							noBrCondition = 0;
							isOpenBr = 1;
						}
						/*If last line was a condition and this actual line is not a bracket, it indents the code and manually prints an open bracket*/
						else if (noBrCondition)
						{
							//printf("NO BRACKET INDENTING\n");
							tab_line(output, indent);
							fprintf(output, "%c\n", '{');
							tmp = 1;
							indent++;
							lastIndent = indent;
							bracketQueue++;
						}
					
						noBrCondition = 0;
						/*Translating the input to pseudo code*/
						trans = input_C(string_line, &indent, &noBrCondition);
						
						if (isOpenBr)
						{
							printf("Indent levels: %d\n", indent-1);
							tab_line(output, indent-1);
							isOpenBr = 0;
						}
						else
						{
							printf("Indent levels: %d\n",indent);
							tab_line(output, indent);
						}
						fprintf(output, "%s\n", trans);
						/*If last line was a condition and this actual line is not another condition, it manually prints a closed bracket*/
						if (tmp && !noBrCondition && lastIndent == indent)
						{
							/*Prints a number of closed brackets according to previous no bracket conditions*/
							for (i = 0; i < bracketQueue; i++)
							{
								indent--;
								tab_line(output, indent);
								fprintf(output, "%c\n", '}');
								tmp = 0;
							}
							bracketQueue = 0;
						}
						//printf("Printing: %s\n", trans);
						openBrCount = 0;
						closedBrCount = 0;
						line = NULL;
						free(trans);
					}
					printf("\n");
					free(string_line);
				}
			}
			fclose(output);
		}
		else
			printf("FILE WRITING ERROR\n");
		fclose(input);
	}
	/*BUG?*/
	else
		printf("FILE OPENING ERROR\n");
}

char * input_C(char row[], int * indent, int * noBrCondition)
{
	list * output = NULL;
	int functionType;
	int i;

	if (strCompare(row,"if ",0) && nextChar(row, 1) == '(')
	{
		//printf("DEBUG 0\n");
		output = list_append(output, "C. if ");
		for (i = ch_index(row, '(', 1); i < ch_index(row, ')', charCount(row, ')'))+1; i++)
			output = list_appendCh(output, row[i]);
		*noBrCondition = 1;
		return toStr(output);
	}
	else if (strCompare(row,"else ",0) || strCompare(row,"else(",0))
	{
		//printf("DEBUG 1\n");
		if (nextChar(row, 3) == 'i')
		{
			output = list_append(output, "C. elif ");
			for (i = ch_index(row, '(', 1); i < ch_index(row, ')', charCount(row, ')'))+1; i++)
				output = list_appendCh(output, row[i]);
			*noBrCondition = 1;
			return toStr(output);
		}
		else
		{
			output = list_append(output, "C. else");
			for (i = ch_index(row, '(', 1); i < ch_index(row, ')', charCount(row, ')'))+1; i++)
				output = list_appendCh(output, row[i]);
			
			*noBrCondition = 1;
			return toStr(output);
		}
	}
	
	else if (strCompare(row,"while",0) && nextChar(row, 4) == '(')
	{
		//printf("DEBUG 2\n");
		output = list_append(output, "C. wh ");
		for (i = ch_index(row, '(', 1); i < ch_index(row, ')', charCount(row, ')'))+1; i++)
			output = list_appendCh(output, row[i]);
		*noBrCondition = 1;
		return toStr(output);
	}
	else if (strCompare(row,"for",0) && nextChar(row, 2) == '(')
	{
		//printf("DEBUG 3\n");
		output = list_append(output, "C. for ");
		for (i = ch_index(row, '(', 1); i < ch_index(row, ')', charCount(row, ')'))+1 && row[i] != '\0'; i++)
			output = list_appendCh(output, row[i]);
		*noBrCondition = 1;
		return toStr(output);
	}
	else if (strCompare(row,"int ",0))
	{
		//printf("DEBUG 4\n");
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
				output = list_append(output, "V.D.I.Cl.i ");
				for (i = 4; row[i] != ';'; i++)
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
	else if (strCompare(row,"char ",0))
	{
		//printf("DEBUG 5\n");
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
				output = list_append(output, "V.D.I.Cl.c ");
				for (i = 5; row[i] != ';'; i++)
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
	else if (strCompare(row,"void ",0))
	{
		//printf("DEBUG 6\n");
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
		//printf("DEBUG 7\n");
		output = list_append(output, "IMPORT ");
		for (i = 9; row[i] != '\0'; i++)
		{
			if (row[i] != '<' && row[i] != '>' && row[i] != '"' && row[i] != '"')
				output = list_appendCh(output, row[i]);
		}
		return toStr(output);
	}
	else if (strCompare(row,"printf",0) && nextChar(row, 5) == '(')
	{
		//printf("DEBUG 8\n");
		output = list_append(output, "F.Cl print");
		for (i = 6; row[i] != '\0' && row[i] != '('; i++)
			;
		for ( ; row[i] != '\0' && row[i] != ';'; i++)
			output = list_appendCh(output, row[i]);
		return toStr(output);
	}
	else if (strCompare(row,"{",0))
	{
		//printf("DEBUG 9\n");
		output = list_appendCh(output, '{');
		*indent = *indent + 1;
		return toStr(output);
	}
	else if (strCompare(row,"}",0 ))
	{
		//printf("DEBUG 10\n");
		output = list_appendCh(output, '}');
		*indent = *indent - 1;
		return toStr(output);
	}
	else if (strCompare(row,"return",0))
	{
		for (i = 0; row[i] != ';'; i++)
			output = list_appendCh(output, row[i]);
		return toStr(output);
	}
	else
	{
		//printf("DEBUG 11\n");
		if (isFunction(row))
		{
			if (charCount(row, '='))
			{
				//printf("DEBUG 12\n");
				output = list_append(output, "V.I.Cl ");
				for (i = 0; row[i] != ';'; i++)
					output = list_appendCh(output, row[i]);
				return toStr(output);
			}
			//printf("DEBUG 13\n");
			output = list_append(output, "F.Cl ");
			for (i = 0; row[i] == ' '; i++)
				;

			for ( ; row[i] != ';'; i++)
				output = list_appendCh(output, row[i]);
			return toStr(output);
		}
		else if (charCount(row, '='))
		{
			//printf("DEBUG 14\n");
			output = list_append(output, "V.I ");
			for (i = 0; row[i] != ';'; i++)
				output = list_appendCh(output, row[i]);
			return toStr(output);
		}
		else if (isIncrement(row))
		{
			output = list_append(output, "V.I ");
			for (i = 0; row[i] != ';'; i++)
				output = list_appendCh(output, row[i]);
			return toStr(output);
		}
		else
		{
			//printf("DEBUG 15\n");
			output = list_append(output, "NA ");
			for (i = 0; row[i] != '\0'; i++)
				output = list_appendCh(output, row[i]);
			return toStr(output);
		}
	}
}

/*Checks if it's a function, returns 1 if a declaration, 2 if an initialization and 3 if a call*/
int isFunction(char row[])
{
	int i;
	int tmp;
	
	if (charCount(row, '('))
	{
		tmp = ch_index(row, '(', 1) - 1;
		/*Checks if there's a mathematic symbol before the first occurance of a parenthesis*/
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

/*Same as above but accepts a char list as an input*/
int isFunctionL(list * head)
{
	int i;
	list * tmp;
	
	if (smartCount(head, '('))
	{
		for (tmp = head; tmp && tmp -> next -> c != '('; tmp = tmp -> next);
		
		//tmp = ch_index(row, '(', 1) - 1;
		/*Checks if there's a mathematic symbol before the first occurance of a parenthesis*/
		if (tmp -> c != ' ' && tmp -> c != '*' && tmp -> c != '+' && tmp -> c != '-' && tmp -> c != '/')
		{
			if (!listCount(head, '='))
			{
				if (listCount(head, ';'))
				{
					if (list_compare(head,"int") || list_compare(head,"char") || list_compare(head,"void"))
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



