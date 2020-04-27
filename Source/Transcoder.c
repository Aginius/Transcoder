#include "TransLib.c"
#include "CPack.c"
#include "PythonPack.c"

int main (int argc, char * argv[])
{
	char * inName = NULL;
	char * outName = NULL;
	char * inLg = NULL;
	char * outLg = NULL;
	int i;

	if (argc> 1)	
	{
		if (strCompare(argv[1], "--out", 0) || strCompare(argv[1], "--in", 0))
		{
			if (argc < 7)
				printf("SOME OPTIONS MISSING, TYPE 'help' FOR HELP\n");
			for (i = 1; i < argc; i = i+3)
			{
				if (strCompare(argv[i], "--out", 0))
				{
					if (strCompare(argv[i+1], "py", 0))
					{
						outLg = strCopy(argv[i+1]);
						outName = strCopy(argv[i+2]);

					}
					else if (strCompare(argv[i+1], "c", 0))
					{
						outLg = strCopy(argv[i+1]);
						outName = strCopy(argv[i+2]);
					}
					else
						printf("YOU MUST SELECT A VALID LANGUAGE\n");
				}
				else if (strCompare(argv[i], "--in", 0))
				{
					if (strCompare(argv[i+1], "py", 0))
					{
						inLg = strCopy(argv[i+1]);
						inName = strCopy(argv[i+2]);
					}
					else if (strCompare(argv[i+1], "c", 0))
					{
						inLg = strCopy(argv[i+1]);
						inName = strCopy(argv[i+2]);
					}
					else
						printf("YOU MUST SELECT A VALID LANGAUGE\n");
				}
				else
					printf("UNKNOWN COMMAND\n");
			}

			if (!inName)
				printf("YOU MUST INSERT AN INPUT FILE NAME\n");
			else if (!outName)
				printf("YOU MUST INSERT AN OUTPUT FILE NAME\n");
			else if (!inLg)
				printf("YOU MUST INSERT AN INPUT FILE LANGUAGE\n");
			else if (!outLg)
				printf("YOU MUST INSERT AN OUTPUT FILE LANGUAGE\n");
			else
			{
				if (strCompare(inLg, "c", 0))
					scan_C(inName);
				else if (strCompare(inLg, "py", 0))
					printf("PYTHON SCANNING NOT AVAILABLE (CURRENTLY UNDER DEVELOPMENT)\n");
				if (strCompare(outLg, "py", 0))
					print_Python(outName);
				else if (strCompare(outLg, "c", 0))
					printf("C PRINTING NOT AVAILABLE (CURRENTLY UNDER DEVELOPMENT)\n");
			}

		}		
	
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
		}
		else
			printf("UNKNOWN COMMAND\n");
	}
	else
		printf("UNKNOWN COMMAND\n");

	return 0;
}


