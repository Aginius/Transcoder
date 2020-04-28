#include "TransLib.c"
#include "CPack.c"
#include "PythonPack.c"

void Options(char **, int, char **);

int main (int argc, char * argv[])
{
	char * commands[4] = {NULL, NULL, NULL, NULL};
	char * inName = NULL;
	char * outName = NULL;
	char * inLg = NULL;
	char * outLg = NULL;
	int i;

	Options(argv, argc, commands);

	inLg = commands[0];
	inName = commands[1];
	outLg = commands[2];
	outName = commands[3];
;
	if (argc == 1)
		printf("Transcoder, type 'help'\n");

	else if (strCompare(argv[1], "help", 0))
	{
		printf("COMMANDS:\n");
		printf("transcode\n");
		printf("help\n");
		printf("OPTIONS:\n");
		printf("--out | Selects the output file\n");
		printf("Accepts two arguments, the first one must be a language choice and the second one a file name\n");
		printf("--in | Selects the input file\n");
		printf("Accepts two arguments, the first must be a language choice and the second a file name\n");
		printf("Currently supported languages:\n");
		printf("Input:\n");
		printf("C | c\n");
		printf("Output:\n");
		printf("Python | py\n");
	}

	else if (!inName)
		printf("Insert an input name\n");
	else if (!outName)
		printf("Insert an output name\n");
	else if (!inLg)
		printf("Insert an input language\n");
	else if (!outLg)
		printf("Insert an output language\n");
	else
	{
		if (strCompare(inLg, "c", 0) || strCompare(inLg, "C", 0))
			scan_C(inName);
		else
			printf("CURRENTLY SUPPORTING C INPUT ONLY\n");
		if (strCompare(outLg, "py", 0) || strCompare(outLg, "Py", 0))
			print_Python(outName);
		else
			printf("CURRENTLY SUPPORTING PYTHON OUTPUT ONLY\n");
	}

	return 0;
}

void Options(char * input[], int len, char * commands[])
{
	int index;
	int i;

	for (i = 1; i < len; i++)
	{
		if (strCompare(input[i], "--in", 0))
		{
			index = 0;	
		}
		else if (strCompare(input[i], "--out", 0))
		{
			index = 2;
		}
		else
		{
			commands[index] = strCopy(input[i]);
			index++;
		}
	}
}
