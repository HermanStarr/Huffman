/** @file */
#include "Structs.h"
#include "Compression.h"
#include "Decompression.h"

#pragma warning(disable : 4996)

/**Function takes arguments from command line and returns given function names as well as checks mode of operation
@param argc argc is a count of command line arguments
@param argv argv is values of command line argumens
@param if_compression if_compression indicates mode of operation
@param file_paths array of path files*/
void argumentHandling(int argc, char** argv, bool* if_compression, char** file_paths);

/**Main function of the program
@param argc argc is a count of command line arguments
@param argv argv is values of command line argumens
@return code*/
int main(int argc, char* argv[]);

/**Function takes arguments from command line and returns given function names as well as checks mode of operation
@param argc argc is a count of command line arguments
@param argv argv is values of command line argumens
@param if_compression if_compression indicates mode of operation
@param file_paths array of path files*/
void argumentHandling(int argc, char** argv, bool* if_compression, char** file_paths)
{
	file_paths[0] = NULL;
	file_paths[1] = NULL;
	for (int i = 0; i < argc; i++)
	{
		/*Check for an input file switch*/
		if (argv[i][0] == '-' && argv[i][1] == 'f')
		{
			/*If next element does not exist*/
			if (i + 1 >= argc)
				endPrg(__func__, "no input file name specified!");
			/*If next argument is not a switch*/
			if (argv[i + 1][0] != 45)
			{
				i++;
				file_paths[0] = argv[i];
			}
		}
		/*Check for an output file switch*/
		else if (argv[i][0] == '-' && argv[i][1] == 'o')
		{
			/*If next element does not exist*/
			if (i + 1 >= argc)
				endPrg(__func__, "no output file name specified!");
			/*If next argument is not a switch*/
			if (argv[i + 1][0] != 45)
			{
				i++;
				file_paths[1] = argv[i];
			}
		}
		/*Check for decompression mode switch*/
		else if (argv[i][0] == '-' && argv[i][1] == 'd')
		{
			*if_compression = false;
		}
		/*Check for compression mode switch*/
		else if (argv[i][0] == '-' && argv[i][1] == 'c')
		{
			*if_compression = true;
		}
		/*If other command line argument is passed besides program path*/
		else if (i != 0)
			endPrg(__func__, "unknown command line argument");
	}
	/*If input file exists*/
	if (file_paths[0] != NULL)
	{
		FILE* input = fopen(file_paths[0], "rb");
		FILE* output = NULL;
		/*If output file was nor specified*/
		if (file_paths[1] == NULL)
		{
			if (*if_compression == false)
			{
				printf("As no output file had been specified, the function wrote to generic_output.txt\n\n");
				/*Create generic output file*/
				file_paths[1] = "generic_output.txt";
				output = fopen(file_paths[1], "w");
			}
			else
			{
				printf("As no output file had been specified, the function will wrote to generic_output.dat\n\n");
				/*Create generic output file*/
				file_paths[1] = "generic_output.dat";
				output = fopen(file_paths[1], "wb");
			}
		}
		/*If output file exists*/
		else
			output = fopen(file_paths[1], "wb");
		 
		/*Check if both of this files had been opened*/
		if (input && output)
		{
			fclose(input);
			fclose(output);
			return;
		}
		endPrg(__func__, "could not open existing files");
	}
	endPrg(__func__, "did not specify input file");

}

/**Main function of the program
@param argc argc is a count of command line arguments
@param argv argv is values of command line argumens
@return code*/
int main(int argc, char* argv[])
{
	/*if_compression checks the mode of operation of the program, set to true by default*/
	bool if_compression = true;
	/*Paths to both files*/
	char* file_paths[2];
	/*files is set to value returned by argumentHandling,
	which takes argc, argv and adress of if_compression to further modify*/
	argumentHandling(argc, argv, &if_compression, file_paths);
	/*If if_compression switch is set to true*/
	if (if_compression)
		/**Start compression process*/
		initCompression(file_paths[0], file_paths[1]);
	/*If compression switch is set to false*/
	else
		/*Start decompression process*/
		initDecompression(file_paths[0], file_paths[1]);

	return 0;
}