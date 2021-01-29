/** @file */
#include "Decompression.h"
#pragma warning(disable : 4996)

/**Function manages through decompression process
@param compression_file is a path to compressed file
@param decompression_file is a path to decompression file*/
void initDecompression(const char* compression_file, const char* decompression_file)
{
	/*Get handle to compression file in read mode*/
	FILE* compression = fopen(compression_file, "rb");
	/*Get handle to decompression file in write mode*/
	FILE* decompression = fopen(decompression_file, "wb");

	if (compression && decompression)
	{
		Keys* root = malloc(sizeof(Keys));
		if (root)
		{
			readKeys(root, compression, decompression);
			decompress(root, compression, decompression);
			fclose(compression);
			fclose(decompression);
			deleteKeys(root);
		}
		else
			endPrg(__func__, "failed to allocate");
	}
	else
		endPrg(__func__, "could not open files");
}
/**Function reads characters and corresponding codes
@param root rooot of Keys tree
@param compression compressed file handle*/
void readKeys(Keys* root, FILE* compression)
{
	(root)->left_node = NULL;
	(root)->right_node = NULL;

	unsigned char character = 0;

	/*While file has not ended
	Reads only character bits*/
	while (fread(&character, 1U, 1, compression) == 1 && character != 0)
	{
		unsigned char length_of_code;
		fread(&length_of_code, 1U, 1, compression);

		/*Allocate huffman as empty string*/
		char* huffman = strdup("");
		/*huffman element is 8bit chunk of huffman code read from file*/
		char huffman_element = 0;
		/*l beeing the length of code*/
		int l = ceiling(length_of_code);

		for (int i = 0; i < l; i++)
		{
			/*Read 8 bits from file*/
			fread(&huffman_element, 1U, 1, compression);
			/*Read through the bits*/
			for (int j = (sizeof(char) * 8) - 1; j >= 0; j--)
			{
				char* to_be_freed = huffman;
				huffman = appendCharacter(strdup(huffman), (huffman_element & (1u << j) ? 1 : 0) + 48);
				length_of_code--;
				free(to_be_freed);
				if (length_of_code == 0)
					break;
			}
		}
		keysBuildTree(root, huffman, character);
		free(huffman);
	}
}

/**Function creates a part of tree using keys from file
@param root is a root of Keys tree
@param code is acode corresponding to certain key
@param character is a character represented by given key*/
void keysBuildTree(Keys* root, char* code, char character)
{
	Keys* codes = (root);
	if (codes)
	{
		/*For every character in code string*/
		for (int i = 0; code[i]; i++)
		{
			/*If code[i] is equal to ASCII 0*/
			if (code[i] == 48)
			{
				codes->character = NULL;
				if (!codes->left_node)
				{
					codes->left_node = malloc(sizeof(Keys));
					if (codes->left_node)
					{
						codes->left_node->left_node = NULL;
						codes->left_node->right_node = NULL;
					}
					else
					{
						endPrg(__func__, "did not properly allocate");
					}
				}
				codes = codes->left_node;
			}
			/*If code[i] is equal to ASCII 1*/
			else if (code[i] == 49)
			{
				codes->character = NULL;
				if (!codes->right_node)
				{
					codes->right_node = malloc(sizeof(Keys));
					if (codes->right_node)
					{
						codes->right_node->left_node = NULL;
						codes->right_node->right_node = NULL;
					}
					else
					{
						endPrg(__func__, "did not properly allocate");
					}
				}
				codes = codes->right_node;
			}
			else
			{
				endPrg(__func__, "illegal character in string");
			}
		}
		codes->character = character;
	}
	else
		endPrg(__func__, "codes was NULL");
}

/**Function decompresses given encoded string in file
@param root rooot of Keys tree
@param compression compressed file handle
@param decompression decompression file handle*/
void decompress(Keys* root, FILE* compression, FILE* decompression)
{
	Keys* keys = root;
	unsigned char c = 0;
	while ((c = getc(compression)) != EOF && c != 255)
	{
		/*Declare stop position for loop*/
		int mainly_zero = 0;
		if (c == 0)
		{
			c = getc(compression);
			if (c == 1)
			{
				mainly_zero = 8 - getc(compression);
				c = getc(compression);
			}
		}
		for (int i = (sizeof(char) * 8) - 1; i >= mainly_zero; i--)
		{
			if (!(c & (1u << i)))
			{
				if (keys)
				{
					if (keys->left_node)
						keys = keys->left_node;
					else
					{
						fprintf(decompression, "%c", keys->character);
						keys = (root)->left_node;
					}
				}
				else
					endPrg(__func__, "keys does not exist");
			}
			else
			{
				if (keys)
				{
					if (keys->right_node)
						keys = keys->right_node;
					else
					{
						fprintf(decompression, "%c", keys->character);
						keys = (root)->right_node;
					}
				}
				else
					endPrg(__func__, "keys does not exist");
			}
		}
	}
	if (keys && keys->character != 0)
	{
		fprintf(decompression, "%c", keys->character);
	}
}
