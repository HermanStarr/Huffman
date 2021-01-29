/** @file */
#include "Compression.h"
#pragma warning(disable : 4996)

/**Function manages compression process
@param input is an input handler
@param output is an output handler*/
void initCompression(char* input, char* output)
{
	Characters* characters = malloc(sizeof(Characters));
	if (characters)
	{
		characters->character = 0;
		characters->next = NULL;
		characters->code = NULL;
		characters->tree = NULL;
		characters->value = 0u;

		/*Call function readCharactersForCompression
		taking input handle and adress of Characters head to further modify*/
		readCharactersForCompression(input, &characters);
		/*Call for list to be sorted using merge sort
		taking adress of Characters head to further modify*/
		mergeSort(&characters);

		/*Create a copy of characters list*/
		Characters* copy = copyCharacters(characters);
		/*Deconstruct Characters list while constructing huffman tree*/
		Keys* keys = huffmanTreeConstruction(&characters);
		/*Assign huffman codes to characters in a tree*/
		assignFirstHuffmanCodes(copy, keys);

		FILE* compression = fopen(output, "wb");
		FILE* for_compression = fopen(input, "rb");
		if (compression && for_compression)
		{
			/*Compress codes into file*/
			compressCodes(compression, copy);
			/*Compress text of input file*/
			compressText(for_compression, compression, copy);

			if (fclose(for_compression) == EOF || fclose(compression) == EOF)
			{
				endPrg(__func__, "failed to close files");
			}
		}

		deleteCharacters(characters);
		deleteCharacters(copy);
		deleteKeys(keys);
	}
	else
	{
		endPrg(__func__, "did not allocate head of Characters list");
	}
}
/**Function reads characters from input file and creates a characters list
@param file_name is a path to the input file for compression
@param head is a head to the Characters list*/
void readCharactersForCompression(char* file_name, Characters** head)
{
	FILE* for_compression_file = fopen(file_name, "rb");
	if (for_compression_file)
	{
		int count = 0;
		char c = 0;
		/**While not END OF FILE*/
		while (fread(&c, 1U, 1, for_compression_file) == 1)
		{
			count++;
			if (c == 0)
				endPrg(__func__, "did not read any characters");
			/*Add character to Characters list*/
			addCharacter(c, head);
		}
		fclose(for_compression_file);
	}
	else
		endPrg(__func__, "could not open input file");
}
/**Function assigns huffman codes based on a huffman tree
*not embeded in the assignHuffmanCodes function to avoid redundant if statements
@param copy is a head of a copy Characters list
@param keys is a head of Keys tree*/
void assignFirstHuffmanCodes(Characters* copy, Keys* keys)
{
	if (!keys->left_node && !keys->right_node)
	{
		/*Set code to 0*/
		copy->code = strdup("0");

		return;
	}
	/*Dynamically allocate lc as "0"*/
	char* lc = strdup("0");
	/*Assign huffman code for left node*/
	assignHuffmanCodes(copy, keys->left_node, lc);
	/*Dynamically allocate rc as "1"*/
	char* rc = strdup("1");
	/*Assign huffman code for left node*/
	assignHuffmanCodes(copy, keys->right_node, rc);
}
/**Function assigns huffman codes based on a huffman tree
@param copy is a head of a copy Characters list
@param keys is a head of Keys tree
@param code is a passed huffman code corresponding to current node*/
void assignHuffmanCodes(Characters* copy, Keys* keys, char* code)
{
	if (keys)
	{
		if (keys->left_node)
		{
			/*Append 0 to the end of code*/
			char* new_code = appendCharacter(strdup(code), 48);
			if (new_code)
				/*Assign Huffman code for left node*/
				assignHuffmanCodes(copy, keys->left_node, new_code);
			else
				endPrg(__func__, "failed to allocate");
		}
		if (keys->right_node)
		{
			/*Append 1 to the end of code*/
			char* new_code = appendCharacter(strdup(code), 49);
			if (new_code)
				/*Assign Huffman code for right node*/
				assignHuffmanCodes(copy, keys->right_node, new_code);
			else
				endPrg(__func__, "failed to allocate");
		}
		/*If leaf*/
		if (!keys->left_node && !keys->right_node)
		{
			/*Assign code corresponding to given character*/
			searchCharacter(keys->character, copy)->code = code;
		}
		/*If not leaf*/
		else
		{
			free(code);
		}
	}
	else
	{
		endPrg(__func__, "Keys node was empty");
	}
}
/**Functions passes information about characters and coding into the output file
@param compression compression file
@param list header to Characters list*/
void compressCodes(FILE* compression, Characters* list)
{
	/*Create temporary value storing list adress*/
	Characters* tmp = list;
	for (; tmp; tmp = tmp->next)
	{
		/*Compute length of code*/
		unsigned char length = strlen(tmp->code);
		/*Write 8-bit word storing character*/
		fwrite(&tmp->character, 1U, 1, compression);
		/*Write 8-bit word storing lenghth of code to be read from the next words*/
		fwrite(&length, 1U, 1, compression);

		unsigned char code_to_be = 0;
		/*Write appropriate ammount of 8-bit words to the file
		ceil(length/8) is rounded up ammount of total bits divided by 8*/
		for (int i = 0; i < ceiling(length); i++)
		{
			int j = 0;
			/*j cannot be more than length of code
			and it cannot be more than (i+1)*8 */
			for (j = i * 8; j < length && j < (i + 1) * 8; j++)
			{
				if (tmp->code[j] == 48)
				{
					/*left shift code_to_be*/
					code_to_be *= 2;
				}
				else
				{
					/*Left shift code_to_be*/
					code_to_be *= 2;
					code_to_be += 1;
				}
			}
			/*If not all 8 bits were written*/
			if (j % 8 != 0)
			{
				/*For remaining bits*/
				for (; j % 8 != 0; j++)
				{
					if (j % 2 == 1)
					{
						/*Left shift*/
						code_to_be *= 2;
						code_to_be += 1;
					}
					else
						/*Shift left*/
						code_to_be *= 2;
				}
				/*Write the result into the file*/
				fwrite(&code_to_be, 1U, 1, compression);
				break;
			}
			/*If all 8 bits were written*/
			else
			{
				/*Write the result into the file*/
				fwrite(&code_to_be, 1U, 1, compression);
				/*Zero the code*/
				code_to_be = 0;
			}

		}
	}
	/*Set guardian value 0*/
	unsigned char to_be_passed = 0;
	/*Print additional guardian line in compression file*/
	fwrite(&to_be_passed, 1U, 1, compression);
}
/**Function passes encoded characters into the output file
@param for_compression file from where all of the uncompressed characters are read
@param compression compression file
@param list header to Characters list*/
compressText(FILE* for_compression, FILE* compression, Characters* list)
{
	unsigned char to_be_passed = 0;
	unsigned char c = 0;
	/*Define count value to manauver through 8 bit chunks*/
	size_t count = 0;
	Characters* element = NULL;
	/*Code of given Characters element*/
	char* code = NULL;

	while ((c = getc(for_compression)) != EOF)
	{
		/*Search for given character*/
		element = searchCharacter(c, list);
		/*If there is no such an element break*/
		if (element == NULL)
			break;
		/*Set code to be equal to code of the element*/
		code = element->code;

		size_t size = strlen(code);
		for (size_t i = 0; i < size; i++)
		{
			if (count == 8)
			{
				/*If code is equal to either 0 or 255 set guardian value*/
				if (to_be_passed == 255 || to_be_passed == 0)
				{
					/*Guardian value equal to 0*/
					unsigned char pass = 0;
					/*Write guardian value*/
					fwrite(&pass, sizeof(char), 1, compression);
				}
				if (fwrite(&to_be_passed, sizeof(char), 1, compression) != 1)
					endPrg(__func__, "fwrite is nuts!");
				to_be_passed = 0;
				count = 0;
			}
			if (code[i] == 49)
			{
				to_be_passed *= 2;
				to_be_passed += 1;
			}
			else
			{
				to_be_passed *= 2;
			}
			count++;
		}
	}
	if (count != 0)
	{
		unsigned char count2 = count;
		/*For each remaining bit*/
		for (; count < 8; count++)
		{
			if (count % 2 == 0)
			{
				to_be_passed *= 2;
				to_be_passed += 1;
			}
			else
				to_be_passed *= 2;
		}
		count = 0;
		/*Write guardian value 0*/
		fwrite(&count, 1U, 1, compression);
		/*Set count to 1*/
		count = 1;
		/*Write second guardian value 1*/
		fwrite(&count, 1U, 1, compression);
		/*Write number of bits to be read*/
		fwrite(&count2, 1U, 1, compression);
		/*Pass the remaining bits*/
		fwrite(&to_be_passed, 1U, 1, compression);
	}
}
